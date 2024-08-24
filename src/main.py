import asyncio
import threading
import settings.constants
import traceback

from vk_api.bot_longpoll import VkBotEventType, VkBotLongPoll
from messageprocessing.message_dispatcher import MessageDispatcher
from bot_logger import logger

from settings.bot_settings import BotSettings
from vk_session import get_session

def print_settings():
    logger.info("Settings: %s" % (BotSettings()._config, ))
    

def main():
    # Initialize config file
    BotSettings(settings.constants.SETTINGS_FILE_NAME)
    print_settings()
    dispatcher = MessageDispatcher()

    # Creating loop to put tasks in here
    loop = asyncio.new_event_loop()
    thread = threading.Thread(target=loop.run_forever)
    thread.daemon = True
    thread.start()
    tasks = []
    while True:
        vk_session = get_session()
        longpoll = VkBotLongPoll(vk_session, BotSettings().get_group_id())
        try:
            for event in longpoll.listen():
                if event.type == VkBotEventType.MESSAGE_NEW:
                    tasks.append(loop.call_soon_threadsafe(loop.create_task, dispatcher.handle(event)))
        except Exception as e:
            logger.warning("Exception while polling messages: \n%s", traceback.format_exc())

if __name__ == "__main__":
    main()
