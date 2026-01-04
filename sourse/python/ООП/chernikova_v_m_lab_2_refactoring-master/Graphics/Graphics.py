import pygame

from Graphics.InterfaceGraphics import InterfaceGraphics
from Graphics.Figure import Figure
from Field.InterfaceField import InterfaceField
from TextFigure.TextFigure import TextFigure

# assign colors
LIGHTGRAY = (225, 225, 225)
DARKGRAY = (160, 160, 160)
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
RED = (255, 0, 0)
BLUE = (0, 0, 255)
GREEN = (0, 128, 0)
GREEN_WIN = (0, 255, 0)

# set up major colors
BGCOLOR = WHITE
FIELDCOLOR = BLACK
BOXCOLOR_COV = DARKGRAY  # covered box color
BOXCOLOR_REV = LIGHTGRAY  # revealed box color
MINECOLOR = BLACK
TEXTCOLOR_1 = BLUE
TEXTCOLOR_2 = RED
TEXTCOLOR_3 = BLACK
HILITECOLOR = GREEN
BUTBGCOLOR = LIGHTGRAY
MINEMARK_COV = RED

# set up font
FONTTYPE = 'Courier New'
FONTSIZE = 20

class Graphics(InterfaceGraphics):
    FPS = 30

    box_size = 30
    gap_size = box_size // 6
    XMARGIN = 0
    YMARGIN = 0


    def __init__(self,field_width:int, field_height:int,window_width_param:int, window_height_param:int):
        self.window_width = window_width_param
        self.window_height = window_height_param
        self.XMARGIN = int((self.window_width - (field_width * (self.box_size + self.gap_size))) / 2)
        self.YMARGIN = int((self.window_height - (field_height * (self.box_size + self.gap_size))) / 4)

        pygame.init()
        self.FPSCLOCK = pygame.time.Clock()
        self.DISPLAYSURFACE = pygame.display.set_mode((self.window_width, self.window_height))
        self.BASICFONT = pygame.font.SysFont(FONTTYPE, FONTSIZE)
        self.tipFont = pygame.font.SysFont(FONTTYPE, 16)

        assert self.box_size ^ 2 * (
                field_height * field_width) < self.window_height * self.window_width, 'Boxes will not fit on screen'
        assert self.box_size / 2 > 5, 'Bounding errors when drawing rectangle, cannot use half-5 in draw_mines_numbers'
        pygame.display.set_caption('Minesweeper')
        self.DISPLAYSURFACE.fill(BGCOLOR)

    def draw_field(self, d_f:InterfaceField):
        # draws field GUI and reset button
        for box_x in range(d_f.field_width):
            for box_y in range(d_f.field_height):
                left, top = self.get_left_top_xy(box_x, box_y)
                pygame.draw.rect(self.DISPLAYSURFACE, BOXCOLOR_REV, (left, top, self.box_size, self.box_size))

    def draw_covers(self, dr_c_field: InterfaceField):
        # draw red cover instead of gray cover over marked mines
        for box_x in range(dr_c_field.field_width):
            for box_y in range(dr_c_field.field_height):
                if not dr_c_field.revealedBoxes[box_x][box_y]:
                    left, top = self.get_left_top_xy(box_x, box_y)
                    if [box_x, box_y] in dr_c_field.markedMines:
                        pygame.draw.rect(self.DISPLAYSURFACE, MINEMARK_COV, (left, top, self.box_size, self.box_size))
                    else:
                        pygame.draw.rect(self.DISPLAYSURFACE, BOXCOLOR_COV, (left, top, self.box_size, self.box_size))

    def get_left_top_xy(self, box_x, box_y):
        # get left & top coordinates for drawing mine boxes
        left = self.XMARGIN + box_x * (self.box_size + self.gap_size)
        top = self.YMARGIN + box_y * (self.box_size + self.gap_size)
        return left, top

    def _get_center_xy(self, box_x, box_y):
        # get center coordinates for drawing mine boxes
        center_x = self.XMARGIN + self.box_size / 2 + box_x * (self.box_size + self.gap_size)
        center_y = self.YMARGIN + self.box_size / 2 + box_y * (self.box_size + self.gap_size)
        return center_x, center_y

    def highlight_box(self, box_x, box_y):
        # highlight box when mouse covers over it
        left, top = self.get_left_top_xy(box_x, box_y)
        pygame.draw.rect(self.DISPLAYSURFACE, HILITECOLOR, (left, top, self.box_size, self.box_size), 4)

    def highlight_button(self, but):
        # highlight button when mouse covers over it
        linewidth = 4
        pygame.draw.rect(self.DISPLAYSURFACE, HILITECOLOR, (
            but.RECT.left - linewidth, but.RECT.top - linewidth, but.RECT.width + 2 * linewidth,
            but.RECT.height + 2 * linewidth),
                         linewidth)

    def draw_mines_numbers(self, dr_field: InterfaceField):
        # draws mines and numbers onto GUI
        # field should have mines and numbers
        half = int(self.box_size * 0.5)
        quarter = int(self.box_size * 0.25)
        eighth = int(self.box_size * 0.125)
        for box_x in range(dr_field.field_width):
            for box_y in range(dr_field.field_height):
                left, top = self.get_left_top_xy(box_x, box_y)
                center_x, center_y = self._get_center_xy(box_x, box_y)
                if dr_field.mineField[box_x][box_y] == '[X]':
                    pygame.draw.circle(self.DISPLAYSURFACE, MINECOLOR, (left + half, top + half), quarter)
                    pygame.draw.circle(self.DISPLAYSURFACE, WHITE, (left + half, top + half), eighth)
                    pygame.draw.line(self.DISPLAYSURFACE, MINECOLOR, (left + eighth, top + half),
                                     (left + half + quarter + eighth, top + half))
                    pygame.draw.line(self.DISPLAYSURFACE, MINECOLOR, (left + half, top + eighth),
                                     (left + half, top + half + quarter + eighth))
                    pygame.draw.line(self.DISPLAYSURFACE, MINECOLOR, (left + quarter, top + quarter),
                                     (left + half + quarter, top + half + quarter))
                    pygame.draw.line(self.DISPLAYSURFACE, MINECOLOR, (left + quarter, top + half + quarter),
                                     (left + half + quarter, top + quarter))
                else:
                    for i in range(1, 9):
                        if dr_field.mineField[box_x][box_y] == '[' + str(i) + ']':
                            if i in range(1, 3):
                                text_color = TEXTCOLOR_1
                            else:
                                text_color = TEXTCOLOR_2
                            oi = TextFigure(str(i))
                            oi.draw(self.BASICFONT, text_color, LIGHTGRAY, center_x, center_y,
                                    self.DISPLAYSURFACE)  # !!!

    def update(self):
        # redraw screen, wait clock tick
        pygame.display.update()
        self.FPSCLOCK.tick(self.FPS)

    def game_over_animation(self, field_anim: InterfaceField, result: str):
        # makes background flash red (loss) or blue (win)
        orig_surf = self.DISPLAYSURFACE.copy()
        flash_surf = pygame.Surface(self.DISPLAYSURFACE.get_size())
        flash_surf = flash_surf.convert_alpha()
        animation_speed = 15
        if result == 'WIN':
            r, g, b = GREEN_WIN
        else:
            r, g, b = RED
        for i in range(3):
            for start, end, step in ((0, 255, 1), (255, 0, -1)):
                for alpha in range(start, end, animation_speed * step):  # animation loop
                    flash_surf.fill((r, g, b, alpha))
                    self.DISPLAYSURFACE.blit(orig_surf, (0, 0))
                    self.DISPLAYSURFACE.blit(flash_surf, (0, 0))
                    pygame.draw.rect(self.DISPLAYSURFACE, FIELDCOLOR, (
                        self.XMARGIN - 5, self.YMARGIN - 5, (self.box_size + self.gap_size) * field_anim.field_width + 5,
                        (self.box_size + self.gap_size) * field_anim.field_height + 5))
                    self.draw_field(field_anim)
                    self.draw_mines_numbers(field_anim)
                    self.draw_covers(field_anim)
                    self.update()

    def terminate_graphic(self):
        # simple function to exit game
        pygame.quit()

    def draw_all(self, field_draw: InterfaceField, but1: Figure, but2: Figure, tip1: Figure, tip2: Figure):

        self.DISPLAYSURFACE.fill(BGCOLOR)
        # границы клеток
        pygame.draw.rect(self.DISPLAYSURFACE, FIELDCOLOR, (
            self.XMARGIN - 5, self.YMARGIN - 5, (self.box_size + self.gap_size) * field_draw.field_width + 5,
            (self.box_size + self.gap_size) * field_draw.field_height + 5))
        self.draw_field(field_draw)
        but1.draw(self.BASICFONT, TEXTCOLOR_3, BUTBGCOLOR, self.window_width / 2,
                  self.window_height - 120, self.DISPLAYSURFACE)
        but2.draw(self.BASICFONT, TEXTCOLOR_3, BUTBGCOLOR, self.window_width / 2,
                  self.window_height - 95, self.DISPLAYSURFACE)
        self.draw_mines_numbers(field_draw)
        # draw covers
        self.draw_covers(field_draw)
        tip1.draw(self.tipFont, TEXTCOLOR_3, BGCOLOR, self.window_width / 2, self.window_height - 60, self.DISPLAYSURFACE)
        tip2.draw(self.tipFont, TEXTCOLOR_3, BGCOLOR, self.window_width / 2, self.window_height - 40, self.DISPLAYSURFACE)