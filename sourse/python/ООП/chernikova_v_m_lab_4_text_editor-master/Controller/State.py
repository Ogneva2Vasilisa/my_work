from abc import ABC, abstractmethod
from CursesWrapper import *
class State(ABC):
    def __init__(self):
        pass

    @abstractmethod
    def set_symbol(self, symbol:chr):
        '''
        Забирает символ пользователя и возвращается список команд и их аргументы
        :return: [[name_cmd, *arg],...]
        '''
        pass

from enum import Enum, auto
class MyState(Enum):
    EDITOR_MODE = auto()
    COMMAND_MODE = auto()
    FIND_MODE = auto()
    NAVIGATION_MODE = auto()
    HELP_MODE = auto()

