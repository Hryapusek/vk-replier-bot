
from messageprocessing.commands.utils import is_user_blocked
from settings.bot_settings import BotSettings
from vkservice.vk_service import send_reply_message
from .i_command import ICommand
from vk_api.bot_longpoll import VkBotMessageEvent

class ListChatsCommand(ICommand):
    def handle(self, event: VkBotMessageEvent) -> None:
        result = is_user_blocked(event.message.from_id)
        if result.is_err():
            send_reply_message(event.message.peer_id, result.err(), event.message.conversation_message_id)
            return
        response_message = ""
        for chat in BotSettings().get_chats():
            response_message += f"ID: {chat.id}\n"
            response_message += f"VkId: {chat.vk_chat_peer_id}\n"
            response_message += f"Title: {chat.title if chat.title else 'None'}\n"
            response_message += "------------\n"
        send_reply_message(event.message.peer_id, response_message, event.message.conversation_message_id)
        
