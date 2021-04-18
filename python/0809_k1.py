#!/usr/bin/python
# -*- encoding: utf-8 -*-

from math import *

class Toga():
    def __init__(self, x, y, alfa, b1,b2):
        self.x=x
        self.y=y
        self.alfa=alfa
        self.b1=b1
        self.b2=b2

    def kot(self):
        return self.alfa

def vsi_minimumi(sez):
    mi=[]
    k=0     #predstavlja da je manjši kot tisti pred njim
    for i in range(len(sez)-1): #zadnjega ne gledamo
        if sez[i]<=sez[i+1]:
            if k==0:
                mi.append(sez[i])
        if sez[i]<sez[i+1]:
            k=1
        else:
            k=0
    if k==0:# popravek za zadnji člen
        mi.append(sez[-1])
    print(k)
    return mi
print('')
print('')
a=[1,2,6,5,5,6,4,5,8]
print(vsi_minimumi(a))

def minimum(sez):
    k=0     #predstavlja da je manjši kot tisti pred njim
    for i in range(len(sez)-1): #zadnjega ne gledamo
        if sez[i]<=sez[i+1]:
            if k==0:
                return sez[i]
        if sez[i]<sez[i+1]:
            k=1
        else:
            k=0
    if k==0:
        return sez[-1]
    return False
print('')
print('')
a=[8,7,6,5,4,5,6,7]
print(minimum(a))

def podzaporedje(a,r):
    vse=[]
    def pod(a1,r,sez, vsota):
        if len(a1)==0:
            return 
        a=a1[:]
        s=sez[:]
        #print("*",vse)
        #print(vsota, s,'kar ostane od a',a)
        '''if len(s)!=0:
            if vsota%r==0:
                print(s)
                vse.append(s)'''
        vse.append(s)
                
        zadnji=a.pop()
        #print(vsota, vsota+zadnji)
        pod(a, r, s, vsota)
        s.append(zadnji)
        pod(a, r, s, vsota+zadnji)

    pod(a,r,[],0) # dobimo vse možne kombinacije 
    nas=[]
    for i in vse:
        print(i)
        vsot=0
        vmes=[]
        k=0
        for j in i:
            vsot+=j
            k=k+1
            vmes.append(j)
            if j==i[-1]:
                if vsot%r!=0:
                    k=0
        if k>len(nas):
            nas=vmes
    print('')
    return nas

print('')
print('')
a=[1,2,3,5,6,9]
print(podzaporedje(a,5))
    
