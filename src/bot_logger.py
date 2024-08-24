from loguru import logger as _logger

_logger.add("log_file.log", format="{time:YYYY-MM-DD at HH:mm:ss} | {level} | {message}", level="DEBUG")

logger = _logger
