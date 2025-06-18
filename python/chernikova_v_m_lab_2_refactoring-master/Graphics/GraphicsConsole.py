from Graphics.InterfaceGraphics import *

class GraphicsConsole(InterfaceGraphics):

    def __init__(self):
        pass

    def game_over_animation(self, field_anim: InterfaceField, result: str):
        print(result)

    def draw_mines_numbers(self, dr_field: InterfaceField):
        pass

    def update(self):
        pass

    def draw_all(self, field_draw: InterfaceField, but1: Figure, but2: Figure, tip1: Figure, tip2: Figure):
        for i in range(field_draw.field_width):
            for j in range(field_draw.field_height):
                if [j, i] in field_draw.markedMines:
                    print('[M]', end=' ')
                elif field_draw.revealedBoxes[j][i]:
                    print(field_draw.mineField[j][i], end=' ')
                else:
                    print('[ ]', end=' ')
            print()
        print()

    def highlight_box(self, box_x, box_y):
        pass

    def terminate_graphic(self):
        pass

    def get_left_top_xy(self, box_x, box_y):
        pass