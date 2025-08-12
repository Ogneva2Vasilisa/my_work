from Controller.State import *
from Model.Buffer import Buffer

class FindMode(State):

    def __init__(self, cmd: Buffer, my_state: type(MyState)):
        super().__init__()
        self.__command=cmd
        self.MyState=my_state
        print("EditorMode is inited")
        pass

    def set_symbol(self, symbol:int):
        for_return=[]
        line_cur = self.__command.get_line()
        print("set_symbol_f", symbol)
        if symbol == KEY_RIGHT:
            for_return.append(["MoveCursorRight"])
        elif symbol == KEY_LEFT:
            for_return.append(["MoveCursorLeft"])
        elif symbol == 8: #BackSpace
            for_return.append(["PrintCmdErase"])
        elif 32 <= symbol <= 126:
            for_return.append(["CmdAppend", chr(symbol)])
            for_return.append(["PrintCmd"])
        elif symbol == 10:
            if line_cur[0] == '/':
                for_return.append(["SearchForward", line_cur[1:]])
            elif line_cur[0] == '?':
                for_return.append(["SearchBack", line_cur[1:]])
            for_return.append(["ChangeMod", self.MyState.NAVIGATION_MODE.value])
        elif symbol == 27:
            for_return.append(["PrintCmdErase"])
            for_return.append(["ChangeMod", self.MyState.NAVIGATION_MODE.value])
            if len(line_cur) == 0:
                for_return.append(["ChangeMod", self.MyState.NAVIGATION_MODE.value])
        else:
            assert symbol is not None
        return for_return
