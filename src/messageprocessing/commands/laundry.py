
from messageprocessing.commands.set_laundry import LAUNDRY_FILE
from messageprocessing.commands.set_laundry_status import LAUNDRY_STATUS_FILE
from messageprocessing.commands.utils import is_user_blocked, read_from_file
from vkservice.vk_service import send_reply_message
from .i_command import ICommand
from vk_api.bot_longpoll import VkBotMessageEvent

class LaundryCommand(ICommand):
    def handle(self, event: VkBotMessageEvent) -> None:
        result = is_user_blocked(event.message.from_id)
        if result.is_err():
            send_reply_message(event.message.peer_id, result.err(), event.message.conversation_message_id)
            return
        
        response_message = read_from_file(LAUNDRY_FILE)
        response_message += "\n\nТекущий статус: "
        response_message += read_from_file(LAUNDRY_STATUS_FILE)

        send_reply_message(event.message.peer_id, response_message, event.message.conversation_message_id)
