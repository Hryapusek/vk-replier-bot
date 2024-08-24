
from loguru import logger
from result import *

from messageprocessing.commands.utils import is_user_blocked
from settings.bot_settings import BotSettings
from vk_session import get_session
from vkservice.vk_service import get_user_id_by_user_argument, is_user_admin, is_user_admin_in_any_chat, send_reply_message
from .i_command import ICommand
from vk_api.bot_longpoll import VkBotMessageEvent

def check_if_user_allowed(event: VkBotMessageEvent, godlike_to_delete: int) -> Result[None, str]:
    result = is_user_blocked(event.message.from_id)
    if result.is_err():
        return Err(result.err())
    
    if event.message.from_id == godlike_to_delete:
        if godlike_to_delete in BotSettings().get_godlike_ids():
            return Ok(None)
        else:
            return Err("godlike не найден")
    
    if event.message.from_id == BotSettings().get_main_godlike_id():
        return Ok(None)
    
    return Err("Вы должны быть main_godlike")

class DeleteGodlikeCommand(ICommand):
    def handle(self, event: VkBotMessageEvent) -> None:
        args = event.message.text.strip().splitlines()[0].split()
        if len(args) < 2:
            send_reply_message(event.message.peer_id, "Использование: /delete_godlike {id}", event.message.conversation_message_id)
            return
        try:
            godlike_to_delete_id = get_user_id_by_user_argument(args[1])
        except Exception:
            send_reply_message(event.message.peer_id, "Пользователь не найден", event.message.conversation_message_id)
            return
        
        result = check_if_user_allowed(event, godlike_to_delete_id)
        if not result.is_ok():
            logger.warning(result.failure)
            send_reply_message(event.message.peer_id, result.err(), event.message.conversation_message_id)
            return

        godlike_ids = BotSettings().get_godlike_ids()
        if godlike_to_delete_id not in godlike_ids:
            send_reply_message(event.message.peer_id, "Godlike не найден", event.message.conversation_message_id)
            return
        godlike_ids.remove(godlike_to_delete_id)
        BotSettings().set_godlike_ids(godlike_ids)
        send_reply_message(event.message.peer_id, "Godlike удален", event.message.conversation_message_id)
