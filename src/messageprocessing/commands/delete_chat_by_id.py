
from result import Err, Ok, Result
from messageprocessing.commands.utils import is_user_blocked
from settings.bot_settings import BotSettings
from vkservice.vk_service import send_reply_message
from .i_command import ICommand
from vk_api.bot_longpoll import VkBotMessageEvent

def check_if_user_allowed(event: VkBotMessageEvent) -> Result[None, str]:
    result = is_user_blocked(event.message.from_id)
    if result.is_err():
        return Err(result.err())
    
    if event.message.from_id in BotSettings().get_godlike_ids():
        return Ok(None)

    return Err("Вы должны быть godlike")

class DeleteChatByIdCommand(ICommand):
    USAGE_STRING = "Использование: /delete_target_by_id {id}"
    def handle(self, event: VkBotMessageEvent) -> None:
        result = check_if_user_allowed(event)
        if result.is_err():
            send_reply_message(event.message.peer_id, result.err(), event.message.conversation_message_id)
            return
        
        args = event.message.text.strip().splitline()[0].split()
        if len(args) < 2:
            send_reply_message(event.message.peer_id, __class__.USAGE_STRING, event.message.conversation_message_id)
            return
        try:
            new_chat_id = int(args[1])
        except Exception:
            send_reply_message(event.message.peer_id, "Неправильный ID", event.message.conversation_message_id)
            return
        
        chats = BotSettings().get_chats()
        if new_chat_id not in [chat.id for chat in chats]:
            send_reply_message(event.message.peer_id, "Целевой чат не найден", event.message.conversation_message_id)
            return

        chat_to_remove = [target_chat for target_chat in chats if target_chat.id == new_chat_id][0]
        chats.remove(chat_to_remove)
        BotSettings().set_chats(chats)
        send_reply_message(event.message.peer_id, "Целевой чат удален", event.message.conversation_message_id)

