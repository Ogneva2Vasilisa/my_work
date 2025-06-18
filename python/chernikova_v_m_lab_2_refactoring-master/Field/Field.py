from random import *

from Graphics.InterfaceGraphics import *


class Field(InterfaceField):

    def __init__(self, mines_count: int, field_width_param,field_height_param):
        self.mines_count=mines_count
        self.field_width=field_width_param
        self.field_height=field_height_param
        self.mineField = []
        self.zeroListXY = []
        self.revealedBoxes = [[False]]  # видимость
        self.markedMines = []


        self.blank_field()
        self.place_mines(mines_count,0, 0)
        self.place_numbers()
        self.blank_revealed_box_data(False)
        pass

    def set_up(self, mines_count: int, x: int, y: int):
        self.mines_count = mines_count
        self.mineField = []
        self.zeroListXY = []
        self.revealedBoxes = [[False]]  # видимость
        self.markedMines = []
        self.blank_field()
        self.place_mines(mines_count, x, y)
        self.place_numbers()
        self.blank_revealed_box_data(False)
        pass

    def blank_field(self):
        # creates blank field_width x field_height data structure
        self.mineField = []
        for x in range(self.field_width):
            self.mineField.append([])
            for y in range(self.field_height):
                self.mineField[x].append('[ ]')

    def place_mines(self, mines_count: int, x_param: int, y_param: int):
        # places mines in field_width x field_height data structure
        # requires blank field as input
        mine_count = 0
        xy = []
        while mine_count < mines_count:
            x = randint(0, self.field_width - 1)
            y = randint(0, self.field_height - 1)
            if not ([x, y] == [x_param, y_param]):
                xy.append([x, y])
                if xy.count([x, y]) > 1:
                    xy.remove([x, y])
                else:
                    self.mineField[x][y] = '[X]'
                    mine_count += 1

    def place_numbers(self):
        # places numbers in field_width x field_height data structure
        # requires field with mines as input
        for x in range(self.field_width):
            for y in range(self.field_height):
                if not self.is_there_mine(x, y):
                    count = 0
                    if x != 0:
                        if self.is_there_mine(x - 1, y):
                            count += 1
                        if y != 0:
                            if self.is_there_mine(x - 1, y - 1):
                                count += 1
                        if y != self.field_height - 1:
                            if self.is_there_mine(x - 1, y + 1):
                                count += 1
                    if x != self.field_width - 1:
                        if self.is_there_mine(x + 1, y):
                            count += 1
                        if y != 0:
                            if self.is_there_mine(x + 1, y - 1):
                                count += 1
                        if y != self.field_height - 1:
                            if self.is_there_mine(x + 1, y + 1):
                                count += 1
                    if y != 0:
                        if self.is_there_mine(x, y - 1):
                            count += 1
                    if y != self.field_height - 1:
                        if self.is_there_mine(x, y + 1):
                            count += 1
                    self.mineField[x][y] = '[%s]' % count

    def blank_revealed_box_data(self, val):
        # returns field_width x field_height data structure different from the field data structure
        # each item in data structure is boolean (val) to show whether box at those field_width & fieldheight coordinates should be revealed
        self.revealedBoxes = []
        for i in range(self.field_width):
            self.revealedBoxes.append([val] * self.field_height)

    def show_mines(self):
        # modifies revealedBox data strucure if chosen box_x & box_y is [X]
        for i in range(self.field_width):
            for j in range(self.field_height):
                if self.mineField[i][j] == '[X]':
                    self.revealedBoxes[i][j] = True

    def is_there_mine(self, x, y):
        # checks if mine is located at specific box on field
        return self.mineField[x][y] == '[X]'

    @staticmethod
    def reveal_adjacent_boxes(self, revealed_boxes, box_x: int, box_y: int):
        # modifies revealedBoxes data structure so that all adjacent boxes to (box_x, box_y) are set to True
        if box_x != 0:
            revealed_boxes[box_x - 1][box_y] = True
            if box_y != 0:
                revealed_boxes[box_x - 1][box_y - 1] = True
            if box_y != self.field_height - 1:
                revealed_boxes[box_x - 1][box_y + 1] = True
        if box_x != self.field_width - 1:
            revealed_boxes[box_x + 1][box_y] = True
            if box_y != 0:
                revealed_boxes[box_x + 1][box_y - 1] = True
            if box_y != self.field_height - 1:
                revealed_boxes[box_x + 1][box_y + 1] = True
        if box_y != 0:
            revealed_boxes[box_x][box_y - 1] = True
        if box_y != self.field_height - 1:
            revealed_boxes[box_x][box_y + 1] = True

    @staticmethod
    def get_adjacent_boxes_xy(self, box_x: int, box_y: int):
        # get box XY coordinates for all adjacent boxes to (box_x, box_y)
        adjacent_boxes_xy = []
        if box_x != 0:
            adjacent_boxes_xy.append([box_x - 1, box_y])
            if box_y != 0:
                adjacent_boxes_xy.append([box_x - 1, box_y - 1])
            if box_y != self.field_height - 1:
                adjacent_boxes_xy.append([box_x - 1, box_y + 1])
        if box_x != self.field_width - 1:
            adjacent_boxes_xy.append([box_x + 1, box_y])
            if box_y != 0:
                adjacent_boxes_xy.append([box_x + 1, box_y - 1])
            if box_y != self.field_height - 1:
                adjacent_boxes_xy.append([box_x + 1, box_y + 1])
        if box_y != 0:
            adjacent_boxes_xy.append([box_x, box_y - 1])
        if box_y != self.field_height - 1:
            adjacent_boxes_xy.append([box_x, box_y + 1])
        return adjacent_boxes_xy

    def show_numbers(self, box_x: int, box_y: int, gr: InterfaceGraphics):
        # modifies revealedBox data strucure if chosen box_x & box_y is [0]
        # show all boxes using recursion
        self.revealedBoxes[box_x][box_y] = True
        self.reveal_adjacent_boxes(self, self.revealedBoxes, box_x, box_y)
        for i, j in self.get_adjacent_boxes_xy(self, box_x, box_y):
            if self.mineField[i][j] == '[0]' and [i, j] not in self.zeroListXY:
                self.zeroListXY.append([i, j])
                self.show_numbers(i, j, gr)