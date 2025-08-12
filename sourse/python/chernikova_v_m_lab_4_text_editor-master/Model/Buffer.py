from MyString import *
from abc import ABC, abstractmethod


class Buffer(ABC):
    #__line: MyString

    def __init__(self):
        self.__line = []
        self.__line.append(MyString())

    def get_line_len(self) -> int:
        return len(self.__line)

    def get_len_str(self, x: int) -> int:
        return self.__line[x].length()

    def get_line(self) -> list:
        return self.__line

    def insert(self, symb: str, x: int, y: int):
        self.__line[x].insert(y, symb)

    def delete(self, x: int, y: int):
        self.__line[x].erase(y, 1)

    def newline(self, x: int, y: int):
        self.__line.insert(x + 1, MyString())
        self.__line[x + 1].append(self.__line[x].substr(y).c_str())
        self.__line[x].erase(y, len(self.__line[x]) - y)
        # self.__line.append(MyString())

    def concat_line(self, x: int):
        if x + 1 < len(self.__line):
            self.__line[x].append(self.__line[x + 1].c_str())

    def delete_line(self, x: int):
        self.__line.pop(x)

    def get_end_word(self, x: int, y: int) -> [int, int]:
        endx = self.get_line_len() - 1
        endy = self.get_len_str(endx)
        end = 0
        for j in range(x, self.get_line_len()):
            if j == x:
                end = y + 1
            else:
                end = 0
            for i in range(end, len(self.__line[j])):
                if self.__line[j][i] == ' ':
                    endx = j
                    endy = i
                    return endx, endy
        return endx, endy

    def get_begin_word(self, x: int, y: int) -> [int, int]:
        beginx = x
        beginy = 0
        for j in range(x, -1, -1):
            begin = 0
            if j == x:
                begin = y - 1
            else:
                begin = self.get_len_str(j)
            for i in range(begin, -1, -1):
                if self.__line[j][i] == ' ':
                    beginy = i
                    beginx = j
                    return beginx, beginy
        return 0, 0

    def delete_word(self, x: int, y: int):
        end = self.get_end_word(x, y)
        begin = self.get_begin_word(x, y)
        self.__line[x].erase(begin, end - begin)

    def get_word(self, x: int, y: int):
        end = self.get_end_word(x, y)
        begin = self.get_begin_word(x, y)
        return self.__line[x].substr(begin, end - begin)

    def compare_line(self, x:int, line):
        if self.__line[x].c_str() == line:
            return True
        return False

    def search_line(self, x:int, y:int, line:str):
        if self.__line[x].find(line, y) >= 0:
            return self.__line[x].find(line, y)
        return -1


class TextMy(Buffer):
    def __init__(self):
        super().__init__()

class CommandLine:
    __line: MyString

    def __init__(self):
        self.__line = MyString()

    def append(self, symbol):
        self.__line.append(symbol)

    def erase(self):
        self.__line.clear()

    def get_line(self):
        return self.__line.c_str()


class FindBuffer(CommandLine):
    def __init__(self):
        super().__init__()
