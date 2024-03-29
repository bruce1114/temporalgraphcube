#include"../TempGCube.hpp"


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
    cerr<<"init done with "<<(end-start)*1000.0/CLOCKS_PER_SEC<<endl;
    cerr<<"start time: "<<tgcube.graphList[0].base+1<<" end: "<<tgcube.graphList[0].end+tgcube.graphList[0].base<<endl;
    cerr<<"vertex num: "<<tgcube.graphList[0].vertexTabl.dataList.size()<<" edge num: "<<tgcube.graphList[0].size<<endl;
    start=end;
    vector<vector<int> > mList={{0},{0,1},{0,1,2},{0,1,2,3},{0,1,2,3,4}};
    // tgcube.fullMaterialize();
    tgcube.selectMaterialize(mList);
    end=clock();
    cerr<<"materialize done with "<<(end-start)*1000.0/CLOCKS_PER_SEC<<endl;

    for(int i=0;i<tgcube.graphList.size();++i){
        TempGraph& curGraph=tgcube.graphList[i];
        for(int j=0;j<curGraph.vertexTabl.attriPosInOri.size();++j){
            cout<<curGraph.vertexTabl.attriPosInOri[j]<<" ";
        }
        cout<<endl;
        for(int j=0;j<curGraph.snapshotVec.size();++j){
            cout<<curGraph.snapshotVec[j].edgePool.size()<<" ";
        }
        cout<<endl<<endl;
    }

    return 0;
}