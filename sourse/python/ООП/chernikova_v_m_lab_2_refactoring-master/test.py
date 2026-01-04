from unittest.mock import patch
from unittest.mock import MagicMock
from Game.MyDriver import *

gg = MyDriver(10, 10, 20)

def test_(monkeypatch, capsys):
    for i in range(gg.play_field.field_width):
        for j in range(gg.play_field.field_height):
            if gg.play_field.mineField[i][j] == '[X]':
                user_input = MagicMock(return_value="dig "+str(i)+' '+str(j))
                monkeypatch.setattr('builtins.input', user_input)
    gg.mine()
    out, err = capsys.readouterr()
    assert "LOSS" in out

def test_field():
    for i in range(gg.play_field.field_width):
        for j in range(gg.play_field.field_height):
            if gg.play_field.mineField[i][j] != '[X]':
                k = 0
                if gg.play_field.mineField[i][j] != '[X]':
                    if (i > 0) and gg.play_field.mineField[i - 1][j] == '[X]':
                        k = k + 1
                    if (i > 0 and j > 0) and gg.play_field.mineField[i - 1][j - 1] == '[X]':
                        k = k + 1
                    if (j > 0) and gg.play_field.mineField[i][j - 1] == '[X]':
                        k = k + 1
                    if (i < gg.play_field.field_width - 1 and j > 0) and gg.play_field.mineField[i + 1][j - 1] == '[X]':
                        k = k + 1
                    if (j < gg.play_field.field_width - 1 and i > 0) and gg.play_field.mineField[i - 1][j + 1] == '[X]':
                        k = k + 1
                    if (j < gg.play_field.field_width - 1 and i < gg.play_field.field_width - 1) and \
                            gg.play_field.mineField[i + 1][j + 1] == '[X]':
                        k = k + 1
                    if (j < gg.play_field.field_width - 1) and gg.play_field.mineField[i][j + 1] == '[X]':
                        k = k + 1
                    if (i < gg.play_field.field_width - 1) and gg.play_field.mineField[i + 1][j] == '[X]':
                        k = k + 1

                assert gg.play_field.mineField[i][j] != k


def test_dig(monkeypatch):
    user_input = MagicMock(return_value="dig 0 1")
    monkeypatch.setattr('builtins.input', user_input)
    gg.event_cons.handle(gg.play_field, gg.graph_cons)
    assert gg.event_cons.current_x == 0
    assert gg.event_cons.current_y == 1
    assert gg.event_cons.dig_box

def test_mark(monkeypatch):
    user_input = MagicMock(return_value="flag 0 1")
    monkeypatch.setattr('builtins.input', user_input)
    gg.event_cons.handle(gg.play_field, gg.graph_cons)
    assert gg.event_cons.current_x == 0
    assert gg.event_cons.current_y == 1
    assert gg.event_cons.flag_box
    # print()
    # print(gg.event_cons.current_x, gg.event_cons.current_y,gg.event_cons.flag_box )

def test_show_all():
    gg.event_cons.show=True
    gg.event_cons.check_but(gg.play_field,gg.but_show,None,None)
    for u in gg.play_field.revealedBoxes:
        assert (False not in u)

