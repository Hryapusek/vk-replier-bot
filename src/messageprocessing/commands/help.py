from __future__ import annotations
from messageprocessing.commands.i_undoable import IUndoable
from vk_session import get_session
from vkservice.vk_service import is_user_admin
from .i_command import ICommand
from vk_api.bot_longpoll import VkBotMessageEvent

class HelpCommand(ICommand):
    def handle(self, event: VkBotMessageEvent) -> HelpCommandUndoable:
        help_text ="""
/help - показывает этот текст
/change_mode - меняет режим работы бота (godlike only)
/delete_source - удалить текущую беседу из списка источников (godlike or admin only)
/delete_target_by_id {id} - удалить беседу по id (godlike only)
/delete_target - удалить текущую беседу из списка получателей (godlike or admin only)
/exit - завершение работы бота (godlike only)
/info - информация о боте (anybody)
/register_source "title" - регистрация текущей беседы как источника (godlike or admin only)
/register_target "title" [id] - регистрация текущей беседы как получателя (godlike or admin only)
/undo - отмена последней команды (godlike or admin only)
_всем_ - рассылка сообщений (anybody)
_всем_важно_ - рассылка сообщений с all (anybody)
"""
        vk_session = get_session()
        result = vk_session.get_api().messages.send(peer_ids=[event.message.peer_id], message=help_text, random_id=0)
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
