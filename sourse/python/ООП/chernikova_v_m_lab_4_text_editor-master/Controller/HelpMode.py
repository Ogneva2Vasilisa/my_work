from Controller.State import *

class HelpMode(State):
    def __init__(self, my_state: type(MyState)):
        super().__init__()
        self.MyState = my_state
        print("HelpMode is inited")
        pass

    def set_symbol(self, symbol:int):
        for_return=[]
        print("set_symbol_h", symbol)
        if symbol == KEY_RIGHT:
            for_return.append(["MoveCursorRight"])
        elif symbol == KEY_LEFT:
            for_return.append(["MoveCursorLeft"])
        elif symbol == KEY_UP:
            for_return.append(["MoveCursorUp"])
        elif symbol == KEY_DOWN:
            for_return.append(["MoveCursorDown"])
        elif symbol == 27:
            for_return.append(["ChangeMod", self.MyState.NAVIGATION_MODE.value])
        else:
            assert symbol is not None
        return for_return
