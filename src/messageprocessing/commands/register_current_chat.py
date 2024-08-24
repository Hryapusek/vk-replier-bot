
from result import *

from bottypes.types import Chat
from messageprocessing.commands.utils import extract_string_argument, is_user_blocked
from settings.bot_settings import BotSettings
from .i_command import ICommand
from vk_api.bot_longpoll import VkBotMessageEvent
from vkservice.vk_service import is_user_admin, send_reply_message

def check_if_user_allowed(event: VkBotMessageEvent) -> Result[None, str]:
    result = is_user_blocked(event.message.from_id)
    if result.is_err():
        return Err(result.err())
    
    if event.message.from_id in BotSettings().get_godlike_ids():
        return Ok(None)
    
    if event.message.from_id in BotSettings().get_moderator_ids():
        return Ok(None)
    
    if is_user_admin(event.message.from_id, event.message.peer_id):
        return Err("Вам нужно обратиться к godlike или модератору для повышения вашего статуса")

    return Err("Вы должны быть main_godlike")

class RegisterCurrentChatCommand(ICommand):
    def handle(self, event: VkBotMessageEvent) -> None:
        result = check_if_user_allowed(event)
        if result.is_err():
            send_reply_message(event.message.peer_id, result.err(), event.message.conversation_message_id)
            return

        args = event.message.text.strip().splitlines()[0].split()
        if len(args) < 2:
            send_reply_message(event.message.peer_id, self.usage, event.message.conversation_message_id)
            return
        
        try:
            new_chat_id = int(args[1])
        except Exception:
            send_reply_message(event.message.peer_id, "Неправильный ID", event.message.conversation_message_id)
            return
        
        if event.message.peer_id in [x.vk_chat_peer_id for x in BotSettings().get_chats()]:
            send_reply_message(event.message.peer_id, "Такой чат уже есть", event.message.conversation_message_id)
            return
        
        if new_chat_id in [x.id for x in BotSettings().get_chats()]:
            send_reply_message(event.message.peer_id, "Чат с таким ID уже есть", event.message.conversation_message_id)
            return

        title = ""
        if len(args) > 2:
            extracted_title = extract_string_argument(" ".join(args[2:]))
            if not extracted_title.is_ok():
                send_reply_message(event.message.peer_id, extracted_title.err() + '\n' + self.usage, event.message.conversation_message_id)
                return
            title = extracted_title.ok_value
        
        
        chats = BotSettings().get_chats()
        chats.append(Chat(id=new_chat_id, title=title, vk_chat_peer_id=event.message.peer_id))
        BotSettings().set_chats(chats)
        if not title:
            send_reply_message(event.message.peer_id, "Чат добавлен", event.message.conversation_message_id)
        else:
            send_reply_message(event.message.peer_id, "Чат добавлен: " + title, event.message.conversation_message_id)
