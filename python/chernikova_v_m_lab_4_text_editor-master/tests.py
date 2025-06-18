import pytest
from Controller.Controller import Controller
from Model.Buffer import TextMy
from Model.Cursour import Cursor
from Model.Model import Model
from UnittestAdapter import UnittestAdapter
from Viewer.Viewer import Viewer



def test_text():
    model = Model()
    c = Controller(model)
    adapter = UnittestAdapter()
    render = Viewer(adapter)
    model.build_cmd_list(render)
    text = TextMy()
    assert text.get_line_len() == 1

    text.insert('a', 0, 0)
    assert text.get_line()[0][0] == 'a'
    assert text.get_len_str(0) == 1

    text.insert(' asdasd dfsadf asdfsdf', 0, 0)
    assert text.get_line()[0].c_str() == ' asdasd dfsadf asdfsdfa'

    _, end = text.get_end_word(0, 0)
    assert end == 7

    _, beg = text.get_begin_word(0, 7)
    assert beg == 0
    assert text.compare_line(0, ' asdasd dfsadf asdfsdfa')
    assert text.search_line(0, 0, 'as') == 1

    text.newline(0, 7)
    assert text.compare_line(1, ' dfsadf asdfsdfa')


def test_cursor():
    model = Model()
    c = Controller(model)
    adapter = UnittestAdapter()
    render = Viewer(adapter)
    model.build_cmd_list(render)
    cursor = Cursor()
    assert cursor.getx() == 0
    assert cursor.gety() == 0

    cursor.move(1233, 123123)
    assert cursor.getx() == 1233
    assert cursor.gety() == 123123

    model.command_list["WriteTextSymbol"].execute('1')
    assert model.get_cursor().getx() == 0
    assert model.get_cursor().gety() == 1

    model.command_list["MoveCursorDown"].execute()
    assert model.get_cursor().getx() == 0

    model.command_list["MoveCursorUp"].execute()
    assert model.get_cursor().getx() == 0

    model.command_list["WriteTextSymbol"].execute('2')
    model.command_list["MoveCursorLeft"].execute()
    assert model.get_cursor().gety() == 1

    model.command_list["MoveCursorRight"].execute()
    assert model.get_cursor().gety() == 2

    model.command_list["MoveCursorRight"].execute()
    assert model.get_cursor().gety() == 2

    model.command_list["WriteTextSymbol"].execute('\n')
    assert model.get_cursor().getx() == 1


def test_0():
    model = Model()
    c = Controller(model)
    adapter = UnittestAdapter()
    render = Viewer(adapter)
    model.build_cmd_list(render)

    model.command_list["OpenFile"].execute('test.txt')
    with open('test.txt', 'r') as f:
        arr = f.readline()
    print()
    assert model.get_text_line().get_line()[0].c_str() == arr[:-1]

    model.command_list["SearchForward"].execute('World')
    assert model.get_cursor().getx() == 9
    assert model.get_cursor().gety() == 3

    model.command_list["MoveCursorToEndFile"].execute()
    model.command_list["SearchBack"].execute('World')
    assert model.get_cursor().getx() == 9
    assert model.get_cursor().gety() == 3

    model.command_list["MoveCursorToBeginFile"].execute()
    assert model.get_cursor().getx() == 0
    assert model.get_cursor().gety() == 0

    l = model.get_text_line().get_len_str(0)
    model.command_list["MoveCursorRight"].execute()
    model.command_list["DeleteTextSymbol"].execute()
    assert model.get_text_line().get_len_str(0) == l - 1

    model.command_list["DeleteLine"].execute()
    assert model.get_cursor().getx() == 0
    assert model.get_cursor().gety() == 0
    assert model.get_text_line().get_len_str(0) == 0

