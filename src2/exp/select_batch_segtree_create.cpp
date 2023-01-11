#include"../TempGCube.hpp"
#include<sys/time.h>
vector<int> TempGCube::logTable=vector<int>();

int main(int argc,char* argv[]){
    if(argc!=3){
        cout<<"parameter error"<<endl;
        return 1;
    }
    int start,end;
    int indexType=5;

    TempGCube tgcube;
    start=clock();
    tgcube.init(argv[1],argv[2]);
    end=clock();
    cerr<<"init done with "<<(end-start)*1000.0/CLOCKS_PER_SEC<<endl;
    cerr<<"start time: "<<tgcube.graphList[0].base+1<<" end: "<<tgcube.graphList[0].end+tgcube.graphList[0].base<<endl;
    cerr<<"vertex num: "<<tgcube.graphList[0].vertexTabl.dataList.size()<<" edge num: "<<tgcube.graphList[0].size<<endl;
    start=end;
    // vector<vector<int> > mList={{0},{0,1},{0,1,2},{0,1,2,3},{0,1,2,3,4}};
    vector<vector<int> > mList={{0,1,2,3,4}};
    // tgcube.fullMaterialize();
    tgcube.selectMaterialize(mList);
    end=clock();
    cerr<<"materialize done with "<<(end-start)*1000.0/CLOCKS_PER_SEC<<endl;

    tgcube.createWithSimilarity(1,10);
    tgcube.createWithSimilarity(1,20);
    tgcube.createWithSimilarity(1,50);
    tgcube.createWithSimilarity(1,80);
    tgcube.materialized[{"pro","field","genre","isadult","averating","10"}]=2;
    tgcube.materialized[{"pro","field","genre","isadult","averating","20"}]=3;
    tgcube.materialized[{"pro","field","genre","isadult","averating","50"}]=4;
    tgcube.materialized[{"pro","field","genre","isadult","averating","80"}]=5;
    cerr<<"create done"<<endl;

    start=clock();
    tgcube.buildIndex(indexType);
    end=clock();
    cerr<<"build prearray done with "<<(end-start)*1000.0/CLOCKS_PER_SEC<<endl;

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
    timeval starttime,endtime;
    gettimeofday(&starttime,NULL);
    for(int i=0;i<allSegs.size();++i){
        timeval partstart,partend;
        gettimeofday(&partstart,NULL);
        for(int j=0;j<allIntParam[i][3];++j){
            TempGraph ans;
            int res=tgcube.query(indexType,allIntParam[i][0],allIntParam[i][1],allIntParam[i][2],false,allSegs[i],ans);
        }
        gettimeofday(&partend,NULL);

        cout<<"single time: "<<(partend.tv_sec-partstart.tv_sec)*1000+(partend.tv_usec-partstart.tv_usec)/1000<<endl;
    }
    gettimeofday(&endtime,NULL);
    cerr<<"time: "<<(endtime.tv_sec-starttime.tv_sec)*1000+(endtime.tv_usec-starttime.tv_usec)/1000<<endl;

    return 0;
}