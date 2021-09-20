#!/usr/bin/env python
# -*- coding: utf-8 -*-

from tkinter import *

class BarvaOkna():
    '''Sestavi preprosto aplikacijo Semafor, ki ima tri gumbe "zelena",
    "rumena" in "rdeča" ter eno področje, ki se obarva s primerno barvo, ko
    pritisnemo na dani gumb. Na sliki je prikazan primer, ko smo pritisnili
    na gumb "zelena".'''
    def __init__(self, glavnoOkno):
        self.canvas = Canvas(glavnoOkno, bg="blue", width=500, height=300)
        self.canvas.grid(row=1,column=0, columnspan=3)
        
        gumb1= Button(glavnoOkno, text="Zelena", command = self.zelena)
        gumb1.grid(row=0,column=0)
        gumb1= Button(glavnoOkno, text="Rumena", command = self.rumena)
        gumb1.grid(row=0,column=1)
        gumb1= Button(glavnoOkno, text="Rdeča", command = self.rdeca)
        gumb1.grid(row=0,column=2)
        
    def zelena(self):
        self.nastavi("green")
    def rumena(self):
        self.nastavi("yellow")
    def rdeca(self):
        self.nastavi("red")
        
    def nastavi(self, barva):
        self.canvas.config(background=barva)
        #self.canvas.bg=barva
    
zunanji=Tk()
BarvaOkna(zunanji)
zunanji.title("okno")
zunanji.mainloop()
