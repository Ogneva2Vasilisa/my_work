from tkinter import *
 
root = Tk()
 
l1 = Label(text="Машинное обучение",
           font="Arial 32")
 
l2 = Label(text="Распознавание образов",
           font=("Comic Sans MS",
                 24, "bold"))
 
l1.config(bd=10, bg='purple')
l2.config(bd=10, bg='blue')
 
l1.pack()
l2.pack()
root.mainloop()