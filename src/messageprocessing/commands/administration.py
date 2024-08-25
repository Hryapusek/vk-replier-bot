
from messageprocessing.commands.set_administration import ADMINISTRATION_FILE
from messageprocessing.commands.set_administration_status import ADMINISTRATION_STATUS_FILE
from messageprocessing.commands.utils import is_user_blocked, read_from_file
from vkservice.vk_service import get_user_fullname, send_reply_message
from .i_command import ICommand
from vk_api.bot_longpoll import VkBotMessageEvent

class AdministrationCommand(ICommand):
    def handle(self, event: VkBotMessageEvent) -> None:
        result = is_user_blocked(event.message.from_id)
        if result.is_err():
            send_reply_message(event.message.peer_id, result.err(), event.message.conversation_message_id)
            return

        response_message = read_from_file(ADMINISTRATION_FILE)
        response_message += "\n\nТекущий статус: "
        response_message += read_from_file(ADMINISTRATION_STATUS_FILE)

        send_reply_message(event.message.peer_id, response_message, event.message.conversation_message_id)
