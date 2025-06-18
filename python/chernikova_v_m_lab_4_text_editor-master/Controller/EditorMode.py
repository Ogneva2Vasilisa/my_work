from Controller.State import *

class EditorMode(State):
    __health: bool
    def __init__(self, my_state: type(MyState)):
        #self.__health = health
        super().__init__()
        self.MyState = my_state
        print("EditorMode is inited")
        pass

    def set_symbol(self, symbol:int):
        for_return=[]
        print("set_symbol_e", symbol)
        if 32 <= symbol <= 126:
            """if self.__health == False:
                for_return.append(["DeleteTextSymbol"])
                for_return.append(["WriteTextSymbol",symbol])
                self.__controller.changeStartmode()
            else:"""
            for_return.append(["WriteTextSymbol",chr(symbol)])

        elif symbol == KEY_RIGHT:
            for_return.append(["MoveCursorRight"])
        elif symbol == KEY_LEFT:
            for_return.append(["MoveCursorLeft"])
        elif symbol == KEY_UP:
            for_return.append(["MoveCursorUp"])
        elif symbol == KEY_DOWN:
            for_return.append(["MoveCursorDown"])
        elif symbol == 10:
            for_return.append(["WriteTextSymbol", "\n"])
            """if self.__health == False:
                self.__controller.changeStartmode()"""
        elif symbol == 8: #BackSpace
            for_return.append(["DeleteTextSymbol"])
        elif symbol == 27:
            for_return.append(["ChangeMod", self.MyState.NAVIGATION_MODE.value])
        else:
            assert symbol is not None
        return for_return
