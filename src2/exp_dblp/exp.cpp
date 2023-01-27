#define crossboid
#include"../TempGCube.hpp"

vector<int> TempGCube::logTable=vector<int>();

bool vertexEqual(const vector<string>& a,const vector<string>& b){
    if(a.size()!=b.size()) return false;
    for(int i=0;i<a.size();++i){
        if(a[i]!=b[i]) return false;
    }
    return true;
}

bool vertexLess(const vector<string>& a,const vector<string>& b){
    int minLen=min(a.size(),b.size());
    for(int i=0;i<minLen;++i){
        if(a[i]==b[i]) continue;
        if(a[i]<b[i]) return true;
        else return false;
    }
    return a.size()<b.size();
}

struct snap_res{
    pair<int,int> idPair;
    Edge e;
    snap_res(pair<int,int> _pair,Edge& _e){
        idPair=_pair;
        e=_e;
    }

    bool operator <(const snap_res& b)const{
        if(idPair.first==b.idPair.first){
            return idPair.second<b.idPair.second;
        }
        return idPair.first<b.idPair.first;
    }
};

struct snap_res_str{
    pair<vector<string>,vector<string> > vertexPair;
    Edge e;
    snap_res_str(pair<vector<string>,vector<string> > _pair,Edge& _e){
        vertexPair=_pair;
        e=_e;
    }

    bool operator <(const snap_res_str& b)const{
        if(vertexEqual(vertexPair.first,b.vertexPair.first)){
            return vertexLess(vertexPair.second,b.vertexPair.second);
        }
        return vertexLess(vertexPair.first,b.vertexPair.first);
    }
};

void print_materialize_ans_snapshot(TempGraph& tgraph,SnapShot& ans,int type){
    vector<snap_res> reorganize_ans;
    for(int i=0;i<ans.edgePool.size();++i){
        if(ans.edgePool[i].count==0) continue;
        if(ans.idPairVec[i].first<ans.idPairVec[i].second){
            reorganize_ans.push_back(snap_res(ans.idPairVec[i],ans.edgePool[i]));
        }else{
            reorganize_ans.push_back(snap_res(pair<int,int>(ans.idPairVec[i].second,ans.idPairVec[i].first),ans.edgePool[i]));
        }
    }

    sort(reorganize_ans.begin(),reorganize_ans.end());

    VertexTabl& target=tgraph.vertexTabl;
    for(int i=0;i<reorganize_ans.size();++i){
        cout<<reorganize_ans[i].idPair.first<<" ";
        for(int j=0;j<target.dataList[reorganize_ans[i].idPair.first].size();++j){
            cout<<target.dataList[reorganize_ans[i].idPair.first][j]<<" ";
        }
        cout<<endl;
        cout<<reorganize_ans[i].idPair.second<<" ";
        for(int j=0;j<target.dataList[reorganize_ans[i].idPair.second].size();++j){
            cout<<target.dataList[reorganize_ans[i].idPair.second][j]<<" ";
        }
        cout<<endl;
        switch (type)
        {
        case 0:
        cout<<reorganize_ans[i].e.count<<endl;
        break;
        case 1:
        cout<<reorganize_ans[i].e.amount<<endl;
        break;
        case 2:
        cout<<reorganize_ans[i].e.amount<<endl;
        break;
        case 3:
        cout<<reorganize_ans[i].e.maxv<<endl;
        break;
        case 4:
        cout<<reorganize_ans[i].e.minv<<endl;
        break;
        default:
            break;
        }
    }
}

void print_crossres_slice(CrossBoidRes& res,int type){
    vector<snap_res_str> reorganize_ans;
    for(int i=0;i<res.snap.edgePool.size();++i){
        if(res.snap.edgePool[i].count==0) continue;
        vector<string>& aVertex=res.aVertexTabl.dataList[res.snap.idPairVec[i].first];
        vector<string>& bVertex=res.bVertexTabl.dataList[res.snap.idPairVec[i].second];
        reorganize_ans.push_back(snap_res_str(pair<vector<string>,vector<string> >(aVertex,bVertex),res.snap.edgePool[i]));
    }

    sort(reorganize_ans.begin(),reorganize_ans.end());

    for(int i=0;i<reorganize_ans.size();++i){
        //cout<<reorganize_ans[i].idPair.first<<" ";
        if(reorganize_ans[i].vertexPair.first[0]!="Jiawei Han 0001"){
            continue;
        }
        for(int j=0;j<reorganize_ans[i].vertexPair.first.size();++j){
            cout<<reorganize_ans[i].vertexPair.first[j]<<" ";
        }
        cout<<endl;
        //cout<<reorganize_ans[i].idPair.second<<" ";

        for(int j=0;j<reorganize_ans[i].vertexPair.second.size();++j){
            cout<<reorganize_ans[i].vertexPair.second[j]<<" ";
        }
        cout<<endl;
        switch (type)
        {
        case 0:
        cout<<reorganize_ans[i].e.count<<endl;
        break;
        case 1:
        cout<<reorganize_ans[i].e.amount<<endl;
        break;
        case 2:
        cout<<reorganize_ans[i].e.amount<<endl;
        break;
        case 3:
        cout<<reorganize_ans[i].e.maxv<<endl;
        break;
        case 4:
        cout<<reorganize_ans[i].e.minv<<endl;
        break;
        default:
            break;
        }
    }
}

int main(int argc,char* argv[]){
    if(argc!=3){
        cout<<"parameter error"<<endl;
        return 1;
    }
    int start,end;

    TempGCube tgcube;
    start=clock();
    tgcube.init(argv[1],argv[2]);
    end=clock();
    // cerr<<"init done with "<<(end-start)*1000.0/CLOCKS_PER_SEC<<endl;
    // cerr<<"start time: "<<tgcube.graphList[0].base+1<<" end: "<<tgcube.graphList[0].end+tgcube.graphList[0].base<<endl;
    // cerr<<"vertex num: "<<tgcube.graphList[0].vertexTabl.dataList.size()<<" edge num: "<<tgcube.graphList[0].size<<endl;
    start=end;
    vector<vector<int> > mList={{1},{2},{1,2}};
    #ifdef crossboid
    tgcube.fullMaterialize();
    #endif
    #ifndef crossboid
    tgcube.selectMaterialize(mList);
    #endif
    end=clock();
    cerr<<"materialize done with "<<(end-start)*1000.0/CLOCKS_PER_SEC<<endl;

    // for(int i=0;i<tgcube.graphList.size();++i){
        
    //     TempGraph& curGraph=tgcube.graphList[i];
    //     for(int j=0;j<curGraph.vertexTabl.attriPosInOri.size();++j){
    //         cout<<curGraph.vertexTabl.attriPosInOri[j]<<" ";
    //     }
    //     cout<<endl;
    //     SnapShot summative;
    //     for(int j=0;j<curGraph.snapshotVec.size();++j){
    //         cout<<curGraph.snapshotVec[j].edgePool.size()<<" ";
    //         SnapShot::mergeSnapShot(summative,curGraph.snapshotVec[j]);
    //     }
    //     cout<<endl;
    //     cout<<curGraph.size<<" "<<summative.edgePool.size()<<" "<<curGraph.size*1.0/summative.edgePool.size()<<endl;

    //     cout<<endl;
    // }

    #ifndef crossboid
    while(true){
        string attrs;
        int tl,tr,type;
        cin>>attrs>>tl>>tr>>type;
        TempGraph ans;
        vector<string> segs;
        split(attrs,segs);
        int res=tgcube.query(0,tl,tr,type,true,segs,0,3,ans);

        print_materialize_ans_snapshot(tgcube.graphList[res],ans.snapshotVec[0],0);
    }
    #endif
    #ifdef crossboid
    while(true){
        string crossA,crossB;
        int timel,timer,type;
        cin>>crossA>>crossB;
        if(crossA=="end") break;

        cin>>timel>>timer;
        vector<string> segsA,segsB;
        split(crossA,segsA);
        split(crossB,segsB);
        cin>>type;
        CrossBoidRes ans;
        
        tgcube.crossQuery(segsA,segsB,0,timel,timer,type,ans);
        print_crossres_slice(ans,type);
    }
    #endif

    return 0;
}