
from result import Err, Ok, Result

from settings.bot_settings import BotSettings
from vkservice.vk_service import send_reply_message
from .i_command import ICommand
from vk_api.bot_longpoll import VkBotMessageEvent

def check_if_user_allowed(event: VkBotMessageEvent) -> Result[None, str]:
    if event.message.from_id in BotSettings().get_godlike_ids():
        return Ok(None)

    source_chat = BotSettings().get_source_chat()

    if not source_chat:
        return Err("Source чат не задан")

    return Err("Вы должны быть godlike")

class DeleteSourceCommand(ICommand):
    def handle(self, event: VkBotMessageEvent) -> None:
        result = check_if_user_allowed(event)
        if result.is_err():
            send_reply_message(event.message.peer_id, result.err(), event.message.conversation_message_id)
            return
        BotSettings().set_source_chat(None)
        send_reply_message(event.message.peer_id, "Source чат удален", event.message.conversation_message_id)
