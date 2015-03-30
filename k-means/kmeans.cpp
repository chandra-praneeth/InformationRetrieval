#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <climits>
using namespace std;

#define MAXPOINTS 100

class Point {
    public:
        int X, Y;

    Point(){
      X = Y = 0;
    }

    double CalculateDistance(Point p){
        int x = this->X - p.X;
        int y = this->Y - p.Y;

        return sqrt(x*x + y*y);
    }
};

class Cluster{
	public:
        Point ChildPoints[MAXPOINTS];
        Point Centroid;
        int NumberOfPoints;

        Cluster(){
            this->NumberOfPoints = 0;
            this->Centroid.X = 0;
            this->Centroid.Y = 0;
        }

        void AddPoint(Point p){
            if(this->NumberOfPoints >= MAXPOINTS){

                    cout<<"MAXPOINTS limit reached";
                    exit(2);
            }

            this->ChildPoints[this->NumberOfPoints].X = p.X;
            this->ChildPoints[this->NumberOfPoints].Y = p.Y;

            this->NumberOfPoints++;

    	}

        void RemovePoint(Point p){
            int index = -1;

            for(int i = 0; i < this->NumberOfPoints; i++){
                if(this->ChildPoints[i].X == p.X
                   && this->ChildPoints[i].Y == p.Y){

                    this->NumberOfPoints--;
                    for(int j = i; j < NumberOfPoints; j++){
            this->ChildPoints[j].X = this->ChildPoints[j + 1].X;
            this->ChildPoints[j].Y = this->ChildPoints[j + 1].Y;
                    }

                }
            }
        }

        void RecalculateCentroid(){
            int x = 0, y = 0;

            for(int i = 0; i < this->NumberOfPoints; i++){
                    x += this->ChildPoints[i].X;
                    y += this->ChildPoints[i].Y;
            }

            x /= this->NumberOfPoints;
            y /= this->NumberOfPoints;

            this->Centroid.X = x;
            this->Centroid.Y = y;
        }
};

bool equal(Point a,Point b){
	
	if(a.X == b.X && a.Y == b.Y)
		return true;
	return false;

}

int main()
{
    ifstream inputFile("k-means.txt", ios::in);

    if(!inputFile)
    {
            cout<<"Cannot open the file";
            return 1;
    }


    // 3 clusters 3-means algo
    Cluster** clusters = new Cluster*[3];

    clusters[0] = new Cluster();

    clusters[1] = new Cluster();

    clusters[2] = new Cluster();

    Point points[MAXPOINTS];

    int counter = 0;  // number of points in total


    while(!inputFile.eof()){
            int x = -1, y = -1;

            inputFile>>x>>y;

            if(x == -1 || y == -1)
                    continue;

            points[counter].X = x;
            points[counter].Y = y;

            counter++;
    }

    int hashTable[MAXPOINTS];

    for(int i = 0; i < counter; i++){
        clusters[i % 3]->AddPoint(points[i]);

        hashTable[i] = i % 3;
    }
    
    // Initial calculation of centroid
    for(int i = 0; i < 3; i++){
        clusters[i]->RecalculateCentroid();
    }



    ///////////////////////////////////////
    Point c1Centroid,c2Centroid,c3Centroid;
   
    do{ 
    	
    	c1Centroid=clusters[0]->Centroid;
    	c2Centroid=clusters[1]->Centroid;
    	c3Centroid=clusters[2]->Centroid;

 	   	double tempDist[3];
		
		for(int i = 0; i < counter; i++){
            
            double minValue = INT_MAX;

            int minIndex = -1; 

            /* 
            1.  minIndex stores the cluster number 
            	to which a point should belong to
        
            2.	find the cluster which is closer to
               	the point 
            */
            for(int j = 0; j < 3; j++){

    tempDist[j] = points[i].CalculateDistance(clusters[j]->Centroid);

                if(tempDist[j] < minValue){
                    minValue = tempDist[j];
                    minIndex = j;
                }
            }

            if(minIndex != hashTable[i]){
                clusters[hashTable[i]]->RemovePoint(points[i]);

                clusters[minIndex]->AddPoint(points[i]);

                hashTable[i] = minIndex;
            }

    	}

		cout<<"\n-----------------------------\n";
		/* 
			centroid should be calculated after all the
			points have been assigned to 
			respective clusters
    	*/
		for(int i = 0; i < 3; i++){
	        
	        clusters[i]->RecalculateCentroid();

		 	cout<<"Cluster "<<i + 1<<": "<<clusters[i]->NumberOfPoints;

		 	cout<<" "<<clusters[i]->Centroid.X<<" "<<clusters[i]->Centroid.Y<<endl;

		}

		cout<<"\n-----------------------------\n";
	} while(!equal(c1Centroid,clusters[0]->Centroid) || !equal(c2Centroid,clusters[1]->Centroid) ||  !equal(c3Centroid,clusters[2]->Centroid) );
    return 0;
}