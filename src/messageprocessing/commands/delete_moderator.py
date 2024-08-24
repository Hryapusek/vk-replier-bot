
from loguru import logger
from result import *

from messageprocessing.commands.utils import is_user_blocked
from settings.bot_settings import BotSettings
from vk_session import get_session
from vkservice.vk_service import get_user_id_by_user_argument, is_user_admin, is_user_admin_in_any_chat, send_reply_message
from .i_command import ICommand
from vk_api.bot_longpoll import VkBotMessageEvent

def check_if_user_allowed(event: VkBotMessageEvent, moderator_to_delete_id: int) -> Result[None, str]:
    result = is_user_blocked(event.message.from_id)
    if result.is_err():
        return Err(result.err())
    
    if event.message.from_id == moderator_to_delete_id:
        if moderator_to_delete_id in BotSettings().get_moderator_ids():
            return Ok(None)
        else:
            return Err("Модератор не найден")

    if event.message.from_id in BotSettings().get_godlike_ids():
        return Ok(None)

    return Err("Вы должны быть либо godlike, либо админом в какой-либо беседе")

class DeleteModeratorCommand(ICommand):
    def handle(self, event: VkBotMessageEvent) -> None:
        args = event.message.text.strip().splitlines()[0].split()
        if len(args) < 2:
            send_reply_message(event.message.peer_id, "Использование: /add_moderator {id}", event.message.conversation_message_id)
            return
        try:
            new_moderator_id = get_user_id_by_user_argument(args[1])
        except Exception:
            send_reply_message(event.message.peer_id, "Пользователь не найден", event.message.conversation_message_id)
            return
        
        moderator_to_delete_id = get_user_id_by_user_argument(args[1])
        result = check_if_user_allowed(event, moderator_to_delete_id)
        if not result.is_ok():
            logger.warning(result.failure)
            send_reply_message(event.message.peer_id, result.err(), event.message.conversation_message_id)
            return

        moderators = BotSettings().get_moderator_ids()
        if new_moderator_id not in moderators:
            send_reply_message(event.message.peer_id, "Модератор не найден", event.message.conversation_message_id)
            return
        moderators.remove(new_moderator_id)
        BotSettings().set_moderator_ids(moderators)
        send_reply_message(event.message.peer_id, "Модератор удален", event.message.conversation_message_id)
