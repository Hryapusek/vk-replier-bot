
from settings.bot_settings import BotSettings
from vkservice.vk_service import send_reply_message
from .i_command import ICommand
from vk_api.bot_longpoll import VkBotMessageEvent

class GetChatIdCommand(ICommand):
    def handle(self, event: VkBotMessageEvent) -> None:
        result = list(filter(BotSettings().get_target_chats(), lambda target_chat: target_chat.vk_chat_peer_id == event.message.peer_id))
        if result:
            send_reply_message(event.message.peer_id, str(result[0].id), event.message.conversation_message_id)
            return

        source_chat = BotSettings().get_source_chat()

        if source_chat and source_chat.vk_chat_peer_id == event.message.peer_id:
            send_reply_message(event.message.peer_id, str(source_chat.id), event.message.conversation_message_id)
            return

        send_reply_message(event.message.peer_id, "Данный чат не найден", event.message.conversation_message_id)
