
import abc
from vk_api.bot_longpoll import VkBotMessageEvent

from .i_undoable import IUndoable

class ICommand(abc.ABC):
    def __init__(self, command_name: str, description: str, usage: str, shortcuts: list[str], roles: str) -> None:
        self.command_name = command_name
        self.description = description
        self.usage = usage
        self.shortcuts = shortcuts
        self.roles = roles

    @abc.abstractmethod
    def handle(self, event: VkBotMessageEvent) -> IUndoable:
        pass
