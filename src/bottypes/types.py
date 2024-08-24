from typing import Optional
import pydantic

class Chat(pydantic.BaseModel):
    vk_chat_peer_id: int
    id: Optional[int] = None # will be set manually
    title: Optional[str] = None
