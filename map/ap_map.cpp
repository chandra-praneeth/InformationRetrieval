#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <map>
using namespace std;
map<int,int> m; // maps document id to number of relevant docs
int number_docs,relevant,non_relevant, no_q=0 ;
float MAP=0, ap=0 ;
float calculate(int four,int doc_id){

        number_docs++;
        if(!four)
                non_relevant++;
        else
                relevant++;
        float precision=(float)relevant/number_docs;
        float recall=(float) relevant/m[doc_id];
        float f_score=0;
        if(relevant)
                f_score=(2*precision*recall)/(precision+recall);
        cout<<setw(13)<<precision<<setw(13)<<recall<<setw(13)<<f_score<<setw(13)<<four<<endl;
return precision;
}
void scan_line(string line){
        string three; // col number
        int curr_one,two,four; // col numbers
        int i=0;
        float precision;
        static int prev_one=-100;
        stringstream ss(line);
        ss>>curr_one>>two>>three>>four;
        if(prev_one==-100 || prev_one==curr_one){
                if(prev_one==-100){
                        no_q++;
                        cout<<"Query ID:"<<curr_one<<endl;
                }
                precision = calculate(four,curr_one);
                if(four){
                	ap+=precision;
    //            	cout<<ap<<endl;
                }                        
                prev_one=curr_one;
        }
        else{
                cout<<"Average precision :"<<ap/m[prev_one]<<endl;
                MAP+=ap/m[prev_one];
                ap=0;
                cout<<"Query ID:"<<curr_one<<endl;
                no_q++;
                number_docs=relevant=non_relevant=0;
                precision= calculate(four,curr_one);
                if(four){
                    ap+=precision;
        //            cout<<ap<<endl;
                }
                prev_one=curr_one;
        }
}
void find_rel(string line){
        string three; // col number
        int curr_one,two,four; // col numbers
        int i=0;
        static int previous_one=-100;
        stringstream ss(line);
        ss>>curr_one>>two>>three>>four;
        if(previous_one==-100 || previous_one==curr_one){
            if(previous_one==-100){                
                // not exists
                m.insert(make_pair(curr_one,0));
                //cout<<"Query ID:"<<curr_one<<endl;
            }
            if(four)
                m[curr_one]++;    
            previous_one=curr_one;
        }
        else{
        	m.insert(make_pair(curr_one,0));
            //cout<<"Query ID:"<<curr_one<<endl;
            if(four)
            	m[curr_one]++;    	
            previous_one=curr_one;
        }
}
int main(){
    string path="prf.txt";
    ifstream myfile;
    myfile.open(path.c_str());
    if(myfile.is_open()){ // finds rel docs per qid
    	string line;
    	while(getline(myfile,line)){
                find_rel(line);
        }
    }
    map<int,int>::iterator it;
    for(it=m.begin();it!=m.end();it++){
    	cout<<(*it).first<<" "<<(*it).second<<endl;
    }
    myfile.close();
    myfile.open(path.c_str());
    if(myfile.is_open()){
        string line;
        //cout<<"Total number of relevant docs per query is "<<TOTAL<<endl;
        cout<<setw(13)<<"precision"<<setw(13)<<"recall "<<setw(13)<<"f_score\n";
        while(getline(myfile,line)){
                scan_line(line);
        }
        it=m.end();
        it--;
        cout<<"Average precision :"<<ap/(*it).second<<endl;
        MAP+=ap/(*it).second;
        cout<<no_q<<" "<<MAP<<endl;
        cout<<"Mean average precision is :"<<MAP/no_q<<endl;
    }
}