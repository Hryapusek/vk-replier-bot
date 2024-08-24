from typing import Literal
import pydantic
import enum

SETTINGS_FILE_NAME = "config.json"

class ConfigKeys(pydantic.BaseModel):
    token: Literal["token"] = "token"
    api_version: Literal["api_version"] = "api_version"
    group_id: Literal["group_id"] = "group_id"
    chats: Literal["chats"] = "chats"
    godlike_ids: Literal["godlike_ids"] = "godlike_ids"
    moderator_ids: Literal["moderator_ids"] = "moderator_ids"
    blocked_user_ids: Literal["blocked_user_ids"] = "blocked_user_ids"
    main_godlike_id: Literal["main_godlike_id"] = "main_godlike_id"
    sender_ids: Literal["sender_ids"] = "sender_ids"