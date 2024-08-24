
from exceptions import ReloadException
from .i_command import ICommand
from vk_api.bot_longpoll import VkBotMessageEvent

class ReloadSettingsCommand(ICommand):
    def handle(self, event: VkBotMessageEvent) -> None:
        raise ReloadException
