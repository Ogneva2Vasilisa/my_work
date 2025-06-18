from Graphics.Figure import *

class TextFigure(Figure):
    Text = ""
    def __init__(self, name:str):
        self.Text= name

    def draw(self, font, color, bg_color, center_x, center_y, surface):
        self.SURF = font.render(self.Text, True, color, bg_color)
        self.RECT = self.SURF.get_rect()
        self.RECT.centerx = center_x
        self.RECT.centery = center_y
        surface.blit(self.SURF, self.RECT)
        return self.SURF, self.RECT