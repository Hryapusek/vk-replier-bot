
from .i_command import ICommand
from vk_api.bot_longpoll import VkBotMessageEvent

class InfoCommand(ICommand):
    def handle(self, event: VkBotMessageEvent) -> None:
        return None
