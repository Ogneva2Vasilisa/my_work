from Controller.Controller import Controller
from CursesWrapper import *
from Model.Model import Model
from Viewer.Viewer import Viewer

model=Model()
c = Controller(model)
curses_wrapper1 = CursesWrapper()
v = Viewer(curses_wrapper1)

model.build_cmd_list(v)
with open("help.txt", 'r') as f:
    model.help=f.readlines()
print("main_init")
c.main_f(curses_wrapper1)
