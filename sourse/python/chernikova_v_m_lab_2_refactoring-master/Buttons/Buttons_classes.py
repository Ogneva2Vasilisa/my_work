from Buttons.Button import Button
from Field.InterfaceField import *
class ButtonReset(Button):

    def __init__(self):
        super().__init__()
        self.Text = "RESET"

    def action(self, a_field: InterfaceField, x_mouse: int, y_mouse: int):
        a_field.set_up(a_field.mines_count, x_mouse, y_mouse)


class ButtonShow(Button):

    def __init__(self):
        super().__init__()
        self.Text = "Show all"

    def action(self, a_field: InterfaceField, x_mouse: int, y_mouse: int):
        a_field.blank_revealed_box_data(True)