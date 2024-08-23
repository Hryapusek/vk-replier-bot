
import abc
from vk_api.bot_longpoll import VkBotEvent

from .i_undoable import IUndoable

class ICommand(abc.ABC):
    @abc.abstractmethod
    def handle(self, event: VkBotEvent) -> IUndoable:
        pass
