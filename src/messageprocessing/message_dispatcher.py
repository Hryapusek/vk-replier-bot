import threading
from messageprocessing.commands.i_command import ICommand
from messageprocessing.commands.i_undoable import IUndoable
from settings.bot_settings import BotSettings
from vkservice.vk_service import send_reply_message
from .commands.all_commands import ALL_COMMANDS

from vk_api.bot_longpoll import VkBotEventType, VkBotEvent, VkBotMessageEvent

from loguru import logger

from result import Ok, Err, Result

def _extract_command(event: VkBotEvent) -> Result[str, None]:
    """event.message.text should not be None"""
    command = str(event.message.text).strip().splitlines()[0].split()[0].lower()
    if not command.startswith("/"):
        return Err(None)
        
    return Ok(command[1:])

class UndoCommand(ICommand):
    def handle(self, event: VkBotMessageEvent) -> None:
        logger.debug("Undoing last command")
        if (not event.message.from_id in BotSettings().get_godlike_ids()
                and not event.message.from_id in BotSettings().get_moderator_ids()):
            send_reply_message(event.message.peer_id, "You're not allowed to do that")
            return
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
        if (not event.message.from_id in BotSettings().get_godlike_ids()
                and not event.message.from_id in BotSettings().get_moderator_ids()):
            send_reply_message(event.message.peer_id, "You're not allowed to do that")
            return
        dispatcher = MessageDispatcher()
        with dispatcher.lock:
            if len(dispatcher.commands_history) > 0:
                dispatcher.commands_history.pop()
            else:
                logger.debug("Nothing to pop")

ALL_COMMANDS.extend([
    UndoCommand(
        "undo",
        "Undo last command",
        "",
        "/undo",
        []
    ),
    SkipOneCommand(
        "skip",
        "Pop last command from commands history",
        "",
        "/skip",
        []
    )
])

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
            self.commands_dispatchering = {}

            for command in ALL_COMMANDS:
                logger.debug("Registering command: {}".format(command))
                self.commands_dispatchering[command.command_name] = command
                for shortcut in command.shortcuts:
                    logger.debug("Registering shortcut: {}".format(shortcut))
                    self.commands_dispatchering[shortcut] = command
                    
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
