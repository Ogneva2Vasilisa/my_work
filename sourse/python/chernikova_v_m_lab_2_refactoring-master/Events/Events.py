
from Events.InterfaceEvents import *

import pygame
from pygame import *

class Events(InterfaceEvent):

    def __init__(self):
        pass

    def handle(self, h_field: InterfaceField, gr: InterfaceGraphics):
        self.dig_box = False
        self.flag_box = False
        # event handling loop
        for event in pygame.event.get():
            if event.type == QUIT or (event.type == KEYUP and event.key == K_ESCAPE):
                gr.terminate_graphic()
                sys.exit()
            elif event.type == MOUSEMOTION:
                self.current_x, self.current_y = event.pos
                return False
            elif event.type == MOUSEBUTTONDOWN:
                if event.button == 1:
                    self.dig_box = True
                    return True
                elif event.button == 3:
                    self.flag_box = True
                    return True

    def check_but(self, ch_b_field: InterfaceField, but: Button, current_x, current_y):
        if but.RECT.collidepoint(current_x, current_y):
            # gr.highlight_button(but)
            if self.dig_box:
                but.action(ch_b_field, self.current_x, self.current_y)

    # жоская графика, но это все через интерфейс))
    def get_box(self, g_b_field: InterfaceField, current_x, current_y, gr: InterfaceGraphics):
        # gets coordinates of box at mouse coordinates
        for box_x in range(g_b_field.field_width):
            for box_y in range(g_b_field.field_height):
                left, top = gr.get_left_top_xy(box_x, box_y)
                box_rect = pygame.Rect(left, top, gr.box_size, gr.box_size)
                if box_rect.collidepoint(current_x, current_y):
                    return box_x, box_y
        return None, None