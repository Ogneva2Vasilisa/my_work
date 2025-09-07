from tkinter import *
from datetime import datetime as dt
 
 
def insert_time():
    t = dt.now().time()
    e1.insert(0, t.strftime('%H:%M:%S  '))
 
 
root = Tk()
e1 = Entry(width=50)
but = Button(text="Время",
             command=insert_time)
e1.pack()
but.pack()
root.mainloop()