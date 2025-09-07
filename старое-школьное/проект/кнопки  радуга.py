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
    if i == '#7fffd4':
        lab['text'] = "aquamarine"           
lab.pack()    
e.pack()
root = Tk()

b1 = Button(text="", width=7, height=4)
b1.config(command=lambda: (change1('#ff0000')))
b1['bg'] = '#ff0000'
b1.pack(side=LEFT, padx=10, pady=10)

b2 = Button(text="", width=7, height=4)
b2.config(command=lambda: (change1('#ff7d00')))
b2['bg'] = '#ff7d00'
b2.pack(side=BOTTOM, padx=10, pady=10)

b3 = Button(text="", width=7, height=4)
b3.config(command=lambda: (change1('#ffff00')))
b3['bg'] = '#ffff00'
b3.pack(side=LEFT, padx=10, pady=10)

b4 = Button(text="", width=7, height=4)
b4.config(command=lambda: (change1('#00ff00')))
b4['bg'] = '#00ff00'
b4.pack(side=LEFT, padx=10, pady=10)

b5 = Button(text="", width=7, height=4)
b5.config(command=lambda: (change1('#7fffd4')))
b5['bg'] = '#7fffd4'
b5.pack(side=BOTTOM, padx=10, pady=10)


b6 = Button(text="", width=7, height=4)
b6.config(command=lambda: (change1('#007dff')))
b6['bg'] = '#007dff'
b6.pack(side=LEFT, padx=10, pady=10)

b7 = Button(text="", width=7, height=4)
b7.config(command=lambda: (change1('#0000ff')))
b7['bg'] = '#0000ff'
b7.pack(side=LEFT, padx=10, pady=10)

b8 = Button(text="", width=7, height=4)
b8.config(command=lambda: (change1('#7d00ff')))
b8['bg'] = '#7d00ff'
b8.pack(side=LEFT, padx=10, pady=10)

b9 = Button(text="", width=7, height=4)
b9.config(command=lambda: (change1('#ff00ff')))
b9['bg'] = '#ff00ff'
b9.pack(side=BOTTOM, padx=10, pady=10)

root.mainloop()