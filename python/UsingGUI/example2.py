#!/usr/bin/env python
# -*- coding: utf-8 -*-

from tkinter import *

class Risem():
    '''Sestavi aplikacijo Risalnik za prostoročno risanje. Če uporabnik
    drži pritisnjen levi gumb in premika miško, naj se riše krivulja,
    ki sledi njegovim gibom. V glavni menu dodaj izbiri "Končaj" in "Počisti".
    Prva zaključi aplikacijo, druga zbriše sliko. Primer uporabe:'''
    def __init__(self, glavnoOkno):
        self.canvas = Canvas(glavnoOkno, width=400, height=300)
        self.canvas.grid(row=1)
        meni=Menu(glavnoOkno)
        glavnoOkno.config(menu=meni)
        meni.add_command(label="Končaj", command=glavnoOkno.destroy)
        meni.add_command(label="Počsti", command=self.pocisti)        

        self.canvas.bind("<Button-1>", self.pritisk)
        self.canvas.bind("<B1-Motion>", self.risi)

        self.p_tockaX=''
        self.p_tockaY=''

    def pocisti(self):
        self.canvas.delete(ALL)
        #self.p_tockaX=''
        #self.p_tockaY=''        
    def pritisk(self, event): #event nosi vse novice kje je pritisk
        #print(event)
        self.p_tockaX=event.x
        self.p_tockaY=event.y

    def risi(self, event):
        self.canvas.create_line(self.p_tockaX, self.p_tockaY, event.x, event.y)
        self.p_tockaX=event.x
        self.p_tockaY=event.y

okno=Tk()
Risem(okno)
okno.title("Nekaj nariši")
okno.mainloop()
