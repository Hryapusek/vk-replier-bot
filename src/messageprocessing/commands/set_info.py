
from result import *
from messageprocessing.commands.info import INFO_FILE
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

class SetInfoCommand(ICommand):
    def handle(self, event: VkBotMessageEvent) -> None:
        result = check_if_user_allowed(event)
        if result.is_err():
            send_reply_message(event.message.peer_id, result.err(), event.message.conversation_message_id)
            return

        text_to_save = event.message.text.splitlines()[1:]
        text_to_save = "\n".join(text_to_save)
        with open(INFO_FILE, "w") as f:
            f.write(text_to_save)
        send_reply_message(event.message.peer_id, "Сохранено", event.message.conversation_message_id)
