from Game.MyGame import *
from Graphics.Graphics import Graphics
from Graphics.GraphicsConsole import GraphicsConsole
from Field.Field import Field
from Events.Events import Events
from Events.EventsConsole import EventsConsole
from Buttons.Buttons_classes import *
from TextFigure.TextFigure import *


# change only this class
class MyDriver:
    gr: InterfaceGraphics
    graph_cons: InterfaceGraphics
    but_res: Button
    but_show: Button
    play_field: InterfaceField
    tip1: TextFigure
    tip2: TextFigure
    ev: InterfaceEvent
    event_cons: InterfaceEvent

    def __init__(self, field_width_param: int, field_height_param: int, mines_count_param: int,
                 fps_param=30, window_width_param=800, window_height_param=600):
        # assertion
        assert mines_count_param < field_height_param * field_width_param, 'More mines than boxes'

        self.gr = Graphics(field_width_param, field_height_param,window_width_param,window_height_param)
        self.graph_cons = GraphicsConsole()
        self.but_res = ButtonReset()
        self.but_show = ButtonShow()
        self.play_field = Field(mines_count_param, field_width_param, field_height_param)
        self.tip1 = TextFigure('Tip: Highlight a box and press space (rather than click the mouse)')
        self.tip2 = TextFigure('to mark areas that you think contain mines.')
        self.ev = Events()
        self.event_cons = EventsConsole()

        Graphics.FPS = fps_param
        Graphics.window_width = window_width_param
        Graphics.window_height = window_height_param

    def mine(self):
        # game = MyGame(self.gr, self.ev, self.play_field, self.but_res, self.but_show, self.tip1, self.tip2)
        game = MyGame(self.graph_cons, self.event_cons, self.play_field, self.but_res, self.but_show, self.tip1, self.tip2)

        game.play()
