from typing import Optional
import pydantic
import json
import threading

from settings.constants import ConfigKeys, Mode
from types.types import *

class __ConfigValues(pydantic.BaseModel):
    mode: Mode
    token: str
    api_version: str
    secret_string: str
    port: int
    group_id: int
    base_url: str
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

    def __init__(self, file_path):
        # Prevent reinitialization during multiple instantiations
        if not hasattr(self, "_initialized"):
            self.file_path = file_path
            self._lock = threading.Lock()
            self._config: __ConfigValues = self._load_config()
            self._initialized = True  # Mark the instance as initialized

    def _load_config(self) -> __ConfigValues:
        with open(self.file_path, 'r') as f:
            raw_json = json.load(f)
        target_chats = [Chat(**chat) for chat in raw_json[ConfigKeys.target_chats]]
        source_chat = Chat(**raw_json[ConfigKeys.source_caht]) if raw_json[ConfigKeys.source_caht] else None
        return __ConfigValues(
            mode=Mode(raw_json[ConfigKeys.mode]),
            token=raw_json[ConfigKeys.token],
            api_version=raw_json[ConfigKeys.api_version],
            secret_string=raw_json[ConfigKeys.secret_string],
            port=raw_json[ConfigKeys.port],
            group_id=raw_json[ConfigKeys.group_id],
            base_url=raw_json[ConfigKeys.base_url],
            target_chats=target_chats,
            source_chat=source_chat,
            godlike_ids=raw_json[ConfigKeys.godlike_ids]
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

    def get_secret_string(self) -> str:
        with self._lock:
            return self._config.secret_string

    def set_secret_string(self, secret_string: str) -> None:
        with self._lock:
            self._config.secret_string = secret_string
            self._save_config()

    def get_port(self) -> int:
        with self._lock:
            return self._config.port

    def set_port(self, port: int) -> None:
        with self._lock:
            self._config.port = port
            self._save_config()

    def get_group_id(self) -> int:
        with self._lock:
            return self._config.group_id

    def set_group_id(self, group_id: int) -> None:
        with self._lock:
            self._config.group_id = group_id
            self._save_config()

    def get_base_url(self) -> str:
        with self._lock:
            return self._config.base_url

    def set_base_url(self, base_url: str) -> None:
        with self._lock:
            self._config.base_url = base_url
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
