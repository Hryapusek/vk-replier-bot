
from result import *

from messageprocessing.commands.utils import extract_string_argument, is_user_blocked
from settings.bot_settings import BotSettings
from vkservice.vk_service import get_user_id_by_user_argument, send_reply_message
from .i_command import ICommand
from vk_api.bot_longpoll import VkBotMessageEvent
from vkservice.vk_service import forward_message_to_chats

def check_if_user_allowed(event: VkBotMessageEvent) -> Result[None, str]:
    result = is_user_blocked(event.message.from_id)
    if result.is_err():
        return Err(result.err())
    
    if event.message.from_id in BotSettings().get_godlike_ids():
        return Ok(None)

    return Err(
        "Вы должны быть godlike. Обратитесь к godlike для получения дополнительных возможностей"
    )

class RegisterSenderCommand(ICommand):
    USAGE_STRING = 'Использование: /register_sender {id}'

    def handle(self, event: VkBotMessageEvent) -> None:
        result = check_if_user_allowed(event)
        if result.is_err():
            send_reply_message(
                event.message.peer_id,
                result.err(),
                event.message.conversation_message_id,
            )
            return

        args = event.message.text.strip().splitline()[0].split()

        if len(args) < 2:
            send_reply_message(
                event.message.peer_id,
                "Неправильное использование команды. " + __class__.USAGE_STRING,
                event.message.conversation_message_id,
            )
            return

        try:
            sender_id = get_user_id_by_user_argument(args[1])
        except Exception:
            send_reply_message(
                event.message.peer_id,
                "Неправильное использование команды. " + __class__.USAGE_STRING,
                event.message.conversation_message_id,
            )
            return
        
        sender_ids = BotSettings().get_sender_ids()
        if sender_id in sender_ids:
            send_reply_message(
                event.message.peer_id,
                "Sender уже добавлен",
                event.message.conversation_message_id,
            )
            return

        sender_ids.append(sender_id)
        BotSettings().set_sender_ids(sender_ids)
        send_reply_message(
            event.message.peer_id,
            "Sender добавлен",
            event.message.conversation_message_id,
        )
