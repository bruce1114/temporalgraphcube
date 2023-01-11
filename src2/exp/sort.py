fin=open("view_size_summative_size_sort.txt","r")
fin.readline()
fin.readline()
fin.readline()

lines=fin.readlines()
lines=lines[:len(lines)-1]

d=dict()
i=0
while True:
    if i==len(lines): break
    line=lines[i].strip()
    theL=len(line)
    if theL not in d:
        d[theL]=list()

    d[theL].append((line,lines[i+1]))

    i=i+2

llist=list(d.keys())
llist.sort()

for theL in llist:
    for group in d[theL]:
        print(group[0])
        temp=group[1]
        temp=temp.strip().split()
        print(round(int(temp[0])/int(temp[1]),2))

