from __future__ import annotations
# Проверить, что VIM работает на больших файлах, отчет, тесты дописать, чтобы они были независимые
from distutils.util import execute

# from Commands import *
from Model.IModel import IModel
from Model.Buffer import *
from Model.Cursour import Cursor


class Model(IModel):
    cursor: Cursor
    text: TextMy
    command_line: CommandLine
    find_buffer: FindBuffer
    buffer: Buffer
    command_list: dict  # of Commands

    def __init__(self):
        self.cursor = Cursor()
        self.text = TextMy()
        self.command_line = CommandLine()
        self.find_buffer = FindBuffer()
        self.buffer = Buffer()
        self.command_list = dict()
        self.file_name=""
        # self.__build_cmd_list() - это в main
        super().__init__()
        pass

    def build_cmd_list(self, viewer: Viewer):
        self.command_list["pass"] = PassP(self, viewer)
        self.command_list["MoveCursor"] = MoveCursor(self, viewer)
        self.command_list["WriteTextSymbol"] = WriteTextSymbol(self, viewer)
        self.command_list["DeleteTextSymbol"] = DeleteTextSymbol(self, viewer)
        self.command_list["DeleteTextSymbolS"] = DeleteTextSymbolS(self, viewer)
        self.command_list["DeleteWord"] = DeleteWord(self, viewer)
        self.command_list["MoveCursorRight"] = MoveCursorRight(self, viewer)
        self.command_list["MoveCursorLeft"] = MoveCursorLeft(self, viewer)
        self.command_list["MoveCursorUp"] = MoveCursorUp(self, viewer)
        self.command_list["MoveCursorDown"] = MoveCursorDown(self, viewer)
        self.command_list["MoveCursorToBeginLine"] = MoveCursorToBeginLine(self, viewer)
        self.command_list["MoveCursorToEndLine"] = MoveCursorToEndLine(self, viewer)
        self.command_list["MoveCursorToEndWord"] = MoveCursorToEndWord(self, viewer)
        self.command_list["MoveCursorToBeginWord"] = MoveCursorToBeginWord(self, viewer)
        self.command_list["MoveCursorToEndFile"] = MoveCursorToEndFile(self, viewer)
        self.command_list["MoveCursorToNStr"] = MoveCursorToNStr(self, viewer)
        self.command_list["RefreshScreen"] = RefreshScreen(self, viewer)
        self.command_list["DownScreen"] = DownScreen(self, viewer)
        self.command_list["UpScreen"] = UpScreen(self, viewer)
        self.command_list["CopyLine"] = CopyLine(self, viewer)
        self.command_list["CopyWord"] = CopyWord(self, viewer)
        self.command_list["CutLine"] = CutLine(self, viewer)
        self.command_list["DeleteLine"] = DeleteLine(self, viewer)
        self.command_list["Paste"] = Paste(self, viewer)
        self.command_list["OpenFile"] = OpenFile(self, viewer)
        self.command_list["WriteFile"] = WriteFile(self, viewer)
        self.command_list["CompareFile"] = CompareFile(self, viewer)
        self.command_list["MoveCursorToBeginFile"] = MoveCursorToBeginFile(self, viewer)
        self.command_list["PrintCmd"] = PrintCmd(self, viewer)
        self.command_list["PrintCmdErase"] = PrintCmdErase(self, viewer)
        self.command_list["CmdAppend"] = CmdAppend(self, viewer)
        self.command_list["DeleteCmd"] = DeleteCmd(self, viewer)
        self.command_list["SearchForward"] = SearchForward(self, viewer)
        self.command_list["SearchBack"] = SearchBack(self, viewer)
        self.command_list["GetFilename"] = GetFilename(self, viewer)

        # You can add here your commands. Don't forget to add its implementations to Commands.py

    def execute_command(self, *args):
        u = list(args)
        print("model ", u[0])

        if len(u[0]) == 1:
            return self.command_list[args[0][0]].execute()
        else:
            return self.command_list[args[0][0]].execute(u[0][1])

    # для pytest
    def get_cursor(self):
        return self.cursor

    def get_text_line(self):
        return self.text



from abc import ABC  # ?? почему abstractmethod не используется ??
from multipledispatch import dispatch
from Model.Buffer import Buffer
from Viewer import Viewer


class Command(ABC):
    __text: Buffer
    __cursor: Cursor
    __model: Model
    __render: Viewer

    def __init__(self, model: IModel, render: Viewer):  # IModel
        self.__text = model.text
        self.__cursor = model.cursor
        self.__buffer = model.buffer
        self.__render = render
        self.__model = model
        pass

    def execute(self):
        pass


class PassP(Command):
    def execute(self, *args):
        pass


class WriteTextSymbol(Command):
    def __init__(self, model: Model, render: Viewer):
        super().__init__(model, render)
        self.__text = model.text
        self.__cursor = model.cursor
        self.__buffer = model.buffer
        self.__render = render
        self.__model = model
        pass

    @dispatch(str)
    def execute(self, symbol: str):
        if symbol == '\n':
            self.__text.newline(self.__cursor.getx(), self.__cursor.gety())
            self.__render.print_text_line(self.__text.get_line())
            self.__model.command_list['MoveCursorToBeginLine'].execute(self.__cursor.getx() + 1)
            self.__model.command_list['MoveCursor'].execute(self.__cursor.getx(), self.__cursor.gety())
        else:
            self.__text.insert(symbol, self.__cursor.getx(), self.__cursor.gety())
            self.__render.print_text_line(self.__text.get_line())
            self.__model.command_list['MoveCursorRight'].execute()


class DeleteTextSymbol(Command):
    def __init__(self, model: Model, render: Viewer):
        super().__init__(model, render)
        self.__text = model.text
        self.__cursor = model.cursor
        self.__buffer = model.buffer
        self.__render = render
        self.__model = model
        pass

    def execute(self):
        if self.__cursor.gety() > 0:
            self.__text.delete(self.__cursor.getx(), self.__cursor.gety() - 1)
            self.__render.print_text_line(self.__text.get_line())
            self.__model.command_list['MoveCursorLeft'].execute()
        elif self.__text.get_line_len() > 0 and self.__cursor.getx() > 0:
            self.__text.concat_line(self.__cursor.getx() - 1)
            self.__text.delete_line(self.__cursor.getx())
            self.__render.print_text_line(self.__text.get_line())
            self.__model.command_list['MoveCursorUp'].execute()
            self.__model.command_list['MoveCursorToEndLine'].execute()


class DeleteTextSymbolS(Command):
    def __init__(self, model: Model, render: Viewer):
        super().__init__(model, render)
        self.__text = model.text
        self.__cursor = model.cursor
        self.__buffer = model.buffer
        self.__render = render
        self.__model = model
        pass

    def execute(self):
        self.__text.delete(self.__cursor.getx(), self.__cursor.gety())
        self.__render.print_text_line(self.__text.get_line())
        self.__model.command_list['MoveCursor'].execute(self.__cursor.getx(), self.__cursor.gety())


class DeleteWord(Command):
    def __init__(self, model: Model, render: Viewer):
        super().__init__(model, render)
        self.__text = model.text
        self.__cursor = model.cursor
        self.__buffer = model.buffer
        self.__render = render
        self.__model = model
        pass

    def execute(self):
        self.__text.delete_word(self.__cursor.getx(), self.__cursor.gety())
        self.__render.print_text_line(self.__text.get_line())


class MoveCursor(Command):
    def __init__(self, model: Model, render: Viewer):
        super().__init__(model, render)
        self.__text = model.text
        self.__cursor = model.cursor
        self.__buffer = model.buffer
        self.__render = render
        self.__model = model
        pass

    @dispatch(int, int)
    def execute(self, x: int, y: int):
        self.__cursor.move(x, y)
        print("NOVE_CUR", x, y, self.__cursor.getx())
        self.__render.print_cursor(self.__cursor.getx(), self.__cursor.gety(), self.__text.get_line())


class MoveCursorRight(Command):
    def __init__(self, model: Model, render: Viewer):
        super().__init__(model, render)
        self.__text = model.text
        self.__cursor = model.cursor
        self.__buffer = model.buffer
        self.__render = render
        self.__model = model
        pass

    def execute(self):
        if self.__cursor.gety() + 1 <= self.__text.get_len_str(self.__cursor.getx()):
            self.__model.command_list['MoveCursor'].execute(self.__cursor.getx(), self.__cursor.gety() + 1)


class MoveCursorLeft(Command):
    def __init__(self, model: Model, render: Viewer):
        super().__init__(model, render)
        self.__text = model.text
        self.__cursor = model.cursor
        self.__buffer = model.buffer
        self.__render = render
        self.__model = model
        pass

    def execute(self):
        self.__model.command_list['MoveCursor'].execute(self.__cursor.getx(), self.__cursor.gety() - 1)


class MoveCursorUp(Command):
    def __init__(self, model: Model, render: Viewer):
        super().__init__(model, render)
        self.__text = model.text
        self.__cursor = model.cursor
        self.__buffer = model.buffer
        self.__render = render
        self.__model = model
        pass

    def execute(self):
        print(self.__model.command_list['MoveCursor'])
        self.__model.command_list['MoveCursor'].execute(self.__cursor.getx() - 1, self.__cursor.gety())


class MoveCursorDown(Command):
    def __init__(self, model: Model, render: Viewer):
        super().__init__(model, render)
        self.__text = model.text
        self.__cursor = model.cursor
        self.__buffer = model.buffer
        self.__render = render
        self.__model = model
        pass

    def execute(self):
        if self.__cursor.getx() + 1 < self.__text.get_line_len():
            self.__model.command_list['MoveCursor'].execute(self.__cursor.getx() + 1, self.__cursor.gety())


class MoveCursorToBeginLine(Command):
    def __init__(self, model: Model, render: Viewer):
        super().__init__(model, render)
        self.__text = model.text
        self.__cursor = model.cursor
        self.__buffer = model.buffer
        self.__render = render
        self.__model = model
        pass

    @dispatch(int)
    def execute(self, x: int):
        self.__model.command_list['MoveCursor'].execute(x, 0)

    @dispatch()
    def execute(self):
        self.__model.command_list['MoveCursor'].execute(self.__cursor.getx(), 0)


class MoveCursorToEndLine(Command):
    def __init__(self, model: Model, render: Viewer):
        super().__init__(model, render)
        self.__text = model.text
        self.__cursor = model.cursor
        self.__buffer = model.buffer
        self.__render = render
        self.__model = model
        pass

    def execute(self):
        self.__model.command_list['MoveCursor'].execute(self.__cursor.getx(),
                                                        self.__text.get_len_str(self.__cursor.getx()))


class MoveCursorToEndWord(Command):
    def __init__(self, model: Model, render: Viewer):
        super().__init__(model, render)
        self.__text = model.text
        self.__cursor = model.cursor
        self.__buffer = model.buffer
        self.__render = render
        self.__model = model
        pass

    def execute(self):
        y = self.__text.get_end_word(self.__cursor.getx(), self.__cursor.gety())
        self.__model.command_list['MoveCursor'].execute(self.__cursor.getx(), y)


class MoveCursorToBeginWord(Command):
    def __init__(self, model: Model, render: Viewer):
        super().__init__(model, render)
        self.__text = model.text
        self.__cursor = model.cursor
        self.__buffer = model.buffer
        self.__render = render
        self.__model = model
        pass

    def execute(self):
        y = self.__text.get_begin_word(self.__cursor.getx(), self.__cursor.gety())
        self.__model.command_list['MoveCursor'].execute(self.__cursor.getx(), y)


class MoveCursorToBeginFile(Command):
    def __init__(self, model: Model, render: Viewer):
        super().__init__(model, render)
        self.__text = model.text
        self.__cursor = model.cursor
        self.__buffer = model.buffer
        self.__render = render
        self.__model = model
        pass

    def execute(self):
        self.__model.command_list['MoveCursor'].execute(0, 0)


class MoveCursorToEndFile(Command):
    def __init__(self, model: Model, render: Viewer):
        super().__init__(model, render)

        self.__render = render
        self.__model = model
        pass

    def execute(self):
        self.__model.command_list['MoveCursor'].execute(self.__model.text.get_line_len() - 1,
                                                        self.__model.text.get_len_str(self.__model.text.get_line_len() - 1))


class MoveCursorToNStr(Command):
    def __init__(self, model: Model, render: Viewer):
        super().__init__(model, render)
        self.__text = model.text
        self.__cursor = model.cursor
        self.__buffer = model.buffer
        self.__render = render
        self.__model = model
        pass

    @dispatch(int)
    def execute(self, x: int):
        if x > self.__text.get_line_len() - 1:
            x = self.__text.get_line_len() - 1
        self.__model.command_list['MoveCursor'].execute(x, 0)


class RefreshScreen(Command):
    def __init__(self, model: Model, render: Viewer):
        super().__init__(model, render)
        self.__text = model.text
        self.__cursor = model.cursor
        self.__buffer = model.buffer
        self.__render = render
        self.__model = model
        pass

    def execute(self):
        self.__render.print_text_line(self.__text.get_line())


class DownScreen(Command):
    def __init__(self, model: Model, render: Viewer):
        super().__init__(model, render)
        self.__text = model.text
        self.__cursor = model.cursor
        self.__buffer = model.buffer
        self.__render = render
        self.__model = model
        pass

    def execute(self):
        x = self.__render.page_down()
        if self.__cursor.getx() + x > self.__model.text.get_line_len():
            x = self.__model.text.get_line_len() - 1 - self.__cursor.getx()
        print("DownScr ", self.__cursor.getx(), x,self.__model.text.get_line_len(), self.__model.text.get_line_len() - 1 - self.__cursor.getx())
        self.__model.command_list['RefreshScreen'].execute()
        self.__model.command_list['MoveCursor'].execute(self.__cursor.getx() + x, 0)


class UpScreen(Command):
    def __init__(self, model: Model, render: Viewer):
        super().__init__(model, render)
        self.__text = model.text
        self.__cursor = model.cursor
        self.__buffer = model.buffer
        self.__render = render
        self.__model = model
        pass

    def execute(self):
        x = self.__render.page_down()
        if self.__cursor.getx() - x > 0:
            x = self.__cursor.getx() - x
        else:
            x=0
        self.__model.command_list['RefreshScreen'].execute()
        print("UpScr ", self.__cursor.getx(), x, self.__model.text.get_line_len(), self.__model.text.get_line_len() - 1 - self.__cursor.getx())
        self.__model.command_list['MoveCursor'].execute(x, 0)


class CopyLine(Command):
    def __init__(self, model: Model, render: Viewer):
        super().__init__(model, render)
        self.__text = model.text
        self.__cursor = model.cursor
        self.__buffer = model.buffer
        self.__render = render
        self.__model = model
        pass

    def execute(self):
        super().__buffer.add(self.__text.get_line()[self.__cursor.getx()])
        self.__model.command_list['MoveCursor'].execute(self.__cursor.getx(), self.__cursor.gety())


class CopyWord(Command):
    def __init__(self, model: Model, render: Viewer):
        super().__init__(model, render)
        self.__text = model.text
        self.__cursor = model.cursor
        self.__buffer = model.buffer
        self.__render = render
        self.__model = model
        pass

    def execute(self):
        self.__buffer.add(self.__text.get_word(self.__cursor.getx(), self.__cursor.gety()))


class CutLine(Command):
    def __init__(self, model: Model, render: Viewer):
        super().__init__(model, render)
        self.__text = model.text
        self.__cursor = model.cursor
        self.__buffer = model.buffer
        self.__render = render
        self.__model = model
        pass

    def execute(self):
        self.__model.command_list['CutLine'].execute()
        self.__text.delete_line(self.__cursor.getx())
        self.__render.print_text_line(self.__text.get_line())
        self.__model.command_list['MoveCursorUp'].execute()


class DeleteLine(Command):
    def __init__(self, model: Model, render: Viewer):
        super().__init__(model, render)
        self.__text = model.text
        self.__cursor = model.cursor
        self.__buffer = model.buffer
        self.__render = render
        self.__model = model
        pass

    def execute(self):
        self.__text.delete_line(self.__cursor.getx())
        self.__text.get_line().insert(self.__cursor.getx(), MyString())
        self.__render.print_text_line(self.__text.get_line())
        self.__model.command_list['MoveCursorToBeginLine'].execute()


class Paste(Command):
    def __init__(self, model: Model, render: Viewer):
        super().__init__(model, render)
        self.__text = model.text
        self.__cursor = model.cursor
        self.__buffer = model.buffer
        self.__render = render
        self.__model = model
        pass

    def execute(self):
        self.__text.insert(self.__buffer.get_line().c_str(), self.__cursor.getx(), self.__cursor.gety())
        self.__render.print_text_line(self.__text.get_line())
        self.__model.command_list['MoveCursor'].execute(self.__cursor.getx(), self.__cursor.gety())


class OpenFile(Command):
    def __init__(self, model: Model, render: Viewer):
        super().__init__(model, render)
        self.__text = model.text
        self.__render = render
        self.__model = model
        pass

    @dispatch(str)
    def execute(self, filename):
        with open(filename) as f:
            arr = f.readlines()
        a = 0
        self.__text.get_line().clear()
        for i in arr:
            self.__text.get_line().append(MyString())
            self.__text.insert(i[:-1], a, 0)
            a += 1
        self.__render.print_text_line(self.__text.get_line())
        self.__model.command_list['MoveCursor'].execute(0, 0)
        self.__model.file_name = filename


class WriteFile(Command):
    def __init__(self, model: Model, render: Viewer):
        super().__init__(model, render)
        self.__text = model.text
        self.__cursor = model.cursor
        self.__buffer = model.buffer
        self.__render = render
        self.__model = model
        pass

    @dispatch(str)
    def execute(self, filename: str):
        with open(filename, 'w') as f:
            for i in range(self.__text.get_line_len()):
                f.writelines(self.__text.get_line()[i].c_str())
                f.write('\n')


class CompareFile(Command):
    def __init__(self, model: Model, render: Viewer):
        super().__init__(model, render)
        self.__text = model.text
        self.__cursor = model.cursor
        self.__buffer = model.buffer
        self.__render = render
        self.__model = model
        pass

    @dispatch(str)
    def execute(self, filename: str) -> bool:
        with open(filename) as f:
            arr = f.readlines()
        for i in range(len(arr)):
            if not self.__text.compare_line(i, arr[i][:-1]):
                return False
        return True

class GetFilename(Command):
    def __init__(self, model: Model, render: Viewer):
        super().__init__(model, render)
        self.__model = model
        pass

    def execute(self):
        return self.__model.file_name


# ------------------------------------------NavMode---------------------------------------------------

class PrintCmd(Command):
    def __init__(self, model: Model, render: Viewer):
        super().__init__(model, render)
        self.__render = render
        self.__model = model
        pass

    def execute(self):
        self.__render.print_command_line(self.__model.command_line.get_line())


class PrintCmdErase(Command):
    def __init__(self, model: Model, render: Viewer):
        super().__init__(model, render)
        self.__command = model.command_line
        self.__model = model
        pass

    def execute(self):
        self.__command.erase()
        self.__model.command_list["PrintCmd"].execute()


class CmdAppend(Command):
    def __init__(self, model: Model, render: Viewer):
        super().__init__(model, render)
        self.__command = model.command_line
        self.__model = model
        pass

    @dispatch(str)
    def execute(self, symbol: str):
        self.__model.command_line.append(symbol)


class DeleteCmd(Command):
    def __init__(self, model: Model, render: Viewer):
        super().__init__(model, render)
        self.__text = model.text
        self.__cursor = model.cursor
        self.__buffer = model.buffer
        self.__render = render
        self.__model = model
        pass

    def execute(self):
        self.__model.command_line.erase()
        self.__model.command_list["PrintCmd"].execute()


# ------------------------------------------FindMode----------------------------------------------------------
class SearchForward(Command):
    def __init__(self, model: Model, render: Viewer):
        super().__init__(model, render)
        self.__text = model.text
        self.__cursor = model.cursor
        self.__buffer = model.buffer
        self.__render = render
        self.__model = model
        pass

    @dispatch(str)
    def execute(self, line: str):
        x = self.__cursor.getx()
        y = self.__cursor.gety()
        s = self.__text.search_line(x, y + 1, line)
        if 0 <= s < self.__text.get_len_str(x):
            self.__model.command_list['MoveCursor'].execute(x, s)
            return
        y = 0
        for i in range(x + 1, self.__text.get_line_len()):
            s = self.__text.search_line(i, y, line)
            if 0 <= s < self.__text.get_len_str(i):
                self.__model.command_list['MoveCursor'].execute(i, s)

class SearchBack(Command):
    def __init__(self, model: Model, render: Viewer):
        super().__init__(model, render)
        self.__text = model.text
        self.__cursor = model.cursor
        self.__buffer = model.buffer
        self.__render = render
        self.__model = model
        pass

    @dispatch(str)
    def execute(self, line: str):
        x = self.__cursor.getx()
        y = self.__cursor.gety()
        s = self.__text.search_line(x, 0, line)
        if 0 <= s < y:
            while 1:
                s1 = self.__text.search_line(x, s + 1, line)
                if s1 >= 0 and s1 < y:
                    s = s1
                else:
                    break
            self.__model.command_list['MoveCursor'].execute(x, s)
            return
        if x > 0:
            for i in range(x - 1, -1, -1):
                s = self.__text.search_line(i, 0, line)
                if 0 <= s < self.__text.get_len_str(i):
                    while 1:
                        s1 = self.__text.search_line(i, s + 1, line)
                        if s1 >= 0 and s1 < self.__text.get_len_str(i):
                            s = s1
                        else:
                            break
                    self.__model.command_list['MoveCursor'].execute(i, s)
                    return