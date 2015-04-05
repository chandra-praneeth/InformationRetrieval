#include <iostream>
#include <iomanip>
#include <cmath>
#include <climits>
#include <fstream>
using namespace std;

#define N 10

class Point{
 	public:
 		float X,Y;
 		Point(){
 			X=Y=0;
 		}
 		~Point(){
 			//cout<<"point destructed\n";	
 		}
 		float calculateDistance(Point p){
 			float x = this->X - p.X;
 			float y = this->Y - p.Y;
 			return sqrt(x*x+y*y);
 		}
};
class Cluster
{
	public:
		int numOfPoints;
		Point childpoints[N];
		// description of points
		int pointindices[N]; 
		// which points belong to this cluster
		Cluster(){
			this->numOfPoints = 0;			
		}
		~Cluster(){

		}
		void addPoint(int index){
			if(this->numOfPoints >= N){
				cout<<"Max points limit reached\n";
				return;
			}					
			this->pointindices[this->numOfPoints++] = index;			
		}
				
		void empty(){
			this->numOfPoints = 0;
		}	
};

int inputSize;
Point points[N];
float dist[N][N];
int hashTable[N]; 
// gives the cluster number to which a point belongs
Cluster** clusters = new Cluster*[N];
// global array
int uniquenumber;
void takeInput(){

	ifstream inputFile("ip.txt", ios::in);	
	
	if(!inputFile){
		cout<<"cannot open file\n";
		return ;
	}
	
	int counter=0;
	
	while(!inputFile.eof()){
		float x=-1.0, y=-1.0;
		inputFile>>x>>y;		
		if(x == -1.0 || y == -1.0)
			continue;

		points[counter].X = x;
		points[counter].Y = y; 
		counter++;
	}
	inputSize = counter;
}

void computeDistanceMatrix(){
	
	for(int i=0;i<inputSize;i++){
		for(int j=0;j<i;j++){
			if(i!=j)
				dist[i][j] = points[i].calculateDistance(points[j]);
		}
	}
}
void printDistanceMatrix(){

	for(int i = 0;i < inputSize;i++){
		for(int j = 0;j < inputSize;j++)
			cout<<setw(10)<<dist[i][j];
		cout<<endl<<endl;
	}

}

void findMinDistance(int &indexI,int &indexJ){
	
	float minDistance = INT_MAX;

	for(int i=0;i<inputSize;i++){
		for(int j=0;j<i;j++){
			if(hashTable[i]!=hashTable[j] && dist[i][j] < minDistance){
				minDistance = dist[i][j];
				indexI = i;
				indexJ = j;
			}
		}
	}
	// indexI and indexJ form a cluster
}

void printHashTable(){

	for(int i=0;i<inputSize;i++)
		cout<<hashTable[i]<<" ";
	cout<<endl;
}

void mergeClusters(int indexI,int indexJ){

	//indexI is always greater than indexJ
	int clusterNoI = hashTable[indexI];
	int clusterNoJ = hashTable[indexJ];
	
	if(indexI == -1 || indexJ == -1){
		return;
	}
//	cout<<indexI<<" "<<indexJ<<" "<<clusterNoI<<" "<<clusterNoJ<<endl;
	
	/*  merge the two clusters
		add the childpoints from I to J then
		remove the childpoints from I
		
	*/
	
	/*cout<<"Before\nIn clusterNoJ : ";
	for(int i=0;i<clusters[clusterNoJ]->numOfPoints;i++){
		cout<<clusters[clusterNoJ]->pointindices[i]<<" ";
	}
	cout<<"\nIn clusterNoI: ";

	for(int i=0;i<clusters[clusterNoI]->numOfPoints;i++){
		cout<<clusters[clusterNoI]->pointindices[i]<<" ";
	}
	cout<<endl;
	
	*/
	for(int i=0;i<clusters[clusterNoI]->numOfPoints;i++){
	//cout<<clusters[clusterNoI]->pointindices[i]<<" ";
	clusters[clusterNoJ]->addPoint(clusters[clusterNoI]->pointindices[i]);
	}
	
	clusters[clusterNoI]->empty();	
	
	//cout<<"\nAfter \nIn clusterNoJ : ";
	cout<<"(";
	for(int i=0;i<clusters[clusterNoJ]->numOfPoints;i++){
		cout<<clusters[clusterNoJ]->pointindices[i]<<",";
	}
	cout<<")";
	/*cout<<"\nIn clusterNoI: ";

	for(int i=0;i<clusters[clusterNoI]->numOfPoints;i++){
		cout<<clusters[clusterNoI]->pointindices[i]<<" ";
	}*/
	cout<<endl;

	for(int i=0;i<clusters[clusterNoJ]->numOfPoints;i++){
		
		hashTable[clusters[clusterNoJ]->pointindices[i]] = clusterNoJ;		
	}

//	printHashTable();cout<<endl;
}

int main(){

	takeInput();		
	// clusters[i] is a pointer to the object cluster
	for(int i=0;i<inputSize;i++){
		clusters[i] = new Cluster();
		clusters[i]->addPoint(i);
		hashTable[i] = i;
	}
	computeDistanceMatrix();
	
	for(int i=0;i<12;i++){
		int indexI=-1,indexJ=-1;
		findMinDistance(indexI,indexJ);		
		mergeClusters(indexI,indexJ);	
	}	
}