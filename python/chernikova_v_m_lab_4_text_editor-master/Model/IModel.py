from abc import ABC, abstractmethod

from Model.Buffer import TextMy
from Model.Cursour import Cursor
from Viewer.Viewer import IViewer

class IModel(ABC):
    cursor: Cursor
    text: TextMy
    file_name:str

    def __init__(self):
        pass

    @abstractmethod
    def execute_command(self, *args):
        pass

    @abstractmethod
    def build_cmd_list(self, viewer: IViewer):
        pass
