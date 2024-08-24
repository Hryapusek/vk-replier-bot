
from messageprocessing.commands.utils import is_user_blocked
from vkservice.vk_service import send_reply_message
from .i_command import ICommand
from vk_api.bot_longpoll import VkBotMessageEvent

class GetVkChatIdCommand(ICommand):
    def handle(self, event: VkBotMessageEvent) -> None:
        result = is_user_blocked(event.message.from_id)
        if result.is_err():
            send_reply_message(event.message.peer_id, result.err(), event.message.conversation_message_id)
            return;
        send_reply_message(event.message.peer_id, str(event.message.peer_id), event.message.conversation_message_id)


