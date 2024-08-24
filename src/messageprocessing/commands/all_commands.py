from messageprocessing.commands.i_command import ICommand
from .add_moderator import AddModeratorCommand
from .block_user import BlockUserCommand
from .delete_chat_by_id import DeleteChatByIdCommand
from .delete_chat_by_vk_id import DeleteChatByVkIdCommand
from .delete_current_chat import DeleteCurrentChatCommand
from .delete_godlike import DeleteGodlikeCommand
from .delete_moderator import DeleteModeratorCommand
from .delete_sender import DeleteSenderCommand
from .exit import ExitCommand
from .get_chat_id import GetChatIdCommand
from .get_vk_chat_id import GetVkChatIdCommand
from .help import HelpCommand
from .info import InfoCommand
from .list_chats import ListChatsCommand
from .list_users import ListUsersCommand
from .register_current_chat import RegisterCurrentChatCommand
from .register_godlike import RegisterGodlikeCommand
from .register_sender import RegisterSenderCommand
from .reload_settings import ReloadSettingsCommand
from .send_with_all import SendWithAllCommand
from .send import SendCommand

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

    GetVkChatIdCommand(
        "get_vk_chat_id",
        "Получить ID VK чата",
        "Доступно для всех",
        "Использование: /get_vk_chat_id",
        []
    ),

    InfoCommand(
        "info",
        "Информация о боте",
        "Доступно для всех",
        "Использование: /info",
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
