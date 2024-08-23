
from .i_command import ICommand
from vk_api.bot_longpoll import VkBotEvent

class SendWithAllCommand(ICommand):
    def handle(self, event: VkBotEvent) -> None:
        return None
