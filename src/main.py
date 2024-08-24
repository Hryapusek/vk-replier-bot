import asyncio
import threading
from exceptions import ExitException, ReloadException
import settings.constants
import traceback

from vk_api.bot_longpoll import VkBotEventType, VkBotLongPoll
from messageprocessing.message_dispatcher import MessageDispatcher
from loguru import logger
from settings.bot_settings import BotSettings
from vk_session import get_session

def print_settings():
    logger.info("Group ID: %s" % (BotSettings().get_group_id(),))
    logger.info("Source chat: %s" % (BotSettings().get_source_chat(),))
    logger.info("Target chats: %s" % (BotSettings().get_target_chats(),))
    logger.info("Godlike IDs: %s" % (BotSettings().get_godlike_ids(),))
    logger.info("Token: %s" % (BotSettings().get_token(),))
    logger.info("API version: %s" % (BotSettings().get_api_version(),))
    logger.info("Moderators IDs: %s" % (BotSettings().get_moderators_ids(),))

    logger.info("Blocked IDs: %s" % (BotSettings().get_blocked_user_ids(),))
    

def main():
    # Initialize config file
    BotSettings(settings.constants.SETTINGS_FILE_NAME)
    print_settings()
    dispatcher = MessageDispatcher()

    # Creating loop to put tasks in here
    loop = asyncio.new_event_loop()
    thread = threading.Thread(target=loop.run_forever)
    thread.setDaemon(True)
    thread.start()
    while True:
        vk_session = get_session()
        longpoll = VkBotLongPoll(vk_session, BotSettings().get_group_id())
        try:
            for event in longpoll.listen():
                if event.type == VkBotEventType.MESSAGE_NEW:
                    loop.call_soon_threadsafe(loop.create_task, dispatcher.handle(event))
        except ExitException:
            logger.info("Exiting")
            break
        except ReloadException:
            logger.info("Reloading settings")
            BotSettings(settings.constants.SETTINGS_FILE_NAME)
        except Exception as e:
            logger.warning("Exception while polling messages: \n%s", traceback.format_exc())

    loop.call_soon_threadsafe(loop.stop)

if __name__ == "__main__":
    main()
