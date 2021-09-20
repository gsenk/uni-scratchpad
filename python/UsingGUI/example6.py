#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
from tkinter import *
from math import *

class krozenje():
    def __init__(self, glavnoOkno):
        self.canvas=Canvas(glavnoOkno, bg="black", width=600, height=600)
        self.canvas.pack()

        os.chdir(sys.path[0])
        
        self.sonce=PhotoImage(file = "example6/sonce.ppm")
        self.zemlja=PhotoImage(file = "example6/zemlja.ppm")
        self.luna=PhotoImage(file = "example6/luna.ppm")
        self.sun=self.canvas.create_image(300, 300, image=self.sonce)
        
        self.cas=0
        self.premik()
    def premik(self):
        if (self.cas!=0):
            self.canvas.delete(self.earth)
            self.canvas.delete(self.moon)
        self.cas=self.cas+0.01
        #[xze, yze]=self.canvas.coords(self.earth)
        #[xlu, ylu]=self.canvas.coords(self.moon)
        
        xze=300+200*cos(self.cas)
        yze=300-200*sin(self.cas)
        xlu=xze+50*cos(self.cas*12)
        ylu=yze+50*sin(self.cas*12)

        self.earth=self.canvas.create_image(xze, yze, image=self.zemlja)
        self.moon=self.canvas.create_image(xlu, ylu, image=self.luna)

        
        self.canvas.after(30, self.premik)

okno=Tk()
krozenje(okno)
okno.title("Kroženje zemlje in lune")
okno.mainloop()
