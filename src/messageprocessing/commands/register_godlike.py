
from result import *
from messageprocessing.commands.utils import is_user_blocked
from settings.bot_settings import BotSettings
from vkservice.vk_service import get_user_id_by_user_argument, send_reply_message
from .i_command import ICommand
from vk_api.bot_longpoll import VkBotMessageEvent

def check_if_user_allowed(event: VkBotMessageEvent) -> Result[None, str]:
    result = is_user_blocked(event.message.from_id)
    if result.is_err():
        return Err(result.err())
    
    if event.message.from_id == BotSettings().get_main_godlike_id():
        return Ok(None)
    
    return Err("Вы должны быть main_godlike")

class RegisterGodlikeCommand(ICommand):
    def handle(self, event: VkBotMessageEvent) -> None:
        result = check_if_user_allowed(event)
        if result.is_err():
            send_reply_message(event.message.peer_id, result.err(), event.message.conversation_message_id)
            return
        args = event.message.text.strip().splitlines()[0].split()
        if len(args) < 2:
            send_reply_message(event.message.peer_id, "Использование: /register_godlike {id}", event.message.conversation_message_id)
            return
        
        try:
            new_godlike_id = get_user_id_by_user_argument(args[1])
        except Exception:
            send_reply_message(event.message.peer_id, "Неправильный ID", event.message.conversation_message_id)
            return
        
        if new_godlike_id in BotSettings().get_godlike_ids():
            send_reply_message(event.message.peer_id, "Такой godlike уже есть", event.message.conversation_message_id)
            return
        
        godlike_ids = BotSettings().get_godlike_ids()
        godlike_ids.append(new_godlike_id)
        BotSettings().set_godlike_ids(godlike_ids)
        send_reply_message(event.message.peer_id, "Godlike добавлен", event.message.conversation_message_id)
