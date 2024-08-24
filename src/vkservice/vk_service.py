
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

def send_reply_message(peer_id: int, message: str) -> bool:
    vk_session = get_session()
    result = vk_session.get_api().messages.send(peer_id=peer_id, message=message, random_id=0)
    return result
