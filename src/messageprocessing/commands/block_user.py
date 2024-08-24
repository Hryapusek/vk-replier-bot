
from settings.bot_settings import BotSettings
from vk_session import get_session
from vkservice.vk_service import is_user_admin, is_user_admin_in_any_chat, send_reply_message
from .i_command import ICommand
from vk_api.bot_longpoll import VkBotMessageEvent
from result import *
from loguru import logger

def check_if_user_allowed(event: VkBotMessageEvent) -> Result[None, str]:
    if event.message.from_id in BotSettings().get_godlike_ids():
        return Ok(None)

    return Err("Вы должны быть godlike")

class BlockUserCommand(ICommand):
    def handle(self, event: VkBotMessageEvent) -> None:
        result = check_if_user_allowed(event)
        if result.is_err():
            send_reply_message(event.message.peer_id, result.err(), event.message.conversation_message_id)
            return

        blocked = BotSettings().get_blocked_user_ids()

        if event.message.from_id in blocked:
            blocked.remove(event.message.from_id)
            BotSettings().set_blocked_user_ids(blocked)
            send_reply_message(event.message.peer_id, "Пользователь разблокирован", event.message.conversation_message_id)
        else:
            blocked.append(event.message.from_id)
            BotSettings().set_blocked_user_ids(blocked)
            send_reply_message(event.message.peer_id, "Пользователь заблокирован", event.message.conversation_message_id)
