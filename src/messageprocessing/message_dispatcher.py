from messageprocessing.commands.i_undoable import IUndoable
from .commands import *

from vk_api.bot_longpoll import VkBotEventType, VkBotEvent, VkBotMessageEvent

from loguru import logger

def _extract_command(event: VkBotEvent) -> str:
    """event.message.text should not be None"""
    return str(event.message.text).strip().splitlines()[0].split()[0].lower()

class MessageDispatcher:

    def __init__(self) -> None:
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
        self.send_with_all_command = SendWithAllCommand()

        self.__commands = {
            "change_mode": self.change_mode_command,
            "delete_source": self.delete_source_command,
            "delete_target": self.delete_target_by_id_command,
            "delete_target_by_id": self.delete_target_by_id_command,
            "exit": self.exit_command,
            "info": self.info_command,
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
        self.__commands_history: list[IUndoable] = []

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

        logger.debug("Extracted command: {}".format(command))

        if command in self.__commands:
            logger.debug("Handling command: {}".format(command))
            self.__commands_history.append(self.__commands[command].handle(event))
        else:
            logger.debug("Ignoring unknown command: {}".format(command))
