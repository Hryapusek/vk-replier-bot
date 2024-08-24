
from result import Err, Ok, Result

from messageprocessing.commands.utils import is_user_blocked
from settings.bot_settings import BotSettings
from vkservice.vk_service import is_user_admin, send_reply_message
from .i_command import ICommand
from vk_api.bot_longpoll import VkBotMessageEvent

def check_if_user_allowed(event: VkBotMessageEvent) -> Result[None, str]:
    result = is_user_blocked(event.message.from_id)
    if result.is_err():
        return Err(result.err())
    
    if event.message.from_id in BotSettings().get_godlike_ids():
        return Ok(None)
    
    if event.message.from_id in BotSettings().get_moderator_ids():
        return Ok(None)

    return Err("Вы должны быть godlike или модератором")

class DeleteCurrentChatCommand(ICommand):
    def handle(self, event: VkBotMessageEvent) -> None:
        result = check_if_user_allowed(event)
        if result.is_err():
            send_reply_message(event.message.peer_id, result.err(), event.message.conversation_message_id)
            return

        chats = BotSettings().get_chats()

        if event.message.peer_id in [chat.vk_chat_peer_id for chat in chats]:
            chats.remove([chat for chat in chats if chat.vk_chat_peer_id == event.message.peer_id][0])
            BotSettings().set_chats(chats)
            send_reply_message(event.message.peer_id, "Чат удален", event.message.conversation_message_id)
        else:
            send_reply_message(event.message.peer_id, "Чат не найден", event.message.conversation_message_id)
