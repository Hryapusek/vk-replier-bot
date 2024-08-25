
import os
from result import *

from settings.bot_settings import BotSettings

def extract_string_argument(text: str) -> Result[str, str]:
    if len(text) < 2:
        return Err("Неправильный аргумент")
    if text[0] != '"':
        return Err("Текст должен быть в кавычках")
    right = text.find('"', 1)
    if right == -1:
        return Err("Текст должен быть в кавычках")
    return Ok(text[1:right])

def is_user_blocked(user_id: int) -> Result[None, str]:
    if user_id in BotSettings().get_blocked_user_ids():
        return Err("Пользователь заблокирован")
    return Ok(None)

def read_from_file(filename: str) -> str:
    if not os.path.exists(filename):
        return ""
    with open(filename, "r") as f:
        return f.read()
