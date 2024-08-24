
from typing import Optional
from vk_session import get_session
from .constants import CHAT_ID_START


def is_user_admin(user_id: int, peer_id: int) -> bool:
    vk_session = get_session()
    result = vk_session.get_api().messages.getConversationMembers(peer_id=peer_id)
    admin_ids = [x.get('member_id') for x in result.get('items') if x.get('is_admin') == 1]
    return user_id in admin_ids

def is_user_admin_in_any_chat(user_id: int, peer_ids: int) -> bool:
    return any([is_user_admin(user_id, peer_id) for peer_id in peer_ids])

def get_chat_members_ids(peer_id: int) -> list:
    vk_session = get_session()
    result = vk_session.get_api().messages.getConversationMembers(peer_id=peer_id)
    return [x.get('member_id') for x in result.get('items')]

def send_reply_message(peer_id: int, message: str, reply_to_conversation_message_id: Optional[int] = None) -> list[dict]:
    vk_session = get_session()
    if reply_to_conversation_message_id is not None:
        result = vk_session.get_api().messages.send(peer_id=peer_id, 
                                                    message=message, 
                                                    random_id=0,
                                                    forward={
                                                        'conversation_message_ids': [reply_to_conversation_message_id],
                                                        'is_reply': 1,
                                                        'peer_id': peer_id
                                                    })
    else:
        result = vk_session.get_api().messages.send(peer_id=peer_id, message=message, random_id=0)
    return result

def get_user_id_by_user_argument(user_argument: str) -> int:
    if user_argument.isdigit():
        return int(user_argument)
    if user_argument.startswith('@'):
        user_argument = user_argument[1:]
    vk_session = get_session()
    result = vk_session.get_api().users.get(user_ids=[user_argument])
    return result[0].get('id')
