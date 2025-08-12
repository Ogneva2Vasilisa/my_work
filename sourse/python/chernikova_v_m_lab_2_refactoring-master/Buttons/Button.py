from TextFigure.TextFigure import *
from Field.InterfaceField import *
class Button(TextFigure):

    def __init__(self):
        super().__init__("Button")
        pass

    @abstractmethod
    def action(self, b_field: InterfaceField, x_mouse: int, y_mouse: int):
        pass