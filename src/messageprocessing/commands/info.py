
from vkservice.vk_service import send_reply_message
from .i_command import ICommand
from vk_api.bot_longpoll import VkBotMessageEvent

INFO_FILE = "info.txt"

def read_from_file(filename: str) -> str:
    with open(filename, "r") as f:
        return f.read()

class InfoCommand(ICommand):
    def handle(self, event: VkBotMessageEvent) -> None:
        send_reply_message(event.message.peer_id, read_from_file(INFO_FILE), event.message.conversation_message_id)
