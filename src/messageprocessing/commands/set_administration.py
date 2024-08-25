
import datetime
from result import *
from messageprocessing.commands.utils import is_user_blocked
from settings.bot_settings import BotSettings
from vkservice.vk_service import BASE_USER_URL, get_user_fullname, send_reply_message
from .i_command import ICommand
from vk_api.bot_longpoll import VkBotMessageEvent

ADMINISTRATION_FILE = "administration.txt"

def check_if_user_allowed(event: VkBotMessageEvent) -> Result[None, str]:
    result = is_user_blocked(event.message.from_id)
    if result.is_err():
        return Err(result.err())

    if event.message.from_id in BotSettings().get_godlike_ids():
        return Ok(None)

    return Err("Вы должны быть godlike")

class SetAdministrationCommand(ICommand):
    def handle(self, event: VkBotMessageEvent) -> None:
        result = check_if_user_allowed(event)
        if result.is_err():
            send_reply_message(event.message.peer_id, result.err(), event.message.conversation_message_id)
            return

        text_to_save = event.message.text.splitlines()[1:]
        text_to_save = "\n".join(text_to_save)
        text_to_save = text_to_save[:5000]
        text_to_save += "\nОбновлено: " + str(datetime.datetime.now())
        text_to_save += "\nАвтор: " + BASE_USER_URL + str(event.message.from_id) + " " + get_user_fullname(event.message.from_id)

        with open(ADMINISTRATION_FILE, "w") as f:
            f.write(text_to_save)
        send_reply_message(event.message.peer_id, "Сохранено", event.message.conversation_message_id)
