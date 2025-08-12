
from Events.InterfaceEvents import *
import parse

class EventsConsole(InterfaceEvent):
    reset = False
    show = False
    def __init__(self):
        pass

    def handle(self, h_field: InterfaceField, gr: InterfaceGraphics):
        self.dig_box = False
        self.flag_box = False
        self.reset = False
        self.show = False
        str_input = input()

        if str_input[:4] == "dig ":
            r = parse.parse("dig {i} {j}", str_input)
            self.dig_box = True
            try:
                self.current_x, self.current_y = int(r["i"]), int(r["j"])
            except:
                print("wrong command")
        elif str_input[:5] == "flag ":
            r = parse.parse("flag {i} {j}", str_input)
            self.flag_box = True
            try:
                self.current_x, self.current_y = int(r["i"]), int(r["j"])
            except:
                print("wrong command")
        elif str_input[:5] == "reset":
            self.reset = True
            self.current_x, self.current_y = None, None
        elif str_input[:4] == "show":
            self.show = True
            self.current_x, self.current_y = None, None
        elif str_input[:4] == "exit":
            sys.exit()

    def check_but(self, ch_b_field: InterfaceField, but:Button, current_x, current_y):
        if self.reset & (but.Text=="RESET"):
            but.action(ch_b_field, self.current_x, self.current_y)
        elif self.show & (but.Text=="Show all"):
            but.action(ch_b_field, self.current_x, self.current_y)

    def get_box(self, g_b_field: InterfaceField, current_x, current_y, gr: InterfaceGraphics):
        return self.current_x, self.current_y
        pass