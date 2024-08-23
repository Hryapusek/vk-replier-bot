
import abc
from vk_api.bot_longpoll import VkBotEvent

class IUndoable(abc.ABC):
    @abc.abstractmethod
    def undo(self, event: VkBotEvent):
        pass
