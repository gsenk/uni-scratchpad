#!/usr/bin/python
# -*- encoding: utf-8 -*-

from tkinter import *
import random

# Gravitacija
g = 9.8 * 10  # 1 meter = 10 točk na zaslonu

# Preprost primer animacije s Tkinter.

class Krogla():
    '''Krogla, ki se premika v pravokotniku dane širine in višine.
    Začetna pozicija in hitrost sta izbrani naključno.'''

    def __init__(self, r, width, height, canvas):
        self.width = width
        self.height = height
        self.r = r
        # Začetna pozicija
        self.x = random.uniform(r, width - r)
        self.y = random.uniform(height/2 - r, height - r)
        # Začetna hitrost
        self.vx = random.uniform(-width/6, width/6)
        self.vy = random.uniform(-height/2, height/2)
        # Narišemo na zaslon
        self.canvas = canvas
        self.id = self.canvas.create_oval(self.x - self.r,
                                          self.y - self.r,
                                          self.x + self.r,
                                          self.y + self.r,
                                          fill = random.choice(["red", "green", "blue", "yellow", "purple"]))


    def premakni(self, dt):
        '''Izračunaj novo stanje krogle po preteku časa dt.'''
        self.x = self.x + self.vx * dt
        self.y = self.y + self.vy * dt
        self.vy = self.vy + g * dt        # os y kaže navzdol!
        # Preverimo odboje
        if self.x - self.r < 0:
            self.x = self.r
            self.vx = -self.vx
        if self.x + self.r > self.width:
            self.x = self.width - self.r
            self.vx = -self.vx
        if self.y - self.r < 0:
            self.y = self.r
            self.vy = -self.vy
        if self.y + self.r > self.height:
            self.y = self.height - self.r
            self.vy = -self.vy
        # Premaknemo na zaslonu
        self.canvas.coords(self.id,
                           self.x - self.r,
                           self.y - self.r,
                           self.x + self.r,
                           self.y + self.r)

class Boing():
    def __init__(self, master, width = 800, height = 300, n = 10):
        self.dt = 0.03
        self.canvas = Canvas(master, width=width, height=height)
        self.canvas.grid(row=0, column=0)
        self.krogla = [Krogla(random.uniform(10, 30), width, height, self.canvas) for i in range(n)]
        # Zaženemo animacijo
        self.animacija()

    def animacija(self):
        for k in self.krogla:
            k.premakni(self.dt)
        self.canvas.after(int(self.dt * 1000), self.animacija)
        

# Glavnemu oknu rečemo "root" (koren), ker so grafični elementi
# organizirani v drevo, glavno okno pa je koren tega drevesa

# Naredimo glavno okno
root = Tk()

root.title("Boing")

aplikacija = Boing(root, n=20)

# Kontrolo prepustimo glavnemu oknu. Funkcija mainloop neha
# delovati, ko okno zapremo.
root.mainloop()
