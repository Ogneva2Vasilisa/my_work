from Controller.State import *
from Model.Buffer import Buffer
from MyString import MyString


class NavigationMode(State):
    __command: Buffer
    __mask: MyString

    def __init__(self, cmd: Buffer, my_state: type(MyState)):
        super().__init__()
        self.__command = cmd
        self.MyState = my_state
        self.__mask = '0123456789'
        print("NavMode is inited")
        pass

    def set_symbol(self, symbol: int):
        for_return = []
        print("set_symbol_n", symbol, chr(symbol))
        line_cur = self.__command.get_line()
        if symbol == KEY_RIGHT:
            for_return.append(["MoveCursorRight"])
        elif symbol == KEY_LEFT:
            for_return.append(["MoveCursorLeft"])
        elif symbol == 8:  # BackSpace
            for_return.append(["PrintCmdErase"])
        elif symbol == 457:
            for_return.append(["DownScreen"])
        elif symbol == 451:
            for_return.append(["UpScreen"])
        elif chr(symbol) == 'n':
            if len(line_cur) > 0:
                if line_cur[0] == '/':
                    for_return.append(["SearchForward", line_cur[1:]])
                elif line_cur[0] == '?':
                    for_return.append(["SearchBack", line_cur[1:]])
        elif chr(symbol) == 'N':
            if len(line_cur) > 0:
                if line_cur[0] == '?':
                    for_return.append(["SearchForward", line_cur[1:]])
                elif line_cur[0] == '/':
                    for_return.append(["SearchBack", line_cur[1:]])
        elif 32 <= symbol <= 126:
            for_return.append(["CmdAppend", chr(symbol)])
            line_cur= line_cur+chr(symbol)
            for_return.append(["PrintCmd"])
            if line_cur == '0' or line_cur == '^':
                for_return.append(["PrintCmdErase"])
                for_return.append(["MoveCursorToBeginLine"])
            elif line_cur == 'i':
                for_return.append(["PrintCmdErase"])
                for_return.append(["ChangeMod", self.MyState.EDITOR_MODE.value])
            elif line_cur == '$':
                for_return.append(["PrintCmdErase"])
                for_return.append(["MoveCursorToEndLine"])
            elif line_cur == 'w':
                for_return.append(["PrintCmdErase"])
                for_return.append(["MoveCursorToEndWord"])
            elif line_cur == 'b':
                for_return.append(["PrintCmdErase"])
                for_return.append(["MoveCursorToBeginWord"])
            elif line_cur == 'gg':
                for_return.append(["PrintCmdErase"])
                for_return.append(["MoveCursorToBeginFile"])
            elif line_cur == 'G':
                for_return.append(["PrintCmdErase"])
                for_return.append(["MoveCursorToEndFile"])
            elif line_cur == 'x':
                for_return.append(["PrintCmdErase"])
                for_return.append(["DeleteTextSymbolS"])
            elif line_cur == 'diw':
                for_return.append(["PrintCmdErase"])
                for_return.append(["DeleteWord"])
            elif line_cur == 'yy':
                for_return.append(["PrintCmdErase"])
                for_return.append(["CopyLine"])
            elif line_cur == 'p':
                for_return.append(["PrintCmdErase"])
                for_return.append(["Paste"])
            elif line_cur == 'dd':
                for_return.append(["PrintCmdErase"])
                for_return.append(["CutLine"])
            elif line_cur == 'yw':
                for_return.append(["PrintCmdErase"])
                for_return.append(["CopyWord"])
            elif line_cur == 'I':
                for_return.append(["PrintCmdErase"])
                for_return.append(["MoveCursorToBeginLine"])
                for_return.append(["ChangeMod", self.MyState.EDITOR_MODE.value])
            elif line_cur == 'A':
                for_return.append(["PrintCmdErase"])
                for_return.append(["MoveCursorToEndLine"])
                for_return.append(["ChangeMod", self.MyState.EDITOR_MODE.value])
            elif line_cur == 'S':
                for_return.append(["PrintCmdErase"])
                for_return.append(["DeleteLine"])
                for_return.append(["ChangeMod", self.MyState.EDITOR_MODE.value])
            elif line_cur == 'r':
                for_return.append(["PrintCmdErase"])
                for_return.append(["ChangeMod", self.MyState.EDITOR_MODE.value])
            elif line_cur == ':':
                for_return.append(["ChangeMod", self.MyState.COMMAND_MODE.value])
            elif line_cur == '/' or line_cur == '?':
                for_return.append(["ChangeMod", self.MyState.FIND_MODE.value])
            elif line_cur[-1] == 'G':
                try:
                    x = int(line_cur[:-1]) - 1
                    for_return.append(["PrintCmdErase"])
                    for_return.append(["MoveCursorToNStr", x])
                except:
                    for_return.append(["PrintCmdErase"])
            elif len(line_cur) == 1 and (
                    line_cur != 'y' and line_cur != 'd' and line_cur != 'g' and line_cur[0] not in self.__mask):
                for_return.append(["PrintCmdErase"])
            elif len(line_cur) == 2 and line_cur != 'di' and line_cur[1] not in self.__mask:
                for_return.append(["PrintCmdErase"])
            else:
                assert symbol is not None
        return for_return
