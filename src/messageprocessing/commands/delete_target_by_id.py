
from result import Err, Ok, Result
from settings.bot_settings import BotSettings
from vkservice.vk_service import send_reply_message
from .i_command import ICommand
from vk_api.bot_longpoll import VkBotMessageEvent

def check_if_user_allowed(event: VkBotMessageEvent) -> Result[None, str]:
    if event.message.from_id in BotSettings().get_godlike_ids():
        return Ok(None)

    return Err("Вы должны быть godlike")

class DeleteTargetByIdCommand(ICommand):
    def handle(self, event: VkBotMessageEvent) -> None:
        result = check_if_user_allowed(event)
        if result.is_err():
            send_reply_message(event.message.peer_id, result.err(), event.message.conversation_message_id)
            return
        
        args = event.message.text.strip().splitline()[0].split()
        if len(args) < 2:
            send_reply_message(event.message.peer_id, "Использование: /delete_target_by_id {id}", event.message.conversation_message_id)
            return
        try:
            new_target_id = int(args[1])
        except Exception:
            send_reply_message(event.message.peer_id, "Неправильный ID", event.message.conversation_message_id)
            return
        
        target_chats = BotSettings().get_target_chats()
        if new_target_id not in [chat.id for chat in target_chats]:
            send_reply_message(event.message.peer_id, "Целевой чат не найден", event.message.conversation_message_id)
            return

        chat_to_remove = [target_chat for target_chat in target_chats if target_chat.id == new_target_id][0]
        target_chats.remove(chat_to_remove)
        BotSettings().set_target_chats(target_chats)
        send_reply_message(event.message.peer_id, "Целевой чат удален", event.message.conversation_message_id)

