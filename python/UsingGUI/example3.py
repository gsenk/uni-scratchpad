#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
from tkinter import *

os.chdir(sys.path[0])

class Pretvori():
    '''Sestavi program Album, ki bo omogočal pregledovanje vnaprej pripravljenih slik.

    1. V programu definiraj seznam, ki bo določal, katere slike bomo pregledovali.
    Elementi seznama naj bodo pari, sestavljeni iz imena datoteke, ki vsebuje sliko,
    in kratkega opisa slike. Slike pripravi v formatu PPM.

    2. Okno naj na vrhu vsebuje gumbe za navigacijo (prva, prejšnja, naslednja in
    zadnja slika), spodaj pa platno, na katerem bomo prikazovali sliko. Vsi gumbi
    naj bodo enako široki. Vsaka slika naj se prikaže na sredini platna.

    3. Program dopolni s poljem za izbiro slike. V njem naj bo vedno prikazan opis
    trenutno prikazane slike. Poskrbi, da se širina polja ne bo spremenila, ko
    bomo izbrali drugo možnost.

    4. Program dopolni tako, da bo reagiral na tipke s puščicami (levo=prejšnja,
    desno=naslednja, gor=prva, dol=zadnja).'''
    def __init__(self, glavnoOkno, seznam):
        self.canvas = Canvas(glavnoOkno, width=600, height=500)
        self.canvas.grid(row=1, column=0, columnspan=5)
        self.ime=[s[0] for s in seznam]
        self.opis=[s[1] for s in seznam]
        self.indeks=0

        self.var=StringVar(glavnoOkno)
        
        gumb1=Button(glavnoOkno, text="prva", command=self.prvi)
        gumb1.grid(row=0, column=0)
        gumb2=Button(glavnoOkno, text="prejšnja", command=self.prejsnji)
        gumb2.grid(row=0, column=1)
        # self.var ima notr shranjen opis slike ki ga python hoče spremniti pa napisat
        gumb3=OptionMenu(glavnoOkno, self.var, *self.opis, command=self.spremeni_sliko)
        gumb3.config(width=20)
        gumb3.grid(row=0, column=2)
        gumb4=Button(glavnoOkno, text="naslednja", command=self.naslednji)
        gumb4.grid(row=0, column=3)
        gumb5=Button(glavnoOkno, text="zadnja", command=self.zadnji)
        gumb5.grid(row=0, column=4)

        glavnoOkno.bind('<Left>', self.prejsnji)
        glavnoOkno.bind('<Right>', self.naslednji)
        glavnoOkno.bind('<Up>', self.zadnji)
        glavnoOkno.bind('<Down>', self.prvi)

        self.prvi()
    def spremeni_sliko(self, val):
        self.prikazi(self.opis.index(val))
        
    def prvi(self, Event=None):
        self.prikazi(0)
        
    def zadnji(self, Event=None):
        self.prikazi(len(self.ime)-1)
        
    def prejsnji(self, Event=None):
        if (self.indeks==0):
            self.prikazi(len(self.ime)-1)
        else:
            self.prikazi(self.indeks-1)
        
    def naslednji(self, Event=None):
        if (self.indeks==(len(self.ime)-1)):
            self.prikazi(0)
        else:
            self.prikazi(self.indeks+1)


    def prikazi(self, indeks):
        self.indeks=indeks
        self.var.set(self.opis[indeks])
        self.slike=PhotoImage(file=self.ime[self.indeks])
        self.canvas.create_image(300,250,image=self.slike)

seznam=[("example3/prva.ppm","broken image"), ("example3/druga.ppm", "dirkališče"), ("example3/tretja.ppm","cestar"), ("example3/cetrta.ppm","graf"), ("example3/peta.ppm","diski")]
    
okno=Tk()
Pretvori(okno, seznam)
okno.title("Album slik")
okno.mainloop()
