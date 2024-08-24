from typing import Literal
import pydantic
import enum

SETTINGS_FILE_NAME = "config.json"

class Mode(enum.Enum):
    WORK = "work"
    CONFIGURATION = "configuration"

class ConfigKeys(pydantic.BaseModel):
    token: Literal["token"] = "token"
    api_version: Literal["api_version"] = "api_version"
    group_id: Literal["group_id"] = "group_id"
    target_chats: Literal["target_chats"] = "target_chats"
    source_chat: Literal["source_chat"] = "source_chat"
    godlike_ids: Literal["godlike_ids"] = "godlike_ids"
    moderator_ids: Literal["moderator_ids"] = "moderator_ids"
    blocked_user_ids: Literal["blocked_user_ids"] = "blocked_user_ids"
    main_godlike_id: Literal["main_godlike_id"] = "main_godlike_id"
