
from result import Err
from messageprocessing.commands.utils import is_user_blocked
from settings.bot_settings import BotSettings
from vkservice.vk_service import send_reply_message
from .i_command import ICommand
from vk_api.bot_longpoll import VkBotMessageEvent

class GetChatIdCommand(ICommand):
    def handle(self, event: VkBotMessageEvent) -> None:
        result = is_user_blocked(event.message.from_id)
        if result.is_err():
            send_reply_message(event.message.peer_id, result.err(), event.message.conversation_message_id)
            return
        
        result = list(filter(BotSettings().get_chats(), lambda chat: chat.vk_chat_peer_id == event.message.peer_id))
        if result:
            send_reply_message(event.message.peer_id, str(result[0].id), event.message.conversation_message_id)
            return

        send_reply_message(event.message.peer_id, "Данный чат не найден", event.message.conversation_message_id)
