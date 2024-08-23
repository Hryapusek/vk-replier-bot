from typing import Optional
import pydantic
import json
import threading

from settings.constants import ConfigKeys, Mode
from bot_types.types import Chat


class _ConfigValues(pydantic.BaseModel):
    class Config:
        arbitrary_types_allowed = True
    mode: Mode
    token: str
    api_version: str
    group_id: int
    target_chats: list[Chat]
    source_chat: Optional[Chat]
    godlike_ids: list[int]

class BotSettings:
    _instance = None
    _lock = threading.Lock()

    def __new__(cls, *args, **kwargs):
        if not cls._instance:
            with cls._lock:
                if not cls._instance:  # Double-checked locking
                    cls._instance = super().__new__(cls)
        return cls._instance

    def __init__(self, file_path=None):
        # Prevent reinitialization during multiple instantiations
        if not hasattr(self, "_initialized"):
            self.file_path = file_path
            self._lock = threading.Lock()
            self._config: _ConfigValues = self._load_config()
            self._initialized = True  # Mark the instance as initialized

    def _load_config(self) -> _ConfigValues:
        with open(self.file_path, 'r') as f:
            raw_json = json.load(f)
        target_chats = [Chat(**chat) for chat in raw_json[ConfigKeys().target_chats]]
        source_chat = Chat(**raw_json[ConfigKeys().source_caht]) if raw_json[ConfigKeys().source_caht] else None
        return _ConfigValues(
            mode=Mode(raw_json[ConfigKeys().mode]),
            token=raw_json[ConfigKeys().token],
            group_id=raw_json[ConfigKeys().group_id],
            target_chats=target_chats,
            api_version=raw_json[ConfigKeys().api_version],
            source_chat=source_chat,
            godlike_ids=raw_json[ConfigKeys().godlike_ids]
        )

    def _save_config(self):
        with open(self.file_path, 'w') as f:
            f.write(json.dumps(self._config.model_dump(), indent=2))
    
    def get_mode(self) -> Mode:
        with self._lock:
            return self._config.mode
        
    def set_mode(self, mode: Mode) -> None:
        with self._lock:
            self._config.mode = mode
            self._save_config()
        
    def get_token(self) -> str:
        with self._lock:
            return self._config.token

    def set_token(self, token: str) -> None:
        with self._lock:
            self._config.token = token
            self._save_config()

    def get_api_version(self) -> str:
        with self._lock:
            return self._config.api_version

    def set_api_version(self, api_version: str) -> None:
        with self._lock:
            self._config.api_version = api_version
            self._save_config()

    def get_group_id(self) -> int:
        with self._lock:
            return self._config.group_id

    def set_group_id(self, group_id: int) -> None:
        with self._lock:
            self._config.group_id = group_id
            self._save_config()

    def get_target_chats(self) -> list[Chat]:
        with self._lock:
            return self._config.target_chats

    def set_target_chats(self, target_chats: list[Chat]) -> None:
        with self._lock:
            self._config.target_chats = target_chats
            self._save_config()

    def get_source_chat(self) -> Optional[Chat]:
        with self._lock:
            return self._config.source_chat

    def set_source_chat(self, source_chat: Chat) -> None:
        with self._lock:
            self._config.source_chat = source_chat
            self._save_config()

    def get_godlike_ids(self) -> list[int]:
        with self._lock:
            return self._config.godlike_ids

    def set_godlike_ids(self, godlike_ids: list[int]) -> None:
        with self._lock:
            self._config.godlike_ids = godlike_ids
            self._save_config()
