from tkinter import *
 
 
def take():
    lab['text'] = "Выдано"
 
 
root = Tk()
 
Label(text="Пункт выдачи", font=("Comic Sans MS",
                 24, "bold")).pack()
Button(text="Взять", command=take).pack()
 
lab = Label(width=10, height=1)
lab.pack()
 
root.mainloop()