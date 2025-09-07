from tkinter import *


def smile():
    label = Label(text=":)", bg="yellow")
    text.window_create(INSERT, window=label)
 
root = Tk()
 
text = Text(width=50, height=10)
text.pack()
 
button = Button(text=":)", command=smile)
button.pack()
 
 
def insert_text():
    s = "лол кек чебурек"
    text.insert(1.0, s)
 
 
def get_text():
    s = text.get(1.0, END)
    label['text'] = s
 
 
def delete_text():
    text.delete(1.0, END)
 
 
root = Tk()
 

 
frame = Frame()
frame.pack()
Button(frame, text="Вставить",
       command=insert_text).pack(side=LEFT)
Button(frame, text="Взять",
       command=get_text).pack(side=LEFT)
Button(frame, text="Удалить",
       command=delete_text).pack(side=LEFT)
 
label = Label()
label.pack()
 
root.mainloop()