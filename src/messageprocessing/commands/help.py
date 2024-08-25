from __future__ import annotations
from messageprocessing.commands.i_undoable import IUndoable
from messageprocessing.commands.utils import is_user_blocked
from vk_session import get_session
from vkservice.vk_service import is_user_admin, send_reply_message
from .i_command import ICommand
from vk_api.bot_longpoll import VkBotMessageEvent

class HelpCommand(ICommand):
    def __init__(self, command_name: str, description: str, usage: str, shortcuts: list[str], roles: str, help_text: str) -> None:
        super().__init__(command_name, description, usage, shortcuts, roles)
        self.help_text: str = help_text

    def handle(self, event: VkBotMessageEvent) -> HelpCommandUndoable:
        result = is_user_blocked(event.message.from_id)
        if result.is_err():
            send_reply_message(event.message.peer_id, result.err(), event.message.conversation_message_id)
            return
        
        result = send_reply_message(event.message.peer_id, self.help_text[:len(self.help_text)//2], event.message.conversation_message_id)
        result = send_reply_message(event.message.peer_id, self.help_text[len(self.help_text)//2:], event.message.conversation_message_id)
        return HelpCommandUndoable(event.message.peer_id, result[0].get('conversation_message_id'))
    
class HelpCommandUndoable(IUndoable):
    def __init__(self, peer_id: int, conversation_message_id: int) -> None:
        super().__init__()
        self.peer_id = peer_id
        self.conversation_message_id = conversation_message_id

    def undo(self, event: VkBotMessageEvent) -> bool:
        vk_session = get_session()
        vk_session.get_api().messages.delete(peer_id=self.peer_id, cmids=[self.conversation_message_id], delete_for_all=1)
        return True
