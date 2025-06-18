from Buttons.Button import *
from Events.InterfaceEvents import *

class MyGame:

    def __init__(self, gr: InterfaceGraphics, ev: InterfaceEvent, play_field: InterfaceField, but_res: Button,
                 but_show: Button,
                 tip1: TextFigure, tip2: TextFigure):
        self.gr = gr
        self.ev = ev
        self.play_field = play_field
        self.but_res = but_res
        self.but_show = but_show
        self.tip1 = tip1
        self.tip2 = tip2

        pass

    def play(self):
        self.gr.draw_all(self.play_field, self.but_res, self.but_show, self.tip1, self.tip2)
        self.gr.update()
        while True:


            self.ev.handle(self.play_field, self.gr)

            box_x, box_y = self.ev.get_box(self.play_field, self.ev.current_x, self.ev.current_y, self.gr)

            # mouse not over a box in play_field
            if (box_x, box_y) == (None, None):
                self.ev.check_but(self.play_field, self.but_res, self.ev.current_x, self.ev.current_y)
                self.ev.check_but(self.play_field, self.but_show, self.ev.current_x, self.ev.current_y)

            # mouse currently over box in play_field
            else:
                # highlight unrevealed box
                if not self.play_field.revealedBoxes[box_x][box_y]:
                    self.gr.highlight_box(box_x, box_y)
                    # mark mines
                    if self.ev.flag_box:
                        self.play_field.markedMines.append([box_x, box_y])
                    # reveal clicked boxes
                    if self.ev.dig_box:
                        self.play_field.revealedBoxes[box_x][box_y] = True
                        # when 0 is revealed, show relevant boxes
                        if self.play_field.mineField[box_x][box_y] == '[0]':
                            self.play_field.show_numbers(box_x, box_y, self.gr)
                        # when mine is revealed, show mines
                        if self.play_field.mineField[box_x][box_y] == '[X]':
                            self.play_field.show_mines()
                            self.gr.game_over_animation(self.play_field, 'LOSS')
                            self.play_field.set_up(self.play_field.mines_count, self.ev.current_x, self.ev.current_y)

            self.gr.draw_all(self.play_field, self.but_res, self.but_show, self.tip1, self.tip2)
            self.gr.update()
            # check if player has won
            if self.check_win(self.play_field):
                self.gr.game_over_animation(self.play_field, 'WIN')
                self.play_field.set_up(self.play_field.mines_count, self.ev.current_x, self.ev.current_y)
            break
        pass

    @staticmethod
    def check_win(ch_field: InterfaceField):
        # check if player has revealed all boxes
        mine_count = 0
        for box_x in range(ch_field.field_width):
            for box_y in range(ch_field.field_height):
                if ch_field.revealedBoxes[box_x][box_y]:
                    if ch_field.mineField[box_x][box_y] != '[X]':
                        mine_count += 1
        if mine_count >= (ch_field.field_width * ch_field.field_height) - ch_field.mines_count:
            return True
        else:
            return False