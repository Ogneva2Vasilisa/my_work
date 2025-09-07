from tkinter import *
 
 
class Block:
    def __init__(self, master, func):
        self.ent1 = Entry(master, width=20, font =(' Times New Roman', 20, 'bold'))
        self.ent2 = Entry(master, width=20, font =(' Times New Roman', 20, 'bold'))
        self.but1 = Button(master, 
                          text="+",width=20, height=1, font =(' Times New Roman', 20, 'bold'), bg='red')
        self.but2 = Button(master,
                          text="-", width=20, height=1, font =(' Times New Roman', 20, 'bold'), bg='orange')
        self.but3 = Button(master,
                          text="*", width=20, height=1, font =(' Times New Roman', 20, 'bold'), bg='yellow')
        self.but4 = Button(master,
                          text="^", width=20, height=1, font =(' Times New Roman', 20, 'bold'), bg='green')
        self.but5 = Button(master,
                          text="/", width=20, height=1, font =(' Times New Roman', 20, 'bold'),  bg='lightblue')
        self.but6 = Button(master,
                          text="&", width=20, height=1, font =(' Times New Roman', 20, 'bold'),  bg='blue')
        self.but7 = Button(master,
                          text="xor", width=20, height=1, font =(' Times New Roman', 20, 'bold'), bg='purple')     
        
        self.lab = Label(master, width=20, height=2, font =(' Times New Roman', 20, 'bold'),
                         bg='black', fg='white')
        self.but1['command'] = getattr(self, 's')
        self.but2['command'] = getattr(self, 'r')
        self.but3['command'] = getattr(self, 'p')
        self.but4['command'] = getattr(self, 'st')
        self.but5['command'] = getattr(self, 'd')
        self.but6['command'] = getattr(self, 'bp')
        self.but7['command'] = getattr(self, 'xor')
        self.ent1.pack()
        self.ent2.pack()
        self.but1.pack()
        self.but2.pack()
        self.but3.pack()
        self.but4.pack()
        self.but5.pack()
        self.but6.pack()
        self.but7.pack()
        self.lab.pack()
 
    def s(self):
        a= self.ent1.get()
        b=self.ent2.get()
        if a.isdigit() and b.isdigit():
            self.lab['text']=str(a)+' + '+str(b)+' = '+str(int(a)+int(b))
        else:
            self.lab['text']='Ââåäèòå ÄÂÀ ×ÈÑËÀ!!!!!!!!!'
   
    def r(self):
        a= self.ent1.get()
        b=self.ent2.get()
        if a.isdigit() and b.isdigit():
            self.lab['text']=str(a)+' - '+str(b)+' = '+str(int(a)-int(b))
        else:
            self.lab['text']='Ââåäèòå ÄÂÀ ×ÈÑËÀ!!!!!!!!!'
            
    def p(self):
        a= self.ent1.get()
        b=self.ent2.get()
        if a.isdigit() and b.isdigit():
            self.lab['text']=str(a)+' * '+str(b)+' = '+str(int(a)*int(b))
        else:
            self.lab['text']='Ââåäèòå ÄÂÀ ×ÈÑËÀ!!!!!!!!!'            
 
    def st(self):
        a= self.ent1.get()
        b=self.ent2.get()
        if a.isdigit() and b.isdigit():
            self.lab['text']=str(a)+' ^ '+str(b)+' = '+str(int(a)**int(b))
        else:
            self.lab['text']='Ââåäèòå ÄÂÀ ×ÈÑËÀ!!!!!!!!!' 
            
    def d(self):
        a= self.ent1.get()
        b=self.ent2.get()
        if a.isdigit() and b.isdigit() and int(b)!=0:
            self.lab['text']=str(a)+' /'+str(b)+' = '+str(int(a)/int(b))
        elif int(b)==0:
            self.lab['text']='ERROR!!!!!!'
        else:
            self.lab['text']='Ââåäèòå ÄÂÀ ×ÈÑËÀ!!!!!!!!!'        
            
    def bp(self):
        a= self.ent1.get()
        b=self.ent2.get()
        if a.isdigit() and b.isdigit():
            self.lab['text']=str(a)+' & '+str(b)+' = '+str(int(a)&int(b))
        else:
            self.lab['text']='Ââåäèòå ÄÂÀ ×ÈÑËÀ!!!!!!!!!'      
            
    def xor(self):
        a= self.ent1.get()
        b=self.ent2.get()
        if a.isdigit() and b.isdigit():
            self.lab['text']=str(a)+' xor '+str(b)+' = '+str(int(a)^int(b))
        else:
            self.lab['text']='Ââåäèòå ÄÂÀ ×ÈÑËÀ!!!!!!!!!'             
            
            
 
root = Tk()
 
block= Block(root,'s')
 
root.mainloop()