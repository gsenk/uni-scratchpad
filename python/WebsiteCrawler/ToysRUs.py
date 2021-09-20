#!/usr/bin/env python
# -*- coding: utf-8 -*-

from lxml import etree
import urllib.request
import os, sys, csv, datetime, time
class inspectError(RuntimeError): pass

def harvest():
  global Q, K, num, ind, dat

  def inspect(ele):
    global Q, K, num, ind, dat
    html=etree.HTML(ele)
    
#==========================================================================================================
#==========================================================================================================

# naslov, ubistu ga ne rabim
    tit=html.xpath('//title')
    try:
      temp=tit[0].text.strip()
      title=temp[0:temp.find(' - ')]
    except Exception:
      print('*** ERROR title:',ind,item_link); raise inspectError()

# redna cena 
    price_redna=-1
    try:
      pr=ele.find('<li class="list">List Price:&nbsp;<span>&#036;')
      if pr>0:
        temp=ele[pr:pr+70].strip()
        price_redna=temp[temp.find('&#036;')+6:temp.find('</span>')]
    except Exception: pass

# spletna cena in prihranek v % in €
    price_spletna=-1
    prihranek_tot=-1
    prihranek_proc=-1
    try:
      pr=ele.find('<li class="retail">Our Price:&nbsp;<span>&#036;')
      if pr>0:
        temp=ele[pr:pr+70].strip()
        price_spletna=temp[temp.find('&#036;')+6:temp.find('</span>')]
    except Exception: pass
    if price_spletna!=-1 and price_redna!=-1:
      prihranek_tot=float(price_redna)-float(price_spletna)
      prihranek_proc=float(prihranek_tot)/float(price_redna)*100

# povprečna ocena
    ocena=-1
    try:
      oc=ele.find('<span class="pr-rating pr-rounded average">')
      if oc>0:
        temp=ele[oc+10:oc+70].strip()
        ocena=temp[temp.find('average">')+9:temp.find('</span>')].strip()
    except Exception: pass


# število mnenj
    mnenj_st=0
    try:
      oc_st=ele.find('<span class="count">')
      if oc_st>0:
        temp=ele[oc_st:oc_st+50].strip()
        mnenj_st=temp[temp.find('"count">')+8:temp.find('</span>')].strip()
    except Exception: pass

#==========================================================================================================
#==========================================================================================================

# Look for Similar Items by Subject (poglej tudi)
    start=ele.find('<div class=\\"mbOffCenterWrapper\\">')
    end=ele[start:].find('"!="null"))')
    poglejTudi = ele[start:start+end]
    html1=etree.HTML(poglejTudi)
    b = html1.xpath('//div/div/ul/li/a')
    L = []    
    for i in range(len(b)):
      #print(i)
      amID = b[i].attrib['href']
      start=amID.find('product/index.jsp?productId')
      end=amID.find('&')
      #end=amID.find('\\"')
      amID = amID[start:end]
      amID = "http://www.toysrus.com/"+amID
      if amID in K.keys():
        jnd = K[amID]
        #print(jnd)
      else:
        num += 1; jnd = num; K[amID] = num; Q.append(amID);
        #print("num "+num+"jnd "+jnd +"\n" )
        #print(amID)
      L.append(jnd)
    #print(Q)
   
    # shrani prebrano stran v file
    dat.writerow([ind,len(Q),len(K),item_link,title,
      price_redna,price_spletna,prihranek_tot,prihranek_proc,ocena,mnenj_st] + L)
    
## harvesting    
  while Q:
    item_link = Q.pop(0); ind = K[item_link]
    #print(K)
    if ind % 5 == 0:
      print("Harvesting status-> current id: %d, time: %s, on queue: %d, # of registered IDs: %d" %
            (ind,datetime.datetime.now().ctime(),len(Q),len(K)) )
    step = 0; delay = 2 # seconds
    while step < 3:
      try:
        with urllib.request.urlopen(item_link) as f:
          ele = f.read().decode('ISO-8859-1','replace'); break
      except Exception:
        step += 1; print('*** RETRY open:',step,ind,item_link)
        time.sleep(delay); delay = 2*delay
    if step>=3:
      print('*** ERROR open:',ind,item_link); continue
    if len(ele) < 500:
      print('*** ERROR length:',ind,item_link); continue
    try: inspect(ele)
    except inspectError: continue 

def HarvestData(csvName,saveName,visited):
  """Program HarvestData(csvName,saveName,visited) is harvesting data
from the desired website. The collected data are stored on csv file csvName.
To interrupt it use Ctrl-C. The data for resuming collection starting
at last item visited are stored on file saveName."""
  global Q, K, num, ind, dat
  print("ToysRUs data mining", datetime.datetime.now().ctime())
  try:
    save = open(saveName,'r')
    K = {}; Q = []
    for line in save.readlines():
      L = line.strip().split(" "); bk = L[1][1:-1]; bi = int(L[0]); K[bk] = bi
      if bi >= visited:
        Q.append(bk)
    save.close()
    print(visited,"/",len(K),"Item IDs read\n")
    newfile=False
  except:
    Q = ["http://www.toysrus.com/product/index.jsp?productId=12066411",
         "http://www.toysrus.com/product/index.jsp?productId=12652571",
         "http://www.toysrus.com/product/index.jsp?productId=2310754"]
    K = { k:(i) for (i,k) in enumerate(Q, start=1)}
    newfile=True
  num = len(K)    
  datFile = open(csvName,'a',newline='',encoding='utf-8') # 'a' ker bi radi da se file ne zbriše
  #vedno sam od sebe + radi bi dodali naslovno vrstico
  dat = csv.writer(datFile,delimiter=';',quotechar='"',quoting=csv.QUOTE_NONNUMERIC)
  if newfile:
    dat.writerow(["ind","len(Q)","len(K)","item_link","title","price_redna","price_spletna","prihranek_tot","prihranek_proc","ocena","stevilo_komentarjev"] + ["L"+str(i+1) for i in range(20)])
  
  try:
    harvest()
    print('\nMining finished', datetime.datetime.now().ctime())
  except Exception as e:
    print(e)
    print("\nMining interrupted", datetime.datetime.now().ctime(),
      "\n at item:", ind, "/", num)
  save=open(saveName,"w")
  for x in sorted(K,key=K.get): save.write(str(K[x])+' "'+x+'"\n')
  save.close()                                      
  datFile.close()

if __name__ == '__main__':
  if len(sys.argv)==4: HarvestData(sys.argv[1],sys.argv[2],sys.argv[3])
  else:
    HarvestData('PodatkiD.csv','save.dat',20314)      
else:
  HarvestData('PodatkiX.csv','saveX.dat',0)
