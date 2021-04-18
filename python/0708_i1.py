#!/usr/bin/python
# -*- encoding: utf-8 -*-

#Kolokvij 07/08 2. Naloga
#Gašper Šenk
def lab(sez,x1,y1,x2,y2):
    s=[] # da izpiše pot po kateri pridemo do cilja
    u=labirint(sez,s ,x1,y1,x2,y2)
    s.append([x2,y2])
    print(s)
    return u

def labirint(sez, s, x1, y1, x2, y2):
    '''Funkcija vrne True, če je možno priti od začetne točke, x1,y1 do x2,y2. Če ni možno
    vrne False. Dovoljene so poteze, levo desno, gor dol.'''
    s.append([x1,y1])# ne dela čist dobr k jih ne briše če zaide z poti
    sez[x1][y1]=2 #damo steno, da se ne more funkcija več prestavt nazaj po isti poti.
    if (x1+1==x2 and y1==y2) or (x1-1==x2 and y1==y2) or (x1==x2 and y1+1==y2) or (x1==x2 and y1-1==y2):
        return True
    if ( ((x1+2)<= len(sez)) and sez[x1+1][y1]==0):
        u=labirint(sez, s, x1+1,y1,x2,y2)
        if u==False:
            s.pop()
        else:
            return True
        
    if (((x1-1)>=0) and sez[x1-1][y1]==0):
        u=labirint(sez, s, x1-1,y1,x2,y2)
        if u==False:
            s.pop()
        else:
            return True
        
    if ( ((y1+2)<= len(sez[0])) and sez[x1][y1+1]==0):
        u=labirint(sez, s, x1,y1+1,x2,y2)
        if u==False:
            s.pop()
        else:
            return True
        
    if ( ((y1-1)>=0) and sez[x1][y1-1]==0):
        u=labirint(sez, s, x1,y1-1,x2,y2)
        if u==False:
            s.pop()
        else:
            return True
        
    return False

def izris(sez1):
    for u in sez1:
        print(u)
#0 pomeni da je polje prazno
#1 pomeni da je tam stena
a=[[0,0,1,1,1,1,0],
   [1,0,0,0,1,0,0],
   [0,1,0,0,1,1,0],
   [0,1,0,1,1,0,0],
   [0,1,0,0,0,0,1],
   [0,1,1,1,1,0,0],
   [0,0,0,0,0,0,1],
   [0,1,1,1,0,1,1]]

a1=[[0,0,0,0,0,0,0,0,0,0,0,0],
   [0,0,0,0,0,0,0,0,0,0,0,0],
   [0,0,0,0,0,0,0,0,0,0,0,0],
   [0,0,0,0,0,0,0,0,0,0,0,0],
   [0,0,0,0,0,0,0,0,0,0,0,0],
   [0,0,0,0,0,0,0,0,0,0,0,0],
   [1,0,0,0,0,0,0,0,0,0,0,0],
   [0,0,0,0,0,0,0,0,0,0,0,0],
   [0,0,0,0,0,0,0,0,0,0,0,0],
   [0,0,0,0,0,0,0,0,0,0,0,0],
   [0,0,0,0,0,0,0,0,0,0,0,0],
   [0,0,0,0,0,0,0,0,0,0,0,0],]

b=lab(a,2,0,0,0)
print(b)
izris(a)

