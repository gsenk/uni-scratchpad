#!/usr/bin/python
# -*- encoding: utf-8 -*-

from random import *
from operator import *
from math import *
import random
#Avtor: Gašper Šenk
    
#======================================================================================

def razdalja(a,b):
    '''vrne razdaljo med točkama na ravnini'''
    #na 0 mestu je x koordinata, na 1 pa y
    return sqrt((a[0]-b[0])**2+(a[1]-b[1])**2)

#======================================================================================

def zdruzi(sez1,sez2,clen):
    '''združi 2 seznama v novega po vrednosti na [clen] mestu v podtabeli'''
    sez=[]
    i,j=0,0
    while (i+j) < (len(sez1)+len(sez2)):
        if  j==len(sez2) or (i<len(sez1) and sez1[i][clen]<sez2[j][clen]):
            sez.append(sez1[i])
            i=i+1
        else:
            sez.append(sez2[j])
            j=j+1
    return sez

#======================================================================================

def najblizji_par1(sez, klic=0):
    '''vrne razdaljo in točki, med katerima je razdalja najmanjša'''
    st_tock=len(sez)
    #če prvič kličemo moramo urediti vse skupaj po x osi
    if klic==0:
        #print('*')
        if st_tock < 2: #če sta vnešeni manj kot 2 točki, razdalje ne morem računat
            return "Vnesti moraš vsaj dve točki"
        #posortiram po x.
        sez.sort()
    #print(".")    
    if st_tock > 3:
        #print('+')
        #kličem levo od razdelilnice brez točke na sredini
        dLevo,tockaLevo1,tockaLevo2, YtockeLevo=najblizji_par1(sez[:st_tock//2], 1)
        #kličem desno, s točko na sredini
        dDesno,tockaDesno1,tockaDesno2, YtockeDesno=najblizji_par1(sez[st_tock//2:], 1)
        #vračam razdaljo in obe točki
        if dLevo <= dDesno: #vrnem minimum od obeh strani in pripadajoci tocki
            d=dLevo
            tocka1=tockaLevo1
            tocka2=tockaLevo2
        else:
            d=dDesno
            tocka1=tockaDesno1
            tocka2=tockaDesno2
        #print(YtockeLevo)
        #print(YtockeDesno)
        #sredina je na sredini med zadnjo točko levo in prvo desno
        #sez je sortan po x, Ytocke pa po y
        sredina= (sez[(st_tock//2)-1][0]+sez[st_tock//2][0])/2
        ''' 
        #Zdaj morm pa pogledat sredino
        #prvo uredim levi in desni podseznam po y:
        #Rad bi naredil da pogleda točke samo na nasprotni strani in odneha ko je razlika
        #višin večja od d
        levo=sort(sez[:st_tock//2],0,st_tock//2,1)
        desno=sort(sez[st_tock//2:],0,st_tock-st_tock//2,1)
        #Pogleda sortirane sezname od spodaj navzor in izbere točke, ki so od 
        #sredine oddaljene za manj kot d
        sredinaTockeLevo= [i for i in levo if abs(i[0]-sredina)<d]
        sredinaTockeDesno= [i for i in desno if abs(i[0]-sredina)<d]
        #dobi najnižjo točko na obeh straneh
        tocka=min(sredinaTockeLevo[0][1],sredinaTockeDesno[0][1])
        for i in range("neki"):
            if tocka in sredinaTockeLevo:
        '''
        
        #urejeno združimo podsezname urejene po y
        Ytocke=zdruzi(YtockeLevo, YtockeDesno, 1);
        #print(d)
        #print(sredina)
        #print(Ytocke)
        #print(Ytocke)
        #zberemo točke, ki so od meje odaljene za manj kot d
        YsredinaTocke= [i for i in Ytocke if abs(i[0]-sredina)<d]
        #print(YsredinaTocke)

        #pogledamo vse točke razen končne
        for i in range(0,len(YsredinaTocke)-1):
            j=1
            #za vsako točkseo pogledamo naslednjih 5 točk po y osi, saj jih več ne more sploh bit
            #saj bi bil d manjši
            while i+j <len(YsredinaTocke) and j<6:
                #pogledamo če je razdalja manjša od sedaj znane
                if razdalja(YsredinaTocke[i],YsredinaTocke[i+j]) < d:
                    d=razdalja(YsredinaTocke[i],YsredinaTocke[i+j])
                    tocka1=YsredinaTocke[i]
                    tocka2=YsredinaTocke[i+j]
                #pogledamo če se točki razlikujeta po y osi za več kot d, potem se naslednja še več
                if abs(YsredinaTocke[i][1]-YsredinaTocke[i+j][1]) > d:
                    break
                j=j+1            
        return d, tocka1, tocka2, Ytocke
    else :
        if st_tock == 3:
            #najdemo najmanjšo razdaljo med tremi točkami
            d=min(razdalja(sez[0],sez[1]), razdalja(sez[1],sez[2]), razdalja(sez[0],sez[2]))
            sez.sort(key=itemgetter(1))
            for i in range (2): #pogledamo med katerima 2 točkama je razdalja najmanjša
                for j in range(i+1,3):
                    if d == razdalja(sez[i],sez[j]):
                        # vrnemo razdaljo in točki, ki sta najbližje, in tičke ureje po y
                        #print('*')
                        return d, sez[i], sez[j], sez
        else: #pogledamo razdaljo in jo vrnemo, skupaj z točkama
            d=razdalja(sez[0],sez[1])
            sez.sort(key=itemgetter(1))
            #print('.')
            return d, sez[0], sez[1], sez

#======================================================================================
       
def izpis_par1(s):
    d,tocka1,tocka2,sez=najblizji_par1(s)
    print("Najbljižja para sta",tocka1,"in",tocka2,", razdalja med njima je",d)
    return     

#======================================================================================

def najblizji_par2(Xsort,Ysort):
    '''deluje podobno kot "najblizji_par" ampak mu vnesemo sortana seznama in jih ne
    rabi urejevat sam'''
    st_tock=len(Xsort)
    if st_tock > 3:
        #razdelimo točke na 2 dela, po x osi
        Xsortlevo=Xsort[:st_tock//2]
        Xsortdesno=Xsort[st_tock//2:]
        sredina=(Xsortlevo[-1][0]+Xsortdesno[0][0])/2
        #zbrati moramo točke, ki so levo od sredino z tistih, ki so urejen po y
        Ysortlevo,Ysortdesno=[],[]
        for i in Ysort:
            if i[0]<=sredina:
                Ysortlevo.append(i)
            else:
                Ysortdesno.append(i)
        #kličemo na podseznamih
        dLevo,tocka1L,tocka2L=najblizji_par2(Xsortlevo,Ysortlevo)
        dDesno,tocka1D,tocka2D=najblizji_par2(Xsortdesno,Ysortdesno)
        #ni treba mergat, sam imamo že urejen seznam po y
        #zberemo tiste točke, ki so bližje
        if dLevo <= dDesno:
            d,tocka1,tocka2=dLevo,tocka1L,tocka2L
        else:
            d,tocka1,tocka2=dDesno,tocka1D,tocka2D
        #zberemo tiste, ki so blizu sredini
        YsredinaTocke= [i for i in Ysort if abs(i[0]-sredina)<d]
        for i in range(0,len(YsredinaTocke)-1):
            #za vsako točko pogledamo naslednjih 5
            j=1
            while j<=5 and j+i<len(YsredinaTocke):
                #pogledamo če je razdalja manjša od sedaj znane
                if razdalja(YsredinaTocke[i],YsredinaTocke[i+j]) < d:
                    d=razdalja(YsredinaTocke[i],YsredinaTocke[i+j])
                    tocka1=YsredinaTocke[i]
                    tocka2=YsredinaTocke[i+j]
                #pogledamo če se točki razlikujeta po y osi za več kot d, potem se naslednja še več
                if abs(YsredinaTocke[i][1]-YsredinaTocke[i+j][1]) > d:
                    break
                j=j+1
        return d,tocka1,tocka2                
    else:
        if st_tock == 3:
            #najdemo najmanjšo razdaljo med tremi točkami
            d=min(razdalja(Xsort[0],Xsort[1]), razdalja(Xsort[1],Xsort[2]), razdalja(Xsort[0],Xsort[2]))
            for i in range (2): #pogledamo med katerima 2 točkama je razdalja najmanjša
                for j in range(i+1,3):
                    if d == razdalja(Xsort[i],Xsort[j]):
                        # vrnemo razdaljo in točki, ki sta najbližje, in tičke ureje po y
                        return d, Xsort[i], Xsort[j]
        else: #pogledamo razdaljo in jo vrnemo, skupaj z točkama
            d=razdalja(Xsort[0],Xsort[1])
            return d, Xsort[0], Xsort[1]

#======================================================================================

def izpis_par2(sez):
    n=len(sez)
    #print('*')
    if n<=1:
        return "Vnesel si premalo točk"
    sez.sort()
    Xsort,Ysort=[],[]
    Xsort=copy(sez)
    #print('.')
    sez.sort(key=itemgetter(1))
    Ysort=copy(sez)
    #print(Xsort)
    #print('.')
    #print(Ysort)
    d,tocka1,tocka2=najblizji_par2(Xsort,Ysort)
    print("Najbljižja para sta",tocka1,"in",tocka2,", razdalja med njima je",d)
    return

#======================================================================================

def izpis(x):
    n=poskus1(x)
    print('Pametno reševanje na prvi način:')
    izpis_par1(n)
    print('Pametno reševanje na drug način:')
    izpis_par2(n)
    print('Reševanje s pregledovanjem vseh možnosti:')
    najblizji_par_slabo(n)
    
#======================================================================================

def copy(s):
    sez=[]
    for i in range(0,len(s)-1):
        sez.append(s[i])
    return sez

#======================================================================================

def najblizji_par_slabo(t):
    '''zračuna najbljižji par točk, tako da gre čez vse in zračuna najbljižjega'''
    d=razdalja(t[0],t[1])
    for i in range(0,len(t)-1):
        for j in range(i+1,len(t)):
            raz=razdalja(t[i],t[j])
            if(raz<d):
                par1=t[i]
                par2=t[j]
                d=raz
    print("Najbljižja para sta",par1,"in",par2,", razdalja med njima je",d)
    return 

#======================================================================================

def poskus(n):
    '''naredi seznam n-1 točk v ravnini'''
    a=list(range(0,n))
    b=list(range(0,n))
    random.shuffle(a)
    random.shuffle(b)
    s=[]
    for i in range(n):
        x=(a[i],b[i])
        s.append(x)
    return s

#======================================================================================

def poskus1(n):
    a = [(randint(0,n*100)/100,randint(0,n*100)/100) for i in range(n)]
    return a

#======================================================================================

izpis(1000)
