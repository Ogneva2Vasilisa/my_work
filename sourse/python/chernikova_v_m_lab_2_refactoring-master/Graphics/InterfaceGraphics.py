from Field.InterfaceField import *
from Graphics.Figure import *

class InterfaceGraphics(abc.ABC):
    @abstractmethod
    def update(self):
        pass

    @abstractmethod
    def game_over_animation(self, field_anim: InterfaceField, result: str):
        pass

    @abstractmethod
    def draw_mines_numbers(self, dr_field: InterfaceField):
        pass

    @abstractmethod
    def highlight_box(self, box_x, box_y):
        pass

    @abstractmethod
    def terminate_graphic(self):
        pass

    @abstractmethod
    def draw_all(self, field_draw: InterfaceField, but1: Figure, but2: Figure, tip1: Figure, tip2: Figure):
        pass

    @abstractmethod
    def get_left_top_xy(self, box_x, box_y):
        pass