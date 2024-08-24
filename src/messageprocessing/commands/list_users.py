
from messageprocessing.commands.utils import is_user_blocked
from settings.bot_settings import BotSettings
from vkservice.vk_service import send_reply_message
from .i_command import ICommand
from vk_api.bot_longpoll import VkBotMessageEvent

BASE_URL = "https://vk.com/id"

class ListUsersCommand(ICommand):
    def handle(self, event: VkBotMessageEvent) -> None:
        result = is_user_blocked(event.message.from_id)
        if result.is_err():
            send_reply_message(event.message.peer_id, result.err(), event.message.conversation_message_id)
            return
        response_message = ""
        response_message += "Main godlike: " + BASE_URL + str(BotSettings().get_main_godlike_id()) + "\n"
        response_message += "------------\n"

        for godlike_id in BotSettings().get_godlike_ids():
            response_message += "Godlike: " + BASE_URL + str(godlike_id) + "\n"

        response_message += "------------\n"

        for sender_id in BotSettings().get_sender_ids():
            response_message += "Sender: " + BASE_URL + str(sender_id) + "\n"

        response_message += "------------\n"

        for moderator_id in BotSettings().get_moderator_ids():
            response_message += "Moderator: " + BASE_URL + str(moderator_id) + "\n"

        response_message += "------------\n"

        send_reply_message(event.message.peer_id, response_message, event.message.conversation_message_id)
        
