
from vkservice.vk_service import send_reply_message
from .i_command import ICommand
from vk_api.bot_longpoll import VkBotMessageEvent

class GetVkChatIdCommand(ICommand):
    def handle(self, event: VkBotMessageEvent) -> None:
        send_reply_message(event.message.peer_id, str(event.message.peer_id), event.message.conversation_message_id)


