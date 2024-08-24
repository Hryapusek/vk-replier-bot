from result import *

from messageprocessing.commands.send import SendCommandUndoable
from messageprocessing.commands.utils import extract_string_argument, is_user_blocked
from settings.bot_settings import BotSettings
from vkservice.vk_service import send_reply_message
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


class SendWithAllCommand(ICommand):
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

        title = "@all"

        if len(args) > 1:
            title_result = extract_string_argument(" ".join(args[1:]))
            if not title_result.is_ok():
                send_reply_message(
                    event.message.peer_id,
                    title_result.err() + "\n" + self.usage,
                    event.message.conversation_message_id,
                )
                return
            title += ", " + title_result.ok_value

        responses = forward_message_to_chats(
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
