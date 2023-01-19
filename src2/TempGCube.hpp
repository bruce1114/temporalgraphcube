#ifndef TEMPGCUBE_H
#define TEMPGCUBE_H

#include<unordered_set>
#include<unordered_map>
#include<string>
#include<fstream>
#include<vector>
#include<algorithm>
#include<iostream>
#include<sstream>
#include<sys/time.h>
#include<random>
#define ll long long
using namespace std;

const double EXP=1e-8;

void split(string victim,vector<string>& holder){
    int i=0,last=0;
    while(i<victim.size()){
        if(victim[i]==','){
            holder.push_back(string(victim.substr(last,i-last)));
            last=i+1;
        }
        ++i;
    }
    holder.push_back(string(victim.substr(last,victim.size()-last)));
}

int lowerbit(int x){
    return x&(-x);
}

int cNumber(int m,int n){
    int top=1;
    int mn=m-n;
    while(m>n){
        top*=m;
        m--;
    }
    int bot=1;
    while(mn>0){
        bot*=mn;
        mn--;
    }
    return top/bot;
}

bool cuboidIsAncestor(vector<int>& son,const vector<int>& father){
    if(son.size()>father.size()) return false;
    int last=0;
    for(int i=0;i<son.size();++i){
        while(last<father.size()&&son[i]!=father[last]){
            last++;
        }
        if(last==father.size()) return false;
        last++;
    }
    return true;
}

struct hashVectorString{
    size_t operator ()(const vector<string>& theKey)const{
        hash<string> hasher=hash<string>();
        size_t res=hasher(theKey[0]);
        for(int i=1;i<theKey.size();++i){
            res=res^hasher(theKey[i]);
        }
        return res;
    }
};

struct hashVectorInt{
    size_t operator()(const vector<int>& theKey)const{
        hash<int> hasher=hash<int>();
        size_t res=hasher(theKey[0]);
        for(int i=1;i<theKey.size();++i){
            res=res^hasher(theKey[i]);
        }
        return res;
    }
};

class VertexTabl{
public:
    int attriNum;
    vector<string> attriVec;
    vector<int> attriPosInOri;
    vector<vector<string> > dataList;
    vector<int> dataCount;
    unordered_map<vector<string>,int,hashVectorString> vecStringMp;

    bool initFrmFile(string vertexFileName);

    int insert(vector<string>& attri);
};

class Edge{
public:
    int count;
    double amount,maxv,minv;

    Edge();
    Edge(int ival,double fval);
};

struct EdgeSum{
    int count;
    double amount;

    EdgeSum(){
        count=0;
        amount=0;
    }

    EdgeSum(int ival,double fval){
        count=ival;
        amount=fval;
    }
};

struct EdgeExt{//暂时没有构造函数
    double maxv,minv;
    EdgeExt(){

    }
    EdgeExt(double fmax,double fmin){
        maxv=fmax;
        minv=fmin;
    }
};

void mergeEdge(Edge& a,Edge& b){//在本算法中保证了b.count>0
    if(a.count==0) a=b;
    else{
        a.count+=b.count;
        a.amount+=b.amount;
        a.maxv=max(a.maxv,b.maxv);
        a.minv=min(a.minv,b.minv);
    }
}

void mergeEdge(EdgeSum& a,Edge& b){
    a.count+=b.count;
    a.amount+=b.amount;
}

void mergeEdgeSum(Edge& a,Edge& b){
    a.count+=b.count;
    a.amount+=b.amount;
}

class SnapShot{
public:
    unordered_map<int,unordered_map<int,int> > edgeMatrix;
    vector<Edge> edgePool;
    vector<pair<int,int> > idPairVec;

    void insert(int aId,int bId,Edge& e);
    void insertSum(int aId,int bId,Edge& e);
    void static mergeSnapShot(SnapShot& a,SnapShot& b);
    void static mergeSnapShotSum(SnapShot& a,SnapShot& b);
};

class SnapShotSum{
public:
    unordered_map<int,unordered_map<int,int> > edgeMatrix;
    vector<EdgeSum> edgePool;
    vector<pair<int,int> > idPairVec;

    void insert(int aId,int bId,EdgeSum& e);
    void insert(int aId,int bId,Edge& e);
    void static mergeSnapShotSum(SnapShotSum& a,SnapShotSum& b);
    void static mergeSnapShot(SnapShotSum& a,SnapShot& b);
    void static copyFrmSnapShot(SnapShotSum& a,SnapShot& b);
    void static copyToSnapShot(SnapShotSum& a,SnapShot& b);
};

class SnapShotExt{
public:
    unordered_map<int,unordered_map<int,int> > edgeMatrix;
    vector<EdgeExt> edgePool;
    vector<pair<int,int> > idPairVec;

    void insert(int aId,int bId,EdgeExt& e);
    void insert(int aId,int bId,Edge& e);
    void static mergeSnapShotExt(SnapShotExt& a,SnapShotExt& b);
    void static mergeSnapShot(SnapShotExt& a,SnapShot& b);
    void static copyFrmSnapShot(SnapShotExt& a,SnapShot& b);
    void static copyToSnapShot(SnapShotExt& a,SnapShot& b);
};

struct TreeNode{
    int timel,timer;
    int lp,rp;
    SnapShotExt snapShotExt;

    TreeNode():lp(-1),rp(-1){}
};

struct CrossBoidRes{
    vector<string> aAttri,bAttri;
    SnapShot snap;
    VertexTabl aVertexTabl,bVertexTabl;

    void insert(vector<string>& aCollect,vector<string>& bCollect,Edge& insertE);
};

void CrossBoidRes::insert(vector<string>& aCollect,vector<string>& bCollect,Edge& insertE){
    int aId=aVertexTabl.insert(aCollect);
    int bId=bVertexTabl.insert(bCollect);
    snap.insert(aId,bId,insertE);
}

class TempGraph{
public:
    VertexTabl vertexTabl;
    vector<SnapShot> snapshotVec;
    int size;
    int base,end;

    //stable related
    vector<vector<SnapShotExt> > stable;
    
    //treeArr related
    vector<SnapShot> treeArr;
    
    //segment tree related
    vector<TreeNode> treeNodePool;
    int treeRoot;

    //prearray related
    vector<SnapShotSum> preArray;

    bool initFrmFile(string vertexFileName,string tempGraphFileName);
    void organizeEdgePool(vector<Edge>& tempEdgePool);
    void initFrmTempGraph(TempGraph& son,vector<int>& attriPosInSon,vector<int>& attriPosInOri);
    

    //treeArr related
    //snapshot中存在count为0的边，就不用展示了
    void buildTreeArr();
    void queryCount(int l,int r,SnapShot& ans);
    void queryCountWithoutMerge(int l,int r,SnapShot& ans);
    void queryAmount(int l,int r,SnapShot& ans);
    void queryMax(int l,int r,SnapShot& ans);
    double enumMax(int l,int r,int aId,int bId,bool& hasValid);
    void queryMin(int l,int r,SnapShot& ans);
    double enumMin(int l,int r,int aId,int bId,bool& hasValid);
    void extendTreeArr(int to);
    void insertToTreeArr(int mark,int aId,int bId,Edge& insertEdge);

    //stable related
    void buildStable();
    void buildInitPlace(int pos);
    void staQueryMax(int l,int r,SnapShotExt& ans);
    void extendStable(int to);
    void stableInsert(int mark,int aId,int bId,Edge& insertEdge);
    void fillStable();
    void fillSinglePlace(int pos);

    //segment tree related
    int buildSegmentTree(int timel,int timer);
    int getNewNode();
    void segQueryCount(int nodeMark,int timel,int timer,SnapShot& ans);
    void segQueryAmount(int nodeMark,int timel,int timer,SnapShot& ans);
    void segQueryMax(int nodeMark,int timel,int timer,SnapShotExt& ans);
    void segQueryMin(int nodeMark,int timel,int timer,SnapShotExt& ans);
    void extendSegTree(int toTime);
    void segInsert(int nodeMark,int timePos,int aId,int bId,Edge& insertEdge);
    void fillSegtree();

    //prearray related
    void buildPreArray();
    void preQueryAmount(int l,int r,SnapShotSum& ans);
    void extendPreArray(int to);
    void preArrayInsert(int mark,int aId,int bId,Edge& insertEdge);
    void fillPreArray();
    //暂不实现preQueryCount

    //baseline
    void baseQueryCount(int l,int r,SnapShotSum& ans);
    void baseQueryAmount(int l,int r,SnapShotSum& ans);
    void baseQueryMax(int l,int r,SnapShotExt& ans);
    void baseQueryMin(int l,int r,SnapShot& ans);

    void extend(int indexType,int to);
    void insert(int indexType,vector<string>& aAttri,vector<string>& bAttri,int theTime,double theVal);
};

class TempGCube{
public:
    static vector<int> logTable;
    vector<TempGraph> graphList;
    
    unordered_map<vector<string>,int,hashVectorString> materialized;
    unordered_map<vector<int>,ll,hashVectorInt> cuboidSize,cuboidDeSize;//记录size，底图size

    void init(string vertexFileName,string tempGraphFileName);
    void fullMaterialize();
    void createWithSimilarity(int theModel,int similarity);
    void selectMaterialize(vector<vector<int> >& mList);
    void partialMaterialize(int k);//k 是实例化的cuboid个数
    vector<vector<int>> partialGreedyOld(int k);
    vector<vector<int>> partialGreedyNew(int k);
    void recordMaterialize();
    bool isAncestor(vector<int>& attriPosList,int possibleSon);
    void buildGraphByExist(vector<int>& attriPosList,int existSon);
    bool cmp(int a,int b){
        return graphList[a].size>graphList[b].size;
    }

    //stable related
    void buildLogTable();

    //segment tree related
    void buildSegmentTree();

    //treeArr related
    void buildIndex(int indexType);
    void buildEmptyIndex(int indexType);
    void fillIndex(int indexType);
    //attributes务必和graph[0]的attri顺序一致
    //querytype__0:count,1:sum,2:ave,3:max,4:min
    //indexType: 0-base,1-treearr,2-segmenttree
    //return: 0 被实例化，1 未被实例化
    int query(int indexType,int timel,int timer,int type,bool returnAns,vector<string>& attributes,int greedyType,TempGraph& ans);
    void queryWithTarget(TempGraph& targetGraph,SnapShot& ansSnapShot,bool returnAns,int indexType,int timel,int timer,int type);
    void crossQuery(vector<string>& aAttributes,vector<string>& bAttributes,int indexType,int timel,int timer,int type,CrossBoidRes& ans);

    // or append，一次一条边，且同一个
    //暂时不考虑删边的情况
    void insert(int indexType,vector<string>& aAttri,vector<string>& bAttri,int theTime,double theVal);
};

void TempGraph::preQueryAmount(int l,int r,SnapShotSum& ans){
    // ans=preArray[r];

    SnapShotSum& minus=preArray[l-1];
    // for(int i=0;i<minus.edgePool.size();++i){
    //     int aId=minus.idPairVec[i].first;
    //     int bId=minus.idPairVec[i].second;
    //     int eIdInAns=ans.edgeMatrix[aId][bId];
    //     ans.edgePool[eIdInAns].count-=minus.edgePool[i].count;
    //     ans.edgePool[eIdInAns].amount-=minus.edgePool[i].amount;
    // }
    
    for(int i=0;i<preArray[r].edgePool.size();++i){
        int aId=preArray[r].idPairVec[i].first;
        int bId=preArray[r].idPairVec[i].second;
        EdgeSum insertEdge=preArray[r].edgePool[i];
        if(minus.edgeMatrix.find(aId)!=minus.edgeMatrix.end()&&minus.edgeMatrix[aId].find(bId)!=minus.edgeMatrix[aId].end()){
            int eIdInMinus=minus.edgeMatrix[aId][bId];
            insertEdge.count-=minus.edgePool[eIdInMinus].count;
            insertEdge.amount-=minus.edgePool[eIdInMinus].amount;
        }
        
        if(insertEdge.count>0) ans.insert(aId,bId,insertEdge);
    }
}

void mergeEdgeSum(EdgeSum& a,EdgeSum& b){
    a.count+=b.count;
    a.amount+=b.amount;
}

void mergeEdgeExt(EdgeExt& a,EdgeExt& b){
    a.maxv=(a.maxv>b.maxv?a.maxv:b.maxv);
    a.minv=(a.minv>b.minv?b.minv:a.minv);
}

void mergeEdgeExt(EdgeExt& a,Edge& b){
    a.maxv=(a.maxv>b.maxv?a.maxv:b.maxv);
    a.minv=(a.minv>b.minv?b.minv:a.minv);
}

void SnapShotSum::insert(int aId,int bId,Edge& e){
    if(edgeMatrix.find(aId)==edgeMatrix.end()||edgeMatrix[aId].find(bId)==edgeMatrix[aId].end()){
        edgePool.push_back(EdgeSum());
        edgeMatrix[aId][bId]=edgeMatrix[bId][aId]=edgePool.size()-1;
        idPairVec.push_back(pair<int,int>(aId,bId));
    }

    EdgeSum& curEdgeSum=edgePool[edgeMatrix[aId][bId]];
    mergeEdge(curEdgeSum,e);
}

void SnapShotExt::insert(int aId,int bId,Edge& e){
    if(edgeMatrix.find(aId)==edgeMatrix.end()||edgeMatrix[aId].find(bId)==edgeMatrix[aId].end()){
        edgePool.push_back(EdgeExt(e.maxv,e.minv));
        edgeMatrix[aId][bId]=edgeMatrix[bId][aId]=edgePool.size()-1;
        idPairVec.push_back(pair<int,int>(aId,bId));
    }else{
        EdgeExt& curEdgeExt=edgePool[edgeMatrix[aId][bId]];
        mergeEdgeExt(curEdgeExt,e);
    }
}

void SnapShotExt::insert(int aId,int bId,EdgeExt& e){
    if(edgeMatrix.find(aId)==edgeMatrix.end()||edgeMatrix[aId].find(bId)==edgeMatrix[aId].end()){
        edgePool.push_back(e);
        edgeMatrix[aId][bId]=edgeMatrix[bId][aId]=edgePool.size()-1;
        idPairVec.push_back(pair<int,int>(aId,bId));
    }else{
        EdgeExt& curEdgeExt=edgePool[edgeMatrix[aId][bId]];
        mergeEdgeExt(curEdgeExt,e);
    }
}

void SnapShotSum::insert(int aId,int bId,EdgeSum& e){
    if(edgeMatrix.find(aId)==edgeMatrix.end()||edgeMatrix[aId].find(bId)==edgeMatrix[aId].end()){
        edgePool.push_back(EdgeSum());
        edgeMatrix[aId][bId]=edgeMatrix[bId][aId]=edgePool.size()-1;
        idPairVec.push_back(pair<int,int>(aId,bId));
    }

    EdgeSum& curEdgeSum=edgePool[edgeMatrix[aId][bId]];
    mergeEdgeSum(curEdgeSum,e);
}

void SnapShotSum::mergeSnapShot(SnapShotSum& a,SnapShot& b){
    for(int i=0;i<b.edgePool.size();++i){
        int aId=b.idPairVec[i].first;
        int bId=b.idPairVec[i].second;
        a.insert(aId,bId,b.edgePool[i]);
    }
}

void SnapShotExt::mergeSnapShotExt(SnapShotExt& a,SnapShotExt& b){
    for(int i=0;i<b.edgePool.size();++i){
        int aId=b.idPairVec[i].first;
        int bId=b.idPairVec[i].second;
        a.insert(aId,bId,b.edgePool[i]);
    }
}

void SnapShotExt::mergeSnapShot(SnapShotExt& a,SnapShot& b){
    for(int i=0;i<b.edgePool.size();++i){
        int aId=b.idPairVec[i].first;
        int bId=b.idPairVec[i].second;
        a.insert(aId,bId,b.edgePool[i]);
    }
}

void SnapShotSum::mergeSnapShotSum(SnapShotSum& a,SnapShotSum& b){
    for(int i=0;i<b.edgePool.size();++i){
        int aId=b.idPairVec[i].first;
        int bId=b.idPairVec[i].second;
        a.insert(aId,bId,b.edgePool[i]);
    }
}

void SnapShotSum::copyToSnapShot(SnapShotSum& a,SnapShot& b){
    b.edgeMatrix=a.edgeMatrix;
    vector<EdgeSum>& sourceEdgePool=a.edgePool;
    b.edgePool.resize(sourceEdgePool.size());
    vector<Edge>& targetEdgePool=b.edgePool;

    for(int i=0;i<sourceEdgePool.size();++i){
        targetEdgePool[i].count=sourceEdgePool[i].count;
        targetEdgePool[i].amount=sourceEdgePool[i].amount;
    }

    b.idPairVec=a.idPairVec;

}

void SnapShotExt::copyToSnapShot(SnapShotExt& a,SnapShot& b){
    b.edgeMatrix=a.edgeMatrix;
    vector<EdgeExt>& sourceEdgePool=a.edgePool;
    b.edgePool.resize(sourceEdgePool.size());
    vector<Edge>& targetEdgePool=b.edgePool;

    for(int i=0;i<sourceEdgePool.size();++i){
        targetEdgePool[i].maxv=sourceEdgePool[i].maxv;
        targetEdgePool[i].minv=sourceEdgePool[i].minv;
        targetEdgePool[i].count=1;//保证合法
    }

    b.idPairVec=a.idPairVec;
}

void SnapShotExt::copyFrmSnapShot(SnapShotExt& a,SnapShot& b){
    a.edgeMatrix=b.edgeMatrix;
    vector<Edge>& sourceEdgePool=b.edgePool;
    a.edgePool.resize(sourceEdgePool.size());
    vector<EdgeExt>& targetEdgePool=a.edgePool;

    for(int i=0;i<sourceEdgePool.size();++i){
        targetEdgePool[i].maxv=sourceEdgePool[i].maxv;
        targetEdgePool[i].minv=sourceEdgePool[i].minv;
    }

    a.idPairVec=b.idPairVec;
}

void SnapShotSum::copyFrmSnapShot(SnapShotSum& a,SnapShot& b){
    a.edgeMatrix=b.edgeMatrix;
    vector<Edge>& sourceEdgePool=b.edgePool;
    a.edgePool.resize(sourceEdgePool.size());
    vector<EdgeSum>& targetEdgePool=a.edgePool;

    for(int i=0;i<sourceEdgePool.size();++i){
        targetEdgePool[i].count=sourceEdgePool[i].count;
        targetEdgePool[i].amount=sourceEdgePool[i].amount;
    }

    a.idPairVec=b.idPairVec;
}

void TempGraph::buildInitPlace(int pos){
    stable[pos].push_back(SnapShotExt());
    SnapShotExt& firstOne=stable[pos][0];
    SnapShotExt::copyFrmSnapShot(firstOne,snapshotVec[pos]);

    int range=2;
    for(int i=1;pos-range+1>=1;++i){
        int halfRange=(range>>1);
        int helperPos=pos-halfRange;
        stable[pos].push_back(stable[pos][i-1]);
        SnapShotExt::mergeSnapShotExt(stable[pos][i],stable[helperPos][i-1]);

        range=(range<<1);
    }
}

void TempGraph::buildStable(){
    if(1>end) return;
    stable.resize(end+1);

    for(int i=1;i<=end;++i){
        buildInitPlace(i);
        //test
        // cout<<i<<endl;
    }
}

void TempGraph::staQueryMax(int l,int r,SnapShotExt& ans){
    int rangeLen=r-l+1;
    if(rangeLen-lowerbit(rangeLen)==0){
        ans=stable[r][TempGCube::logTable[rangeLen]];
    }else{
        int logVal=TempGCube::logTable[rangeLen];
        ans=stable[r][logVal];
        int mVal=(1<<logVal);
        SnapShotExt::mergeSnapShotExt(ans,stable[l+mVal-1][logVal]);
    }
}

void TempGraph::extendStable(int to){
    stable.resize(to+1);
    for(int i=end+1;i<=to;++i){
        buildInitPlace(i);
    }
}

void TempGraph::stableInsert(int mark,int aId,int bId,Edge& insertEdge){
    for(int i=0;i<stable[mark].size();++i){
        stable[mark][i].insert(aId,bId,insertEdge);
    }
}

//stable[pos][0]搞定后使用，填好stable[pos][1]及以后
void TempGraph::fillSinglePlace(int pos){
    int range=2;
    for(int i=1;pos-range+1>=1;++i){
        int halfRange=(range>>1);
        int helperPos=pos-halfRange;
        stable[pos].push_back(stable[pos][i-1]);
        SnapShotExt::mergeSnapShotExt(stable[pos][i],stable[helperPos][i-1]);

        range=(range<<1);
    }
}

//一个一个添加某tempgraph的边到其stable中
void TempGraph::fillStable(){
    int totalEdge=size;
    int edgeMilestone=totalEdge/10;
    int edgePart=edgeMilestone;
    int edgecnt=0;
    timeval starttime,endtime;

    cout<<"stable insert start"<<endl;
    gettimeofday(&starttime,NULL);
    for(int i=0;i<snapshotVec.size();++i){
        for(int j=0;j<snapshotVec[i].edgePool.size();++j){
            int aId=snapshotVec[i].idPairVec[j].first;
            int bId=snapshotVec[i].idPairVec[j].second;

            if(i>=stable.size()){
                for(int k=stable.size();k<i;++k){
                    stable.push_back(vector<SnapShotExt>());
                    stable[k].push_back(SnapShotExt());
                    fillSinglePlace(k);
                }
                stable.push_back(vector<SnapShotExt>());
                stable[i].push_back(SnapShotExt());
                
                stable[i][0].insert(aId,bId,snapshotVec[i].edgePool[j]);
                fillSinglePlace(i);
            }else{
                for(int k=0;k<stable[i].size();++k){
                    stable[i][k].insert(aId,bId,snapshotVec[i].edgePool[j]);
                }
            }

            edgecnt++;
            if(edgecnt==edgeMilestone){
                gettimeofday(&endtime,NULL);
                cout<<"part time: "<<(endtime.tv_sec-starttime.tv_sec)*1000+(endtime.tv_usec-starttime.tv_usec)/1000<<endl;
                
                //统计size
                ll stablesize=0;
                for(int k=0;k<stable.size();++k){
                    for(int l=0;l<stable[k].size();++l){
                        stablesize+=stable[k][l].edgePool.size();
                    }
                }
                cout<<"till stable size: "<<stablesize<<endl;
                
                // starttime=endtime;
                gettimeofday(&starttime,NULL);
                edgeMilestone+=edgePart;
            }
        }
    }
    cout<<"stable insert end"<<endl;
}

//一个一个添加某tempgraph的边到其stree中
void TempGraph::fillSegtree(){
    int totalEdge=size;
    int edgeMilestone=totalEdge/10;
    int edgePart=edgeMilestone;
    int edgecnt=0;
    timeval starttime,endtime;

    treeRoot=getNewNode();
    treeNodePool[treeRoot].timel=0+base;
    treeNodePool[treeRoot].timer=0+base;

    cout<<"segtree insert start"<<endl;
    gettimeofday(&starttime,NULL);
    for(int i=0;i<snapshotVec.size();++i){
        for(int j=0;j<snapshotVec[i].edgePool.size();++j){
            int aId=snapshotVec[i].idPairVec[j].first;
            int bId=snapshotVec[i].idPairVec[j].second;

            if(i+base>treeNodePool[treeRoot].timer){
                extendSegTree(i+base);
            }
            segInsert(treeRoot,i+base,aId,bId,snapshotVec[i].edgePool[j]);

            edgecnt++;
            if(edgecnt==edgeMilestone){
                gettimeofday(&endtime,NULL);
                cout<<"part time: "<<(endtime.tv_sec-starttime.tv_sec)*1000+(endtime.tv_usec-starttime.tv_usec)/1000<<endl;
                
                //统计size
                ll segsize=0;
                for(int k=0;k<treeNodePool.size();++k){
                    segsize+=treeNodePool[k].snapShotExt.edgePool.size();
                }
                cout<<"till segtree size: "<<segsize<<endl;
                
                // starttime=endtime;
                gettimeofday(&starttime,NULL);
                edgeMilestone+=edgePart;
            }
        }
    }
    cout<<"segtree insert end"<<endl;
}

void TempGraph::fillPreArray(){
    int totalEdge=size;
    int edgeMilestone=totalEdge/10;
    int edgePart=edgeMilestone;
    int edgecnt=0;
    timeval starttime,endtime;
    
    cout<<"prearray insert start"<<endl;
    gettimeofday(&starttime,NULL);
    for(int i=0;i<snapshotVec.size();++i){
        for(int j=0;j<snapshotVec[i].edgePool.size();++j){
            int aId=snapshotVec[i].idPairVec[j].first;
            int bId=snapshotVec[i].idPairVec[j].second;

            if(i>=preArray.size()){
                extendPreArray(i);
            }
            preArrayInsert(i,aId,bId,snapshotVec[i].edgePool[j]);

            edgecnt++;
            if(edgecnt==edgeMilestone){
                gettimeofday(&endtime,NULL);
                cout<<"part time: "<<(endtime.tv_sec-starttime.tv_sec)*1000+(endtime.tv_usec-starttime.tv_usec)/1000<<endl;
                
                //统计size
                ll presize=0;
                for(int k=0;k<preArray.size();++k){
                    presize+=preArray[k].edgePool.size();
                }
                cout<<"till prearray size: "<<presize<<endl;

                // starttime=endtime;
                gettimeofday(&starttime,NULL);
                edgeMilestone+=edgePart;
            }
        }
    }

    cout<<"prearray insert end"<<endl;

    //方案2
    // int totalTime=snapshotVec.size();
    // int markMilestone=totalTime/10;
    // int markPart=markMilestone;
    // int markcnt=0;
    // timeval starttime,endtime;
    
    // cout<<"prearray insert start"<<endl;
    // gettimeofday(&starttime,NULL);
    // for(int i=0;i<snapshotVec.size();++i){
    //     for(int j=0;j<snapshotVec[i].edgePool.size();++j){
    //         int aId=snapshotVec[i].idPairVec[j].first;
    //         int bId=snapshotVec[i].idPairVec[j].second;

    //         if(i>=preArray.size()){
    //             extendPreArray(i);
    //         }
    //         preArrayInsert(i,aId,bId,snapshotVec[i].edgePool[j]);


    //     }


    //     markcnt++;
    //     if(markcnt==markMilestone){
    //         gettimeofday(&endtime,NULL);
    //         cout<<"part time: "<<(endtime.tv_sec-starttime.tv_sec)*1000+(endtime.tv_usec-starttime.tv_usec)/1000<<endl;
            
    //         //统计size
    //         ll presize=0;
    //         for(int k=0;k<preArray.size();++k){
    //             presize+=preArray[k].edgePool.size();
    //         }
    //         cout<<"till prearray size: "<<presize<<endl;
    //         cout<<"till prearray last size: "<<preArray[i].edgePool.size()<<endl;

    //         // starttime=endtime;
    //         gettimeofday(&starttime,NULL);
    //         markMilestone+=markPart;
    //     }

    // }

    // cout<<"prearray insert end"<<endl;
}

void TempGraph::buildPreArray(){
    if(1>end) return;
    preArray.resize(end+1);

    //初始化第一个
    SnapShotSum::copyFrmSnapShot(preArray[1],snapshotVec[1]);

    for(int i=2;i<=end;++i){
        preArray[i]=preArray[i-1];
        SnapShotSum::mergeSnapShot(preArray[i],snapshotVec[i]);
        //test
        // cout<<i<<endl;
    }
}

void dfs(int left,int start,int n,vector<int>& holder,vector<vector<int> >& ans){
    if(left==0){
        ans.push_back(holder);
        return;
    }
    left--;
    for(int i=start;i<n-left;++i){
        holder.push_back(i);
        dfs(left,i+1,n,holder,ans);
        holder.pop_back();
    }
}

void getSameLayerCubeVertex(int n,int need,vector<vector<int> >& ans){
    vector<int> holder;
    need--;
    for(int i=0;i<n-need;++i){
        holder.push_back(i);
        dfs(need,i+1,n,holder,ans);
        holder.pop_back();
    }
}

void SnapShot::mergeSnapShot(SnapShot& a,SnapShot& b){
    for(int i=0;i<b.edgePool.size();++i){
        int aId=b.idPairVec[i].first;
        int bId=b.idPairVec[i].second;
        a.insert(aId,bId,b.edgePool[i]);
    }
}

void SnapShot::mergeSnapShotSum(SnapShot& a,SnapShot& b){
    for(int i=0;i<b.edgePool.size();++i){
        int aId=b.idPairVec[i].first;
        int bId=b.idPairVec[i].second;
        a.insertSum(aId,bId,b.edgePool[i]);
    }
}

void TempGCube::buildEmptyIndex(int indexType){
    if(indexType==3){
        buildLogTable();
    }
}

void TempGCube::fillIndex(int indexType){
    if(indexType==3){//stable
        for(int i=2;i<graphList.size();++i){
            cout<<"graph No."<<i<<endl;
            graphList[i].fillStable();
        }
    }else if(indexType==5){
        for(int i=2;i<graphList.size();++i){
            cout<<"graph No."<<i<<endl;
            graphList[i].fillSegtree();
        }
    }else if(indexType==4){
        for(int i=2;i<graphList.size();++i){//只实验create的数据集
            cout<<"graph No."<<i<<endl;
            graphList[i].fillPreArray();
        }
    }
}

void TempGCube::buildIndex(int indexType){
    if(indexType==1){
        for(int i=0;i<graphList.size();++i){
            graphList[i].buildTreeArr();
        }
    }else if(indexType==2){
        for(int i=0;i<graphList.size();++i){
            graphList[i].buildPreArray();
            graphList[i].treeRoot=graphList[i].buildSegmentTree(graphList[i].base+1,graphList[i].base+graphList[i].end);
        }
    }else if(indexType==3){
        buildLogTable();
        for(int i=2;i<graphList.size();++i){//注意，暂时跳过base view
            graphList[i].buildStable();
        }
    }else if(indexType==4){
        for(int i=2;i<graphList.size();++i){//注意，暂时跳过base view
            graphList[i].buildPreArray();
        }
    }else if(indexType==5){
        for(int i=1;i<graphList.size();++i){//暂时跳过base view
            graphList[i].treeRoot=graphList[i].buildSegmentTree(graphList[i].base+1,graphList[i].base+graphList[i].end);
        }
    }
}

void TempGCube::buildLogTable(){
    int timeLimit=1000;
    logTable.resize(timeLimit+5,0);
    for(int i=1,j=0;i<=timeLimit;i<<=1,++j){
        logTable[i]=j;
    }
    int lastVal=1;
    for(int i=3;i<=timeLimit;++i){
        if(logTable[i]==0) logTable[i]=lastVal;
        else{
            lastVal=logTable[i];
        }
    }
}

void TempGCube::buildSegmentTree(){
    for(int i=0;i<graphList.size();++i){
        graphList[i].treeRoot=graphList[i].buildSegmentTree(graphList[i].base+1,graphList[i].base+graphList[i].end);
    }
}

int TempGraph::getNewNode(){
    treeNodePool.push_back(TreeNode());
    return treeNodePool.size()-1;
}

//需确保timel和timer在1+base和end+base范围内
void TempGraph::segQueryCount(int nodeMark,int timel,int timer,SnapShot& ans){
    // if(nodeMark==-1) return;
    // if(timel==treeNodePool[nodeMark].timel&&timer==treeNodePool[nodeMark].timer){
    //     SnapShot::mergeSnapShot(ans,treeNodePool[nodeMark].snapShot);
    //     return;
    // }

    // int mid=(treeNodePool[nodeMark].timel+treeNodePool[nodeMark].timer)/2;
    // if(timel>mid) segQueryCount(treeNodePool[nodeMark].rp,timel,timer,ans);
    // else if(timer<=mid) segQueryCount(treeNodePool[nodeMark].lp,timel,timer,ans);
    // else{
    //     segQueryCount(treeNodePool[nodeMark].lp,timel,mid,ans);
    //     segQueryCount(treeNodePool[nodeMark].rp,mid+1,timer,ans);
    // }
    // return;
}

void TempGraph::segQueryAmount(int nodeMark,int timel,int timer,SnapShot& ans){
    segQueryCount(nodeMark,timel,timer,ans);
}

void TempGraph::segQueryMax(int nodeMark,int timel,int timer,SnapShotExt& ans){
    if(nodeMark==-1) return;
    if(timel==treeNodePool[nodeMark].timel&&timer==treeNodePool[nodeMark].timer){
        SnapShotExt::mergeSnapShotExt(ans,treeNodePool[nodeMark].snapShotExt);
        return;
    }

    int mid=(treeNodePool[nodeMark].timel+treeNodePool[nodeMark].timer)/2;
    if(timel>mid) segQueryMax(treeNodePool[nodeMark].rp,timel,timer,ans);
    else if(timer<=mid) segQueryMax(treeNodePool[nodeMark].lp,timel,timer,ans);
    else{
        segQueryMax(treeNodePool[nodeMark].lp,timel,mid,ans);
        segQueryMax(treeNodePool[nodeMark].rp,mid+1,timer,ans);
    }
    return;
}

void TempGraph::segQueryMin(int nodeMark,int timel,int timer,SnapShotExt& ans){
    segQueryMax(nodeMark,timel,timer,ans);
}

int TempGraph::buildSegmentTree(int timel,int timer){
    //默认timel<=timer
    int curNode=getNewNode();
    treeNodePool[curNode].timel=timel;
    treeNodePool[curNode].timer=timer;
    if(timel==timer){
        int mark=timel-base;
        treeNodePool[curNode].timel=timel;
        treeNodePool[curNode].timer=timer;
        treeNodePool[curNode].lp=treeNodePool[curNode].rp=-1;
        SnapShotExt::copyFrmSnapShot(treeNodePool[curNode].snapShotExt,snapshotVec[mark]);
        return curNode;
    }

    int mid=(timel+timer)/2;
    int lp=buildSegmentTree(timel,mid);
    int rp=buildSegmentTree(mid+1,timer);
    treeNodePool[curNode].lp=lp;treeNodePool[curNode].rp=rp;
    SnapShotExt::mergeSnapShotExt(treeNodePool[curNode].snapShotExt,treeNodePool[lp].snapShotExt);
    SnapShotExt::mergeSnapShotExt(treeNodePool[curNode].snapShotExt,treeNodePool[rp].snapShotExt);
    return curNode;
}

//记录被materialize的cuboid
void TempGCube::recordMaterialize(){
    for(int i=0;i<graphList.size();++i){
        materialized[graphList[i].vertexTabl.attriVec]=i;
    }
}

void TempGraph::baseQueryCount(int l,int r,SnapShotSum& ans){
    
    while(l<=r){
        SnapShotSum::mergeSnapShot(ans,snapshotVec[l]);
        l++;
    }
}
//暂时这么写，问题不大
void TempGraph::baseQueryAmount(int l,int r,SnapShotSum& ans){
    baseQueryCount(l,r,ans);
}
void TempGraph::baseQueryMax(int l,int r,SnapShotExt& ans){
    // baseQueryCount(l,r,ans);
    while(l<=r){
        SnapShotExt::mergeSnapShot(ans,snapshotVec[l]);
        l++;
    }
}
void TempGraph::baseQueryMin(int l,int r,SnapShot& ans){
    // baseQueryCount(l,r,ans);
}

//l,r处理放在外面做
void TempGraph::queryCount(int l,int r,SnapShot& ans){
    SnapShot preSnap;
    
    int start=r;
    while(start>0){
        SnapShot::mergeSnapShot(ans,treeArr[start]);
        start-=lowerbit(start);
    }

    start=l-1;
    while(start>0){
        SnapShot::mergeSnapShot(preSnap,treeArr[start]);
        start-=lowerbit(start);
    }

    for(int i=0;i<preSnap.edgePool.size();++i){
        int aId=preSnap.idPairVec[i].first;
        int bId=preSnap.idPairVec[i].second;
        int eIdInFull=ans.edgeMatrix[aId][bId];
        ans.edgePool[eIdInFull].count-=preSnap.edgePool[i].count;
    }
}

void TempGraph::queryAmount(int l,int r,SnapShot& ans){
    SnapShot preSnap;
    
    int start=r;
    while(start>0){
        SnapShot::mergeSnapShot(ans,treeArr[start]);
        start-=lowerbit(start);
    }

    start=l-1;
    while(start>0){
        SnapShot::mergeSnapShot(preSnap,treeArr[start]);
        start-=lowerbit(start);
    }

    for(int i=0;i<preSnap.edgePool.size();++i){
        int aId=preSnap.idPairVec[i].first;
        int bId=preSnap.idPairVec[i].second;
        int eIdInFull=ans.edgeMatrix[aId][bId];
        ans.edgePool[eIdInFull].count-=preSnap.edgePool[i].count;
        ans.edgePool[eIdInFull].amount-=preSnap.edgePool[i].amount;
    }
}

void TempGCube::insert(int indexType,vector<string>& aAttri,vector<string>& bAttri,int theTime,double theVal){
    for(int i=0;i<graphList.size();++i){
        vector<string> realAattri,realBattri;
        for(int j=0;j<graphList[i].vertexTabl.attriPosInOri.size();++j){
            int tempPos=graphList[i].vertexTabl.attriPosInOri[j];
            realAattri.push_back(aAttri[tempPos]);
            realBattri.push_back(bAttri[tempPos]);
        }
        graphList[i].insert(indexType,realAattri,realBattri,theTime,theVal);
    }
}

void TempGraph::queryMax(int l,int r,SnapShot& ans){
    SnapShot preSnap;
    
    int start=r;
    while(start>0){
        SnapShot::mergeSnapShot(ans,treeArr[start]);
        start-=lowerbit(start);
    }

    start=l-1;
    while(start>0){
        SnapShot::mergeSnapShot(preSnap,treeArr[start]);
        start-=lowerbit(start);
    }

    for(int i=0;i<preSnap.edgePool.size();++i){
        int aId=preSnap.idPairVec[i].first;
        int bId=preSnap.idPairVec[i].second;
        int eIdInFull=ans.edgeMatrix[aId][bId];
        if(ans.edgePool[eIdInFull].maxv>=preSnap.edgePool[i].maxv-EXP&&ans.edgePool[eIdInFull].maxv<=preSnap.edgePool[i].maxv+EXP){
            if(ans.edgePool[eIdInFull].count-preSnap.edgePool[i].count==0){
                ans.edgePool[eIdInFull].count=0;
                continue;
            }
            bool hasValid;
            double ansVal=enumMax(l,r,aId,bId,hasValid);
            if(hasValid){
                ans.edgePool[eIdInFull].maxv=ansVal;
            }else{
                ans.edgePool[eIdInFull].count=0;//no use
            }
        }
    }
}

void TempGraph::queryMin(int l,int r,SnapShot& ans){
    SnapShot preSnap;
    
    int start=r;
    while(start>0){
        SnapShot::mergeSnapShot(ans,treeArr[start]);
        start-=lowerbit(start);
    }

    start=l-1;
    while(start>0){
        SnapShot::mergeSnapShot(preSnap,treeArr[start]);
        start-=lowerbit(start);
    }

    for(int i=0;i<preSnap.edgePool.size();++i){
        int aId=preSnap.idPairVec[i].first;
        int bId=preSnap.idPairVec[i].second;
        int eIdInFull=ans.edgeMatrix[aId][bId];
        if(ans.edgePool[eIdInFull].minv>=preSnap.edgePool[i].minv-EXP&&ans.edgePool[eIdInFull].minv<=preSnap.edgePool[i].minv+EXP){
            if(ans.edgePool[eIdInFull].count-preSnap.edgePool[i].count==0){
                ans.edgePool[eIdInFull].count=0;
                continue;
            }
            bool hasValid;
            double ansVal=enumMin(l,r,aId,bId,hasValid);
            if(hasValid){
                ans.edgePool[eIdInFull].minv=ansVal;
            }else{
                ans.edgePool[eIdInFull].count=0;//no use
            }
        }
    }
}

double TempGraph::enumMin(int l,int r,int aId,int bId,bool& hasValid){
    double ans=0;
    bool isValid=false;
    int start=r;
    while(start>=l){
        int next=start-lowerbit(start);
        if(next<l-1){
            SnapShot& curSnap=snapshotVec[start];
            if(curSnap.edgeMatrix.find(aId)!=curSnap.edgeMatrix.end()){
                if(curSnap.edgeMatrix[aId].find(bId)!=curSnap.edgeMatrix[aId].end()){
                    if(isValid==false){
                        ans=curSnap.edgePool[curSnap.edgeMatrix[aId][bId]].minv;
                        isValid=true;
                    }else{
                        ans=min(ans,curSnap.edgePool[curSnap.edgeMatrix[aId][bId]].minv);
                    }
                }
            }
            start--;
        }else{
            SnapShot& curSnap=treeArr[start];
            if(curSnap.edgeMatrix.find(aId)!=curSnap.edgeMatrix.end()){
                if(curSnap.edgeMatrix[aId].find(bId)!=curSnap.edgeMatrix[aId].end()){
                    if(isValid==false){
                        ans=curSnap.edgePool[curSnap.edgeMatrix[aId][bId]].minv;
                        isValid=true;
                    }else{
                        ans=min(ans,curSnap.edgePool[curSnap.edgeMatrix[aId][bId]].minv);
                    }
                }
            }
            start=next;
        }
    }
    hasValid=(isValid?true:false);
    return ans;
}

double TempGraph::enumMax(int l,int r,int aId,int bId,bool& hasValid){
    double ans=0;
    bool isValid=false;
    int start=r;
    while(start>=l){
        int next=start-lowerbit(start);
        if(next<l-1){
            SnapShot& curSnap=snapshotVec[start];
            if(curSnap.edgeMatrix.find(aId)!=curSnap.edgeMatrix.end()){
                if(curSnap.edgeMatrix[aId].find(bId)!=curSnap.edgeMatrix[aId].end()){
                    if(isValid==false){
                        ans=curSnap.edgePool[curSnap.edgeMatrix[aId][bId]].maxv;
                        isValid=true;
                    }else{
                        ans=max(ans,curSnap.edgePool[curSnap.edgeMatrix[aId][bId]].maxv);
                    }
                }
            }
            start--;
        }else{
            SnapShot& curSnap=treeArr[start];
            if(curSnap.edgeMatrix.find(aId)!=curSnap.edgeMatrix.end()){
                if(curSnap.edgeMatrix[aId].find(bId)!=curSnap.edgeMatrix[aId].end()){
                    if(isValid==false){
                        ans=curSnap.edgePool[curSnap.edgeMatrix[aId][bId]].maxv;
                        isValid=true;
                    }else{
                        ans=max(ans,curSnap.edgePool[curSnap.edgeMatrix[aId][bId]].maxv);
                    }
                }
            }
            start=next;
        }
    }
    hasValid=isValid;
    return ans;
}

void TempGCube::queryWithTarget(TempGraph& targetGraph,SnapShot& ansSnapShot,bool returnAns,int indexType,int timel,int timer,int type){
    int l,r;
    //处理lr
    if((indexType==2||indexType==5)&&type>2){
        if(timel<targetGraph.base+1) l=targetGraph.base+1;
        else l=timel;
        if(timer>targetGraph.base+targetGraph.end) r=targetGraph.base+targetGraph.end;
        else r=timer;
    }else{
        if(timel-targetGraph.base<1) l=1;
        else l=timel-targetGraph.base;
        if(timer-targetGraph.base>targetGraph.end) r=targetGraph.end;
        else r=timer-targetGraph.base;
    }

    if(l>r) return;
    
    switch (type)
    {
    case 0://count
    // {
    //     if(indexType==0){
    //         SnapShotSum ansSnapShotSum;
    //         targetGraph.baseQueryCount(l,r,ansSnapShotSum);
    //     }
    //     else{
    //         SnapShotSum ansSnapShotSum;
    //         targetGraph.preQueryAmount(l,r,ansSnapShotSum);
    //     }
    //     // else if(indexType==1) targetGraph.queryCount(l,r,ansSnapShot);
    //     // else if(indexType==2) targetGraph.segQueryCount(targetGraph.treeRoot,l,r,ansSnapShot);
    // }
    // break;
    case 1://amount
    {
        SnapShotSum ansSnapShotSum;
        if(indexType==0){
            
            targetGraph.baseQueryCount(l,r,ansSnapShotSum);
        }
        else{
            targetGraph.preQueryAmount(l,r,ansSnapShotSum);
        }
        if(returnAns){
            SnapShotSum::copyToSnapShot(ansSnapShotSum,ansSnapShot);
        }
        // else if(indexType==1) targetGraph.queryAmount(l,r,ansSnapShot);
        // else if(indexType==2) targetGraph.segQueryAmount(targetGraph.treeRoot,l,r,ansSnapShot);
    }
    break;
    case 2://ave
    {
        //warning no use, not modified
        // if(indexType==0){
        //     targetGraph.baseQueryAmount(l,r,ansSnapShot);
        // }else if(indexType==1){
        //     targetGraph.queryAmount(l,r,ansSnapShot);
        // }else if(indexType==2){
        //     targetGraph.segQueryAmount(targetGraph.treeRoot,l,r,ansSnapShot);
        // }
        
        // for(int i=0;i<ansSnapShot.edgePool.size();++i){
        //     if(ansSnapShot.edgePool[i].count!=0){
        //         ansSnapShot.edgePool[i].amount/=ansSnapShot.edgePool[i].count;
        //     }
        // }
        
    }
    break;
    case 3://max
    {
        SnapShotExt ansSnapShotExt;
        if(indexType==0){
            targetGraph.baseQueryMax(l,r,ansSnapShotExt);
            if(returnAns){
                SnapShotExt::copyToSnapShot(ansSnapShotExt,ansSnapShot);
            }
        }
        else if(indexType==1) targetGraph.queryMax(l,r,ansSnapShot);
        else if(indexType==2||indexType==5){
            
            targetGraph.segQueryMax(targetGraph.treeRoot,l,r,ansSnapShotExt);
            if(returnAns){
                SnapShotExt::copyToSnapShot(ansSnapShotExt,ansSnapShot);
            }
        }else if(indexType==3){
            targetGraph.staQueryMax(l,r,ansSnapShotExt);
            if(returnAns){
                SnapShotExt::copyToSnapShot(ansSnapShotExt,ansSnapShot);
            }
        }
    }
    break;
    case 4://min
    {
        //暂时不用这里
        // if(indexType==0) targetGraph.baseQueryMin(l,r,ansSnapShot);
        // else if(indexType==1) targetGraph.queryMin(l,r,ansSnapShot);
        // else if(indexType==2) targetGraph.segQueryMin(targetGraph.treeRoot,l,r,ansSnapShot);
    }
    default:
        break;
    }
}

//ans中的snapshot的边的count若是0
int TempGCube::query(int indexType,int timel,int timer,int type,bool returnAns,vector<string>& attributes,int greedyType,TempGraph& ans){
    if(attributes[0]=="name"||materialized.find(attributes)!=materialized.end()){
        int graphid=0;
        if(attributes[0]!="name") graphid=materialized[attributes];
        TempGraph& targetGraph=graphList[graphid];
        ans.snapshotVec.push_back(SnapShot());
        SnapShot& ansSnapShot=ans.snapshotVec[0];

        if(graphid==0) indexType=0;
        queryWithTarget(targetGraph,ansSnapShot,returnAns,indexType,timel,timer,type);
        return graphid;
    }else{
        //未被实例化
        //ans里面需自行装载vertexTabl
        ans.snapshotVec.push_back(SnapShot());
        SnapShot& ansSnapShot=ans.snapshotVec[0];
        VertexTabl& ansVertexTable=ans.vertexTabl;
        unordered_map<vector<string>,int,hashVectorString>& tempMp=ansVertexTable.vecStringMp;

        //找attributes的最小后代，这里就是base
        //but 为什么要找最小后代？什么最小？为何不是index最小？
        //感觉不好衡量，还好这里只能找base

        //集成关于size和desize的greedy
        vector<int> attriPosInOri;
        for(int i=0,j=0;i<graphList[0].vertexTabl.attriVec.size()&&j<attributes.size();++i){
            if(attributes[j]==graphList[0].vertexTabl.attriVec[i]){
                attriPosInOri.push_back(i);j++;
            }
        }
        int materializeId=0;
        int materializeValue;
        if(greedyType!=0){
            if(greedyType==1){
                materializeValue=graphList[0].size;
            }else if(greedyType==2){
                materializeValue=cuboidDeSize[{-1,0,1,2,3,4,5}];
            }
            for(int i=1;i<graphList.size();++i){
                if(cuboidIsAncestor(attriPosInOri,graphList[i].vertexTabl.attriPosInOri)){
                    int thisvalue=0;
                    if(greedyType==1){
                        thisvalue=cuboidSize[graphList[i].vertexTabl.attriPosInOri];
                    }else if(greedyType==2){
                        thisvalue=cuboidDeSize[graphList[i].vertexTabl.attriPosInOri];
                    }

                    if(thisvalue<materializeValue){
                        materializeValue=thisvalue;
                        materializeId=i;
                    }
                }
            }
        }

        TempGraph& materializedSon=graphList[materializeId];
        SnapShot biggerAns;
        if(materializeId==0) indexType=0;
        //ave 需要特别处理
        if(type==2) queryWithTarget(materializedSon,biggerAns,returnAns,indexType,timel,timer,1);
        else queryWithTarget(materializedSon,biggerAns,returnAns,indexType,timel,timer,type);

        vector<int> posInSon;
        int last=0;
        for(int i=0;i<attributes.size();++i){
            while(last<materializedSon.vertexTabl.attriNum){
                if(materializedSon.vertexTabl.attriVec[last]==attributes[i]){
                    posInSon.push_back(last);
                    last++;
                    break;
                }
                last++;
            }
        }
        
        int dataIndex=0;
        for(int i=0;i<biggerAns.idPairVec.size();++i){
            if(biggerAns.edgePool[i].count==0) continue;//不引入无效边//似乎只有treearr会有？
            int sonAid=biggerAns.idPairVec[i].first;
            int sonBid=biggerAns.idPairVec[i].second;
            vector<string> collect(posInSon.size());
            vector<string>& sonAdata=materializedSon.vertexTabl.dataList[sonAid];
            vector<string>& sonBdata=materializedSon.vertexTabl.dataList[sonBid];
            for(int j=0;j<posInSon.size();++j){
                collect[j]=sonAdata[posInSon[j]];
            }
            if(tempMp.find(collect)==tempMp.end()){
                tempMp[collect]=dataIndex++;
                ansVertexTable.dataList.push_back(collect);
            }
            int aId=tempMp[collect];
            collect=vector<string>(posInSon.size());
            for(int j=0;j<posInSon.size();++j){
                collect[j]=sonBdata[posInSon[j]];
            }
            if(tempMp.find(collect)==tempMp.end()){
                tempMp[collect]=dataIndex++;
                ansVertexTable.dataList.push_back(collect);
            }
            int bId=tempMp[collect];

            ansSnapShot.insert(aId,bId,biggerAns.edgePool[i]);
        }

        if(type==2){
            for(int i=0;i<ansSnapShot.edgePool.size();++i){
                //不会有count==0
                ansSnapShot.edgePool[i].amount/=ansSnapShot.edgePool[i].count;
            }
        }
        return -1;
    }
}

//crossboid
void TempGCube::crossQuery(vector<string>& aAttributes,vector<string>& bAttributes,int indexType,int timel,int timer,int type,CrossBoidRes& ans){
    //暂时全注释
    // //获得ncd
    // vector<string>& totalAttributes=graphList[0].vertexTabl.attriVec;
    // vector<string> ncdAttributes;
    // vector<int> aAttriPos,bAttriPos;
    // int aIndex=0;int bIndex=0;
    
    // for(int i=0;i<totalAttributes.size();++i){
    //     bool yes=false;
    //     if(aIndex<aAttributes.size()&&totalAttributes[i]==aAttributes[aIndex]){
    //         yes=true;aIndex++;
    //         aAttriPos.push_back(ncdAttributes.size());
    //     }
    //     if(bIndex<bAttributes.size()&&totalAttributes[i]==bAttributes[bIndex]){
    //         yes=true;bIndex++;
    //         bAttriPos.push_back(ncdAttributes.size());
    //     }

    //     if(yes) ncdAttributes.push_back(totalAttributes[i]);
    // }

    // TempGraph ncdAns;
    // int res;
    // if(type==2) res=query(indexType,timel,timer,1,ncdAttributes,ncdAns);
    // else res=query(indexType,timel,timer,type,ncdAttributes,ncdAns);

    // //合并成二分图
    // SnapShot& ncdSnap=ncdAns.snapshotVec[0];
    // VertexTabl& ncdVertexTabl=(res>=0?graphList[res].vertexTabl:ncdAns.vertexTabl);
    // ans.aAttri=aAttributes;
    // ans.bAttri=bAttributes;
    
    // for(int i=0;i<ncdSnap.idPairVec.size();++i){
    //     if(ncdSnap.edgePool[i].count==0) continue;//重要
    //     int aNcdId=ncdSnap.idPairVec[i].first;
    //     int bNcdId=ncdSnap.idPairVec[i].second;
    //     vector<string> aCollect,bCollect;
    //     vector<string> aCollectRev,bCollectRev;
    //     for(int j=0;j<aAttriPos.size();++j){
    //         aCollect.push_back(ncdVertexTabl.dataList[aNcdId][aAttriPos[j]]);
    //         aCollectRev.push_back(ncdVertexTabl.dataList[bNcdId][aAttriPos[j]]);
    //     }
    //     for(int j=0;j<bAttributes.size();++j){
    //         bCollect.push_back(ncdVertexTabl.dataList[bNcdId][bAttriPos[j]]);
    //         bCollectRev.push_back(ncdVertexTabl.dataList[aNcdId][bAttriPos[j]]);
    //     }

    //     ans.insert(aCollect,bCollect,ncdSnap.edgePool[i]);
    //     ans.insert(aCollectRev,bCollectRev,ncdSnap.edgePool[i]);
    // }

    // if(type==2){//处理平均值
    //     for(int i=0;i<ans.snap.idPairVec.size();++i){
    //         ans.snap.edgePool[i].amount/=ans.snap.edgePool[i].count;
    //     }
    // }
    
}

void TempGraph::buildTreeArr(){
    treeArr.resize(end+1);
    for(int i=1;i<=end;++i){
        int start=i;
        //insert
        while(start<=end){
            SnapShot::mergeSnapShot(treeArr[start],snapshotVec[i]);
            start+=lowerbit(start);
        }
    }
}

bool TempGCube::isAncestor(vector<int>& attriPosList,int possibleSon){
    //实现效率似乎较低，暂时如此
    for(int i=0;i<attriPosList.size();++i){
        string& realAttri=graphList[0].vertexTabl.attriVec[attriPosList[i]];
        vector<string>& possibleSonAttrArr=graphList[possibleSon].vertexTabl.attriVec;
        bool found=false;
        for(int j=0;j<graphList[possibleSon].vertexTabl.attriNum;++j){
            if(realAttri==possibleSonAttrArr[j]){
                found=true;
                break;
            }
        }
        if(found==false) return false;
    }
    return true;
}

void TempGCube::buildGraphByExist(vector<int>& attriPosList,int existSon){
    graphList.push_back(TempGraph());
    vector<string>& overallAttriArr=graphList[0].vertexTabl.attriVec;

    TempGraph& son=graphList[existSon];//用于生成当前图的后继
    vector<int> attriPosInSon;//选中属性在son graph的属性表中的下标列表
    int last=0;//last<son.vertexTabl.attrinum
    for(int i=0;i<attriPosList.size();++i){
        while(last<son.vertexTabl.attriNum){
            if(son.vertexTabl.attriVec[last]==overallAttriArr[attriPosList[i]]){
                attriPosInSon.push_back(last);
                last++;
                break;
            }
            last++;
        }
    }

    TempGraph& newGraph=graphList[graphList.size()-1];
    newGraph.initFrmTempGraph(son,attriPosInSon,attriPosList);
    
}

void TempGraph::initFrmTempGraph(TempGraph& son,vector<int>& attriPosInSon,vector<int>& attriPosInOri){
    base=son.base;
    end=son.end;

    vertexTabl.attriNum=attriPosInSon.size();
    //准备属性行
    vertexTabl.attriPosInOri=attriPosInOri;
    for(int i=0;i<attriPosInSon.size();++i){
        vertexTabl.attriVec.push_back(son.vertexTabl.attriVec[attriPosInSon[i]]);
    }

    // 准备数据行
    int dataIndex=0;
    vector<int> sonVertexToFather(son.vertexTabl.dataList.size());
    unordered_map<vector<string>,int,hashVectorString>& tempMp=vertexTabl.vecStringMp;
    vector<vector<string> >& sonDataList=son.vertexTabl.dataList;
    for(int i=0;i<sonDataList.size();++i){
        vector<string> collect(attriPosInSon.size());
        for(int j=0;j<attriPosInSon.size();++j){
            collect[j]=sonDataList[i][attriPosInSon[j]];
        }

        if(tempMp.find(collect)==tempMp.end()){
            tempMp[collect]=dataIndex++;
            vertexTabl.dataList.push_back(collect);
            vertexTabl.dataCount.push_back(0);
        }
        
        //just for count
        vertexTabl.dataCount[tempMp[collect]]+=son.vertexTabl.dataCount[i];
        
        // sonVertexToFather.push_back(tempMp[collect]);--bug
        sonVertexToFather[i]=tempMp[collect];
    }

    //准备图结构
    snapshotVec.resize(son.snapshotVec.size());
    for(int i=1;i<son.snapshotVec.size();++i){
        SnapShot& sonSnapShot=son.snapshotVec[i];
        for(int j=0;j<sonSnapShot.edgePool.size();++j){
            int aId=sonVertexToFather[sonSnapShot.idPairVec[j].first];
            int bId=sonVertexToFather[sonSnapShot.idPairVec[j].second];
            snapshotVec[i].insert(aId,bId,sonSnapShot.edgePool[j]);
        }
    }
    size=0;//统计tempgraph的size
    for(int i=1;i<snapshotVec.size();++i){
        size+=snapshotVec[i].edgePool.size();
    }

    //建索引（可选）
}



vector<vector<int>> TempGCube::partialGreedyNew(int k){
    // unordered_map<vector<int>,ll,hashVectorInt> cuboidSize,cuboidDeSize;//记录size，底图size
    unordered_map<vector<int>,double,hashVectorInt> cuboidP;//记录分布

    vector<vector<int> > cuboidList;

    ifstream fin;
    fin.open("../partial_greedy_size_desize_imdb.config");
    if(fin.is_open()==false){
        cerr<<"read err"<<endl;
        return vector<vector<int>>();
    }

    string dataline1;
    string dataline2;
    while(!fin.eof()){
        getline(fin,dataline1);
        dataline1.erase(dataline1.size()-1);
        stringstream ss1(dataline1);
        getline(fin,dataline2);
        stringstream ss2(dataline2);

        vector<int> attriPos;
        ll size,deSize;

        while(!ss1.eof()){
            int val;
            ss1>>val;
            attriPos.push_back(val);
        }
        ss2>>size>>deSize;
        cuboidSize[attriPos]=size;
        cuboidDeSize[attriPos]=deSize;
        cuboidList.push_back(attriPos);
        
    }

    vector<int> sourcePos={-1,0,1,2,3,4,5};
    unordered_set<vector<int>,hashVectorInt> S;
    S.insert(sourcePos);

    k--;
    while(k--){
        unordered_map<vector<int>,ll,hashVectorInt> benefit;
        for(int i=0;i<cuboidList.size();++i){
            if(S.find(cuboidList[i])==S.end()){
                //未实例化
                benefit[cuboidList[i]]=0;
                for(int j=0;j<cuboidList.size();++j){
                    if(j==i) continue;
                    if(S.find(cuboidList[j])!=S.end()) continue;
                    if(cuboidIsAncestor(cuboidList[j],cuboidList[i])==false) continue;
                    unordered_set<vector<int>,hashVectorInt>::iterator it=S.begin();

                    ll small=-1;
                    while(it!=S.end()){
                        if(cuboidIsAncestor(cuboidList[j],*it)){
                            if(small==-1){
                                small=cuboidDeSize[*it];
                            }else{
                                if(small>cuboidDeSize[*it]){
                                    small=cuboidDeSize[*it];
                                }
                            }
                        }
                        it++;
                    }
                    if(cuboidDeSize[cuboidList[i]]<small){
                        benefit[cuboidList[i]]+=small-cuboidDeSize[cuboidList[i]];
                    }
                }
            }
        }

        unordered_map<vector<int>,ll,hashVectorInt>::iterator it=benefit.begin();
        ll bigbenefit=it->second;
        vector<int> bigcuboid=it->first;
        it++;
        while(it!=benefit.end()){
            if(bigbenefit<it->second){
                bigbenefit=it->second;
                bigcuboid=it->first;
            }
            it++;
        }

        S.insert(bigcuboid);

    }

    //print ans
    vector<vector<int>> ans;
    unordered_set<vector<int>,hashVectorInt>::iterator it=S.begin();
    while(it!=S.end()){
        const vector<int>& theans=(*it);
        if(theans[0]!=-1){
            ans.push_back(theans);
        }
        it++;
    }
    
    return ans;
}

vector<vector<int>> TempGCube::partialGreedyOld(int k){
    // unordered_map<vector<int>,ll,hashVectorInt> cuboidSize,cuboidDeSize;//记录size，底图size
    unordered_map<vector<int>,double,hashVectorInt> cuboidP;//记录分布

    // vector<pair<int,int> > cubeVertexList;
    // cubeVertexList.push_back(pair<int,int>(graphList[0].size,0));
    // int vertexAttriNum=graphList[0].vertexTabl.attriNum;

    // for(int i=vertexAttriNum-1;i>=1;--i){
    //     vector<vector<int> > singleLans;
    //     //获得属性数量相同的所有属性组合
    //     getSameLayerCubeVertex(vertexAttriNum,i,singleLans);
    //     for(int j=0;j<singleLans.size();++j){
    //         vector<int>& curAns=singleLans[j];
    //         for(int k=cubeVertexList.size()-1;k>=0;--k){//必定有一个
    //             if(isAncestor(curAns,cubeVertexList[k].second)){
    //                 buildGraphByExist(curAns,cubeVertexList[k].second);
    //                 break;
    //             }
    //         }
    //     }
    //     cout<<"layer: "<<i<<endl;
    //     // break;//just test

    //     int start=graphList.size()-singleLans.size();
    //     for(int j=start;j<graphList.size();++j){
    //         cubeVertexList.push_back(pair<int,int>(graphList[j].size,j));
    //     }
    //     sort(cubeVertexList.begin()+start,cubeVertexList.end(),greater<pair<int,int> >());//为了下一排的cube找size最小的son
    // }

    // //原图的size
    // vector<int> tempans;
    // for(int i=0;i<vertexAttriNum;++i){
    //     tempans.push_back(i);
    // }
    // cuboidSize[tempans]=graphList[0].size;

    // //cuboid的size
    // for(int i=1;i<graphList.size();++i){
    //     TempGraph& curTempGraph=graphList[i];
    //     cuboidSize[curTempGraph.vertexTabl.attriPosInOri]=curTempGraph.size;
    // }

    // //计算每个cuboid的deSize
    // for(int i=1;i<graphList.size();++i){
    //     SnapShot tempSnap;
    //     TempGraph& curTempGraph=graphList[i];
    //     for(int j=0;j<curTempGraph.snapshotVec.size();++j){
    //         SnapShot::mergeSnapShot(tempSnap,curTempGraph.snapshotVec[j]);
    //     }
    //     cuboidDeSize[curTempGraph.vertexTabl.attriPosInOri]=tempSnap.edgePool.size();
    // }

    // SnapShot tempSnap;
    // for(int i=0;i<graphList[0].snapshotVec.size();++i){
    //     SnapShot::mergeSnapShot(tempSnap,graphList[0].snapshotVec[i]);
    // }
    // cuboidDeSize[tempans]=tempSnap.edgePool.size();

    // //print
    // unordered_map<vector<int>,int,hashVectorInt>::iterator it=cuboidSize.begin();
    // while(it!=cuboidSize.end()){
    //     const vector<int>& theKey=it->first;
    //     for(int i=0;i<theKey.size();++i){
    //         cout<<theKey[i]<<" ";
    //     }
    //     cout<<endl;
    //     cout<<it->second<<" "<<cuboidDeSize[theKey]<<endl;
    //     it++;
    // }

    vector<vector<int> > cuboidList;

    ifstream fin;
    fin.open("../partial_greedy_size_desize_imdb.config");
    if(fin.is_open()==false){
        cerr<<"read err"<<endl;
        return vector<vector<int>>();
    }

    string dataline1;
    string dataline2;
    while(!fin.eof()){
        getline(fin,dataline1);
        dataline1.erase(dataline1.size()-1);
        stringstream ss1(dataline1);
        getline(fin,dataline2);
        stringstream ss2(dataline2);

        vector<int> attriPos;
        ll size,deSize;

        while(!ss1.eof()){
            int val;
            ss1>>val;
            attriPos.push_back(val);
        }
        ss2>>size>>deSize;
        cuboidSize[attriPos]=size;
        cuboidDeSize[attriPos]=deSize;
        cuboidList.push_back(attriPos);
        
    }

    vector<int> sourcePos={-1,0,1,2,3,4,5};
    unordered_set<vector<int>,hashVectorInt> S;
    S.insert(sourcePos);

    // k--;
    while(k--){
        unordered_map<vector<int>,ll,hashVectorInt> benefit;
        for(int i=0;i<cuboidList.size();++i){
            if(S.find(cuboidList[i])==S.end()){
                //未实例化
                benefit[cuboidList[i]]=0;
                for(int j=0;j<cuboidList.size();++j){
                    if(j==i) continue;
                    if(S.find(cuboidList[j])!=S.end()) continue;
                    if(cuboidIsAncestor(cuboidList[j],cuboidList[i])==false) continue;
                    unordered_set<vector<int>,hashVectorInt>::iterator it=S.begin();

                    ll small=-1;
                    while(it!=S.end()){
                        if(cuboidIsAncestor(cuboidList[j],*it)){
                            if(small==-1){
                                small=cuboidSize[*it];
                            }else{
                                if(small>cuboidSize[*it]){
                                    small=cuboidSize[*it];
                                }
                            }
                        }
                        it++;
                    }
                    if(cuboidSize[cuboidList[i]]<small){
                        benefit[cuboidList[i]]+=small-cuboidSize[cuboidList[i]];
                    }
                }
            }
        }

        unordered_map<vector<int>,ll,hashVectorInt>::iterator it=benefit.begin();
        ll bigbenefit=it->second;
        vector<int> bigcuboid=it->first;
        it++;
        while(it!=benefit.end()){
            if(bigbenefit<it->second){
                bigbenefit=it->second;
                bigcuboid=it->first;
            }
            it++;
        }

        S.insert(bigcuboid);

    }

    //print ans
    
    vector<vector<int>> ans;
    unordered_set<vector<int>,hashVectorInt>::iterator it=S.begin();
    while(it!=S.end()){
        const vector<int>& theans=(*it);
        if(theans[0]!=-1){
            ans.push_back(theans);
        }
        it++;
    }
    
    return ans;

}

void TempGCube::partialMaterialize(int k){
    int sum=0;
    int attriNum=graphList[0].vertexTabl.attriNum;
    int lastLayer,cNum=0;
    if(k>=((1<<attriNum)-1)) k=((1<<attriNum)-1)-1;//最后一层不用了//有问题，但达不到
    for(lastLayer=1;lastLayer<=attriNum&&sum<k;++lastLayer){
        cNum=cNumber(attriNum,lastLayer);
        sum+=cNum;
    }
    k=(k-sum<0?cNum+k-sum:cNum);//最后一层要实例化多少个
    lastLayer--;//最后一层标号

    //下面与fullMaterialize类似
    vector<pair<int,int> > cubeVertexList;
    cubeVertexList.push_back(pair<int,int>(graphList[0].size,0));

    for(int i=lastLayer;i>=1;--i){
        vector<vector<int> > singleLans;
        getSameLayerCubeVertex(attriNum,i,singleLans);
        if(i==lastLayer){
            singleLans=vector<vector<int> >(singleLans.begin(),singleLans.begin()+k);
        }
        for(int j=0;j<singleLans.size();++j){
            vector<int>& curAns=singleLans[j];
            for(k=cubeVertexList.size()-1;k>=0;--k){//必定有一个
                if(isAncestor(curAns,cubeVertexList[k].second)){
                    buildGraphByExist(curAns,cubeVertexList[k].second);
                    break;
                }
            }
        }

        int start=graphList.size()-singleLans.size();
        for(int j=start;j<graphList.size();++j){
            cubeVertexList.push_back(pair<int,int>(graphList[j].size,j));
        }
        sort(cubeVertexList.begin()+start,cubeVertexList.end(),greater<pair<int,int> >());//为了下一排的cube找size最小的son
    }

    recordMaterialize();
}

void TempGCube::selectMaterialize(vector<vector<int> >& mList){
    for(int i=0;i<mList.size();++i){
        buildGraphByExist(mList[i],0);
    }

    recordMaterialize();
}

//随机版本
void TempGCube::createWithSimilarity(int theModel,int similarity){

    int nodecnt=graphList[theModel].vertexTabl.dataList.size();
    ll edgeMax=(nodecnt-1)*(ll)nodecnt/2;
    ll realEdge=graphList[theModel].size;

    int wantS=realEdge/similarity;
    if(wantS>edgeMax){
        cerr<<"false similarity"<<endl;
        return;
    }

    graphList.push_back(TempGraph());

    TempGraph& newGraph=graphList[graphList.size()-1];
    newGraph.base=graphList[theModel].base;
    newGraph.end=graphList[theModel].end;
    newGraph.size=graphList[theModel].size;

    newGraph.snapshotVec.resize(graphList[theModel].snapshotVec.size());
    Edge insertEdge(1,1);

    //test

    //第一步，确保覆盖S
    vector<int> idavec,idbvec;
    int snapMark=newGraph.end;
    int k=0;
    for(int i=0;i<nodecnt-1;++i){
        if(k>=wantS){
            break;
        }
        for(int j=i+1;j<nodecnt;++j){
            if(k>=wantS){
                break;
            }

            k++;
            newGraph.snapshotVec[snapMark].insert(i,j,insertEdge);
            idavec.push_back(i);
            idbvec.push_back(j);

            --snapMark;
            if(snapMark==0) snapMark=newGraph.end;

            
        }
    }

    int leftedge=realEdge-wantS;
    k=0;
    // snapMark=newGraph.end;
    srand(time(0));
    snapMark=rand()%newGraph.end+1;
    
    while((leftedge--)>0){
        
        while(newGraph.snapshotVec[snapMark].edgePool.size()>=wantS){
            // snapMark--;
            // if(snapMark==0) snapMark=newGraph.end;
            snapMark=rand()%newGraph.end+1;
        }

        while(!(newGraph.snapshotVec[snapMark].edgeMatrix.find(idavec[k])==newGraph.snapshotVec[snapMark].edgeMatrix.end()||newGraph.snapshotVec[snapMark].edgeMatrix[idavec[k]].find(idbvec[k])==newGraph.snapshotVec[snapMark].edgeMatrix[idavec[k]].end())){
            k++;
            if(k==idavec.size()) k=0;
        }

        newGraph.snapshotVec[snapMark].insert(idavec[k],idbvec[k],insertEdge);
        k++;
        if(k==idavec.size()) k=0;
        // snapMark--;
        // if(snapMark==0) snapMark=newGraph.end;
        snapMark=rand()%newGraph.end+1;
    }

}

// void TempGCube::createWithSimilarity(int theModel,int similarity){

//     int nodecnt=graphList[theModel].vertexTabl.dataList.size();
//     ll edgeMax=(nodecnt-1)*(ll)nodecnt/2;
//     ll realEdge=graphList[theModel].size;

//     int wantS=realEdge/similarity;
//     if(wantS>edgeMax){
//         cerr<<"false similarity"<<endl;
//         return;
//     }

//     graphList.push_back(TempGraph());

//     TempGraph& newGraph=graphList[graphList.size()-1];
//     newGraph.base=graphList[theModel].base;
//     newGraph.end=graphList[theModel].end;
//     newGraph.size=graphList[theModel].size;

//     newGraph.snapshotVec.resize(graphList[theModel].snapshotVec.size());
//     Edge insertEdge(1,1);

//     //test

//     //第一步，确保覆盖S
//     vector<int> idavec,idbvec;
//     int snapMark=newGraph.end;
//     int k=0;
//     for(int i=0;i<nodecnt-1;++i){
//         if(k>=wantS){
//             break;
//         }
//         for(int j=i+1;j<nodecnt;++j){
//             if(k>=wantS){
//                 break;
//             }

//             k++;
//             newGraph.snapshotVec[snapMark].insert(i,j,insertEdge);
//             idavec.push_back(i);
//             idbvec.push_back(j);

//             --snapMark;
//             if(snapMark==0) snapMark=newGraph.end;

            
//         }
//     }

//     int leftedge=realEdge-wantS;
//     k=0;
//     snapMark=newGraph.end;
//     while((leftedge--)>0){
        
//         while(newGraph.snapshotVec[snapMark].edgePool.size()>=wantS){
//             snapMark--;
//             if(snapMark==0) snapMark=newGraph.end;
//         }

//         while(!(newGraph.snapshotVec[snapMark].edgeMatrix.find(idavec[k])==newGraph.snapshotVec[snapMark].edgeMatrix.end()||newGraph.snapshotVec[snapMark].edgeMatrix[idavec[k]].find(idbvec[k])==newGraph.snapshotVec[snapMark].edgeMatrix[idavec[k]].end())){
//             k++;
//             if(k==idavec.size()) k=0;
//         }

//         newGraph.snapshotVec[snapMark].insert(idavec[k],idbvec[k],insertEdge);
//         k++;
//         if(k==idavec.size()) k=0;
//         snapMark--;
//         if(snapMark==0) snapMark=newGraph.end;
//     }

// }

void TempGCube::fullMaterialize(){
    //用于罗列graphList中graph的下标，
    //会对每一层根据graph size排序（降序）
    vector<pair<int,int> > cubeVertexList;
    cubeVertexList.push_back(pair<int,int>(graphList[0].size,0));
    int vertexAttriNum=graphList[0].vertexTabl.attriNum;

    for(int i=vertexAttriNum-1;i>=1;--i){
        vector<vector<int> > singleLans;
        //获得属性数量相同的所有属性组合
        getSameLayerCubeVertex(vertexAttriNum,i,singleLans);
        for(int j=0;j<singleLans.size();++j){
            vector<int>& curAns=singleLans[j];
            for(int k=cubeVertexList.size()-1;k>=0;--k){//必定有一个
                if(isAncestor(curAns,cubeVertexList[k].second)){
                    buildGraphByExist(curAns,cubeVertexList[k].second);
                    break;
                }
            }
        }
        // break;//just test

        int start=graphList.size()-singleLans.size();
        for(int j=start;j<graphList.size();++j){
            cubeVertexList.push_back(pair<int,int>(graphList[j].size,j));
        }
        sort(cubeVertexList.begin()+start,cubeVertexList.end(),greater<pair<int,int> >());//为了下一排的cube找size最小的son
    }

    recordMaterialize();
    //done
}

Edge::Edge(){
    count=0;
    amount=0;
}

Edge::Edge(int ival,double fval){
    count=ival;
    amount=maxv=minv=fval;
}

void SnapShot::insert(int aId,int bId,Edge& e){
    if(edgeMatrix.find(aId)==edgeMatrix.end()||edgeMatrix[aId].find(bId)==edgeMatrix[aId].end()){
        edgePool.push_back(Edge());
        edgeMatrix[aId][bId]=edgeMatrix[bId][aId]=edgePool.size()-1;
        idPairVec.push_back(pair<int,int>(aId,bId));
    }

    Edge& curEdge=edgePool[edgeMatrix[aId][bId]];
    mergeEdge(curEdge,e);
}

void SnapShot::insertSum(int aId,int bId,Edge& e){
    if(edgeMatrix.find(aId)==edgeMatrix.end()||edgeMatrix[aId].find(bId)==edgeMatrix[aId].end()){
        edgePool.push_back(Edge());
        edgeMatrix[aId][bId]=edgeMatrix[bId][aId]=edgePool.size()-1;
        idPairVec.push_back(pair<int,int>(aId,bId));
    }

    Edge& curEdge=edgePool[edgeMatrix[aId][bId]];
    mergeEdgeSum(curEdge,e);
}

int VertexTabl::insert(vector<string>& attri){
    if(vecStringMp.find(attri)==vecStringMp.end()){
        vecStringMp[attri]=dataList.size();
        dataList.push_back(attri);
    }
    return vecStringMp[attri];
}

bool VertexTabl::initFrmFile(string vertexFileName){
    ifstream fin;
    fin.open(vertexFileName,ios::in);

    if(fin.is_open()==false) return false;

    //读取属性行
    string attributeLine;
    getline(fin,attributeLine);
    vector<string> segs;
    split(attributeLine,segs);
    attriNum=segs.size()-1;
    for(int i=1;i<segs.size();++i){
        attriVec.push_back(segs[i]);
        attriPosInOri.push_back(i-1);
    }

    //读取数据行
    string dataLine;
    while(!fin.eof()){
        getline(fin,dataLine);
        if(dataLine.size()==0) continue;

        vector<string> segs;
        vector<string> attributes;
        split(dataLine,segs);
        int theId=atoi(segs[0].c_str());
        for(int i=1;i<segs.size();++i) attributes.push_back(segs[i]);
        if(theId>=dataList.size()){
            dataList.resize(theId+1);

            //just for count
            dataCount.resize(theId+1);
        }
        dataCount[theId]=1;
        dataList[theId]=attributes;
        vecStringMp[attributes]=theId;

    }

    //done
    
}

void TempGraph::extendPreArray(int to){
    int oldsize=preArray.size();
    preArray.resize(to+1);

    if(oldsize==0) return;
    for(int i=oldsize;i<=to;++i){
        preArray[i]=preArray[oldsize-1];
    }
}

//需在extend snapshotVec但不更新end的基础上
void TempGraph::extendTreeArr(int to){
    treeArr.resize(to+1);

    //对于每个i，不使用直接的查询结果，即[till,i-1]的查询结果，因为查询会引入count为0的边
    for(int i=end+1;i<=to;++i){
        int till=i-lowerbit(i)+1;
        if(till<=end){
            int start=i-1;
            while(start>=till){
                SnapShot::mergeSnapShot(treeArr[i],treeArr[start]);
                start-=lowerbit(start);
            }
        }
    }
}

void TempGraph::extendSegTree(int toTime){
    if(toTime<=treeNodePool[treeRoot].timer) return;
    //一直翻倍，直到能覆盖totime
    //实质是一直新建treeroot，并不会创建整个右子树
    while(treeNodePool[treeRoot].timer<toTime){
        int newRoot=getNewNode();
        int oldTimel=treeNodePool[treeRoot].timel;
        int oldTimer=treeNodePool[treeRoot].timer;
        treeNodePool[newRoot].timel=oldTimel;
        treeNodePool[newRoot].timer=oldTimer+1+oldTimer-oldTimel;
        treeNodePool[newRoot].lp=treeRoot;
        // SnapShot::mergeSnapShot(treeNodePool[newRoot].snapShot,treeNodePool[treeRoot].snapShot);
        treeNodePool[newRoot].snapShotExt=treeNodePool[treeRoot].snapShotExt;
        treeRoot=newRoot;
    }

}

void TempGraph::extend(int indexType,int to){
    if(to<=end) return;
    snapshotVec.resize(to+1);

    if(indexType==1) extendTreeArr(to);
    else if(indexType==2){
        extendSegTree(to+base);//他需要真实时间值
        extendPreArray(to);
    }
    end=to;
}

void TempGraph::insertToTreeArr(int mark,int aId,int bId,Edge& insertEdge){
    //前提是treearr已经完成extend
    while(mark<=end){
        treeArr[mark].insert(aId,bId,insertEdge);
        mark+=lowerbit(mark);
    }
}

//前提是segmentTree已经完成extend
void TempGraph::segInsert(int nodeMark,int timePos,int aId,int bId,Edge& insertEdge){
    //nodemark==-1的情况应该在外面处理，这里不考虑
    treeNodePool[nodeMark].snapShotExt.insert(aId,bId,insertEdge);
    if(treeNodePool[nodeMark].timer==treeNodePool[nodeMark].timel) return;

    int mid=(treeNodePool[nodeMark].timel+treeNodePool[nodeMark].timer)/2;
    if(timePos<=mid){
        if(treeNodePool[nodeMark].lp==-1){
            int newNode=getNewNode();
            treeNodePool[nodeMark].lp=newNode;
            treeNodePool[newNode].timel=treeNodePool[nodeMark].timel;
            treeNodePool[newNode].timer=mid;
        }
        segInsert(treeNodePool[nodeMark].lp,timePos,aId,bId,insertEdge);
    }
    else{
        if(treeNodePool[nodeMark].rp==-1){
            int newNode=getNewNode();
            treeNodePool[nodeMark].rp=newNode;
            treeNodePool[newNode].timel=mid+1,
            treeNodePool[newNode].timer=treeNodePool[nodeMark].timer;
        }
        segInsert(treeNodePool[nodeMark].rp,timePos,aId,bId,insertEdge);
    }

}

void TempGraph::insert(int indexType,vector<string>& aAttri,vector<string>& bAttri,int theTime,double theVal){
    //处理节点
    if(vertexTabl.vecStringMp.find(aAttri)==vertexTabl.vecStringMp.end()){
        vertexTabl.vecStringMp[aAttri]=vertexTabl.dataList.size();
        vertexTabl.dataList.push_back(aAttri);
    }
    if(vertexTabl.vecStringMp.find(bAttri)==vertexTabl.vecStringMp.end()){
        vertexTabl.vecStringMp[bAttri]=vertexTabl.dataList.size();
        vertexTabl.dataList.push_back(bAttri);
    }

    int aId,bId;
    aId=vertexTabl.vecStringMp[aAttri];
    bId=vertexTabl.vecStringMp[bAttri];

    int mark=theTime-base;
    if(mark>end) extend(indexType,mark);//延长，更新end

    Edge insertEdge(1,theVal);
    snapshotVec[mark].insert(aId,bId,insertEdge);
    // return;//just for test
    if(indexType==0) return;
    if(indexType==1) insertToTreeArr(mark,aId,bId,insertEdge);
    else if(indexType==2){
        preArrayInsert(mark,aId,bId,insertEdge);
        segInsert(treeRoot,mark+base,aId,bId,insertEdge);
    }
}

void TempGraph::preArrayInsert(int mark,int aId,int bId,Edge& insertEdge){
    preArray[mark].insert(aId,bId,insertEdge);
}

void TempGCube::init(string vertexFileName,string tempGraphFileName){
    graphList.push_back(TempGraph());
    graphList[0].initFrmFile(vertexFileName,tempGraphFileName);
}

bool TempGraph::initFrmFile(string vertexFileName,string tempGraphFileName){
    //加载vertex table
    vertexTabl.initFrmFile(vertexFileName);

    //加载temp graph
    ifstream fin;
    fin.open(tempGraphFileName);
    if(fin.is_open()==false) return false;

    string dataLine;
    getline(fin,dataLine);

    int vertexNum=vertexTabl.dataList.size();

    vector<pair<int,int> > idPairVec;
    vector<int> timeVec;
    vector<double> valVec;
    // vector<Edge> tempEdgePool;
    
    int bigTime=-1,smallTime=-1;
    while(!fin.eof()){
        getline(fin,dataLine);
        if(dataLine.size()==0) continue;

        vector<string> segs;
        split(dataLine,segs);
        int aId,bId,timeStamp;
        double attri=0;
        aId=atoi(segs[0].c_str());
        bId=atoi(segs[1].c_str());
        timeStamp=atoi(segs[2].c_str());
        if(segs.size()>3){
            attri=atof(segs[3].c_str());
        }

        idPairVec.push_back(pair<int,int>(aId,bId));
        timeVec.push_back(timeStamp);
        valVec.push_back(attri);

        if(smallTime==-1) smallTime=timeStamp;
        else smallTime=min(smallTime,timeStamp);
        if(bigTime==-1) bigTime=timeStamp;
        else bigTime=max(bigTime,timeStamp);
        
    }

    base=smallTime-1;
    end=bigTime-base;
    snapshotVec.resize(end+1);
    for(int i=0;i<idPairVec.size();++i){
        int theIndex=timeVec[i]-base;
        Edge paramEdge(1,valVec[i]);
        snapshotVec[theIndex].insert(idPairVec[i].first,idPairVec[i].second,paramEdge);
    }
    size=0;//统计tempgraph的size
    for(int i=1;i<snapshotVec.size();++i){
        size+=snapshotVec[i].edgePool.size();
    }
    
    //建索引（可选）

}

#endif