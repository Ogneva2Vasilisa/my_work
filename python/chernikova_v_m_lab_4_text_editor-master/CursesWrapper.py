import curses
from curses import *


class CursesWrapper:

    def __init__(self):
        self.x=0
        self.y=0
        self.__screen = curses.initscr()
        curses.noecho()  # Не выводить вводимые символы
        curses.cbreak()  # Отключение обработки Ctrl+C и т.д.
        self.__screen.keypad(True)  # Включение обработки функциональных клавиш
        #self.__screen.scrollok(True)
        self.text = ""
        self.xs, self.ys = self.__screen.getmaxyx()
        self.__screen.refresh()

    def scroll(self, x:int):
        assert x>=0
        #for _ in range(0,x):
        self.__screen.scroll()

    def insert_char(self,x ,y , ch):
        self.__screen.addstr(x, y, ch)

    def move_cursor(self, x:int, y:int):
        print("Wrapper x, y:", x, y)
        self.__screen.move(x, y)

    def render(self):
        self.__screen.refresh()

    def render_mode(self, c:int, x:int,y:int, x_max:int, filename:str):
        self.xs, self.ys = self.__screen.getmaxyx()
        xx, yy =self.__screen.getyx()
        st = ''
        if c == 1:
            st = "edit"
        elif c == 2:
            st = " cmd"
        elif c == 4:
            st = " nav"
        elif c == 3:
            st = "find"
        else:
            st = "help"
        self.insert_char(self.xs-1, self.ys - 5, st)
        st=" "+str(x+1)+":"+str(y) +", count "+str(x_max)+" "
        self.insert_char(self.xs - 1, self.ys - (7+len(st)) , st)
        self.insert_char(self.xs - 1, self.ys - (7+len(st+filename)) , filename)
        self.__screen.move(xx, yy)
        self.__screen.refresh()

    def clean_screen(self):
        self.__screen.clear()

    def getch(self):
        return self.__screen.getch()

    def del_ln(self, x):
        self.__screen.move(x, 0)
        self.__screen.clrtoeol()

    def get_xs_xy(self):
        return self.__screen.getmaxyx()