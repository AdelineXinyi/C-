//B99292359FFD910ED13A7E6C7F9705B8742F0D79
#include <vector>
#include <deque>
#include <getopt.h>
#include <iostream>
#include <string>
#include <algorithm>
#include"xcode_redirect.hpp"
using namespace std;


class superMarco {
private:
    uint32_t room;
    uint32_t room_lenth;
    bool stack=false;
    bool out_M=true;
    bool isValid=true;
    struct loc{
        uint32_t room;
        uint32_t row;
        uint32_t col;
    };
    struct tile{
        char val='.';
        char prev='.';
    };
    loc start = {0,0,0};
    loc cherry = {0,0,0};
    uint32_t tile_num = 0;
    vector< vector < vector <tile> > >map;
    const int dx[4] = {-1, 0, 1, 0};
    const int dy[4] = {0, 1, 0, -1};
   
    
public:
    //default constructor
    superMarco():room(0),room_lenth(0),isValid(true){}
    
    void getMode(int argc, char * argv[]) {
        opterr = false;
        int choice;
        int index = 0;
        int count = 0;
        option long_options[] = {
            {"stack",no_argument,nullptr,'s'},
            {"queue",no_argument,nullptr,'q'},
            {"output",required_argument,nullptr,'o'},
            {"help",no_argument,nullptr,'h'},
            { nullptr, 0, nullptr, '\0' }};
        
        while ((choice = getopt_long(argc, argv, "sqo:h",
                                     long_options, &index)) != -1) {
            switch (choice) {
                case 's':
                    stack = true;
                    count+=1;
                    break;
                case 'q':
                    count+=1;
                    break;
                case 'o': {
                    string arg{optarg};
                    if (arg != "M" && arg != "L") {
                        cerr << "Error: invalid mode\n";
                        cerr << "  I don't recognize: " << arg << "\n";
                        exit(1);
                    }
                    if (arg == "L") {
                        out_M = false;
                    }
                    break;
                }
                case 'h':
                    printHelp(argv);
                    exit(0);
                    
                default:
                    cerr << "Unknown command line option\n";
                    exit(1);
            }
        }
        if (count>1) {
            cerr << "Stack or queue can only be specified once\n";
            exit(1);
        }
        if (count==0) {
            cerr << "Stack or queue must be specified\n";
            exit(1);
        }
    }

    void read() {
        char in_mode = 'N';
        cin>>in_mode>>room>>room_lenth;
        tile t={'.','.'};
        map.resize(room,vector < vector < tile > >
                   (room_lenth, vector<tile>(room_lenth,t)));
        if(in_mode=='L'){
            readL();
        }
        else{
            readM();
        }
    }
    
    void readL(){
        char in='0';
        loc l={0,0,0};
        string cmmt="";
        uint32_t rm_index=0;
        uint32_t row_l=0;
        uint32_t column_l=0;
        while(cin>>in){
            char junk='0';
            if(in=='/'){
                getline(cin,cmmt);
            }
            if (in=='('){
                cin>>rm_index>>junk>>row_l>>junk>>column_l>>junk>>in;
                checkChar(in);
                checkNum(rm_index, row_l, column_l);
                l.row = row_l;
                l.col = column_l;
                l.room = rm_index;
                map[l.room][l.row][l.col].val = in;
                updateSC(in, l);
            }
        }
    }
    
    void readM(){
        uint32_t index = 0;
        string cmmt="";
        loc l={0,0,0};
        char in='0';
        while(cin>>in){//rm_read and index
            if(in=='/'){
               getline(cin,cmmt);
            }
            else{
                checkChar(in);
                updatePos(l, index);
                map[l.room][l.row][l.col].val = in;
                index++;
                updateSC(in, l);
            }
        }
    }
    
    void updatePos(loc& l,uint32_t index){
        l.row = (index%(room_lenth*room_lenth))/room_lenth;
        l.col = (index%(room_lenth*room_lenth))%room_lenth;
        l.room = index/(room_lenth*room_lenth);
        if(l.row >= room_lenth || l.col >= room_lenth){
            l.row=0;
            l.col=0;
        }
    }
    
    void updateSC(char in, loc& l){
        if(in=='S'){
            start=l;
        }
        if(in=='C'){
            cherry=l;
        }
    }
    
    void checkNum(uint32_t rm, uint32_t rowc, uint32_t colc){
        if(rm>=room || room>10){
            cerr<<"Invalid room number\n";
            exit(1);
        }
        if(rowc>=room_lenth){
            cerr<<"Invalid row number\n";
            exit(1);
        }
        if(colc>=room_lenth){
            cerr<<"Invalid column number\n";
            exit(1);
        }
    }
    
    void checkChar(char in){
        if(notChar(in)){
            cerr<<"Unknown map character\n";
            exit(1);
        }
    }
    
    bool notChar(char in){
        return in!='#' && in!='!' && in!='.'&&
        !isdigit(in) && in!='S' && in!='C';
    }
    
    void search(){
        std::deque<loc>game;
        game.push_back(start);
        loc l={0,0,0};
        while (!game.empty()){
            if (stack){
                l=game.back();
                game.pop_back();
            }
            else {
                l=game.front();
                game.pop_front();
            }
           
            if(isdigit(map[l.room][l.row][l.col].val)){//is pipe
                uint32_t check_n =
                static_cast<uint32_t>(map[l.room][l.row][l.col].val-'0');
                if(checkPipe(check_n, l.row, l.col)){
                    loc n={check_n,l.row,l.col};
                    game.push_back(n);//add to the deque
                    map[check_n][l.row][l.col].prev = static_cast<char>(l.room+'0');
                    tile_num++;
                }
                if(check_n < room && map[check_n][l.row][l.col].val=='C'){
                    return ;
                }
            }
            
            //not pipe
            else {
                int index=0;
                tile check;
                
                // for(auto se:seq){
                for (int i = 0; i < 4; i++) {
                    int row_check=static_cast<int>(l.row)+dx[i];
                    int col_check=static_cast<int>(l.col)+dy[i];
                    uint32_t row_key=0;
                    uint32_t col_key=0;
                    bool range = false;
                    //range
                    if(checkRange(row_check,col_check)){
                       row_key = static_cast<uint32_t>(row_check);
                       col_key = static_cast<uint32_t>(col_check);
                       check = map[l.room][row_key][col_key];
                       range = true;
                    }
                    //specific char
                    if (check.val!='#'
                        && check.val!='!'
                        && check.prev=='.' //take more time
                        && range){
                        loc n={l.room,row_key,col_key};
                        game.push_back(n);
                        setPrev(index, check);
                        map[n.room][n.row][n.col]=check;
                        tile_num++;
                    }
                    if(check.val=='C'){
                        return ;
                    }
                    index++;
                }
            }
            if(game.empty()){//
                isValid=false;
                return ;
            }
        }
    }
    
    bool checkRange(int row_check, int col_check){
        return row_check >= 0 &&
           row_check < static_cast<int>(room_lenth) &&
           col_check >= 0 &&
        col_check < static_cast<int>(room_lenth);
    }
    
   bool checkPipe(uint32_t check_n, uint32_t row, uint32_t col){
     return check_n < room && //valid room
         map[check_n][row][col].prev=='.'  //not discovered
         && map[check_n][row][col].val!='#' //not wall/minions
       && map[check_n][row][col].val!='!';
    }
    
    void setPrev(int index, tile& check){
        if(index==0){
            check.prev='n';
        }else if (index==1){
            check.prev='e';
        }else if (index==2){
            check.prev='s';
        }else if (index==3){
            check.prev='w';
        }
    }
    
    void backTraceL(){
        tile route_t = map[cherry.room][cherry.row][cherry.col];
        loc l = {cherry.room,cherry.row,cherry.col};
        while(route_t.val!='S'){
            if(route_t.prev=='w'){
                route_t=map[l.room][l.row][l.col+1];
                map[l.room][l.row][l.col+1].val = map[l.room][l.row][l.col].prev;
                l.col=l.col+1;
            }
            else if(route_t.prev=='e'){
                route_t=map[l.room][l.row][l.col-1];
                map[l.room][l.row][l.col-1].val = map[l.room][l.row][l.col].prev;
                l.col=l.col-1;
            }
            else if(route_t.prev=='s'){
                route_t=map[l.room][l.row-1][l.col];
                map[l.room][l.row-1][l.col].val = map[l.room][l.row][l.col].prev;
                l.row=l.row-1;
            }
            else if(route_t.prev=='n'){
                route_t= map[l.room][l.row+1][l.col];
                map[l.room][l.row+1][l.col].val =  map[l.room][l.row][l.col].prev;
                l.row=l.row+1;
            }
            else if(isdigit(route_t.prev)){
                uint32_t roomie = static_cast<uint32_t>(route_t.prev-'0');
                route_t = map[roomie][l.row][l.col];
                l.room = roomie;
            }
        }
        map[start.room][start.row][start.col].val=map[l.room][l.row][l.col].val;
    }
    void backTraceM(){
        tile route_t = map[cherry.room][cherry.row][cherry.col];
        loc l = {cherry.room,cherry.row,cherry.col};
        while(route_t.val!='S'){
            if(route_t.prev=='w'){
                route_t=map[l.room][l.row][l.col+1];
                map[l.room][l.row][l.col+1].val = map[l.room][l.row][l.col].prev;
                l.col=l.col+1;
            }
            else if(route_t.prev=='e'){
                route_t=map[l.room][l.row][l.col-1];
                map[l.room][l.row][l.col-1].val = map[l.room][l.row][l.col].prev;
                l.col=l.col-1;
            }
            else if(route_t.prev=='s'){
                route_t=map[l.room][l.row-1][l.col];
                map[l.room][l.row-1][l.col].val = map[l.room][l.row][l.col].prev;
                l.row=l.row-1;
            }
            else if(route_t.prev=='n'){
                route_t= map[l.room][l.row+1][l.col];
                map[l.room][l.row+1][l.col].val =  map[l.room][l.row][l.col].prev;
                l.row=l.row+1;
            }
            else {
                uint32_t roomie = static_cast<uint32_t>(route_t.prev-'0');
                map[roomie][l.row][l.col].val='p';
                route_t = map[roomie][l.row][l.col];
                l.room = roomie;
            }
        }
        map[start.room][start.row][start.col].val=map[l.room][l.row][l.col].val;
    }
    
    void print(){
        if (!isValid){
            cout<<"No solution, "<<tile_num<<" tiles discovered.\n";
            return ;
        }
            if(out_M){
                backTraceM();
                printM();
                return;
            }
                backTraceL();
                printL();
            
    }

    void printM(){
        cout<<"Start in room " << start.room << ", row " << start.row << ", column " << start.col << "\n";
        for(uint32_t i=0;i<room;i++){
            cout << "//castle room " << i << "\n";
            for(uint32_t j = 0 ; j < room_lenth; j++){
                for(uint32_t m = 0; m < room_lenth; m++){
                    cout<<map[i][j][m].val;
                }
                cout<<"\n";
            }
        }
    }
    void printL(){
        cout<<"Path taken:\n";
        tile s = map[start.room][start.row][start.col];
        loc l = start;
        vector<loc> print;
        vector<char> value;
        while(s.val!='C'){
            print.push_back(l);
            if(s.val=='n'){
                l.row-=1;
            } else if (s.val=='s'){
                l.row+=1;
            } else if (s.val=='e'){
                l.col+=1;
            } else if (s.val=='w'){
                l.col-=1;
            } else if (isdigit(s.val)){
                l.room=static_cast<uint32_t>(s.val-'0');
                s.val='p';//
            }
            value.push_back(s.val);
            s = map[l.room][l.row][l.col];
        }
                
        for(uint32_t i = 0;i<print.size();i++){
            cout<<"("
            <<print[i].room<<","
            <<print[i].row<<","
            <<print[i].col<<","
            <<value[i]<<")\n";
        }
        
    }
    void printHelp(char *argv[]) {
        cout<<argv[0]<<"\n";
    }
    
    void play(){
        read();
        search();
        print();
    }


};

int main(int argc, char *argv[]) {
  ios_base::sync_with_stdio(false);
  xcode_redirect(argc, argv);
    superMarco m;
    m.getMode(argc, argv);
    m.play();
    return 0;
}
