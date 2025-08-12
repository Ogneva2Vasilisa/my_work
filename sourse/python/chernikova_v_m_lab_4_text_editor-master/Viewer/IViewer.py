from abc import ABC, abstractmethod
class IViewer(ABC):
    def __init__(self):
        pass

    @abstractmethod
    def print_text_line(self, arr: list):
        pass

    @abstractmethod
    def print_command_line(self, arr: list):
        pass

    @abstractmethod
    def print_cursor(self, x: int, y: int, arr: list):
        pass

    @abstractmethod
    def clear_command_line(self):
        pass

    @abstractmethod
    def up_screen(self):  # здесь есть баг страницы
        pass

    @abstractmethod
    def down_screen(self):  # здесь есть баг страницы
        pass
