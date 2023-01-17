attlist=["pro","field","genre","isadult","averating","hotness"]
val=1
group=dict()
while val<(1<<6):
    needle=1
    needlepos=1
    curattlist=[]
    while needle<(1<<6):
        if (val&needle)!=0:
            curattlist.append(attlist[needlepos-1])

        needle=needle<<1
        needlepos=needlepos+1

    if len(curattlist) not in group:
        group[len(curattlist)]=list()
    group[len(curattlist)].append(curattlist)

    val=val+1

segslist=["1878 1880",
"1891 1895",
"1906 1910",
"1921 1925",
"1936 1940",
"1951 1955",
"1966 1970",
"1981 1985",
"1996 2000",
"2011 2015",
"1878 1883",
"1888 1898",
"1903 1913",
"1918 1928",
"1933 1943",
"1948 1958",
"1963 1973",
"1978 1988",
"1993 2003",
"2008 2018",
"1878 1888",
"1883 1903",
"1898 1918",
"1913 1933",
"1928 1948",
"1943 1963",
"1958 1978",
"1973 1993",
"1988 2008",
"2003 2023",
"1878 1903",
"1878 1918",
"1883 1933",
"1898 1948",
"1913 1963",
"1928 1978",
"1943 1993",
"1958 2008",
"1973 2023",
"1988 2028",
"1878 1928",
"1878 1943",
"1878 1958",
"1878 1973",
"1888 1988",
"1903 2003",
"1918 2018",
"1933 2028",
"1948 2028",
"1963 2028"]

fout=open("test_greedy_input.config","w")
repeatseglist=[]
for i in range(6,0,-1):
    repeat=8-i
    temprepeatlist=[]
    for segs in segslist:
        for j in range(repeat):
            temprepeatlist.append(segs)

    atts=group[i]
    for j in range(len(temprepeatlist)):
        temprepeatlist[j]=",".join(atts[j%len(atts)])+" "+temprepeatlist[j]+" 3 1"
    
    fout.write("\n".join(temprepeatlist)+"\n")