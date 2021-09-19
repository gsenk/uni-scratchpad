#!/usr/bin/env python
# -*- coding: utf-8 -*-

from tkinter import *
from tkinter import simpledialog
from tkinter import messagebox
import random
import time

class kviz():
    def __init__(self, glavnoOkno):
        self.glavno=glavnoOkno
        self.enacba=StringVar()
        self.cas=StringVar()
        self.pravilni_str=StringVar()
        #self.cas_num=10
        self.pravilni=0
        self.i=0
        self.rez=StringVar()
        
        self.okno=Frame(glavnoOkno)
        self.okno.pack(expand=True, padx=15, pady=15)
        self.bes=Label(self.okno, textvariable=self.enacba)
        self.bes.pack(side=LEFT)
        self.res=Entry(self.okno, textvariable=self.rez)
        self.res.pack(side=RIGHT)

        self.okno1=Frame(glavnoOkno)
        self.okno1.pack(anchor=S)
        self.cas_okno=Label(self.okno1, textvariable=self.cas)
        self.cas_okno.pack(side=RIGHT)
        self.cas_okno=Label(self.okno1, textvariable=self.pravilni_str)
        self.cas_okno.pack(side=LEFT)
        
        self.res.bind("<Return>", self.preveri)

        
        self.stevilo=simpledialog.askinteger("","Število enačb")
        self.cas_max=simpledialog.askinteger("","Čas reševanja enačb")
        self.klici_enacbe()
        
    def klici_enacbe(self):
        self.i=self.i+1
        self.rez.set("")
        self.pravilni_str.set(str(self.pravilni))
        print(self.i)
        print(self.stevilo)
        if (int(self.i)>=int(self.stevilo)):
            messagebox.showinfo("Rezultat", "Dosegli ste "+str(self.pravilni)+" število točk")
            self.glavno.destroy()
           # break
        self.dobi_enac()

    def dobi_enac(self):
        x=random.randint(0,100)
        y=random.randint(0,100-x)
        self.enacba.set(str(x)+"+"+str(y)+"=")
        self.rezultat=x+y
        self.cas_num=self.cas_max
        self.zmanjsaj_cas()

    def preveri(self, event=None):
        #print(self.rez.get())
        #print(self.rezultat)
        if (int(self.rez.get())==self.rezultat):
            self.pravilni=self.pravilni+1
        self.okno.after_cancel(self.koda)
        self.klici_enacbe()
        
    def zmanjsaj_cas(self):
        self.cas_num=self.cas_num-0.1
        self.cas.set("{:.1f}".format(self.cas_num))
        if (self.cas_num<=0):
            self.klici_enacbe()
        else:    
            self.koda=self.okno.after(100, self.zmanjsaj_cas)

  
okno=Tk()
kviz(okno)
okno.title("Kviz seštevanja")
okno.mainloop()
