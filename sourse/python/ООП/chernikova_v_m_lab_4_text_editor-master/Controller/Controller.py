from Controller.CommandMode import CommandMode
from Controller.FindMode import FindMode
from Controller.HelpMode import HelpMode
from Controller.IController import IController
from Controller.NavigationMode import NavigationMode
from Controller.State import MyState, State
from Controller.EditorMode import EditorMode
from CursesWrapper import CursesWrapper
from Model.Buffer import TextMy
from Model.Model import Model


class Controller(IController):
    list_mode: dict
    __cur_mode: int
    for_help: TextMy

    def __init__(self, model: Model):
        super().__init__(model)
        self.for_help = TextMy()
        self.for_help_filename = self.main.file_name
        self.list_mode = {
            MyState.EDITOR_MODE.value: EditorMode(MyState),  # 1 : EditorMode() : State
            MyState.NAVIGATION_MODE.value: NavigationMode(self.main.command_line, MyState),
            MyState.FIND_MODE.value: FindMode(self.main.command_line, MyState),
            MyState.COMMAND_MODE.value: CommandMode(self.main.command_line, MyState, self.main),
            MyState.HELP_MODE.value: HelpMode(MyState)
        }
        self.__cur_mode = MyState.NAVIGATION_MODE.value  # -----------------------------------!!!!!!!!!!!!!!!!!
        pass

    def main_f(self, curses_wrapper1: CursesWrapper):
        curses_wrapper1.render_mode(self.get_cur_mode(), self.main.cursor.getx(), self.main.cursor.gety(),
                                    self.main.cursor.gety(), self.main.file_name)  # надо править, так как слетает курсор
        while True:
            ch = curses_wrapper1.getch()  # забираем символ с экрана и с помощью состояния определяем, что надо делать
            cmd = self.list_mode[self.__cur_mode].set_symbol(ch)
            print("contr ",self.__cur_mode, cmd)
            for c in cmd:
                if c[0] == "ChangeMod":
                    self.change_mode(c[1])
                    curses_wrapper1.render_mode(self.get_cur_mode(), self.main.cursor.getx(), self.main.cursor.gety(),
                                                self.main.text.get_line_len(), self.main.file_name)
                    continue
                elif c[0] == "ChangeModHelp":
                    self.for_help = self.main.text
                    self.for_help_filename = self.main.file_name
                    self.main.execute_command(["OpenFile", 'help.txt'])
                    curses_wrapper1.render_mode(self.get_cur_mode(), self.main.cursor.getx(), self.main.cursor.gety(),
                                                self.main.text.get_line_len(), self.main.file_name)
                    continue
                elif c[0] == "ChangeModUnHelp":
                    self.main.file_name = self.for_help_filename
                    if self.main.file_name != "":
                        self.main.execute_command(["OpenFile", self.main.file_name])
                        curses_wrapper1.render_mode(self.get_cur_mode(), self.main.cursor.getx(), self.main.cursor.gety(),
                                                    self.main.text.get_line_len(), self.main.file_name)
                    self.main.text = self.for_help
                    continue
                self.main.execute_command(c)
            curses_wrapper1.render_mode(self.get_cur_mode(), self.main.cursor.getx(), self.main.cursor.gety(), self.main.text.get_line_len(),
                                        self.main.file_name)

    def change_mode(self, mode: int):
        self.__cur_mode = mode
        pass

    def get_cur_mode(self):
        return self.__cur_mode
