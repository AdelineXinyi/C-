//AD48FB4835AF347EB0CA8009E24C3B13F8519882
#include <vector>
#include <queue>
#include <getopt.h>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include"xcode_redirect.hpp"
#include"P2random.h"
using namespace std;

class Galaxy{
public:
    void read(){
        stringstream ss;
        bool PR = readhead();
        
        if (PR) {
                    uint32_t seed;
                    uint32_t num_d;
                    uint32_t a_r;
                    string junk;
                    cin>>junk>>seed>>junk>>num_d>>junk>>a_r;
                    P2random::PR_init(ss, seed, gen_num, planet_num, num_d, a_r);
        }
                
        istream &inputStream = PR ? ss : cin;
        char type;
        char in;
        string junk;
        int ts;
        int g_id;
        int p_id;
        int f;
        int tr;
        dep d;
        uint32_t seq=0;
        cout<<"Deploying troops...\n";
        while (inputStream >> ts >> type >> junk >> in>> g_id >> in>>
                       p_id >> in >> f >> in >> tr) {
                check(ts,g_id,p_id,f,tr);
                uint32_t uts=static_cast<uint32_t>(ts);
                uint32_t up_id=static_cast<uint32_t>(p_id);
                if(uts!=time_stamp){
                  if(m){
                    printM();
                  }
                  time_stamp=uts;
                  d.timestamp=uts;
                }
                    d.force=static_cast<uint32_t>(f);
                    d.troops=static_cast<uint32_t>(tr);
                    d.g_id=static_cast<uint32_t>(g_id);
                    d.seq=seq;
                    
                    if(type=='J'){
                        g[d.g_id].j_num+=static_cast<uint32_t>(tr);
                        p[up_id].Jedi.push(d);
                        p[up_id].w.updateAttackJ(d);
                        p[up_id].w.updateAmbushJ(d);
                    }
                    else if(type=='S'){
                        g[d.g_id].s_num+=static_cast<uint32_t>(tr);
                        p[up_id].Sith.push(d);
                        p[up_id].w.updateAttackS(d);
                        p[up_id].w.updateAmbushS(d);
                    }
                    seq+=1;
                    attack(up_id);
                }
        printend();
   }
    
   
void getMode(int argc, char * argv[]) {
            opterr = false;
            int choice;
            int index = 0;
            option long_options[] = {
                {"verbose",no_argument,nullptr,'v'},
                {"median",no_argument,nullptr,'m'},
                {"general-eval",no_argument,nullptr,'g'},
                {"watcher",no_argument,nullptr,'w'},
                { nullptr, 0, nullptr, '\0' }};
            
            while ((choice = getopt_long(argc, argv, "vmgw",
                                         long_options, &index)) != -1) {
                switch (choice) {
                    case 'v':
                        v=true;
                        break;
                    case 'm':
                        m=true;
                        break;
                    case 'g':
                        ge=true;
                        break;
                    case 'w':
                        w=true;
                        break;
                    default:
                        cerr << "Unknown command line option\n";
                        exit(1);
                }
            }
       
    }
    
    uint32_t get_total(){
        uint32_t sum=0;
        for(uint32_t i=0;i<planet_num;i++){
            sum+=p[i].battles;
        }
        return sum;
    }
    
    void printW(){
        cout<<"---Movie Watcher---\n";
        for(uint32_t i=0;i<planet_num;i++){
            if(p[i].w.s1jedi.troops==0||p[i].w.s1sith.troops==0||p[i].w.s1jedi.force>p[i].w.s1sith.force){
                cout<<"A movie watcher would not see an interesting ambush on planet "
                   <<i<<".\n";
            }
            else{
                    cout<<"A movie watcher would enjoy an ambush on planet "<<i<<" with Sith at time "<<p[i].w.s1sith.timestamp<<" and Jedi at time "<<
                    p[i].w.s1jedi.timestamp<<" with a force difference of " << p[i].w.s1sith.force-p[i].w.s1jedi.force <<".\n";
            }
            
            if(p[i].w.mwjedi.troops==0||p[i].w.mwsith.troops==0||p[i].w.mwjedi.force>p[i].w.mwsith.force){
              cout<<"A movie watcher would not see an interesting attack on planet "
                <<i<<".\n";
            }
            else {
                cout<<"A movie watcher would enjoy an attack on planet "<<i
                <<" with Jedi at time "<< p[i].w.mwjedi.timestamp<<" and Sith at time "<<p[i].w.mwsith.timestamp<<" with a force difference of "<<p[i].w.mwsith.force-p[i].w.mwjedi.force<<".\n";
            }
        }
    }
    
    void printG(){
        cout<<"---General Evaluation---\n";
        for(uint32_t i=0;i<gen_num;i++){
            cout<<"General "<<i<<" deployed "<<g[i].j_num<<" Jedi troops and "<<
            g[i].s_num<<" Sith troops, and "<<g[i].s_num+g[i].j_num-g[i].reduce<<"/"<<g[i].s_num+g[i].j_num<<
            " troops survived.\n";
        }
    }
    
    void updateM(uint32_t p_id, uint32_t val){
        if(val>=p[p_id].max.top())
            p[p_id].max.push(val);
        
           else //Otherwise it is a lower number
               p[p_id].min.push(val);

           if(p[p_id].max.size()-p[p_id].min.size()==2){
               p[p_id].min.push(p[p_id].max.top());
               p[p_id].max.pop();
           } else if( p[p_id].min.size()- p[p_id].max.size()==2){
               p[p_id].max.push(p[p_id].min.top());
               p[p_id].min.pop();
           }
    }
    
    uint32_t getMedian(uint32_t p_id) const {
        if(p[p_id].max.size()==p[p_id].min.size())
             return (p[p_id].max.top()+p[p_id].min.top())/2;
           else if(p[p_id].max.size()>p[p_id].min.size())
             return p[p_id].max.top();
           else
             return p[p_id].min.top();
      }
    
    void printM(){
        for(uint32_t i=0;i<planet_num;i++){
            if(p[i].battles!=0){
                cout<<"Median troops lost on planet "<<i<<" at time "<<time_stamp
                <<" is "<< getMedian(i)<<".\n";
            }
        }
    }
    
    
    
    void attack(uint32_t p_id){
        if(p[p_id].Sith.size()!=0 &&
           p[p_id].Jedi.size()!=0){
            
            dep sith = p[p_id].Sith.top();
            dep jedi = p[p_id].Jedi.top();
            uint32_t amt = min(sith.troops,jedi.troops);
            if(sith.force >= jedi.force){
                p[p_id].Sith.top().troops -= amt;
                p[p_id].Jedi.top().troops -= amt;
                p[p_id].battles += 1;
                g[sith.g_id].reduce += amt;
                g[jedi.g_id].reduce += amt;
                if(amt >0){
                    updateM(p_id,2*amt);
                    if(v){
                        cout<<"General " << sith.g_id 
                        << "'s battalion attacked General "
                        <<jedi.g_id
                        <<"'s battalion on planet "<<p_id<<". "<< 2*amt
                        <<" troops were lost.\n";
                    }
                }
                if(p[p_id].Sith.top().troops==0){
                    p[p_id].Sith.pop();
                }
                if(p[p_id].Jedi.top().troops==0){
                    p[p_id].Jedi.pop();
                }
                if(p[p_id].Jedi.top().force<=p[p_id].Sith.top().force){
                    attack(p_id);
                }
            }
        }
    }
    
   
    
    void printend(){
        if(m){
            printM();
        }
        cout<<"---End of Day---\n"<<"Battles: "<<
        get_total()<<"\n";
        if (ge){
            printG();
        }
        if(w){
            printW();
        }
    }

   
    bool readhead(){
        string line;
        bool PR = false;
        getline(cin,line);
        getline(cin, line);
        
        for(auto c:line){
            if(c=='P'){
                PR=true;
            }
        }
        cin>>line>>gen_num>>line>>planet_num;
        g.resize(gen_num);
        p.resize(planet_num);
        for(uint32_t i=0;i<planet_num;i++){
            p[i].max.push(1000000);
            p[i].min.push(0);
        }
        return PR;
    }
    
    void check(int ts,int g_id,int p_id,int f,int tr){
    
    if(static_cast<uint32_t>(g_id)>=gen_num||g_id<0){
        cerr<<"Invalid general ID\n";
        exit(1);
    }
    else if(static_cast<uint32_t>(p_id)>=planet_num||p_id<0){
        cerr<<"Invalid planet ID\n";
        exit(1);
    }
    else if(f<=0){
        cerr<<"Invalid force sensntivity level\n";
        exit(1);
    }
    else if(tr<=0){
        cerr<<"Invalid number of troops\n";
        exit(1);
    }
    else if(static_cast<uint32_t>(ts)<time_stamp){//
        cerr<<"Invalid decreasing timestamp\n";
        exit(1);
    }
}
    Galaxy(){
    }
    
private:
    uint32_t time_stamp = 0;
    bool m=false;
    bool v=false;
    bool ge=false;
    bool w=false;
    
    struct general{
        uint32_t s_num=0;
        uint32_t j_num=0;
        uint32_t reduce=0;
    };
    struct dep{
        uint32_t timestamp=0;//conversion from int?
        uint32_t seq=0;
        uint32_t force=0;
        mutable uint32_t troops=0;//can't
        uint32_t g_id=0;
    };
    struct less{
        bool operator()(const dep &d1, const dep &d2) const {
            if(d1.force<d2.force){//if true then switch
                return true;
            }else if(d1.force==d2.force && d1.seq > d2.seq){//if true then switch
                return true;
            }
            return false;
        }
    };
   
    struct greater{
        bool operator()(const dep &d1, const dep &d2) const {
            if(d1.force > d2.force){
                return true;
            }else if(d1.force==d2.force && d1.seq > d2.seq){
                return true;
            }
            return false;
        }
    };
    
    class watcher{
    public:
        watcher(){
        }
        
        void updateAttackJ(dep& jedi){
            //initiate
            if(mwjedi.troops==0){
               mwjedi=jedi;
            }
            //if there's previous jedi and no war happened
            //if best jedi force smaller than current
            else if(mwjedi.force>jedi.force &&
                    mwsith.troops==0){
                    mwjedi=jedi;
            }
            //there's previous jedi but has a match
            else if(mwjedi.force>jedi.force &&
                   mwsith.troops!=0){
                if(s2jedi.troops==0||s2jedi.force>jedi.force){//
                    s2jedi=jedi;//
                }//
              
            }
        }
        
        void updateAttackS(dep& sith){
            if(mwjedi.troops!=0 && mwsith.troops==0 && 
               sith.force>=mwjedi.force){//added
               mwsith=sith;
            }//added
            //attack: if jedi already assigned then add sith
            if(mwjedi.troops!=0 && 
               mwsith.troops!=0 &&
               mwsith.force < sith.force &&
               s2jedi.troops == 0 &&
               sith.force >= mwjedi.force){
               mwsith = sith;
            }
           
            //if there's maybe jedi,new sith-maybe jedi better than previous
            if(s2jedi.troops!=0 &&
               sith.force-s2jedi.force>mwsith.force-mwjedi.force &&
               sith.force >= mwjedi.force){
                mwsith = sith;
                
                if(sith.force-s2jedi.force>sith.force-mwjedi.force &&
                   mwsith.force>=s2jedi.force){//added
                    mwjedi=s2jedi;
                    s2jedi.troops=0;
                }//added
            }
        }
        
        void updateAmbushJ(dep& jedi){
            if(s1sith.troops!=0 && s1jedi.troops==0 &&
               jedi.force<=s1sith.force){//added
               s1jedi=jedi;
            }//added
            //attack: if jedi already assigned then add sith
            if(s1sith.troops!=0 &&
               s1jedi.troops!=0 &&
               s1jedi.force > jedi.force &&
               s2sith.troops == 0 &&
               jedi.force <= s1sith.force){
               s1jedi = jedi;
            }
            //if there's maybe jedi,new sith-maybe jedi better than previous
            if(s2sith.troops!=0 &&
               s2sith.force-jedi.force>s1sith.force-s1jedi.force &&
               jedi.force <= s1sith.force){
                s1jedi = jedi;
                
                if(s2sith.force-jedi.force>s1sith.force-jedi.force &&
                   s2sith.force>=s1jedi.force){//added
                    s1sith=s2sith;
                    s2sith.troops=0;
                }//added
            }
            }
        
        void updateAmbushS(dep& sith){
            if(s1sith.troops==0){
               s1sith=sith;
            }
            //if there's previous jedi and no war happened
            //if best jedi force smaller than current
            else if(s1sith.force<sith.force &&
                    s1jedi.troops==0){
                    s1sith=sith;
            }
            //there's previous jedi but has a match
            else if(s1sith.force<sith.force &&
                   s1jedi.troops!=0){
                if(s2sith.troops==0||s2sith.force<sith.force){//
                    s2sith=sith;//
                }//
              
            }
        }
        
        dep mwjedi;
        dep mwsith;
        dep s2jedi;
        dep s1jedi;
        dep s1sith;
        dep s2sith;
    };
    struct planet{
        priority_queue<dep, vector<dep>, greater> Jedi;
        priority_queue<dep, vector<dep>, less> Sith;
        uint32_t battles;
        priority_queue<uint32_t> min;
        priority_queue<uint32_t,vector<uint32_t>,std::greater<uint32_t> > max;
        watcher w;
        
    };
    
    vector<planet> p;
    vector<general> g;
    uint32_t gen_num=0;
    uint32_t planet_num=0;

};














int main(int argc, char*argv[]){
ios_base::sync_with_stdio(false);
xcode_redirect(argc, argv);
    Galaxy g;
    g.getMode(argc, argv);
    g.read();
}
