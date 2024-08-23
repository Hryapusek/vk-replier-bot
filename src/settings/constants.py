from typing import Literal
import pydantic
import enum

class Mode(enum.Enum):
    WORK = "work"
    CONFIGURATION = "configuration"

class ConfigKeys(pydantic.BaseModel):
    mode: Literal["mode"] = "mode"
    token: Literal["token"] = "token"
    api_version: Literal["api_version"] = "api_version"
    secret_string: Literal["secret_string"] = "secret_string"
    port: Literal["port"] = "port"
    group_id: Literal["group_id"] = "group_id"
    base_url: Literal["base_url"] = "base_url"
    target_chats: Literal["target_chats"] = "target_chats"
    source_caht: Literal["source_chat"] = "source_chat"
    godlike_ids: Literal["godlike_ids"] = "godlike_ids"
