
from result import *

def extract_string_argument(text: str) -> Result[str, str]:
    if len(text) < 2:
        return Err("Неправильный аргумент")
    if text[0] != '"':
        return Err("Текст должен быть в кавычках")
    right = text.find('"', 1)
    if right == -1:
        return Err("Текст должен быть в кавычках")
    return Ok(text[1:right])
