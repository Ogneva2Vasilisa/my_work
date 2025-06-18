from Controller.State import *
from Model.Buffer import Buffer
from Model.Model import IModel
from MyString import MyString

class CommandMode(State):
    __command:Buffer
    __mask: MyString

    def __init__(self, cmd: Buffer, my_state: type(MyState), my_model: IModel):
        super().__init__()
        self.__command=cmd
        self.MyState=my_state
        self.__model=my_model
        self.__mask = '0123456789'
        print("CmdMode is inited")
        pass

    def set_symbol(self, symbol: chr):
        for_return = []
        line_cur = self.__command.get_line()
        file_name=self.__model.file_name
        print("set_symbol_cmd", symbol)
        if symbol == KEY_RIGHT:
            for_return.append(["MoveCursorRight"])
        elif symbol == KEY_LEFT:
            for_return.append(["MoveCursorLeft"])
        elif symbol == 8: #BackSpace
            for_return.append(["PrintCmdErase"])
        elif symbol == 27:
            for_return.append(["ChangeMod", self.MyState.NAVIGATION_MODE.value])
        elif 32 <= symbol <= 126:
            for_return.append(["CmdAppend", chr(symbol)])
            for_return.append(["PrintCmd"])
        elif symbol == 10:
                if line_cur == ':q!':
                    for_return.append(["PrintCmdErase"])
                    exit(0)
                elif line_cur[1] == 'o':
                    for_return.append(["OpenFile", line_cur[3:]])
                    for_return.append(["PrintCmdErase"])
                    for_return.append(["ChangeMod", self.MyState.NAVIGATION_MODE.value])
                elif line_cur[1] == 'x' or line_cur == ':wq!':
                    if file_name != '':
                        for_return.append(["WriteFile", file_name])
                        for_return.append(["PrintCmdErase"])
                        exit(0)
                    else:
                        for_return.append(["PrintCmdErase"])
                        for_return.append(["ChangeMod", self.MyState.NAVIGATION_MODE.value])
                elif line_cur[1] == 'w':
                    if len(line_cur) > 2:
                        for_return.append(["WriteFile", line_cur[3:]])
                    elif file_name != '':
                        for_return.append(["WriteFile", file_name])
                    for_return.append(["PrintCmdErase"])
                    for_return.append(["ChangeMod", self.MyState.NAVIGATION_MODE.value])
                elif line_cur == ':q':
                    if file_name != '':
                        if self.__model.execute_command(["CompareFile", file_name]): #------------
                            exit(0)
                        else:
                            for_return.append(["PrintCmdErase"])
                            for_return.append(["ChangeMod", self.MyState.NAVIGATION_MODE.value])
                    else:
                        exit(0)
                elif line_cur == ':h':
                    for_return.append(["ChangeModHelp"])
                    for_return.append(["PrintCmdErase"])
                    for_return.append(["ChangeMod", self.MyState.NAVIGATION_MODE.value])
                elif line_cur == ':h!':
                    print("want q from help")
                    for_return.append(["ChangeModUnHelp"])
                    for_return.append(["PrintCmdErase"])
                    for_return.append(["ChangeMod", self.MyState.NAVIGATION_MODE.value])
        else:
            assert symbol is not None
        return for_return