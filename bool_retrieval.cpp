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
#define N 1000
using namespace std;

typedef map<string,int> doc; // file name and number of times 
map<string,doc> m; // word and the map
doc map_swords;  // contains stopwords
doc map_filename; // list of file names
string query;  // holds the retrieval query
vector<string> v; // has individual words of the query

void insert_word(char word[],string filename){
	string s=word;
	if(filename!="stop_words.txt"){
//		cout<<s<<"HH"<<endl;
		if(m.find(s)!=m.end()){ // word already exists		
			m[s][filename]+=1;
		}
		else{ // word doesnot exist			
			m.insert(make_pair(s,doc()));
			m[s].insert(make_pair(filename,1));
		}
	}
	else{ // stopwords		
		map_swords.insert(make_pair(s,1));
	}		
}

void print(){

	map<string,int>::iterator it_doc;
	map<string,doc>::iterator it_m;

	for(it_m=m.begin();it_m!=m.end();it_m++){
		cout<<setw(12)<<(*it_m).first<<"|";
		for(it_doc=(*it_m).second.begin();it_doc!=(*it_m).second.end();it_doc++){
			cout<<setw(12)<<(*it_doc).first<<"--"<<(*it_doc).second<<"|";
		}
		cout<<endl;
	}
}

void print_stopwords(){
	map<string,int>::iterator it_swords;
	cout<<"stopwords are here\n";
	for(it_swords=map_swords.begin();it_swords!=map_swords.end();it_swords++){
		cout<<(*it_swords).first<<endl;
	}
}
bool not_stop_word(string word){
	if(map_swords.find(word)==map_swords.end()) //word is not a stopword
		return true;
	else
		return false;
}
void scan_line(string line,string filename){

	int i=0,top=0;
	char word[N]="\0";
	transform(line.begin(),line.end(),line.begin(),::tolower); // conversion to lower case
	string s;
	while(line[i]){		
		if(line[i]>=97 && line[i]<=122){ //ascii values a-z
			word[top]=line[i];
			top++;
			i++;
		}
		else{
//			cout<<"h\n";
			s=word;
			if( ( !s.empty() )&& not_stop_word(s) ){
				// insert if not a stop word
				insert_word(word,filename);
			}
			top=0;
			while(line[i]!=0 && line[i]<97 || line[i]>122){				
				i++;
//				cout<<"exec\n";
			}
				memset(word ,'\0',sizeof word);			
		}				
	}
	s=word;
	if( ( !s.empty() ) && not_stop_word(word) ){		
//		cout<<"after\n";
		insert_word(word,filename);	
	}
	memset(word ,'\0',sizeof word);			
}
void read_filename(string filename){
	map_filename.insert(make_pair(filename,1));
}
void scan_file(string path,string filename){

	DIR *dptr;
	dptr=opendir((path+filename).c_str()); // to check for a file 
                                  // not a directory
	if(!dptr){
		cout<<filename<<endl;
		string line;
		ifstream myfile;
		if(filename!="stop_words.txt")
			read_filename(filename);		
		myfile.open((path+filename).c_str());

		if(myfile.is_open()){
			//cout<<"opened file\n";
			while(getline(myfile,line)){				
//				cout<<line<<endl;
				scan_line(line,filename);
			}
			myfile.close();	
		}
	}	
}
bool search(string word){
	if(m.find(word)!=m.end()){ // word exists
		map<string,int>::iterator it_doc;
		map<string,doc>::iterator it_m=m.find(word);
		cout<<"The word exists, and occurs in \n";
		for(it_doc=(*it_m).second.begin();it_doc!=(*it_m).second.end();it_doc++){
			cout<<(*it_doc).first<<" "<<(*it_doc).second<<" times\n";
		}
		return true;
	}
	else{
		cout<<"Sorry but it doesnot exist\n";
		return false;
	}
}
void build_td_matrix(){
	
	map<string,int>::iterator it_filename;
	cout<<"building term-document matrix\n";
	cout<<setw(15)<<"Term"<<"|";
	for(it_filename=map_filename.begin();it_filename!=map_filename.end();it_filename++){
		cout<<setw(15)<<(*it_filename).first<<"|";
	}
	cout<<endl;
	vector<string>::iterator it;
	for(it=v.begin();it!=v.end();it++){//for every word in query
		cout<<setw(15)<<(*it)<<"|";

		map<string,doc>::iterator it_m;
		map<string,int>::iterator it_doc;
		it_m=m.find((*it)); //find the word in the map
		// in every file
		it_filename=map_filename.begin();
		it_doc=(*it_m).second.begin();
		
		while(it_filename!=map_filename.end() && it_doc!=(*it_m).second.end() ){
			//cout<<"hell\n";
			//cout<<(*it_doc).first<<" "<<(*it_filename).first;
			if((*it_doc).first==(*it_filename).first){
				cout<<setw(15)<<"1"<<"|";
				it_filename++;				
				it_doc++;
			}
			else{
				cout<<setw(15)<<"0"<<"|";
				it_filename++;
			}
		}
		if(it_filename!=map_filename.end()){
		//	cout<<"flag\n";	
			cout<<setw(15)<<"0"<<"|";
				it_filename++;				
		}
		cout<<endl;	
	}
}
void boolean_retrieval(){
	
	string token;
	string delimiter="and";
	size_t pos=0;	
	cout<<"in retrieval\n";

	while((pos=query.find(delimiter))!=string::npos){
		token=query.substr(0,pos);
		cout<<token<<endl;
		v.push_back(token);   //tokens are stored in vector
		query.erase(0,pos+delimiter.length());
	} 

	if(!token.empty()){
		// to add the last token
		v.push_back(query);
		cout<<query<<endl;
	}  	
	
		
	// retrieval starts here
//find the document that contains all the words of the query
	// assume the words are all distinct
	// build term-document matrix
	
		build_td_matrix();
}
void trim_spaces(){
	int i=0;
	while(query[i]){
		if(query[i]<97 ||query[i]>122){  
			query.erase(query.begin()+i);
		}
		i++;
	}
}
int main(){
	DIR *dptr;
	struct dirent *fptr;
	string path_sample_files="dirhandle/";  // path for the input files
	// two directories exist dirhandle and txt_files
	scan_file("","stop_words.txt");
	dptr=opendir(path_sample_files.c_str());//converting string to const char*
	
	if(dptr){
		while((fptr=readdir(dptr))!=NULL){			
			string filename=fptr->d_name;			
			scan_file(path_sample_files,filename);
		}
	}
//	print(); 
//	print_stopwords();
	char word[N];
	
	cout<<"Boolean retrieval mode\n";	
	do{
		cout<<"Enter the query\n";
		//cin>>query;
		getline(cin,query);
		cout<<query<<endl;
		trim_spaces(); // remove spaces and other unwanted characters
		cout<<query<<endl;		
		boolean_retrieval();
	}while(0);
/*	do{  
		memset(word,'\0',sizeof word);		
		cout<<"Enter the word to be searched for\n";
		cin>>word;
	}while(search(word));*/	
}
