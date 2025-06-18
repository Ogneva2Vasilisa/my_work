from __future__ import annotations
from multipledispatch import dispatch
from Buffer import Buffer
from Model import Model
from Model.Cursour import Cursor
from Viewer.Viewer import Viewer


class Command:
    __text: Buffer
    __cursor: Cursor
    __model:Model
    __render: Viewer
    def __init__(self, model: Model, render: Viewer):  # IModel
        self.__text = model.text
        self.__cursor = model.cursor
        self.__buffer = model.buffer
        self.__render = render
        self.__model = model
        pass

    def execute(self):
        pass

class PassP (Command):
    def execute(self, *args):
        pass

#------------------------------------------EditMode---------------------------------------------------

class WriteTextSymbol(Command):
    def __init__(self, model: Model, render: Viewer):
        super().__init__(model, render)
        self.__text = model.text
        self.__cursor = model.cursor
        self.__buffer = model.buffer
        self.__render = render
        self.__model = model
        pass

    #@dispatch(chr)
    def execute(self, symbol:chr):
        if symbol == '\n':
            self.__text.newline(self.__cursor.getx(), self.__cursor.gety())
            self.__render.print_text_line(self.__text.get_line())
            self.__model.command_list['MoveCursorToBeginLine'].execute(self.__cursor.getx() + 1)
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
        self.__model.command_list['MoveCursor'](self.__cursor.getx(), self.__cursor.gety())


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
        self.__model.command_list['MoveCursor'](self.__cursor.getx(), 0)


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
        self.__model.command_list['MoveCursor'](self.__cursor.getx(), self.__text.get_len_str(self.__cursor.getx()))


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
        self.__model.command_list['MoveCursor'](self.__cursor.getx(), y)


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
        self.__model.command_list['MoveCursor'](self.__cursor.getx(), y)


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
        self.__model.command_list['MoveCursor'](0, 0)


class MoveCursorToEndFile(Command):
    def __init__(self, model: Model, render: Viewer):
        super().__init__(model, render)
        self.__text = model.text
        self.__cursor = model.cursor
        self.__buffer = model.buffer
        self.__render = render
        self.__model = model
        pass

    def execute(self):
        self.__model.command_list['MoveCursor'](self.__text.get_line_len() - 1,
                                                self.__text.get_len_str(self.__text.get_line_len() - 1))


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
        self.__model.command_list['MoveCursor'](x, 0)


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
        self.__render.down_screen()
        self.__model.command_list['MoveCursorDown'].execute()
        self.__model.command_list['RefreshScreen'].execute()


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
        self.__render.up_screen()
        self.__model.command_list['MoveCursorUp'].execute()
        self.__model.command_list['RefreshScreen'].execute()


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
        self.__model.command_list['MoveCursor'](self.__cursor.getx(), self.__cursor.gety())


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
        self.__model.command_list['CutLine']()
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
        self.MoveCursorToBeginLine()


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
        self.__model.command_list['MoveCursor'](self.__cursor.getx(), self.__cursor.gety())


class OpenFile(Command):
    def __init__(self, model: Model, render: Viewer):
        super().__init__(model, render)
        self.__text = model.text
        self.__cursor = model.cursor
        self.__buffer = model.buffer
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
        self.__model.command_list['MoveCursor'](0, 0)

#------------------------------------------NavMode---------------------------------------------------

class PrintCmd(Command):
    def __init__(self, model: Model, render: Viewer):
        super().__init__(model, render)
        self.__text = model.text
        self.__cursor = model.cursor
        self.__buffer = model.buffer
        self.__render = render
        self.__model = model
        pass

    @dispatch(str)
    def execute(self, symbol:str):
        self.__model.command_line.append(symbol)
        self.__render.print_command_line(self.__model.command_line.get_line())

class MoveCursorToBeginLineCmd(Command):
    def __init__(self, model: Model, render: Viewer):
        super().__init__(model, render)
        self.__command = model.command_line
        self.__cursor = model.cursor
        self.__buffer = model.buffer
        self.__render = render
        self.__model = model
        pass

    @dispatch(chr)
    def execute(self, symbol:chr):
        self.__command.erase()
        self.__render.print_command_line(self.__command.get_line())
        self.__model.command_list["MoveCursorToBeginLine"].execute()

