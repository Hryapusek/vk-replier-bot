
from settings.bot_settings import BotSettings
from settings.constants import Mode
from vk_session import get_session
from vkservice.vk_service import is_user_admin, is_user_admin_in_any_chat, send_reply_message
from .i_command import ICommand
from vk_api.bot_longpoll import VkBotMessageEvent
from result import *
from loguru import logger

def check_if_user_allowed(event: VkBotMessageEvent) -> Result[None, str]:
    if event.message.from_id in BotSettings().get_godlike_ids():
        return Ok(None)

    source_chat = BotSettings().get_source_chat()

    if source_chat and is_user_admin(event.message.from_id, source_chat.vk_chat_peer_id):
        return Ok(None)

    if is_user_admin_in_any_chat(event.message.from_id, BotSettings().get_target_chats()):
        return Ok(None)

    return Err("Вы должны быть либо godlike, либо админом в какой-либо беседе")

class ChangeModeCommand(ICommand):
    def handle(self, event: VkBotMessageEvent) -> None:
        result = check_if_user_allowed(event)
        if result.is_err():
            logger.warning(result.failure)
            send_reply_message(event.message.peer_id, result.failure)
            return
        logger.info("Changing mode")
        if BotSettings().get_mode() == Mode.WORK:
            BotSettings().set_mode(Mode.CONFIGURATION)
            logger.info("Mode changed to {}".format(Mode.CONFIGURATION.value))
        else:
            BotSettings().set_mode(Mode.WORK)
            logger.info("Mode changed to {}".format(Mode.WORK.value))
        vk_session = get_session()
        vk_session.get_api().messages.send(peer_id=event.message.peer_id, message="Режим работы изменен", random_id=0)
