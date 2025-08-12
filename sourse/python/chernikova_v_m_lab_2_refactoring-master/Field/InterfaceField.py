
NAME = "InterfaceField"

import abc
from abc import abstractmethod

class InterfaceField(abc.ABC):
    mineField = []
    zeroListXY = []
    revealedBoxes = [[False]]  # видимость
    markedMines = []
    field_width: int
    field_height: int
    mines_count = 0

    @abstractmethod
    def set_up(self, mines_count:int, x: int, y: int):
        pass

    @abstractmethod
    def blank_field(self):
        pass

    @abstractmethod
    def show_mines(self):
        # modifies revealedBox data structure if chosen box_x & box_y is [X]
        pass

    @abstractmethod
    def blank_revealed_box_data(self, val):
        pass

    @abstractmethod
    def show_numbers(self, box_x, box_y, gr):
        pass