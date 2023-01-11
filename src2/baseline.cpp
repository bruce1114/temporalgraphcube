#include"TempGCube.hpp"

#include<iostream>
using namespace std;

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
    pair<vector<string>,vector<string> > vertexPair;
    pair<int,int> idPair;
    Edge e;
    snap_res(pair<vector<string>,vector<string> > _pair,pair<int,int> _idPair,Edge& _e){
        vertexPair=_pair;
        idPair=_idPair;
        e=_e;
    }

    bool operator <(const snap_res& b)const{
        if(vertexEqual(vertexPair.first,b.vertexPair.first)){
            return vertexLess(vertexPair.second,b.vertexPair.second);
        }
        return vertexLess(vertexPair.first,b.vertexPair.first);
    }
};

void print_snapshot(SnapShot& snapshot){
    for(int i=0;i<snapshot.idPairVec.size();++i){
        cout<<snapshot.idPairVec[i].first<<" "<<snapshot.idPairVec[i].second<<" ";
        cout<<snapshot.edgePool[i].count<<" "<<snapshot.edgePool[i].amount<<" "<<snapshot.edgePool[i].maxv<<" "<<snapshot.edgePool[i].minv<<endl;
    }
}

void print_ans_snapshot(VertexTabl& ansVertexTabl,SnapShot& ans,int type){
    vector<snap_res> reorganize_ans;
    for(int i=0;i<ans.edgePool.size();++i){
        if(ans.edgePool[i].count==0) continue;
        vector<string>& aVertex=ansVertexTabl.dataList[ans.idPairVec[i].first];
        vector<string>& bVertex=ansVertexTabl.dataList[ans.idPairVec[i].second];
        if(vertexLess(aVertex,bVertex)){
            reorganize_ans.push_back(snap_res(pair<vector<string>,vector<string> >(aVertex,bVertex),pair<int,int>(ans.idPairVec[i].first,ans.idPairVec[i].second),ans.edgePool[i]));
        }else{
            reorganize_ans.push_back(snap_res(pair<vector<string>,vector<string> >(bVertex,aVertex),pair<int,int>(ans.idPairVec[i].second,ans.idPairVec[i].first),ans.edgePool[i]));
        }
    }

    sort(reorganize_ans.begin(),reorganize_ans.end());

    for(int i=0;i<reorganize_ans.size();++i){
        //cout<<reorganize_ans[i].idPair.first<<" ";
        for(int j=0;j<reorganize_ans[i].vertexPair.first.size();++j){
            cout<<reorganize_ans[i].vertexPair.first[j]<<" ";
        }

        //cout count
        cout<<ansVertexTabl.dataCount[reorganize_ans[i].idPair.first];
        cout<<endl;
        //cout<<reorganize_ans[i].idPair.second<<" ";

        for(int j=0;j<reorganize_ans[i].vertexPair.second.size();++j){
            cout<<reorganize_ans[i].vertexPair.second[j]<<" ";
        }
        
        //cout count
        cout<<ansVertexTabl.dataCount[reorganize_ans[i].idPair.second];
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

void print_tgraph(TempGraph& tgraph){
    cout<<"vertex table:"<<endl;
    cout<<"attri:";
    for(int i=0;i<tgraph.vertexTabl.attriVec.size();++i){
        cout<<tgraph.vertexTabl.attriVec[i]<<" ";
    }
    cout<<endl;
    cout<<"data:"<<endl;
    for(int i=0;i<tgraph.vertexTabl.dataList.size();++i){
        cout<<i<<" ";
        for(int j=0;j<tgraph.vertexTabl.dataList[i].size();++j){
            cout<<tgraph.vertexTabl.dataList[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<"----------"<<endl;
    cout<<"snapshots: "<<endl;
    for(int i=1;i<tgraph.snapshotVec.size();++i){
        cout<<"time: "<<i+tgraph.base<<":"<<endl;
        print_snapshot(tgraph.snapshotVec[i]);
    }
}

void patch_insert(string fileName,TempGCube& tgcube){
    ifstream fin;
    fin.open(fileName);
    string dataLine;
    int attriNum=tgcube.graphList[0].vertexTabl.attriNum;
    while(!fin.eof()){
        getline(fin,dataLine);
        if(dataLine.size()==0) continue;

        vector<string> segs;
        split(dataLine,segs);
        vector<string> aAttri(segs.begin(),segs.begin()+4);
        vector<string> bAttri(segs.begin()+4,segs.begin()+8);

        tgcube.insert(0,aAttri,bAttri,atoi(segs[8].c_str()),atof(segs[9].c_str()));
    }
}

int main(int argc,char* argv[]){
    if(argc!=3){
        cout<<"parameter error"<<endl;
        return 1;
    }

    TempGCube tgcube;
    tgcube.init(argv[1],argv[2]);
    cerr<<"init done"<<endl;
    vector<vector<int> > mList={{0},{0,1},{0,1,2},{0,1,2,3},{0,1,2,3,4}};
    // vector<vector<int> > mList={{0},{0,1},{0,1,2},{0,2}};
    // vector<vector<int> > mList={{1},{2},{1,2}};
    // vector<vector<int> > mList;
    // tgcube.fullMaterialize();
    tgcube.selectMaterialize(mList);
    cerr<<"materialize done"<<endl;

    // for(int i=0;i<tgcube.graphList.size();++i){
    //     print_tgraph(tgcube.graphList[i]);
    // }
    cerr<<tgcube.graphList[0].vertexTabl.dataCount.size()<<" "<<tgcube.graphList[0].size<<endl;

    //build empty index
    int indexType=0;
    // tgcube.buildIndex(indexType);
    tgcube.buildEmptyIndex(indexType);

    //fill index
    tgcube.fillIndex(indexType);

    while(true){
        string attriList;
        int timel,timer,type;
        cin>>attriList;
        if(attriList=="end") break;

        if(attriList=="insert"){
            string insertFile;
            cin>>insertFile;
            patch_insert(insertFile,tgcube);

            // for(int i=0;i<tgcube.graphList.size();++i){
            //     print_tgraph(tgcube.graphList[i]);
            // }
            continue;
        }

        // vector<string> segs;
        // split(attriList,segs);

        // print_tgraph(tgcube.graphList[tgcube.materialized[segs]]);

        cin>>timel>>timer;
        vector<string> segs;
        split(attriList,segs);
        cin>>type;
        TempGraph ans;
        int res=tgcube.query(indexType,timel,timer,type,true,segs,ans);
        if(res>=0){
            cerr<<"res>=0"<<endl;
            print_ans_snapshot(tgcube.graphList[res].vertexTabl,ans.snapshotVec[0],type);
        }else{
            cerr<<"res<0"<<endl;
            print_ans_snapshot(ans.vertexTabl,ans.snapshotVec[0],type);
        }
    }
}