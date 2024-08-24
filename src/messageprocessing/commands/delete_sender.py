
from result import Err, Ok, Result

from messageprocessing.commands.utils import is_user_blocked
from settings.bot_settings import BotSettings
from vkservice.vk_service import get_user_id_by_user_argument, send_reply_message
from .i_command import ICommand
from vk_api.bot_longpoll import VkBotMessageEvent

def check_if_user_allowed(event: VkBotMessageEvent, sender_id: int) -> Result[None, str]:
    result = is_user_blocked(event.message.from_id)
    if result.is_err():
        return Err(result.err())
    
    if event.message.from_id in BotSettings().get_godlike_ids():
        return Ok(None)

    if sender_id == event.message.from_id:
        return Ok(None)

    return Err("Вы должны быть godlike")

class DeleteSenderCommand(ICommand):
    def handle(self, event: VkBotMessageEvent) -> None:
        args = event.message.text.strip().splitline()[0].split()

        if len(args) < 2:
            send_reply_message(event.message.peer_id, "Использование: /delete_sender {id}", event.message.conversation_message_id)
            return

        try:
            sender_id = get_user_id_by_user_argument(args[1])
        except Exception:
            send_reply_message(event.message.peer_id, "Пользователь не найден", event.message.conversation_message_id)
            return

        result = check_if_user_allowed(event, sender_id)
        if result.is_err():
            send_reply_message(event.message.peer_id, result.err(), event.message.conversation_message_id)
            return

        sender_ids = BotSettings().get_sender_ids()
        if sender_id not in sender_ids:
            send_reply_message(event.message.peer_id, "Sender не найден", event.message.conversation_message_id)
            return
        sender_ids.remove(sender_id)
        BotSettings().set_sender_ids(sender_ids)
        send_reply_message(event.message.peer_id, "Sender удален", event.message.conversation_message_id)
