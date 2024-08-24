
from result import *

from messageprocessing.commands.i_undoable import IUndoable
from messageprocessing.commands.utils import extract_string_argument, is_user_blocked
from settings.bot_settings import BotSettings
from vkservice.vk_service import SendMessageResponse, delete_message, send_reply_message
from .i_command import ICommand
from vk_api.bot_longpoll import VkBotMessageEvent
from vkservice.vk_service import forward_message_to_chats

def check_if_user_allowed(event: VkBotMessageEvent) -> Result[None, str]:
    result = is_user_blocked(event.message.from_id)
    if result.is_err():
        return Err(result.err())
    
    if event.message.from_id in BotSettings().get_godlike_ids():
        return Ok(None)

    if event.message.from_id in BotSettings().get_sender_ids():
        return Ok(None)

    return Err(
        "Вы должны быть godlike или sender. Обратитесь к godlike для получения дополнительных возможностей"
    )

class SendCommand(ICommand):
    def handle(self, event: VkBotMessageEvent) -> None:
        result = check_if_user_allowed(event)
        if result.is_err():
            send_reply_message(
                event.message.peer_id,
                result.err(),
                event.message.conversation_message_id,
            )
            return

        args = event.message.text.strip().splitlines()[0].split()

        title = ""

        if len(args) > 1:
            title_result = extract_string_argument(" ".join(args[1:]))
            if not title_result.is_ok():
                send_reply_message(
                    event.message.peer_id,
                    title_result.err() + "\n" + self.usage,
                    event.message.conversation_message_id,
                )
                return
            title = title_result.ok_value

        responses: list[SendMessageResponse] = forward_message_to_chats(
            [
                chat
                for chat in BotSettings().get_chats()
                if chat.vk_chat_peer_id != event.message.peer_id
            ],
            title,
            event.message.conversation_message_id,
            event.message.peer_id,
        )

        send_reply_message(
            event.message.peer_id,
            "Сообщение отправлено",
            event.message.conversation_message_id,
        )
        return SendCommandUndoable(responses)

class SendCommandUndoable(IUndoable):
    def __init__(self, responses: list[SendMessageResponse]) -> None:
        self.responses = responses
    def undo(self, event: VkBotMessageEvent):
        result = check_if_user_allowed(event)
        if result.is_err():
            send_reply_message(
                event.message.peer_id,
                result.err(),
                event.message.conversation_message_id,
            )
            return

        count = 0
        for response in self.responses:
            if delete_message(response.peer_id, response.conversation_message_id):
                count += 1

        send_reply_message(event.message.peer_id, "Было удалено {} сообщений".format(count), event.message.conversation_message_id)
