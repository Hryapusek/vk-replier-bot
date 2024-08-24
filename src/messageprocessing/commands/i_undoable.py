
import abc
from vk_api.bot_longpoll import VkBotMessageEvent

class IUndoable(abc.ABC):
    @abc.abstractmethod
    def undo(self, event: VkBotMessageEvent):
        pass
