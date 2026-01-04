from CursesWrapper import *
from Viewer.IViewer import IViewer

class Viewer(IViewer):
    __screen_x: int
    """
    Хранит номер в text той строки, которая самая первая
    """

    def __init__(self, adapter: CursesWrapper):
        super().__init__()
        self.__screen_x = 0
        self.adapter_v = adapter

    def print_text_line(self, arr: list):
        self.adapter_v.clean_screen()
        xs, ys = self.adapter_v.get_xs_xy()
        offset = 0
        if self.__screen_x > len(arr) - 1:
            self.__screen_x = len(arr) - 1
        for i in range(self.__screen_x, len(arr)):
            if i - self.__screen_x + offset >= xs - 1:
                break
            #print(i, len(arr))
            if i - self.__screen_x + offset + len(arr[i]) // ys >= xs - 1:
                y = xs - 1 - (i - self.__screen_x + offset)
                self.adapter_v.insert_char(i - self.__screen_x + offset, 0, arr[i].c_str()[:ys * y])
            else:
                self.adapter_v.insert_char(i - self.__screen_x + offset, 0, arr[i].c_str())
            offset += len(arr[i]) // ys
        self.adapter_v.render()

    def print_command_line(self, arr: list):
        x, y = self.adapter_v.get_xs_xy()
        self.adapter_v.del_ln(x-1)
        self.adapter_v.insert_char(x - 1, 0, str(arr))
        self.adapter_v.render()

    def print_cursor(self, x: int, y: int, arr: list):
        xs, ys = self.adapter_v.get_xs_xy()
        offset = 0
        if self.__screen_x > len(arr) - 1:
            self.__screen_x = len(arr) - 1

        for i in range(self.__screen_x, x):
            offset += len(arr[i]) // ys
        if x - self.__screen_x + offset + y // ys >= xs - 1:
            self.__screen_x += (x - self.__screen_x + offset + y // ys) - (xs - 2)
            self.print_text_line(arr)
        elif x - self.__screen_x + offset + y // ys < 0:
            self.__screen_x += x - self.__screen_x + offset + y // ys
            self.print_text_line(arr)
        print("print_cur ", x, self.__screen_x, len(arr), "\n", x , self.__screen_x , offset + y // ys, y % ys)
        if (x - self.__screen_x) == 0:
            self.adapter_v.move_cursor(0, y % ys)
        else:
            self.adapter_v.move_cursor(x - self.__screen_x + offset + y // ys, y % ys)

    def clear_command_line(self):
        self.adapter_v.clean_screen()
        self.adapter_v.render()

    def up_screen(self):  # здесь есть баг страницы
        if self.__screen_x > 0:
            self.__screen_x -= 1

    def down_screen(self):
        self.__screen_x += 1

    def page_down(self):
        x, y = self.adapter_v.get_xs_xy()
        return x