// Project Identifier: 9B734EC0C043C5A836EA0EBE4BEFEA164490B2C7
#include <vector>
#include <getopt.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <limits>
#include"xcode_redirect.hpp"

using namespace std;

class among{
private:
    char mode;
    struct point{
        int x;
        int y;
        bool vis;
        point(int x1,int y1):x(x1),y(y1),vis(false){}
    };
    struct check{
        double d = numeric_limits<double>::infinity();
        bool vis = false;
    };
    vector<point>map;
    vector<double>dist;
    vector<size_t>pred;
    
    vector<size_t>path;// a copy of map, false vis
    vector<size_t>best;
    bool de=false;//TODO: needed?
    bool lab=false;
    bool other=false;
    double currBest=0;
    double currCost=0;
    vector<vector<double>>dm;
    
public:
    among(){}
    void constructOPT(){
        currBest=lower();
        calDM();
        path.pop_back();
//        currBest = 336.74;
//        for(int i =0; i< 11;i++){
//            path.push_back(i);
//        }
        best=path;
        
        genPerms(1);//TODO:?
    }
  
    void calDM(){
        dm.resize(map.size(),vector<double>(map.size()));
        for(size_t i = 0; i < map.size(); i++){
            for(size_t j = 0; j < map.size(); j++){
                if(i!=j)
                dm[i][j] = distanceO(i, j);
            }
        }
    }
    double CMST(size_t permLength){
        vector<check>c;
        c.resize(map.size()-permLength);
    
        c[0].d=0;
        
        //placeholder
        size_t min=0;
        double sum=0;
        
        for(size_t i = 0; i < c.size();i++){
            double tdist=numeric_limits<double>::infinity();
            
            //find the smallest dist
            for(size_t j = 0; j < c.size(); j++){
                if(!c[j].vis && c[j].d < tdist){
                    tdist =  c[j].d;
                        min = j;
                }
            }
           
            c[min].vis=true;
            sum += sqrt(c[min].d);
            
            for(size_t j = 0;j < c.size(); j++){
                if(!c[j].vis){
                    double dn=dm[path[j+permLength]][path[min+permLength]];
                    if(dn < c[j].d){
                        c[j].d=dn;
                    }
                }
            }
        }
        
        return sum;
    }
    
    void genPerms(size_t permLength) {
      if(currCost>=currBest) return ;
      if (permLength == map.size()) {
          update();
          return ;
      }
      if (!promising(permLength)) return;
      for (size_t i = permLength; i < map.size(); ++i) {
          swap(path[permLength], path[i]);
          double tem=distanceT(path[permLength-1], path[permLength]);
          currCost+=tem;
          genPerms(permLength + 1);
          currCost-=tem;
          swap(path[permLength], path[i]);
      }
    }
    void update(){
        double cost = distanceT(path[path.size()-1], 0);
        currCost+=cost;//add the last edge
        if(currCost<currBest){
            currBest=currCost;
            best=path;
        }
        currCost-=cost;
    }
    bool promising(size_t permLength){
        if(currCost>=currBest) return false;
        if(path.size()-permLength < 5) return true;
        double d1=numeric_limits<double>::infinity();
        double d2=numeric_limits<double>::infinity();
        for(size_t i=permLength;i<path.size();i++){
            double df=dm[0][path[i]];
            double dl=dm[path[permLength-1]][path[i]];
            if(df<d1){
                d1=df;
            }
            if(dl<d2){
                d2=dl;
            }
        }
        double sum = sqrt(d1)+sqrt(d2);
        if(currCost+sum>=currBest) return false;
        double est=CMST(permLength)+sum;
        
//        for (size_t i = 0; i < path.size(); ++i)
//            cerr << setw(2) << path[i] << ' ';
//        cerr << setw(4) << permLength << setw(10) << currCost;
//        cerr << setw(10) << sqrt(d1) << setw(10) << sqrt(d2);
//        cerr << setw(10) << CMST(permLength) << setw(10) << currCost+est << "  " << (est+currCost<currBest) << '\n';

        if(est+currCost<currBest) {
            return true;
        }
        return false;
    }
  
    
    
    
    

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    void read(){
        uint32_t size;
        cin>>size;
        int x;
        int y;
        while(cin>>x>>y){
            map.emplace_back(x,y);
            if(mode=='m') MSTcheck(x, y);
        }
        dist.resize(map.size(),numeric_limits<double>::infinity());
        if(mode=='m')pred.resize(map.size());
    }
 
    double lower(){
      path = constructFAST();
       
        return sumWeightF(path);
    }
    void printVertex(){
        vector<size_t>v;
        if(mode=='f'){
            v = constructFAST();
            cout<<sumWeightF(v)<<"\n";
            for(uint32_t i=0;i<v.size()-1;i++){
                cout<<v[i]<<" ";
            }
        }else{
            constructOPT();
            cout<<currBest<<"\n";
            for(uint32_t i=0;i<best.size();i++){
                cout<<best[i]<<" ";
            }
        }
    }
    
    
             void MSTout(){
                 if(!de&&lab&&other){
                     cerr<<"Cannot construct MST\n";
                     exit(1);
                 }
                 constructMST();
                 cout<<sumWeight()<<"\n";
                 printEdge();
             }
    double distanceO(size_t count,size_t count1){
        double xdif=map[count].x-map[count1].x;
        double ydif=map[count].y-map[count1].y;
        double in=xdif*xdif+ydif*ydif;
        return in;
    }
   
             
             double distanceT(size_t count,size_t count1){
                 double xdif=map[count].x-map[count1].x;
                 double ydif=map[count].y-map[count1].y;
                 double in=xdif*xdif+ydif*ydif;
                 return sqrt(in);
             }
    
    void MSTcheck(int x, int y){
        if((x==0&&y<0)||(x<0&&y==0)||(x==0&&y==0))de=true;//TODO: needed?
        else if(lab==false&&(x<0&&y<0)){
            lab=true;
        }
        else if(other==false&&(x>0||y>0)){
            other=true;
        }
    }
    double distance(size_t count,size_t count1){
        int fx=map[count].x;
        int sx=map[count1].x;
        int fy=map[count].y;
        int sy=map[count1].y;
        if((fx<0&&fy<0)||(sx<0&&sy<0)){
            if(fx>0||fy>0||sx>0||sy>0){
                return numeric_limits<double>::infinity();
            }
        }
        double xdif=map[count].x-map[count1].x;
        double ydif=map[count].y-map[count1].y;
        double in=xdif*xdif+ydif*ydif;
        return in;
    }
    double sumWeight(){
        double sum=0;
        for(auto d:dist){
            sum+=sqrt(d);
        }
        return sum;
    }
    void printEdge(){
        for(size_t i = 1;i<map.size();i++){
            if(i < pred[i]) cout<<i<<" "<<pred[i]<<"\n";
            else{
                cout<<pred[i]<<" "<<i<<"\n";
            }
        }
    }
    void constructMST(){
        dist[0]=0;
        size_t min=0;
        for(size_t i = 0; i < map.size();i++){
            double tdist=numeric_limits<double>::infinity();
            for(size_t j=0; j<map.size(); j++){
                if(!map[j].vis && dist[j] < tdist){
                        tdist = dist[j];
                        min = j;
                }
            }
            map[min].vis=true;
            for(size_t j = 0;j < map.size(); j++){
                if(!map[j].vis){
                    double d=distance(min, j);
                    if(d < dist[j]){
                        dist[j]=d;
                        pred[j]=min;
                    }
                }
            }
        }
    }
 
    
    
    vector<size_t> constructFAST(){
        vector<size_t> tour;
             tour.push_back(0);
             map[0].vis = true;
        size_t ini = 0;
             double tdist = numeric_limits<double>::infinity();
             for (size_t i = 1; i < map.size(); ++i) {
                 double distt = distanceO(0, i);
                   if (distt < tdist) {
                       tdist = distt;
                       ini = i;
                   }
               }
               tour.push_back(ini);
               map[ini].vis = true;
               tour.push_back(0);

            // remaining
            for (size_t i = 2; i < map.size(); ++i) {
                size_t next = 0;
                double change = numeric_limits<double>::infinity();

                for (size_t j = 1; j < map.size(); ++j) {
                    if (!map[j].vis) {
                        next = j;
                        break;
                    }
                }

                size_t pos = 0;
                change = numeric_limits<double>::infinity();

                for (size_t j = 0; j < tour.size()-1; ++j) {
                    size_t A = tour[j];
                    size_t B = tour[j + 1];

                    double increase = distanceT(A,next) + distanceT(B,next) - distanceT(A,B);

                    if (increase < change) {
                        change = increase;
                        pos = j + 1;
                    }
                }

                tour.insert(tour.begin() + static_cast<uint32_t>(pos), next);
                map[next].vis = true;
            }
          return tour;
    }
   
   
    double sumWeightF(vector<size_t>&v){
        double sum=0;
        for(uint32_t i=0;i<v.size()-1;i++){
            sum+=distanceT(v[i], v[i+1]);
        }
        return sum;
    }
  
  
    void out(){
        if(mode=='m') MSTout();
        else {
            printVertex();
        }
    }
    void getMode(int argc, char * argv[]);

    
    
   

};

void among::getMode(int argc, char * argv[]) {
            opterr = false;
            int choice;
            int index = 0;
            int count = 0;
            option long_options[] = {
                {"help",no_argument,nullptr,'h'},
                {"mode",required_argument,nullptr,'m'},
                { nullptr, 0, nullptr, '\0' }};
            
            while ((choice = getopt_long(argc, argv, "m:h",
                                         long_options, &index)) != -1) {
                switch (choice) {
                    case 'h':
                        cerr << "idk\n";
                        exit(0);
                    case 'm':{
                        string arg{optarg};
                        if(arg=="MST"){
                            mode='m';
                            count+=1;
                        }
                        else if(arg=="FASTTSP"){
                            mode='f';
                            count+=1;
                        }
                        else if (arg=="OPTTSP"){
                            mode='o';
                            count+=1;
                        }
                        else {
                            cerr<<"Error: Invalid mode\n";
                            exit(1);
                        }
                        break;
                    }
                     default:
                        cerr << "Error: Invalid command line option\n";
                        exit(1);
                }
            }
            if (count==0) {
                cerr << "Error: No mode specified\n";
                exit(1);
            }
    }



int main(int argc, char*argv[]){
    //cerr << fixed << showpoint << setprecision(2);
    cout << std::setprecision(2);
    cout << std::fixed;
    ios_base::sync_with_stdio(false);
    among a;
    a.getMode(argc, argv);
    xcode_redirect(argc, argv);
    a.read();
    a.out();
    
   
}
