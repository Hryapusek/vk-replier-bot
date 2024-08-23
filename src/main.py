import asyncio
import vk_api
import settings.constants
import loguru
import traceback
from vk_api.bot_longpoll import VkBotEventType

_logger = loguru.Logger(__name__)

from settings.bot_settings import BotSettings

def main():
    # Initialize config file
    BotSettings(settings.constants.SETTINGS_FILE_NAME)

    # Creating loop to put tasks in here
    loop = asyncio.new_event_loop()

    while True:
        vk_session = vk_api.VkApi(token=BotSettings().get_token())
        vk = vk_session.get_api()
        longpoll = vk_api.VkBotLongPoll(vk_session, BotSettings().get_group_id())
        try:
            for event in longpoll.listen():
                if event.type == VkBotEventType.MESSAGE_NEW and event.from_user:
                    # TODO: loop.create_task()
                    pass
        except Exception as e:
            _logger.warning("Exception while polling messages: \n%s", traceback.format_exc())


if __name__ == "__main__":
    main()
