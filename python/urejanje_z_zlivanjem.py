#!/usr/bin/python
# -*- encoding: utf-8 -*-

import random

def merge_sort(sez):
    """urejamo z zlivanjem, torej nazačetku razdelimo seznam na 2 dela, potem pa jih
zlijemo tako da imenično jemljemo manjše elemente z enega ali drugega seznama.
Nakoncu naredimo nov seznam, ki je urejen"""
    n=len(sez)
    if n<=1:
        return sez

    levo = merge_sort(sez[:n//2])
    desno= merge_sort(sez[n//2:])
    
    #print("Leva stran = ", end='')
    #print(levo)
    #print("Desna stran  = ", end='')
    #print(desno)
    #print("kličem združi", end="")

    Nsez=zdruzi(levo, desno)
    #print(Nsez)
    return Nsez
    

def zdruzi(sezL,sezD):
    sez=[]
    i,j=0,0
    while (i+j)<(len(sezL)+len(sezD)):
        if j==len(sezD) or (i<len(sezL) and sezL[i]<sezD[j]):
            sez.append(sezL[i])
            i=i+1
        else:
            sez.append(sezD[j])
            j=j+1
    return sez

a = list(range(10,30))
random.shuffle(a)
print(a)
b=merge_sort(a)
print(b)
def merge_sort0(sez):
    merge_sort1(sez, 0, len(sez)-1)
    return

def merge_sort1(sez, prvi, zadnji):
    """urejamo z zlivanjem, torej nazačetku razdelimo seznam na 2 dela, potem pa jih
zlijemo tako da imenično jemljemo manjše elemente z enega ali drugega seznama.
UREDIMO PODAN SEZNAM"""
    #print(prvi,zadnji, (zadnji+prvi)//2)
    if zadnji-prvi<1:
        return
    #print("-", prvi, zadnji)
    merge_sort1(sez, prvi, (zadnji+prvi)//2)
    merge_sort1(sez, (zadnji+prvi)//2+1, zadnji)
    #print("*",prvi, (zadnji+prvi)//2)
    #print("+",(zadnji+prvi)//2+1, zadnji)
    zdruzi1(sez, prvi, zadnji, (zadnji+prvi)//2)
    return

def zdruzi1(sez, pr, zad, sred):
    sez1=[]
    i=pr
    j=sred+1
    #print(pr, zad, sred)
    #print(sez)
    #print(sez[i])
    #print(sez[j])
    while i<=sred and j<=zad:
        if(sez[i]<=sez[j]):
            sez1.append(sez[i])
            i += 1
        else:
            sez1.append(sez[j])
            j=j+1
    while i<= sred:
        sez1.append(sez[i])
        i= i+1
    while j<= zad:
        sez1.append(sez[j])
        j=j+1
    for k in range(0, zad-pr+1):
        sez[k+pr]=sez1[k]
    #print(sez1, end='   ')
    #print(sez[pr:zad+1])
    return

print("#================================================================================")
merge_sort0(a)
print(a)










