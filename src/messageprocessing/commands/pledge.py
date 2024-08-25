
from messageprocessing.commands.set_pledge import PLEDGE_FILE
from messageprocessing.commands.utils import is_user_blocked, read_from_file
from vkservice.vk_service import get_user_fullname, send_reply_message
from .i_command import ICommand
from vk_api.bot_longpoll import VkBotMessageEvent

class PledgeCommand(ICommand):
    def handle(self, event: VkBotMessageEvent) -> None:
        result = is_user_blocked(event.message.from_id)
        if result.is_err():
            send_reply_message(event.message.peer_id, result.err(), event.message.conversation_message_id)
            return

        response_message = read_from_file(PLEDGE_FILE)

        send_reply_message(event.message.peer_id, response_message, event.message.conversation_message_id)
