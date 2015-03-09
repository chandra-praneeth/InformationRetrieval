#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <iomanip>
using namespace std;
typedef map<string,int> relevance; // docid and relevance_judgement
map<int,relevance> m; // qrels file qid docid and relevance

void insert(int qid,string docid,int rel){
	if(m.find(qid)==m.end()){ // qid doesnot exist
		m.insert(make_pair(qid,relevance()));
		m[qid].insert(make_pair(docid,rel));
	}
	else{ // qid exists
		m[qid].insert(make_pair(docid,rel));		
	}
}
void scan_file(string path){
	ifstream qrel_file;
	qrel_file.open(path.c_str());
	if(qrel_file.is_open()){
		string line;
		while(getline(qrel_file,line)){
			int qid,rel; 
			string docid;
			stringstream ss(line);
			ss>>qid>>docid>>rel;
			insert(qid,docid,rel);
		}
	}
}
void search(int qid,string docid){
    // qrels contain qid docid relevance_judgement
	  //line by line search
    /*string qrel_path="qrels.txt";
    ifstream qrel_file;
    qrel_file.open(qrel_path.c_str());
    if(qrel_file.is_open()){
        string line;
        while(getline(qrel_file,line)){
            int one,three;
            string two;
            stringstream ss(line);
            ss>>one>>two>>three; //
            if(qid==one && docid==two){
                    cout<<one<<" "<<two<<" "<<th<<endl;
                    break;
            }
        }
    }
    */
    // search via map
    cout<<setw(8)<<m[qid][docid]<<endl;
}
void scan_line(string line){
        int qid,rank;
        string docid;
        stringstream ss(line);
        ss>>qid>>docid>>rank; // qid docid rank
        cout<<setw(7)<<qid;
        cout<<setw(7)<<docid;
        cout<<setw(9)<<rank;
        search(qid,docid); // search in qrels file
}
int main(){
	string inl_path="inl.txt",qrels_path="qrels.txt";
	// inl contains qid docid rank
	scan_file(qrels_path);
	ifstream inl_file;	
	inl_file.open(inl_path.c_str());
	if(inl_file.is_open()){
        string line;
        cout<<setw(8)<<"qid";
        cout<<setw(8)<<"docid";
        cout<<setw(8)<<"rank";
        cout<<setw(12)<<"relevance\n";
        while(getline(inl_file,line)){            
			scan_line(line);
		}
	}
}