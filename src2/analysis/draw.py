import pylab as pl
import math
from scipy.interpolate import splev, splprep
import matplotlib.patches as mpathes
import matplotlib.pyplot as plt

fig,ax=pl.subplots()

height=20
width=20

fin=open("graph_area_2001_2006.txt","r")

drawed_vertex=set()
drawed_vertex_co=dict()

def getnextp(x,y,angle):
    oriangle=math.atan2(y,x)
    orilen=math.sqrt(x*x+y*y)
    return orilen*math.cos(oriangle-angle),orilen*math.sin(oriangle-angle)

basex=width/2
basey=height/2

startx=0
starty=height/3

def draw_p(name,val):
    global startx,starty
    realx=startx+basex
    realy=starty+basey
    drawed_vertex_co[name]=(startx,starty)
    
    vertex=mpathes.Circle((startx+basex,starty+basey),1)
    bx=pl.gca()
    bx.add_patch(vertex)
    plt.text(realx,realy,name+str(val),fontsize=17)
    

    startx,starty=getnextp(startx,starty,math.pi/5)

while True:
    segf=fin.readline().strip().split(' ')
    segt=fin.readline().strip().split(' ')
    val=fin.readline().strip()

    if val=='':
        break

    # 特别规则
    # area_list=['DB','AI','SE']
    # if segf[0] not in area_list or segt[0] not in area_list:
    #     continue



    # 特别规则
    if len(segf)>2:
        segf=[segf[0]+','+segf[1],segf[2]]
        segt=[segt[0]+','+segt[1],segt[2]]


    if segf[0] not in drawed_vertex:
        drawed_vertex.add(segf[0])
        draw_p(segf[0],int(segf[1]))
    
    if segt[0] not in drawed_vertex:
        drawed_vertex.add(segt[0])
        draw_p(segt[0],int(segt[1]))

    # 特别规则
    if int(val)<5000:
        continue

    fromx,fromy=drawed_vertex_co[segf[0]]

    if segf[0]==segt[0]:
        plt.text(fromx+basex,fromy+basey-0.5,val,fontsize=16)
        continue

    tox,toy=drawed_vertex_co[segt[0]]
    line=plt.Line2D((fromx+basex,tox+basex),(fromy+basey,toy+basey))
    ax.add_line(line)

    plt.text((fromx+(tox-fromx)*0.4)+basex,(fromy+(toy-fromy)*0.4)+basey,val,fontsize=16)
    




pl.xlim((0, width))
pl.ylim((0, height))

pl.show()