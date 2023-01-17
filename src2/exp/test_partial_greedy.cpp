#include"../TempGCube.hpp"

vector<int> TempGCube::logTable=vector<int>();


int main(int argc,char* argv[]){
    if(argc!=3){
        cout<<"parameter error"<<endl;
        return 1;
    }
    int start,end;
    int k;
    cin>>k;

    TempGCube tgcube;
    // start=clock();
    tgcube.init(argv[1],argv[2]);
    // vector<vector<int> > mList={{0,1,2,3,4,5}};
    // tgcube.selectMaterialize(mList);
    // cout<<tgcube.graphList[1].size<<" ";

    // SnapShotSum tempsnap;
    // for(int i=0;i<tgcube.graphList[1].snapshotVec.size();++i){
    //     SnapShotSum::mergeSnapShot(tempsnap,tgcube.graphList[1].snapshotVec[i]);
    // }
    // cout<<tempsnap.edgePool.size()<<endl;
    // end=clock();
    // cerr<<"init done with "<<(end-start)*1000.0/CLOCKS_PER_SEC<<endl;
    // cerr<<"start time: "<<tgcube.graphList[0].base+1<<" end: "<<tgcube.graphList[0].end+tgcube.graphList[0].base<<endl;
    // cerr<<"vertex num: "<<tgcube.graphList[0].vertexTabl.dataList.size()<<" edge num: "<<tgcube.graphList[0].size<<endl;

    int greedyType;
    cin>>greedyType;
    switch (greedyType)
    {
    case 0:
    {
        tgcube.partialMaterialize(k);
        tgcube.buildIndex(5);
    }
    break;
    case 1:
    {
        vector<vector<int>> res=tgcube.partialGreedyOld(k);
        tgcube.selectMaterialize(res);
        tgcube.buildIndex(5);
    }
    break;
    case 2:
    {
        vector<vector<int>> res=tgcube.partialGreedyNew(k);
        tgcube.selectMaterialize(res);
        tgcube.buildIndex(5);
    }
    default:
        break;
    }


    // start=end;
    // vector<string> oldres,newres;
    // oldres=tgcube.partialGreedyOld(k);
    // end=clock();
    // cerr<<"greedy old with "<<(end-start)*1000.0/CLOCKS_PER_SEC<<endl;

    // start=end;
    // newres=tgcube.partialGreedyNew(k);
    // end=clock();
    // cerr<<"greedy new with "<<(end-start)*1000.0/CLOCKS_PER_SEC<<endl;

    // int diffcnt=0;
    // for(int i=0;i<oldres.size();++i){
    //     if(oldres[i]!=newres[i]){
    //         diffcnt++;
    //         break;
    //     }
    // }
    // cout<<diffcnt<<endl;
    vector<vector<string> > allSegs;
    vector<vector<int> > allIntParam;

    while(true){
        string attriList;
        int timel,timer,type,times;
        cin>>attriList;
        
        if(attriList=="end") break;
        allIntParam.push_back(vector<int>());
        int last=allIntParam.size()-1;
        allIntParam[last].resize(4);

        cin>>allIntParam[last][0]>>allIntParam[last][1]>>allIntParam[last][2]>>allIntParam[last][3];
        vector<string> segs;
        split(attriList,segs);
        allSegs.push_back(segs);

    }

    //批量查询
    start=clock();
    for(int i=0;i<allSegs.size();++i){
        for(int j=0;j<allIntParam[i][3];++j){
            TempGraph ans;
            int res=tgcube.query(5,allIntParam[i][0],allIntParam[i][1],allIntParam[i][2],false,allSegs[i],greedyType,ans);
        }
    }
    end=clock();
    cerr<<"time: "<<(end-start)*1000.0/CLOCKS_PER_SEC<<endl;

    return 0;
}