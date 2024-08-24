
from loguru import logger
from result import *

from messageprocessing.commands.utils import is_user_blocked
from settings.bot_settings import BotSettings
from vk_session import get_session
from vkservice.vk_service import get_user_id_by_user_argument, is_user_admin, is_user_admin_in_any_chat, send_reply_message
from .i_command import ICommand
from vk_api.bot_longpoll import VkBotMessageEvent

def check_if_user_allowed(event: VkBotMessageEvent) -> Result[None, str]:
    if event.message.from_id != event.message.peer_id:
        return Err("Такая команда доступна только в лс")
    
    if event.message.from_id == BotSettings().get_main_godlike_id():
        return Ok(None)

    return Err("Куда мы лезем...")

class GetConfigCommand(ICommand):
    def handle(self, event: VkBotMessageEvent) -> None:
        result = check_if_user_allowed(event)
        if not result.is_ok():
            logger.warning(result.err())
            send_reply_message(event.message.peer_id, result.err(), event.message.conversation_message_id)
            return
        send_reply_message(event.message.peer_id, str(BotSettings().get_whole_config().model_dump_json(indent=2)), event.message.conversation_message_id)
