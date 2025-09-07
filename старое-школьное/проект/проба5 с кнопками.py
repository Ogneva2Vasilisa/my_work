from tkinter import *
Label(text='').pack()
lab = Label()
e = Entry(width=36)
def change1(i):
    e.delete(0, END)
    e.insert(0, i)
    if i == '#ff0000':
        lab['text'] = "red"
    if i == '#ff7d00':
        lab['text'] = "orange"
    if i == '#ffff00':
        lab['text'] = "yellow"  
    if i == '#00ff00':
        lab['text'] = "green" 
    if i == '#007dff':
        lab['text'] = "light blue"     
    if i == '#0000ff':
        lab['text'] = "blue"  
    if i == '#7d00ff':
        lab['text'] = "purple"     
    if i == '#ff00ff':
        lab['text'] = "fuchsia"
lab.pack()    
e.pack()
root = Tk()

b1 = Button(text="", width=30, height=2)
b1.config(command=lambda: (change1('#ff0000')))
b1['bg'] = '#ff0000'
b1.pack()

b2 = Button(text="", width=30, height=2)
b2.config(command=lambda: (change1('#ff7d00')))
b2['bg'] = '#ff7d00'
b2.pack()

b3 = Button(text="", width=30, height=2)
b3.config(command=lambda: (change1('#ffff00')))
b3['bg'] = '#ffff00'
b3.pack()

b4 = Button(text="", width=30, height=2)
b4.config(command=lambda: (change1('#00ff00')))
b4['bg'] = '#00ff00'
b4.pack()

b5 = Button(text="", width=30, height=2)
b5.config(command=lambda: (change1('#007dff')))
b5['bg'] = '#007dff'
b5.pack()

b6 = Button(text="", width=30, height=2)
b6.config(command=lambda: (change1('#0000ff')))
b6['bg'] = '#0000ff'
b6.pack()

b7 = Button(text="", width=30, height=2)
b7.config(command=lambda: (change1('#7d00ff')))
b7['bg'] = '#7d00ff'
b7.pack()

b8 = Button(text="", width=30, height=2)
b8.config(command=lambda: (change1('#ff00ff')))
b8['bg'] = '#ff00ff'
b8.pack()

root.mainloop()