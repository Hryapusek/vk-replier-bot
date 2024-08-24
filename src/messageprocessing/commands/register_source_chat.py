
from result import *
from bottypes.types import Chat
from messageprocessing.commands.utils import extract_string_argument, is_user_blocked
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
    
    if BotSettings().get_source_chat():
        return Err("Source чат уже задан")

    return Err("Вы должны быть godlike")

class RegisterSourceChatCommand(ICommand):
    USAGE_STRING = 'Использование: /register_source_chat ["{title}"]'
    def handle(self, event: VkBotMessageEvent) -> None:
        result = check_if_user_allowed(event)
        if result.is_err():
            send_reply_message(event.message.peer_id, result.err(), event.message.conversation_message_id)
            return

        args = event.message.text.strip().splitline()[0].split()

        title = ""

        if len(args) > 1:
            title_result = extract_string_argument(" ".join(args[2:]))
            if not title_result.is_ok():
                send_reply_message(event.message.peer_id, title_result.err() + '\n' + __class__.USAGE_STRING, event.message.conversation_message_id)
                return
            title = title_result.ok_value

        BotSettings().set_source_chat(Chat(id=None, title=title, vk_chat_peer_id=event.message.peer_id))
        if not title:
            send_reply_message(event.message.peer_id, "Source добавлен", event.message.conversation_message_id)
        else:
            send_reply_message(event.message.peer_id, "Source добавлен: " + title, event.message.conversation_message_id)
