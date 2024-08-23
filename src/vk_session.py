from vk_api.vk_api import VkApi

from settings.bot_settings import BotSettings

def get_session() -> VkApi:
    return VkApi(token=BotSettings().get_token(), api_version=BotSettings().get_api_version())
