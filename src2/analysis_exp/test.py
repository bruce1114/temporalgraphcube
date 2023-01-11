from logging.config import valid_ident


def get_timevals(fin):
    vallist=[]
    for line in fin.readlines():
        segs=line.strip().split(' ')
        if len(segs)==0:
            continue

        val=int(segs[2])
        vallist.append(val)
    
    return vallist

fin1=open("testOutputCreateBaselineMax.txt","r")
fin2=open("testOutputCreateStableMax.txt","r")

vallist1=get_timevals(fin1)
vallist2=get_timevals(fin2)
ratiolist=[]

for i in range(len(vallist1)):
    if vallist2[i]==0: #临时措施
        ratiolist.append(vallist1[i])
    else:
        ratiolist.append(vallist1[i]/vallist2[i])

cnt=0
sumlist1=[]
sumlist2=[]
avelist=[]
sum1=0
sum2=0
ratiosum=0
for i in range(len(vallist1)):
    sum1=sum1+vallist1[i]
    sum2=sum2+vallist2[i]
    ratiosum=ratiosum+ratiolist[i]

    cnt=cnt+1
    if cnt%50==0:
        sumlist1.append(sum1)
        sumlist2.append(sum2)
        avelist.append(ratiosum/50)
        sum1=0
        sum2=0
        ratiosum=0

print(sumlist1)
print(sumlist2)
print(avelist)
