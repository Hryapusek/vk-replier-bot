import threading
from types import NoneType
from messageprocessing.commands.i_command import ICommand
from messageprocessing.commands.i_undoable import IUndoable
from .commands import *

from vk_api.bot_longpoll import VkBotEventType, VkBotEvent, VkBotMessageEvent

from loguru import logger

from result import Ok, Err, Result

def _extract_command(event: VkBotEvent) -> Result[str, NoneType]:
    """event.message.text should not be None"""
    command = str(event.message.text).strip().splitlines()[0].split()[0].lower()
    if not command.startswith("/"):
        return Err(None)
        
    return Ok(command[1:])

class UndoCommand(ICommand):
    def handle(self, event: VkBotMessageEvent) -> None:
        logger.debug("Undoing last command")
        dispatcher = MessageDispatcher()
        with dispatcher.lock:
            if len(dispatcher.commands_history) > 0:
                if dispatcher.commands_history[-1].undo(event):
                    logger.debug("Command undone")
                    dispatcher.commands_history.pop()
            else:
                logger.debug("Nothing to undo")

class SkipOneCommand(ICommand):
    def handle(self, event: VkBotMessageEvent) -> None:
        logger.debug("Popping command from history")
        dispatcher = MessageDispatcher()
        with dispatcher.lock:
            if len(dispatcher.commands_history) > 0:
                dispatcher.commands_history.pop()
            else:
                logger.debug("Nothing to pop")

class MessageDispatcher:
    _instance = None

    def __new__(cls):
        if not cls._instance:
            cls._instance = super().__new__(cls)
        return cls._instance

    def __init__(self) -> None:
        if not hasattr(self, "_initialized"):
            self._initialized = True
            self.lock = threading.RLock()
            self.change_mode_command = ChangeModeCommand()
            self.delete_source_command = DeleteSourceCommand()
            self.delete_target_command = DeleteTargetCommand()
            self.delete_target_by_id_command = DeleteTargetByIdCommand()
            self.exit_command = ExitCommand()
            self.info_command = InfoCommand()
            self.register_godlike_command = RegisterGodlikeCommand()
            self.register_source_command = RegisterSourceCommand()
            self.register_target_command = RegisterTargetCommand()
            self.help_command = HelpCommand()
            self.send_command = SendCommand()
            self.undo_command = UndoCommand()
            self.send_with_all_command = SendWithAllCommand()

            self.commands_dispatchering = {
                "sus": self.change_mode_command,
                "delete_source": self.delete_source_command,
                "delete_target": self.delete_target_by_id_command,
                "delete_target_by_id": self.delete_target_by_id_command,
                "exit": self.exit_command,
                "info": self.info_command,
                "undo": self.undo_command,
                "register_godlike": self.register_godlike_command,
                "register_source": self.register_source_command,
                "register_target": self.register_target_command,
                "help": self.help_command,
                "cm": self.change_mode_command,
                "ds": self.delete_source_command,
                "dt": self.delete_target_command,
                "dtbi": self.delete_target_by_id_command,
                "i": self.info_command,
                "rg": self.register_godlike_command,
                "rs": self.register_source_command,
                "rt": self.register_target_command,
                "h": self.help_command,
                "всем": self.send_command,
                "всем_важно": self.send_with_all_command
            }
            self.commands_history: list[IUndoable] = []

    async def handle(self, event: VkBotEvent) -> None:
        logger.debug("Handling event: {}".format(event))
        if event.type != VkBotEventType.MESSAGE_NEW:
            logger.debug("Ignoring event type: {}", event.type)
            return

        event: VkBotMessageEvent

        if event.message is None:
            logger.debug("Ignoring event without message: {}".format(event))
            return
        
        if event.message.text is None:
            logger.debug("Ignoring event without text: {}".format(event))
            return

        logger.debug("Event message: {}".format(event.message.text))
        command = _extract_command(event)

        if not command.is_ok():
            logger.debug("Ignoring event without command: {}".format(event))
            return
        
        command = command.unwrap()

        logger.debug("Extracted command: {}".format(command))

        if command in self.commands_dispatchering:
            logger.debug("Handling command: {}".format(command))
            undoable = self.commands_dispatchering[command].handle(event)
            if undoable is not None:
                with self.lock:
                    self.commands_history.append(undoable)
        else:
            logger.debug("Ignoring unknown command: {}".format(command))
