
import abc
from vk_api.bot_longpoll import VkBotMessageEvent

from .i_undoable import IUndoable

class ICommand(abc.ABC):
    @abc.abstractmethod
    def handle(self, event: VkBotMessageEvent) -> IUndoable:
        pass
