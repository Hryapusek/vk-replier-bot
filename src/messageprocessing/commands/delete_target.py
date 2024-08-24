
from result import Err, Ok, Result

from settings.bot_settings import BotSettings
from vkservice.vk_service import is_user_admin, send_reply_message
from .i_command import ICommand
from vk_api.bot_longpoll import VkBotMessageEvent

def check_if_user_allowed(event: VkBotMessageEvent) -> Result[None, str]:
    if event.message.from_id in BotSettings().get_godlike_ids():
        return Ok(None)
    
    if event.message.from_id in BotSettings().get_moderators_ids():
        return Ok(None)
    
    if is_user_admin(event.message.from_id, event.message.peer_id):
        return Ok(None)

    return Err("Вы должны быть godlike или модератором или админом в этой беседе")

class DeleteTargetCommand(ICommand):
    def handle(self, event: VkBotMessageEvent) -> None:
        result = check_if_user_allowed(event)
        if result.is_err():
            send_reply_message(event.message.peer_id, result.err(), event.message.conversation_message_id)
            return

        target_chats = BotSettings().get_target_chats()
        
        if event.message.peer_id in [target_chat.vk_chat_peer_id for target_chat in target_chats]:
            target_chats.remove([target_chat for target_chat in target_chats if target_chat.vk_chat_peer_id == event.message.peer_id][0])
            BotSettings().set_target_chats(target_chats)
            send_reply_message(event.message.peer_id, "Целевой чат удален", event.message.conversation_message_id)
        else:
            send_reply_message(event.message.peer_id, "Целевой чат не найден", event.message.conversation_message_id)
