import matplotlib.pyplot as plt
import math

# name = ['graph size', 'kClist', 'PIVOTER','PEANUTS',  'DPColor', 'DPColorPath']
# name = ['MPC', 'MPC+', 'MQPC','MQPC+']
# name=['MQPCO-B','MQPCO-B+','MQPCO-E','MQPCO-E+']
name=['Random','MinLevel-$l_0=2$','MinLevel-$l_0=3$','MinLevel-$l_0=4$']
# col = ['magenta', "#5e3448",'black','blue','#20457c','coral']
# col = ['#003399', '#701112','#037365', '#0aa0d9','#e63f0a', '#bea32e']
col = ['#003399', '#0aa0d9','#e63f0a', '#bea32e']
# col = ['lightcoral', 'lightsteelblue', 'magenta', 'g', ]
# lj = [277.449512+16.146156, 2706.948,    1584.688, 6511916.0/1024,1605.584, 1606.284,]
# ber = [ 53.195760 +2.740924, 261.908,  294.568, 1234896.0/1024,299.068,301.136]        

# 4 4 5 clique
# dblp=[11480,12408,8353,9096]
# imdb=[65559,68550,77359,81911]
# hospital=[1170,717,87,48]
# lkml=[1500000+200000,1500000+200000,127435,113122]
# enron=[930145,1052619,24648,24480]

# 4 6 5 clique
# dblp=[7721,7783,6147,6335]
# imdb=[47552,46134,43822,44998]
# hospital=[40,61,27,40]
# lkml=[1000000+100000,1000000+100000,84629,71328]
# enron=[814691,592926,11005,9172]

# 4 6 5 core
# dblp=[7950,7981,6680,6853]
# imdb=[47420,45003,64455,65602]
# hospital=[41,63,28,42]
# lkml=[1000000+100000,1000000+100000,54025,41272]
# enron=[761204,566714,16101,13488]

data=[
    [591150,611916,600403,378592],
    [587477,589516,557325,330050],
    [596565,586713,523027,326943],
    [445001,585203,439161,190269],
    [393173,568459,394048,189310]
]


# dName = ['web-BerkStan','com-LiveJournal']
dName = ['k=6','k=8','k=10','k=12','k=14']
patterns = ['.', 'x', '\\', '//']#, 'O', '.']
fz = 23

plt.rcParams['pdf.fonttype'] = 42
plt.rcParams['ps.fonttype'] = 42
# plt.rcParams['font.sans-serif'] = ['Arial']  # 如果要显示中文字体,则在此处设为：SimHei
plt.rcParams['axes.unicode_minus'] = False  # 显示负号

plt.figure(figsize=(14, 6))
d = []
# for i in ber:
#     d.append(i)
# for i in range(1):
#     d.append(0)
# for i in lj:
#     d.append(i)
for sindata in data:
    for val in sindata:
        d.append(val/1000)
    d.append(170) #
d.pop()

# maxD = max(d)
plt.ylim([170, 700])
y_ticks=[200,300,400,500,600]
labellist=[]
for val in y_ticks:
    labellist.append(str(val))

plt.yticks(y_ticks,labellist,fontsize=fz)
# d.append(0.)
# d.append(0.)
# d.append(0.)
# d.append(0.)
# d.append(0.)
col.append('white')
patterns.append('')

bars = plt.bar(range(len(d)), d, width=1, color='white',  edgecolor=col, linewidth=2)
for bar in bars:
    height=bar.get_height()
    plt.text(bar.get_x(),height+20,str(round(height,1)),size=fz-11)

i = 0
for bar in bars:
    bar.set_hatch(patterns[i%len(col)])#
    if i < len(name):
    #     bar.set_hatch(patterns[i])#
        bar.set_label(name[i])#
    # else:
    #     bar.set_label(name[i - len(name)])
    i += 1
# for bar,mark,na in zip(bars, patterns, name):
#     bar.set_hatch(mark)
#     bar.set_label(na)
 
# plt.xlabel(dName[0]+"            "+dName[1]+"          "+dName[2]+"          "+dName[3]+"            "+dName[4], fontsize=fz)
x_ticks=[]
x_labels=dName
for i in range(len(bars)):
    if i%5==2:
        x_ticks.append(bars[i].get_x())


plt.xticks(x_ticks,x_labels,fontsize=fz)
plt.ylabel("Time (s)", fontsize=fz)

# plt.yticks(fontsize=fz)
# plt.legend(fontsize=fz)
# plt.legend(ncol=2)
plt.legend(ncol=4, prop={"size":fz,"weight":'bold'})
# leg = plt.gca().get_legend()
# ltext = leg.get_texts()
# plt.setp(ltext, fontsize=fz-4)

fig = plt.gcf()
plt.show()
fig.savefig('./minlevel.pdf', bbox_inches='tight', format='pdf') 
plt.close()
# 参数说明：

# left: 每一个柱形左侧的X坐标

# height:每一个柱形的高度

# width: 柱形之间的宽度

# bottom: 柱形的Y坐标

# color: 柱形的颜色