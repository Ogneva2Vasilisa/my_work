from Controller.State import State
from CursesWrapper import CursesWrapper
from Model.Model import *


class IController(ABC):
    main: IModel

    def __init__(self, model:Model):
        self.main=model
        pass

    @abstractmethod
    def change_mode(self, mode: State):
        pass

    @abstractmethod
    def main_f(self, curses_wrapper1 : CursesWrapper):
        pass