#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <dirent.h>
#include <map>
#include <cstring>
#include <algorithm>
#include <iomanip>
#include <iterator>
#define N 1000
using namespace std;
typedef map<string,int> doc; // file name and number of times
map<string,doc> m; // word and the map
doc map_filename; // list of file names
string query;  // holds the retrieval query
vector<string> v; // has individual words of the query
struct node
{
	string filename;
	float probability;	
};
vector<node> v_prob;  // probability for every file
void insert_word(char word[],string filename){
    string s=word;
    if(m.find(s)!=m.end()){ 
    	// word already exists
        m[s][filename]+=1;
    }
    else{ 
    	// word doesnot exist
        m.insert(make_pair(s,doc()));
        m[s].insert(make_pair(filename,1));
    }
}
void scan_line(string line,string filename){
	int i=0,top=0;
    char word[N]="\0";
     // convert to lower case
    transform(line.begin(),line.end(),line.begin(),::tolower);
    string s;
    while(line[i]){
    	//ascii values a-z make a word
        if(line[i]>=97 && line[i]<=122){ 
            word[top]=line[i];
            top++;
            i++;
        }
        else{
            s=word;
            if(!s.empty()){
                // insert if not empty
                insert_word(word,filename);
            }
            top=0;
            // traverse to the next word
            while(line[i]!=0 && line[i]<97 || line[i]>122){                        
                i++;
            }
            memset(word ,'\0',sizeof word);
        }
    }
    s=word;
    if(!s.empty()){
    	// insertion of final word in a line
	 	insert_word(word,filename);
    }
    memset(word ,'\0',sizeof word);
}
void scan_file(string path,string filename){
	DIR *dptr;
	dptr=opendir((path+filename).c_str());
	if(!dptr){
	    string line;
	    ifstream myfile;
	    // insert the file name
	    map_filename.insert(make_pair(filename,1));
	    // open the file
	    myfile.open((path+filename).c_str());
	    if(myfile.is_open()){
	    	// read each line        
	        while(getline(myfile,line)){
	            scan_line(line,filename);
	        }
	        myfile.close();
	    }
    }
}
void find_words(){
    int i=0,top=0;
    char word[N]="\0";
     // convert to lower case
    transform(query.begin(),query.end(),query.begin(),::tolower);
 	string s;
    while(query[i]){
        if(query[i]>=97 && query[i]<=122){ //ascii values a-z
            word[top]=query[i];
            top++;
            i++;
        }
        else{
            s=word;
            if(!s.empty()){
                v.push_back(s);
            }
            top=0;
            while(query[i]!=0 && query[i]<97 || query[i]>122){
                    i++;
            }
            memset(word ,'\0',sizeof word);
        }
    }
    s=word;
    if(!s.empty()){
        v.push_back(s);
    }
    memset(word ,'\0',sizeof word);
}
void print_query(){
    vector<string> ::iterator it;
    for(it=v.begin();it!=v.end();it++)
        cout<<(*it)<<endl;
}
void language_model(float param){    
    int lc=0; // total number of words in the collection
    doc ::iterator it;    	
    map<string,doc>::iterator it_m;    
    for(it_m=m.begin();it_m!=m.end();it_m++){
        for(it=(*it_m).second.begin();it!=(*it_m).second.end();it++){
            lc+=(*it).second;
        }
    }
    cout<<"words in collection "<<lc<<endl;
    //  probality for every file
    for(it=map_filename.begin();it!=map_filename.end();it++){
  		// (*it).first contains file name              
        int ld=0; // total number of words in a document
        // for every word find the frequency in a document
        map<string,doc>::iterator it_m;
        for(it_m=m.begin();it_m!=m.end();it_m++){
                ld+=m[(*it_m).first][(*it).first];
        }
        cout<<"words in document "<<(*it).first<<" "<<ld<<endl;
        int tf_d=0,tf_c=0; // term frequency in a document
        vector<string> ::iterator it_v;
        float p=1.0;
        for(it_v=v.begin();it_v!=v.end();it_v++){
        	// (*it_v) is a query term
            tf_d=m[(*it_v)][(*it).first];
            cout<<(*it_v)<<" "<<tf_d<<" ";
            tf_c=0;
            // find the word in the map
            doc ::iterator it;    	
    		map<string,doc>::iterator it_m;    
            for(it=map_filename.begin();it!=map_filename.end();it++){
            	tf_c+=m[(*it_v)][(*it).first];
            }
        	cout<<tf_c<<endl;
        	p*=( (param*tf_d)/ld )+(((1.0-param)*(tf_c))/lc);
        }
        node a;
        a.filename=(*it).first;
        a.probability=p;
		v_prob.push_back(a);		        
    }
}
bool comparefn(node a,node b){
	return a.probability>b.probability;
}
void rank(){
	sort(v_prob.begin(),v_prob.end(),comparefn);
	vector<node>::const_iterator it_p;	
	
	for(it_p=v_prob.begin();it_p!=v_prob.end();it_p++){
		cout<<(*it_p).probability<<" "<<(*it_p).filename<<endl;
	}
}
int main(){
        DIR *dptr;
        struct dirent *fptr;
		string path_sample_files="dirhandle/";  // i/p file path		
        dptr=opendir(path_sample_files.c_str());
        // open directory
        if(dptr){
        	// read files  from directory
            while((fptr=readdir(dptr))!=NULL){
                string filename=fptr->d_name;
                scan_file(path_sample_files,filename);
            // scan each file and make a dictionary        
            }
        }
        cout<<"Enter query\n";
        getline(cin,query);        
        find_words();
        //print_query();
        float param;
        cout<<"Enter the parameter value:\t";
        cin>>param;
        language_model(param);
        rank();
}
