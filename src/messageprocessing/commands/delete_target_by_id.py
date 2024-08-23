
from .i_command import ICommand
from vk_api.bot_longpoll import VkBotMessageEvent

class DeleteTargetByIdCommand(ICommand):
    def handle(self, event: VkBotMessageEvent) -> None:
        return None
