//01BD41C3BF016AD7E8B6F837DF18926EC3E83350
#include <vector>
#include <deque>
#include <getopt.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#include"xcode_redirect.hpp"

using namespace std;

class logman{
private:
    struct log{
        uint32_t id;
        int64_t time;
        string t_print;
        string cat;
        string cat_lower;
        string mes;
        log(uint32_t idp,
                int64_t timep,
                string& t_printp,
                string& catp,
                string& cat_lowerp,
                string& mesp):
            id(idp),
            time(timep),
            t_print(t_printp),
            cat(catp),
            cat_lower(cat_lowerp),
            mes(mesp){}
    };
    string to_lower(const string& s) {
        string g = s;
        for(char &c : g) {
            if(isalpha(c))
                c = static_cast<char>(tolower(c));
        }
        return g;
    }
    //trannsform
struct comp{
    bool operator()(const log &l1, const log &l2) {
        if(l1.time<l2.time)return true;
        else if(l1.time==l2.time&&l1.cat_lower<l2.cat_lower){//strcasesmp
            return true;
        }
        else if(l1.time==l2.time
                &&l1.cat_lower==l2.cat_lower
                &&l1.id<l2.id){
            return true;
        }
        return false;
    }

};

    struct TimeComp {
        bool operator()(const log &l1, const int64_t &t2){
            return l1.time<t2;
        }
        bool operator()(const int64_t &t1, const log &l2){
            return t1<l2.time;
        }
    };
    vector<log> v;
    vector<uint32_t>append_id;
    deque<uint32_t> excerpt;
    vector<uint32_t>recent;
    unordered_map<string, vector<uint32_t>> c;
    unordered_map<string, vector<uint32_t>> k;
    
public:
    logman(){
    }
    void read(ifstream& entry){
        string cat_lower;
        string cat;
        string mes;
        string t;
        uint32_t count=0;
        while(getline(entry,t,'|')){
            string t_p=t;
            t.erase(remove(t.begin(), t.end(), ':'), t.end());
            getline(entry,cat,'|');
            getline(entry,mes);
            cat_lower=to_lower(cat);
            v.emplace_back(count,stol(t),t_p,cat,cat_lower,mes);
            count++;
        }
        sort(v.begin(),v.end(),comp());
        sub_ck();
        cout<<count<<" entries read\n";
    }
    
    void ini_k(string& s, uint32_t index){
        string word="";
        string wordl;
        for(auto c:s){
            if (isalpha(c) || isdigit(c)) {
                word+=c;
            }
            else {
                wordl = to_lower(word);
                auto it = k.find(wordl);
                if(it==k.end()||it->second.back()!=index){
                    k[wordl].push_back(index);
                }
                word="";
            }
        }
        wordl=to_lower(word);
        auto it = k.find(wordl);
        if(it==k.end()||it->second.back()!=index)
            k[wordl].push_back(index);
    }
    void ini_k_c(string& s, uint32_t index){
        string word="";
        for(auto c:s){
            if (isalpha(c) || isdigit(c)) {
                word+=c;
            }
            else {
                auto it = k.find(word);
                if(it==k.end()||it->second.back()!=index){
                    k[word].push_back(index);
                }
                word="";
            }
        }
        auto it = k.find(word);
        if(it==k.end()||it->second.back()!=index)
            k[word].push_back(index);
    }
    void sub_ck(){
        uint32_t index=0;
        append_id.resize(v.size());
        for(auto lg:v){
            append_id[lg.id]=index;
            ini_k(lg.mes, index);
            ini_k_c(lg.cat_lower, index);
            c[lg.cat_lower].push_back(index);
            index++;
        }
    }
    
    void get_command(){
        char cmd;
        string junk;
        char prev='n';
        recent.resize(v.size());
        do{
            cout<<"% ";
            cin>>cmd;
            while (cin.peek() == ' ')
                   cin.get();
            switch(cmd){
                case '#':
                    getline(cin,junk);
                    break;
                case 'a':
                    append();
                    break;
                case 'g':
                    printG(prev);
                    break;
                case 'p':
                    printE();
                    break;
                case 'd':
                    del();
                    break;
                case 'b':
                    begin();
                    break;
                case 's':
                    sortE();
                    break;
                case 'l':
                    clearE();
                    break;
                case 'e':
                    end();
                    break;
                case 'r':
                    rappend(prev);
                    break;
                case 't':
                    recent.clear();
                    search_time();
                    prev=cmd;
                    break;
                case 'm':
                    recent.clear();
                    search_match();
                    prev=cmd;
                    break;
                case 'c':
                    recent.clear();
                    search_cat();
                    prev=cmd;
                    break;
                case 'k':
                    recent.clear();
                    search_key();
                    prev=cmd;
                    break;
                case 'q':
                    break;
                default:
                    cerr<<cmd<<"doesn't exist\n";
                    break;
            }
        }while(cmd!='q');
    }
    
    
    void par_k(string& s, vector<string>& vec){
        string word="";
        for(auto c:s){
            if (isalpha(c) || isdigit(c)) {
                word+=c;
            }
            else if(word!=""){
                vec.push_back(to_lower(word));
                word="";
            }
        }
        if(word!="")vec.push_back(to_lower(word));
    }
    
    void search_key(){
       vector<string> word;
       string key;
       getline(cin, key);
       par_k(key,word);//TODO: ?
       auto last = k.find(word[word.size()-1]);
       word.pop_back();
        if(word.size()>0){
            bool begin = true;
            for(auto fd:word){
                auto keywordIt = k.find(fd);
                if(keywordIt!=k.end() && last!=k.end()){
                    if(begin){
                        set_intersection(keywordIt->second.begin(), keywordIt->second.end(), last->second.begin(), last->second.end(), back_inserter(recent));
                        begin=false;
                    }
                    else{
                       auto it = set_intersection(keywordIt->second.begin(), keywordIt->second.end(), recent.begin(), recent.end(),recent.begin());
                        recent.erase(it,recent.end());
                    }
                }
                else{
                    recent.clear();
                    break;
                }
            }
        }
        else{
            if(last!=k.end())
            recent=last->second;
        }
       cout<<"Keyword search: "<<recent.size()<<" entries found\n";
}
    //O(1)
    void search_cat(){
        size_t count=0;
        string cat;
        getline(cin,cat);
        auto it = c.find(to_lower(cat));
        if(it!=c.end()){
            count = it->second.size();
            recent = it->second;
        }
        cout<<"Category search: "<<count<<" entries found\n";
    }
   
    //log(n)
    void search_time(){
        string t_check;
        string t1;
        string t2;
        getline(cin, t_check);
        if(t_check.size()!=29||t_check[14]!='|'){//14+14+1
            cerr<<"Time stamp wrong size\n";//TODO: need to return something to show reprompt
            return;
        }
        stringstream s;
        s<<t_check;
        getline(s,t1,'|');
        getline(s,t2);
        t1.erase(remove(t1.begin(), t1.end(), ':'), t1.end());
        t2.erase(remove(t2.begin(), t2.end(), ':'), t2.end());
        int64_t it1=stol(t1);
        int64_t it2=stol(t2);
        auto l1=lower_bound(v.begin(), v.end(), it1, TimeComp());
        recent.push_back(static_cast<uint32_t>(l1-v.begin()));
        auto l2=upper_bound(l1, v.end(), it2, TimeComp());
        recent.push_back(static_cast<uint32_t>(l2-v.begin()));
        cout<<"Timestamps search: "<<distance(l1,l2)<<" entries found\n";
    }
    
    //log(n)
    void search_match(){
        string t;
        getline(cin,t);
        if(t.size()!=14){//14+14+1
            cerr<<"Time stamp wrong size\n";//TODO: need to return something to show reprompt
            return;
        }
        t.erase(remove(t.begin(), t.end(), ':'), t.end());
        int64_t it=stol(t);
        auto l=lower_bound(v.begin(), v.end(), it, TimeComp());
        auto h=upper_bound(l, v.end(), it, TimeComp());
        if(distance(l,h)!=0){
            recent.push_back(static_cast<uint32_t>(l-v.begin()));
            recent.push_back(static_cast<uint32_t>(h-v.begin()));
        }
        cout<<"Timestamp search: "<<distance(l,h)<<" entries found\n";
    }
    
    void clearE(){
        cout<<"excerpt list cleared\n";
        if(excerpt.empty()) {
            cout<<"(previously empty)\n";
            return;
        }
        log first = v[excerpt.front()];
        log end = v[excerpt.back()];
        size_t size = excerpt.size();
        excerpt.clear();
        cout<<"previous contents:\n"<<"0|";
        printLog(first);
        cout<<"...\n"<<size-1<<"|";
        printLog(end);
    }
    

    static void printLog(const log& lg){
        cout<<lg.id<<"|"<<lg.t_print<<"|"<<lg.cat<<"|"<<lg.mes<<"\n";
    }
    
    
    void sortE(){
        cout<<"excerpt list sorted\n";
        if(excerpt.empty()) {
            cout<<"(previously empty)\n";
            return;
        }
        log first = v[excerpt.front()];
        log end = v[excerpt.back()];
        sort(excerpt.begin(),excerpt.end());
        cout<<"previous ordering:\n"<<"0|";
        printLog(first);
        cout<<"...\n";
        cout<<excerpt.size()-1<<"|";
        printLog(end);
        
        first = v[excerpt.front()];
        end = v[excerpt.back()];
        cout<<"new ordering:\n"<<"0|";
        printLog(first);
        cout<<"...\n";
        cout<<excerpt.size()-1<<"|";
        printLog(end);
    }
    
    void del(){
        int pos;
        cin>>pos;
        if(pos<0 || pos>=static_cast<int>(excerpt.size())) return;
        if(excerpt.begin()+pos!=excerpt.end()){
            excerpt.erase(excerpt.begin()+pos);
        }
        cout<<"Deleted excerpt list entry "<<pos<<"\n";
    }
    void begin(){
        int pos;
        cin>>pos;
        if(pos<0 || pos>=static_cast<int>(excerpt.size())) return;
        if(excerpt.begin()+pos!=excerpt.end()){
            excerpt.push_front(*(excerpt.begin()+pos));
            excerpt.erase(excerpt.begin()+pos+1);
        }
        cout<<"Moved excerpt list entry "<<pos<<"\n";
    }
    
    void end(){
        int pos;
        cin>>pos;
        if(pos<0 || pos>=static_cast<int>(excerpt.size())) return;
        if(excerpt.begin()+pos!=excerpt.end()){
            excerpt.push_back(*(excerpt.begin()+pos));
            excerpt.erase(excerpt.begin()+pos);
        }
        cout<<"Moved excerpt list entry "<<pos<<"\n";
    }
    
    void append(){
            uint32_t id;
            cin>>id;
            if(id>=v.size()) return;
            excerpt.push_back(append_id[id]);
            cout<<"log entry "<<id<<" appended\n";
    }
    void rappend(char prev){
            if(prev=='t'||prev=='m'){
                if(recent.size()!=0){
                    for(uint32_t i=recent.front();i<recent.back();i++){
                        excerpt.push_back(i);
                    }
                    cout<<recent.back()-recent.front()<<" log entries appended\n";
                    return;
                }
                cout<<0<<" log entries appended\n";
            }
            else if(prev=='c'||prev=='k'){
                for(auto res:recent){
                    excerpt.push_back(res);
                }
                cout<<recent.size()<<" log entries appended\n";
            }
            else{
                cerr<<"no previous search\n";
            }
    }
    void printG(char prev){
        if(prev=='t'||prev=='m'){
            if(recent.size()!=0){
                for(size_t i=recent.front();i<recent.back();i++){
                    printLog(v[i]);
                }
            }
            return;
        }
        else if(prev=='c'||prev=='k'){
            for(auto res:recent) printLog(v[res]);
            return;
        }
            cerr<<"no previous search\n";
    }
    void printE(){
        for(size_t i=0;i<excerpt.size();i++){
            cout<<i<<"|";
            printLog(v[excerpt[i]]);
        }
    }

};

void getMode(int argc, char * argv[]) {
            opterr = false;
            int choice;
            int index = 0;
            option long_options[] = {
                {"help",no_argument,nullptr,'h'},
                { nullptr, 0, nullptr, '\0' }};
            
            while ((choice = getopt_long(argc, argv, "h",
                                         long_options, &index)) != -1) {
                switch (choice) {
                    case 'h':
                        cerr << "idk\n";
                        exit(0);
                   
                    default:
                        cerr << "Unknown command line option\n";
                        exit(1);
                }
            }
       
    }


int main(int argc, char*argv[]){
ios_base::sync_with_stdio(false);
    getMode(argc, argv);
    xcode_redirect(argc, argv);
    string arg(argv[1]);
    ifstream file(arg);
    if (!file.is_open()) {
        cout << "Error opening " << arg << endl;
        return 1;
    }
        logman g;
        g.read(file);
        g.get_command();
}

