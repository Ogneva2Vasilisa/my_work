from tkinter import *
 
 
def change():
    b1['text'] = "Изменено"
    b1['bg'] = '#550055'
    b1['activebackground'] = '#005555'
    b1['fg'] = '#ffffff'
    b1['activeforeground'] = '#ffffff'
 
 
root = Tk()
b1 = Button(text="Изменить", 
            width=15, height=3)
b1.config(command=change)
b1.pack()
root.mainloop()