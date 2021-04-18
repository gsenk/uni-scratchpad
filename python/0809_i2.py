#!/usr/bin/python
# -*- encoding: utf-8 -*-

#drugi izpit 0809
#Naloga 1:
import re

def bijektivna(sez):
    '''Funkcije vrne True če je preslikava x1:f(x1), x2:f(x2),... bijektivna, sicer vrne
    false'''
    s=[]
    for i in sez:
        #print(s)
        print(i,'  ', sez[i])
        for j in range(len(s)):
            #print('===',s[j],' ',sez[i])
            if s[j]==sez[i]:
                #print('*')
                return False
        s.append(sez[i])
    #print(s)
    return True

u=bijektivna({1:2, 2:3, 3:5, 4:4, 5:1})
print(u)
print('')
v=bijektivna({1:2, 2:3, 3:5, 4:5, 5:1})
print(v)
