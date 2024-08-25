from messageprocessing.commands.i_command import ICommand
from .add_moderator import AddModeratorCommand
from .administration import AdministrationCommand
from .block_user import BlockUserCommand
from .delete_chat_by_id import DeleteChatByIdCommand
from .delete_chat_by_vk_id import DeleteChatByVkIdCommand
from .delete_current_chat import DeleteCurrentChatCommand
from .delete_godlike import DeleteGodlikeCommand
from .delete_moderator import DeleteModeratorCommand
from .delete_sender import DeleteSenderCommand
from .exit import ExitCommand
from .get_chat_id import GetChatIdCommand
from .get_config import GetConfigCommand
from .get_vk_chat_id import GetVkChatIdCommand
from .help import HelpCommand
from .bot_info import BotInfoCommand
from .list_chats import ListChatsCommand
from .list_users import ListUsersCommand
from .register_current_chat import RegisterCurrentChatCommand
from .register_godlike import RegisterGodlikeCommand
from .register_sender import RegisterSenderCommand
from .reload_settings import ReloadSettingsCommand
from .send_with_all import SendWithAllCommand
from .send import SendCommand
from .set_bot_info import SetBotInfoCommand
from .set_administration import SetAdministrationCommand
from .set_administration_status import SetAdministrationStatusCommand

from .gym import GymCommand
from .set_gym import SetGymCommand
from .set_gym_status import SetGymStatusCommand

from .laundry import LaundryCommand
from .set_laundry import SetLaundryCommand
from .set_laundry_status import SetLaundryStatusCommand

from .linen import LinenCommand
from .set_linen import SetLinenCommand
from .set_linen_status import SetLinenStatusCommand

from .rules import RulesCommand
from .set_rules import SetRulesCommand

from .pledge import PledgeCommand
from .set_pledge import SetPledgeCommand

from .lyrics import LyricsCommand
from .set_lyrics import SetLyricsCommand

ALL_COMMANDS = [
    AddModeratorCommand(
        "add_moderator",
"""Добавить модератора. Примеры: 
/add_moderator 20
/add_moderator @hryapusek""",
        "Доступно для godlike",
        "Использование: /add_moderator <id|tag>",
        ["am"]
    ),

    AdministrationCommand(
        "администрация",
        "Получить информацию об администрации",
        "Доступно для всех",
        "Использование: /administration",
        ["ad"]
    ),

    BlockUserCommand(
        "block_user",
"""Заблокировать пользователя. Примеры:
/block_user 20
/block_user @hryapusek""",
        "Доступно для godlike",
        "Использование: /block_user <id|tag>",
        ["bu"]
    ),

    DeleteChatByIdCommand(
        "delete_chat_by_id",
"""Удалить чат по ID. Примеры:
/delete_chat_by_id 20""",
        "Доступно для godlike",
        "Использование: /delete_chat_by_id <id>",
        ["dtbi"]
    ),

    DeleteChatByVkIdCommand(
        "delete_chat_by_vk_id",
"""Удалить чат по ID. Примеры:
/delete_chat_by_vk_id 2000000123""",
        "Доступно для godlike",
        "Использование: /delete_chat_by_vk_id <id>",
        ["dtbvi"]
    ),

    DeleteCurrentChatCommand(
        "delete_current_chat",
"""Удалить текущий чат""",
        "Доступно для godlike, модераторов и админу беседы",
        "Использование: /delete_current_chat",
        ["dct"]
    ),

    DeleteGodlikeCommand(
        "delete_godlike",
"""Удалить godlike. Примеры:
/delete_godlike 175640282
/delete_godlike @hryapusek""",
        "Доступно для main_godlike",
        "Использование: /delete_godlike <id>",
        ["dgg"]
    ),

    DeleteModeratorCommand(
        "delete_moderator",
"""Удалить модератора. Примеры:
/delete_moderator 175640282
/delete_moderator @hryapusek""",
        "Доступно для main_godlike",
        "Использование: /delete_moderator <id>",
        ["dm"]
    ),

    DeleteSenderCommand(
        "delete_sender",
"""Удалить отправителя. Примеры:
/delete_sender 175640282
/delete_sender @hryapusek""",
        "Доступно для godlike",
        "Использование: /delete_sender <id>",
        ["ds"]
    ),

    ExitCommand(
        "exit",
        "Завершение работы бота",
        "Доступно для main_godlike",
        "Использование: /exit",
        []
    ),

    GetChatIdCommand(
        "get_chat_id",
        "Получить ID чата",
        "Доступно для всех",
        "Использование: /get_chat_id",
        []
    ),

    GetConfigCommand(
        "get_config",
        "Получить конфигурацию",
        "Доступно только для создателя и только в лс",
        "Использование: /get_config",
        []
    ),

    GetVkChatIdCommand(
        "get_vk_chat_id",
        "Получить ID VK чата",
        "Доступно для всех",
        "Использование: /get_vk_chat_id",
        []
    ),

    GymCommand(
        "зал",
        "Информация о зале",
        "Доступно для всех",
        "Использование: /зал",
        []
    ),

    LaundryCommand(
        "прачка",
        "Информация о прачке",
        "Доступно для всех",
        "Использование: /прачка",
        []
    ),

    LinenCommand(
        "кастелянша",
        "Информация о кастелянше",
        "Доступно для всех",
        "Использование: /кастелянша",
        []
    ),

    BotInfoCommand(
        "bot_info",
        "Информация о боте",
        "Доступно для всех",
        "Использование: /bot_info",
        []
    ),

    ListChatsCommand(
        "list_chats",
        "Список чатов",
        "Доступно для всех",
        "Использование: /list_chats",
        []
    ),

    ListUsersCommand(
        "list_users",
        "Список пользователей",
        "Доступно для всех",
        "Использование: /list_users",
        []
    ),

    PledgeCommand(
        "первак",
        "Информация для перваков",
        "Доступно для всех",
        "Использование: /первак",
        []
    ),


    RegisterGodlikeCommand(
        "register_godlike",
"""Регистрация godlike. Примеры:
/register_godlike 175640282
/register_godlike @hryapusek""",
        "Доступно для main_godlike",
        "Использование: /register_godlike <id>",
        ["rgg"]
    ),

    RegisterCurrentChatCommand(
        "register_current_chat",
"""Регистрация чата. Примеры:
/register_current_chat 23 "23 квартира"
/register_current_chat 23""",
        "Доступно для godlike и модераторов",
        "Использование: /register_current_chat <id>",
        ["rc"]
    ),

    RegisterSenderCommand(
        "register_sender",
"""Регистрация отправителя. Примеры:
/register_sender 175640282
/register_sender @hryapusek""",
        "Доступно для godlike",
        "Использование: /register_sender <id>",
        ["rs"]
    ),

    ReloadSettingsCommand(
        "reload_settings",
        "Перезагрузка настроек",
        "Доступно для main_godlike",
        "Использование: /reload_settings",
        []
    ),

    RulesCommand(
        "правила",
        "Правила бота",
        "Доступно для всех",
        "Использование: /правила",
        []
    ),

    SendCommand(
        "send",
        """Отправить сообщение всем пользователям. Примеры:
/send "Хватит ссать в раковины"
Ребята ну это просто какой то кошмар. Сколько можно. Не понимаю... Блокирую

/всем "Отключение горячей воды"
Отключение горячей воды с 5 числа по 50 число""",
        "Доступно для godlike и sender",
        'Использование: /send ["заголовок"]\n'
        '"текст"\n',
        ["всем"]
    ),

    SendWithAllCommand(
        "sendtagged",
        """Отправить сообщение всем пользователям с пометкой all. Примеры:
/sendtagged "Хватит ссать в раковины"
Ребята ну это просто какой то кошмар. Сколько можно. Не понимаю... Блокирую

/всемважно "Отключение горячей воды"
Отключение горячей воды с 5 числа по 50 число""",
        "Доступно для godlike и sender",
        'Использование: /send_with_all ["заголовок"]\n'
        '"текст"\n',
        ["всемважно"]
    ),

    SetBotInfoCommand(
        "set_bot_info",
        "Сохранить информацию о боте",
        "Доступно только для создателя и только в лс",
        "Использование: /set_bot_info\n"
        '"текст"\n',
        ["si"]
    ),

    SetPledgeCommand(
        "set_первак",
        "Сохранить информацию о перваке",
        "Доступно только для создателя и только в лс",
        "Использование: /set_первак\n"
        '"текст"\n',
        ["sp"]
    ),

    SetAdministrationCommand(
        "set_администрация",
        "Сохранить информацию об администрации",
        "Доступно только для создателя и godlike",
        "Использование: /set_администрация\n"
        '"текст"\n',
        ["sa"]
    ),

    SetAdministrationStatusCommand(
        "set_администрация_статус",
        "Сохранить информацию об статусе администрации",
        "Доступно только для всех",
        "Использование: /set_администрация_статус\n"
        '"текст"\n',
        ["ss"]
    ),

    SetGymCommand(
        "set_зал",
        "Сохранить информацию о зале",
        "Доступно только для создателя и godlike",
        "Использование: /set_зал\n"
        '"текст"\n',
        ["sg"]
    ),
    
    SetGymStatusCommand(
        "set_зал_статус",
        "Сохранить информацию о статусе зала",
        "Доступно только для всех",
        "Использование: /set_зал_статус\n"
        '"текст"\n',
        ["ssg"]
    ),

    SetLinenCommand(
        "set_кастелянша",
        "Сохранить информацию о кастелянше",
        "Доступно только для создателя и godlike",
        "Использование: /set_кастелянша\n"
        '"текст"\n',
        ["sl"]
    ),

    SetLinenStatusCommand(
        "set_кастелянша_статус",
        "Сохранить информацию о статусе кастелянши",
        "Доступно только для всех",
        "Использование: /set_кастелянша_статус\n"
        '"текст"\n',
        ["ss"]
    ),

    SetLaundryCommand(
        "set_прачка",
        "Сохранить информацию о прачечной",
        "Доступно только для создателя и godlike",
        "Использование: /set_прачка\n"
        '"текст"\n',
        ["slg"]
    ),

    SetRulesCommand(
        "set_правила",
        "Сохранить информацию о правилах",
        "Доступно только для создателя и godlike",
        "Использование: /set_правила\n"
        '"текст"\n',
        ["sr"]
    ),

    SetLaundryStatusCommand(
        "set_прачка_статус",
        "Сохранить информацию о статусе прачечной",
        "Доступно только для всех",
        "Использование: /set_прачка_статус\n"
        '"текст"\n',
        ["ssl"]
    ),

    LyricsCommand(
        "lyrics",
        "Получить текст песен",
        "Доступно для всех",
        "Использование: /lyrics\n"
        '"текст"\n',
        [],
    ),

    SetLyricsCommand(
        "set_lyrics",
        "Сохранить текст песен",
        "Доступно только для создателя и godlike",
        "Использование: /set_lyrics\n"
        '"текст"\n',
        ["sl"]
    )
]

def get_help_text(ALL_COMMANDS: list[ICommand]) -> str:
    help_text = ""
    for command in ALL_COMMANDS:
        help_text += f"/{command.command_name}\n"
        help_text += f"{command.description}\n"
        help_text += f"{command.roles}\n"
        help_text += f"{command.usage}\n"
        help_text += "---------\n"

    return help_text

ALL_COMMANDS.append(
    HelpCommand(
        "help",
        "Справка по командам",
        "Доступно для всех",
        "Использование: /help",
        [],
        get_help_text(ALL_COMMANDS)
    )
)
