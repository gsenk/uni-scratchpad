#!/usr/bin/env python
# -*- coding: utf-8 -*-

from tkinter import *
from math import *
import time
from datetime import*

class analogna_ura():
    def __init__(self, glavnoOkno):
        self.canvas=Canvas(glavnoOkno, width=200, height=200)
        self.canvas.grid(row=0, column=0)
        self.canvas.create_rectangle(0,0,200,200, fill="grey") #naredimo ozadje
        self.canvas.after(100, self.posodobi)
        self.sekunda=0
    def posodobi(self):
        if (self.sekunda!=0):
            self.canvas.delete(self.sekunda)
            self.canvas.delete(self.minuta)
            self.canvas.delete(self.ura)
        self.cas=datetime.now()
        self.ura=self.narisi(self.cas.hour*5, "red", 10)
        self.minuta=self.narisi(self.cas.minute, "blue", 5)
        self.sekunda=self.narisi(self.cas.second, "black", 2)
        self.canvas.after(1000, self.posodobi)
        
    def narisi(self, cas, barva, debelina):
        return self.canvas.create_line(100,100, 100+100*sin(radians(6*int(cas))), 100-100*cos(radians(6*int(cas))), fill=barva,  width=debelina)        
        print(100+100*sin(radians(6*int(cas))))
        print(100+100*cos(radians(6*int(cas))))
okno=Tk()
analogna_ura(okno)
okno.title("Trenutni čas")
okno.mainloop()
