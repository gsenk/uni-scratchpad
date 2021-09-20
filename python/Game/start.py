#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
from tkinter import *
from math import *
import time
# igra by Gašper Šenk 2012

os.chdir(sys.path[0])

class Platforma():
    def __init__(self, Okno):
        '''skonstruiram celo igro z trigerji uvozi slik in booleani'''
        # izris canvasov
        self.canvas=Canvas(Okno, width=1400, height=750)
        self.canvas.grid(row=0, columnspan=2) #ker damo meni zgoraj
        
        self.stats=Canvas(Okno, width=1300, height=50)
        self.stats.grid(row=1, column=0) 
        self.casovni_okvir=Canvas(Okno, width=100, height=50)
        self.casovni_okvir.grid(row=1, column=1)

        # gradnja menija
        meni=Menu(Okno)
        meni.add_command(label="Restart", command=self.restart)
        meni.add_command(label="Score", command=lambda : Lestvica())# ker bi želel da se lestvica ohranja skozi resete igre
        meni.add_command(label="Izhod", command=Okno.destroy)
        Okno.config(menu=meni)

        #naloži slike
        self.trava=PhotoImage(file = "grafika/trava.gif")
        self.trava_p0=PhotoImage(file = "grafika/trava_pada0.gif")
        self.trava_p1=PhotoImage(file = "grafika/trava_pada1.gif")
        self.trava_p2=PhotoImage(file = "grafika/trava_pada2.gif")
        self.trava_p3=PhotoImage(file = "grafika/trava_pada3.gif")
        self.trava_p4=PhotoImage(file = "grafika/trava_pada4.gif")
        self.trava1=[self.trava_p0,self.trava_p1,self.trava_p2,self.trava_p3,self.trava_p4]
        self.stone=PhotoImage(file = "grafika/stone.gif")
        self.kljuc=PhotoImage(file = "grafika/dragonball.gif")
        self.vrata1=PhotoImage(file = "grafika/vrata1.gif")
        self.vrata2=PhotoImage(file = "grafika/vrata2.gif")
        self.grm=PhotoImage(file = "grafika/grm.gif")
        self.led=PhotoImage(file = "grafika/led.gif")
        self.igralecL=PhotoImage(file = "grafika/gokuL.gif")
        self.igralecD=PhotoImage(file = "grafika/gokuD.gif")
        self.nasprotnikL=PhotoImage(file = "grafika/nasprotnikL.gif")
        self.nasprotnikD=PhotoImage(file = "grafika/nasprotnikD.gif")
        self.umru=PhotoImage(file = "grafika/mrtu.gif")

        #komande ko pritisnem in spustim smerne tipke
        Okno.bind('<KeyPress>', self.pritisni)
        Okno.bind('<KeyRelease>', self.spusti)

        self.createGame()

    def createGame(self):
        # naredimo ozadje
        self.bg = PhotoImage(file="grafika/ozadje.gif")
        self.canvas.create_image(700, 375, image=self.bg)

        #začetna postavitev igralca
        self.igralecX=50
        self.igralecY=650
        self.orientacija=1        

        self.koda_d=False               # trigger če se premikam v desno
        self.koda_l=False               # trigger če se premikam v levo
        self.koda_skok=False            # trigger če sem v skoku, torej če se premikam navzor v zraku
        self.koda_padec=False           # trigger če padam v zraku, torej pod mano ni tal in sem zaključil skok
        self.vrata_funk_item=False      # povem da nisem še nikoli klical funkcije vrata da se bodo narisala samo enkrat
        self.nad_padajoco_travo=False   # indikator ki pove če je player nad padajočo travo
        self.znizaj_travo_item=False    # indikator če laufa funckcija za znižat travo
        self.casovni_korak_item=False   # indikator laufanja časa
        self.x=0
        self.num_kljuc=[]
        self.num_trava_pada=[]
        self.seznam_trava_pada_indeks=[] # seznam indeksov kako globoko je padla trava

        #================================================
        self.dolz_cas=60000
        self.dolz_cas_max=self.dolz_cas/1000
        self.score=0                    # rezultat zapisan v integer, ki ga damo v self.score_str
        self.score_str=StringVar()      # string kjer je zapisan score
        self.score_str.set(str(self.score))
        self.indeks_vrata=0             # spravljen katera vrata se prikažejo, 0 so vrata 2, 1 so vrata 1

        # nariši timer in score spodaj na desni
        self.stats.create_text(70, 25, text="Preostal čas:", fill="black", font=("Helvetica", "16")) 
        self.kvadrat_cas=self.stats.create_rectangle(150, 15, 150+self.dolz_cas/self.dolz_cas_max, 35, fill="blue")
        self.stats.create_text(1260, 25, text="Točke:", fill="black", font=("Helvetica", "16"))
        self.score_label=Label(self.stats, text='Preostal čas', width=200)
        self.bes=Label(self.casovni_okvir, textvariable=self.score_str, font=("Helvetica", 16)) # ne dela zravn nardisanih v canvas
        self.bes.grid(row=0, column=1)
        
        #velikost področja 
        self.meja_spod=700
        self.meja_levo=0
        self.meja_desno=1400


        self.Level1(); #narišemo level

        self.ori=1 # orientacija nasprotnika, začnem na levem bregu
        # začetne koordinate
        self.nasprotnikY=370
        self.nasprotnikX=500
        # narišem lik, s čimer se izognem errorju pri delete
        self.nasprotnik=self.canvas.create_image(self.nasprotnikX, self.nasprotnikY, image=self.nasprotnikD)

        # zaženem funkcije ki niso povezane zcasovni_korak in laufajo veliko redkeje
        self.odstevaj_cas();    # žačnem odštevat čas za timer
        self.vrata_funk();      # poženem funkcijo ki nariše vrata in če si pobral vse ključe začnejo utripat
        self.znizaj_travo();    # poženem funkcijo ki bo nižala travo v nekih normalnih presledkih
        self.casovni_korak();   # funkcija ki se zgodi vsako miliskundo in premika vse
        # postavim igralca na polje, da ne povzroča težav pri delete       
        self.igralec1=self.canvas.create_image(self.igralecX, self.igralecY, image=self.igralecD)

    def cancelPeriodicFunctions(self):
        self.stats.after_cancel(self.odstevaj_cas_item)    # končam odštevanje časa, kar omogoči igralcu približen ogled kako hiter je bil
        self.canvas.after_cancel(self.vrata_funk_item)      # ugasnem utripanje vrat
        self.canvas.after_cancel(self.znizaj_travo_item)    # neham gledat padanje trave
        self.canvas.after_cancel(self.casovni_korak_item)   # končam še vse premike ker se zdaj vse odvija v casovni_korak

        
#==============================================================================================
# restartiram celo seanso, a moral bi obržati rezultate naše igre

    def restart(self):
        '''gumb restart ponovno začne z igro'''
        print("restart")
        self.cancelPeriodicFunctions()
        self.canvas.delete(ALL)
        self.stats.delete(ALL)
        self.score_str.set("")
        self.createGame()
        
#==============================================================================================
# trigerji na tipkah

    def pritisni(self, event):
        '''dogodek ob pritisku smernih gumbov'''
        if event.keysym=="Right" and self.koda_d is False:
            self.koda_d=True
        if event.keysym=="Left" and self.koda_l is False:
            self.koda_l=True
        if event.keysym=="Up" and self.koda_skok is False and self.koda_padec is False:
            self.koda_skok=True
            
    def spusti(self,event):
        '''dogodek ob spustu smernih gumbov'''
        if event.keysym=="Right":
            if self.koda_d is not False:
                self.koda_d=False
        if event.keysym=="Left":
            if self.koda_l is not False:
                self.koda_l=False
                
#==============================================================================================
#==============================================================================================               
                
    def casovni_korak(self):
        ''' funkcija ki je master časa in se zgodi vsako milisekundo'''
            
        self.nasprotnik_igr()           # ugasnjen ker drugače se lik ne premika kot bi moral
        tla=self.tla()                  # ker želim samo enkrat jit čez vse v self.tla
        
        if tla and self.koda_skok is False:
            self.x=0
        if tla or self.koda_skok is True:
            self.koda_padec=False
        else:
            self.koda_padec=True
            
        if self.koda_padec==True:
            self.padaj()
        if self.koda_d==True:
            self.desno()
        if self.koda_l==True:
            self.levo()
        if self.koda_skok==True:        # and self.koda_padec is False:
            self.skoci()

        self.pobri_kljuc()              # začnem s statičnimi funkcijami ki se izvedejo vsak korak
        
        # vedno je treba pogledat če sem še živ
        if  self.uboj():
            self.canvas.delete(ALL)     # pobrišen canvas, ker bi rad na njem izpisal poraz
            self.canvas.create_image(700, 375, image=self.umru)
            self.slip(1)
            self.restart()
        else:
            self.casovni_korak_item=self.canvas.after(10,self.casovni_korak)

        if len(self.seznam_kljuc)==0:
            self.zmaga(); # ko poberem vse ključe zaženem funkcijo zmaga ki gleda če smo prišli v vrata
            
            
#==============================================================================================               
#==============================================================================================               
# navadne delujoče funkcije premikanja levo in desno

    def levo(self):
        ''' funkcija kjer se premakenm v levo stran'''
        self.orientacija=-1
        if not self.zaleti():
            self.canvas.delete(self.igralec1)
            self.igralec1=self.canvas.create_image(self.igralecX, self.igralecY, image=self.igralecL)
            self.igralecX+=-5
        
    def desno(self):
        ''' funkcija kjer se premakenm v desno stran'''
        self.orientacija=1
        if not self.zaleti():
            self.canvas.delete(self.igralec1)
            self.igralec1=self.canvas.create_image(self.igralecX, self.igralecY, image=self.igralecD)
            self.igralecX+=5
                      
#==============================================================================================
# slabo delujoč skok in malo boljši padec

    def skoci(self):
        '''funkcija skoka, pokriva tisti del ko se giralec dviga'''
        self.canvas.delete(self.igralec1)
        self.igralecY-=12*cos(self.x*2*pi/360)
        if self.orientacija==1:
            self.igralec1=self.canvas.create_image(self.igralecX, self.igralecY, image=self.igralecD)
        else:
            self.igralec1=self.canvas.create_image(self.igralecX, self.igralecY, image=self.igralecL)
        self.x+=5
            
        if self.x>=90 or self.strop():
            #print("prišli smo do vrha oz stropa")
            # prišli smo do vrha skoka oziroma zaleteli smo se v strop
            self.x=0 # resetiram , da bom lahko pri padanju normalno padal z sinusno funkcijo
            self.koda_skok=False
            self.koda_padec=True
                
    def padaj(self):
        '''padec igralca, padamo sinusno, ko pa presežem nek prag pa naprej linearno'''
        self.canvas.delete(self.igralec1)
        if self.x<90:
            self.igralecY+=12*sin(self.x*2*pi/360)
        else:
            self.igralecY+=12
        if self.orientacija==1:
            self.igralec1=self.canvas.create_image(self.igralecX, self.igralecY, image=self.igralecD)
        else:
            self.igralec1=self.canvas.create_image(self.igralecX, self.igralecY, image=self.igralecL)
        self.x+=5
              
#==============================================================================================
        
    def Level1(self):
        '''Zgradimo grafiko in elemente za celo sobo'''
        self.seznam_trava=[[25,725],[75,725],[125,725],[175,725],[225,725],[275,725],[325,725],[375,725],[425,725],
                      [475,725],[525,725],[575,725],[625,725],[675,725],[725,725],[775,725],[825,725],[875,725],
                      [925,725],[975,725],[1025,725],[1075,725],[1125,725],[1175,725],[1225,725],[1275,725],
                      [1325,725],[1375,725],[225,600],[275,600],[325,600],[375,600],
                      [425,600],[475,600],[525,600],[575,600],[625,600],
                      [675,600],[725,600],[775,600],[825,600],[875,600],[925,600],[975,600],[1025,600],[1075,600],[1125,600],
                      [1325,550],[1375,550],
                      [1325,450],[1375,450],
                      [375,425],[425,425],[475,425],[525,425],[575,425],[625,425],[675,425],[725,425],[775,425],[825,425],
                      [875,425],[925,425],[975,425],[1025,425],[1075,425],[1125,425],[1175,425],[1225,425],
                      [25,425],[75,425],[125,425],[175,425],
                      [25,300],[75,300],
                      [25,225],[75,225],[125,225],[175,225],[225,225],[275,225],[325,225],
                      [525,225],[575,225],[625,225],[675,225],[725,225],[775,225],[825,225],[875,225],
                      [1075,225],[1125,225],[1175,225],[1225,225],[1275,225],
                      [1325,225],[1375,225]]
        
        self.seznam_trava_pada=[[375,225],[425,225],[475,225],[925,225],[975,225],[1025,225]] #[425,600],[475,600],[525,600], spadajo v navadno travo
        # treba narest še enak dolg seznam indeksov kako globoko je trava padla
        for i in self.seznam_trava_pada:
            self.seznam_trava_pada_indeks.append(0)
        
        self.seznam=[[1175,550],[1225,550],[1275,550],[825,400],[875,400],[925,400],[975,400]]
        self.seznam_grm=[[625,550],[1025,375],[875,175],[1175,175]]
        self.seznam_led=[[325,25],[575,20]]
        self.seznam_smert=self.seznam_led+self.seznam_grm
        self.seznam_kljuc=[[1375,275],[925,175],[225,25],[575,75],[1375,25],[675,675]]
        # self.seznam_kljuc=[[675,675]]

        for i in range(len(self.seznam_trava)):
            self.canvas.create_image(self.seznam_trava[i][0], self.seznam_trava[i][1], image=self.trava)
        for i in range(len(self.seznam_trava_pada)):
            self.num_trava_pada.append(self.canvas.create_image(self.seznam_trava_pada[i][0], self.seznam_trava_pada[i][1], image=self.trava_p0))
        for i in range(len(self.seznam)):
            self.canvas.create_image(self.seznam[i][0], self.seznam[i][1], image=self.stone)    
        for i in range(len(self.seznam_grm)):
            self.canvas.create_image(self.seznam_grm[i][0], self.seznam_grm[i][1], image=self.grm)
        for i in range(len(self.seznam_led)):
            self.canvas.create_image(self.seznam_led[i][0], self.seznam_led[i][1], image=self.led)
        for i in range(len(self.seznam_kljuc)):
            self.num_kljuc.append(self.canvas.create_image(self.seznam_kljuc[i][0], self.seznam_kljuc[i][1], image=self.kljuc))
            
#==============================================================================================
# naslednje funkcije pogledajo robne pogoje, tore če se kak funkcija prekine zaradi tal, stropa ali robnih pogojev

    def zaleti(self):
        '''pogledamo če se zaletimo v rob polja, nato pa če se zaletime z leve ali desne v kamnite kocke'''
        if (self.igralecX+33>self.meja_desno and self.orientacija==1) or (self.igralecX-33<self.meja_levo and self.orientacija==-1):
            return True
        for i in range(len(self.seznam)):
            if (self.orientacija==1 and (self.seznam[i][0]-33-30<self.igralecX<self.seznam[i][0]-33-18 and
                self.seznam[i][1]-75<self.igralecY<self.seznam[i][1]+75)):
                self.igralecX=self.seznam[i][0]-33-25
            if (self.orientacija==-1 and (self.seznam[i][0]+33+18<self.igralecX<self.seznam[i][0]+33+30 and
                self.seznam[i][1]-75<self.igralecY<self.seznam[i][1]+70)):
                self.igralecX=self.seznam[i][0]+33+25
        return False
    
    
    def tla(self):
        '''Pogledamo če imamo pod nogami tla. Le ta so treh vrst: kamnita, travnata in podirajoča'''
        for i in range(len(self.seznam)): #kaminta tla
            if (self.seznam[i][0]-33-25<self.igralecX<self.seznam[i][0]+33+25 and
                self.seznam[i][1]-82<self.igralecY<self.seznam[i][1]-68):
                self.igralecY=self.seznam[i][1]-75
                #print("padli smo na kaminta tla")
                return True     
        for i in range(len(self.seznam_trava)): #travnata tla
            if (self.seznam_trava[i][0]-33-25<self.igralecX<self.seznam_trava[i][0]+33+25 and
                self.seznam_trava[i][1]-82<self.igralecY<self.seznam_trava[i][1]-68):
                self.igralecY=self.seznam_trava[i][1]-75
                #print("padli smo na travnata tla")
                return True
        for i in range(len(self.seznam_trava_pada)): # tla kjer se trava niža
            if (self.seznam_trava_pada[i][0]-33-25<self.igralecX<self.seznam_trava_pada[i][0]+33+25 and
                self.seznam_trava_pada[i][1]-82<self.igralecY<self.seznam_trava_pada[i][1]-68):
                self.igralecY=self.seznam_trava_pada[i][1]-75
                #print("padli smo na podirajoča tla")
                return True  
        return False
    
    def strop(self):
        '''Pogledamo če se zaletimo v strop, tokrat so veljavne le kamnite kocke'''
        for i in range(len(self.seznam)):
            if (self.seznam[i][0]-33-25<self.igralecX<self.seznam[i][0]+33+25 and self.seznam[i][1]+70<self.igralecY<self.seznam[i][1]+80):
                #print("Zaleteli smo se v strop")
                return True
        return False
    
#==============================================================================================               
# naslednje funkcije so možno povezane z casovni_korak in se izvedejo ob vsaki milisekndi prez presledka, tudi če ne držimo nobenega gumba

    def nasprotnik_igr(self):
        '''nasprotni igralec hodi od x=375 do 775 pri y=425 travnatih tilih. Sam je dimenzij 40x60'''
        if self.nasprotnikX >= 780: # prvo pogledam robne pogoje in uredim orientacijo
            self.ori=-1
        elif self.nasprotnikX <= 370:
            self.ori=1
        self.canvas.delete(self.nasprotnik)
        # ko smo zbrisali nasprotnika je čas da ga ponovno narišemo
        if self.ori==1:
            self.nasprotnik=self.canvas.create_image(self.nasprotnikX, self.nasprotnikY, image=self.nasprotnikD)
        else:
            self.nasprotnik=self.canvas.create_image(self.nasprotnikX, self.nasprotnikY, image=self.nasprotnikL)
        self.nasprotnikX+=self.ori*3
        
    def uboj(self):
        for i in range(len(self.seznam_smert)):
            if (self.seznam_smert[i][0]-33-25<self.igralecX<self.seznam_smert[i][0]+33+25 and
               self.seznam_smert[i][1]-70<self.igralecY<self.seznam_smert[i][1]+70):
                #print("ubil si se na prepreki")
                return True
            if (self.nasprotnikX-33-20<self.igralecX<self.nasprotnikX+33+20 and
               self.nasprotnikY-80<self.igralecY<self.nasprotnikY+80):
                #print("nasprotnik te je ubil")
                return True
        return False

    def pobri_kljuc(self):
        for i in range(len(self.seznam_kljuc)):
            if (self.seznam_kljuc[i][0]-33-25<self.igralecX<self.seznam_kljuc[i][0]+33+25 and
                self.seznam_kljuc[i][1]-75<self.igralecY<self.seznam_kljuc[i][1]+75):
                #print("kljuc pobran")
                self.seznam_kljuc.pop(i)
                self.canvas.delete(self.num_kljuc[i])
                self.num_kljuc.pop(i)
                self.score=self.score+50
                self.score_str.set(str(self.score))
                #print("KLJUČ JE ZBRISAN")
                print(self.score)
                break
        
#==============================================================================================
# funkcija odvisna samo od pobri_kljuc, ko so pobrani vsi ključi, potem se pogleda vsako 1milisekundo

    def zmaga(self):
        '''funkcija ki ko sem pobral vse ključe pogleda če sem prišel do končnih vrat'''
        if 1320<self.igralecX<1370 and 620<self.igralecY<700:   # koordiante vrat
            self.cancelPeriodicFunctions()
            self.score=self.score+floor(self.dolz_cas/100)      # vsakih 0.1s se šteje kot 1 točka, zaokrožimo navzdol do prvega integerja

            # v seznam dodam sedanji rezultat
            for indeks in range(len(seznam)):
                if self.score>seznam[indeks]:
                    if indeks==0:
                        seznam[indeks]=self.score
                    else:
                        seznam[indeks-1]=seznam[indeks]
                        seznam[indeks]=self.score
                else:
                    break
            
            
            # zmagovito ozadje
            self.bg_zmaga = PhotoImage(file="grafika/zmaga.gif")
            self.canvas.delete(ALL)                             # pobrišen canvas, ker bi rad na njem izpisal čestitko
            self.canvas.create_image(700, 375, image=self.bg_zmaga)
         
            self.canvas.create_text(700, 500, text='Čestitam, dosegel si '+str(self.score)+' točk', font=("Helvetica", "30"))  # čestitka

            # tuki je treba da mal pokaže score potem pa restarta
            self.slip(1)
            self.restart()
                    
#============================================================================================== 
# naslednje tri funkcije so neodvisne od našega časovnega koraka

    def odstevaj_cas(self):
        '''limita časovnega reševanja naloge'''
        if self.dolz_cas<=0:
            self.canvas.delete(ALL) # pobrišen canvas, ker bi rad na njem izpisal poraz
            self.canvas.create_image(700, 375, image=self.umru)
            self.slip(1)
            self.restart()
        else:
            self.dolz_cas=self.dolz_cas-1000
            self.stats.delete(self.kvadrat_cas)
            self.kvadrat_cas=self.stats.create_rectangle(150, 15, 150+self.dolz_cas/self.dolz_cas_max, 35, fill="blue")
            self.odstevaj_cas_item=self.stats.after(1000,self.odstevaj_cas)
            
    def vrata_funk(self):
        '''prvič postavim ena vrata, spreminjam jih ko poberem vse ključe'''
        if self.vrata_funk_item==False: # ker bi jih rad postavil samo enkrat
            self.vrata=self.canvas.create_image(1360, 645, image=self.vrata2)
        if len(self.seznam_kljuc)==0: # ko ni več ključev na prostem
            self.canvas.delete(self.vrata)
            self.indeks_vrata=(self.indeks_vrata+1)%2
            if self.indeks_vrata==0:
                self.vrata=self.canvas.create_image(1360, 645, image=self.vrata2)
            else:
                self.vrata=self.canvas.create_image(1360, 645, image=self.vrata1)
        self.vrata_funk_item=self.canvas.after(300,self.vrata_funk)
            
    def znizaj_travo(self):
        '''funkcija ki pogle vse točke zemlje nad katerimi stojimo in jih niža po določenem zakonu'''
        partial_sez=[]  # seznam vseh kosov trave za nižanje, da ni problema s prekratkim seznamom
        for i in range(len(self.seznam_trava_pada)):
            # tukaj vidim nad katerimi vsemi kvadrati se nahaja, največkrat se bo nad 2 možnostima
            if (self.seznam_trava_pada[i][0]-33-25<self.igralecX<self.seznam_trava_pada[i][0]+33+25 and
                self.seznam_trava_pada[i][1]-80<self.igralecY<self.seznam_trava_pada[i][1]-69):
                #print("ta trava bi morala past"+str(i)+" njen indeks = "+str(self.seznam_trava_pada_indeks[i]))
                # povečam padec trave za 1 stopnjo
                self.seznam_trava_pada_indeks[i]=self.seznam_trava_pada_indeks[i]+1
                # zbrišem prejšnjo sliko
                self.canvas.delete(self.num_trava_pada[i])
                if self.seznam_trava_pada_indeks[i]==5:
                    # poseben seznam rabim ker tukaj for zanka laufa čez cel seznam in mora priti do konca da lahko zniža vse elemente
                    partial_sez.append(i)
                else:
                    # tole vrjetn ne dela samo na konc p morm pripet določeno cifro med 0 in 4
                    self.num_trava_pada[i]=self.canvas.create_image(self.seznam_trava_pada[i][0], self.seznam_trava_pada[i][1]+5*self.seznam_trava_pada_indeks[i], image=self.trava1[self.seznam_trava_pada_indeks[i]])#image=self.trava_p+self.seznam_trava_pada_indeks[i])
                    
        partial_sez.sort()          # posortiram, ker gremo od večjih proti manjšim in se tako izognemo prekratkim seznamom
        partial_sez.reverse()
        for i in partial_sez:
            # če je trava padla že 5x pomeni da zgine in ne deluje več kot tla
            self.seznam_trava_pada_indeks.pop(i)    # indeks na kateri stopnji padanja je trava
            self.num_trava_pada.pop(i)              # objekt trave, da lahko deletam vsako posebej
            self.seznam_trava_pada.pop(i)           # trava v seznamu, ker nočem da še deluje kot legitimna tla
            
        self.znizaj_travo_item=self.canvas.after(300,self.znizaj_travo)

#==============================================================================================
# mal malih funkcij za konec

    def slip(self, var):
        time.sleep(var)
        pass
        
#==============================================================================================
# nov class, kjer naj bi se prikazovali rezultati in ne bi bil brisan vedno ko pritisnem restart

class Lestvica():
    ''' v točkah so zapisani seznami z podseznami imen in točk'''
    def __init__(self):
        scoreboard=Tk()
        scoreboard.title("Lestvica, of ALL time")
        self.canvas=Canvas(scoreboard, width=300, height=250)
        self.canvas.pack()

        for indeks in range(len(seznam)):
            standing=str(seznam[indeks])
            self.canvas.create_text(150, 225-50*indeks, text=standing, font=("Helvetica", "15"))
        scoreboard.mainloop()

#==============================================================================================
# začetna incializacija

okno=Tk()
seznam=[0,0,0,0,0]
Platforma(okno)
okno.title("Super Platformer over 9000")
okno.mainloop()
