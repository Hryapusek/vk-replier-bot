
import json
from typing import Optional

from loguru import logger
import pydantic
from bottypes.types import Chat
from vk_session import get_session
from .constants import CHAT_ID_START
from vk_api.exceptions import ApiError


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

class Error(pydantic.BaseModel):
    code: Optional[int] = None
    description: Optional[str] = None

class SendMessageResponse(pydantic.BaseModel):
    class Config:
        arbitrary_types_allowed = True
    conversation_message_id: Optional[int] = None
    peer_id: Optional[int] = None
    message_id: Optional[int] = None
    error: Optional[Error] = None

def send_reply_message(peer_id: int, message: str, reply_to_conversation_message_id: Optional[int] = None) -> list[SendMessageResponse]:
    vk_session = get_session()
    if reply_to_conversation_message_id is not None:
        result = vk_session.get_api().messages.send(peer_ids=[peer_id], 
                                                    message=message, 
                                                    random_id=0,
                                                    forward=json.dumps({
                                                        'peer_id': peer_id,
                                                        'conversation_message_ids': [reply_to_conversation_message_id],
                                                    }))
    else:
        result = vk_session.get_api().messages.send(peer_ids=[peer_id], message=message, random_id=0)
    return SendMessageResponse(**(result[0]))

def get_user_id_by_user_argument(user_argument: str) -> int:
    if user_argument.isdigit():
        return int(user_argument)
    
    if user_argument.startswith('[') and '|' in user_argument:
        start = user_argument.find('|') + 1
        end = user_argument.find(']')
        return int(user_argument[3:start - 1])
    
    if user_argument.startswith('@'):
        user_argument = user_argument[1:]
    vk_session = get_session()
    result = vk_session.get_api().users.get(user_ids=[user_argument])
    return result[0].get('id')
    

def forward_message_to_chats(chats: list[Chat], message: str, forward_message_conversation_id: int, forward_message_peer_id: int) -> list[SendMessageResponse]:
    chat_packets: list[list[Chat]] = [chats[i:i+100] for i in range(0, len(chats), 100)]
    vk_session = get_session()
    responses: list[SendMessageResponse] = []
    for chat_packet in chat_packets:
        result = vk_session.get_api().messages.send(peer_ids=[x.vk_chat_peer_id for x in chat_packet], 
                                                    message=message, 
                                                    random_id=0,
                                                    forward=json.dumps({
                                                        'conversation_message_ids': [forward_message_conversation_id],
                                                        'peer_id': forward_message_peer_id,
                                                    }))
        for failed_chat in [x for x in result if 'error' in x]:
            logger.warning("Failed to forward message to chat {}".format(failed_chat))
        responses.extend([SendMessageResponse(**x) for x in result])
    return responses
    
def delete_message(peer_id: int, conversation_message_id: int) -> bool:
    vk_session = get_session()
    try:
        vk_session.get_api().messages.delete(peer_id=peer_id, cmids=[conversation_message_id], delete_for_all=1)
        return True
    except ApiError as e:
        logger.exception(e)
        return False
