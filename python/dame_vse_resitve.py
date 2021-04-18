#!/usr/bin/python
# -*- encoding: utf-8 -*-

# Dame na sahovnici, poiscemo vse resitve

def dame(n, f):
    """Na sahovnico velikosti n x n postavi n sahovskih dam, tako da se ne napadajo.
    Poiscemo vse resitve. Vsakic, ko najdemo resitev r, izvedemo f(r)."""

    def ne_napadajo(r,i,j):
        """Ze razvrscene dame v seznamu r ne napadajo polja (i,j)."""
        for (u,v) in enumerate(r):
            # (u,v) je pozicija ene dame v seznamu r
            # (i,j) je polje, ki ga opazujemo
            if v == j or u == i or abs(u - i) == abs(v - j):
                return False
        return True
                
    def mozne_poteze(r):
        """V katere stolpce lahko postavimo naslednjo damo?"""
        i = len(r) # damo postavjamo v vrstico i
        return [j for j in range(n) if ne_napadajo(r,i,j)]
    
    def sestopaj(r):
        if len(r) == n:
            # Nasli smo resitev in jo obdelamo s funkcijo f
            f(r)
        else:
            for p in mozne_poteze(r):
                r.append(p) # dodamo potezo p
                sestopaj(r)
                r.pop()

    # zacetni klic
    sestopaj([])

def narisi(n, r):
    """Narisi dame na sahovnici velikosti n."""
    # Prazna sahovnica
    s = [[' .' for i in range(n)] for j in range(n)]
    # Postavimo dame
    for (u,v) in enumerate(r):
        s[u][v] = ' *'
    # Izpisemo
    for p in s:
        for c in p: print(c,end='')
        print()

# funkcija za izpis
def izpis(r):
    print ("Resitev:")
    narisi(n,r)
    print ()

# Glavni program
n = int(input ("Prosim vpisi velikost sahovnice: "))
dame(n, izpis)
