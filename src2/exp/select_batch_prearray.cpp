#include"../TempGCube.hpp"
#include<sys/time.h>


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
    start=end;
    tgcube.buildIndex(4);
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
            int res=tgcube.query(4,allIntParam[i][0],allIntParam[i][1],allIntParam[i][2],allSegs[i],ans);
        }
        gettimeofday(&partend,NULL);

        cout<<"single time: "<<(partend.tv_sec-partstart.tv_sec)*1000+(partend.tv_usec-partstart.tv_usec)/1000<<endl;
    }
    gettimeofday(&endtime,NULL);
    cerr<<"time: "<<(endtime.tv_sec-starttime.tv_sec)*1000+(endtime.tv_usec-starttime.tv_usec)/1000<<endl;

    return 0;
}