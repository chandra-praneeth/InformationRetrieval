#include <iostream>
#include <fstream>
#include <climits>
#define TEMP 1000
using namespace std;

struct Point{    
    int x, y;
};

bool checkBounds(int x, int y, int xMin, int yMin, int xMax, int yMax){
    return xMin <= x && xMax >= x && yMin <= y && yMax >= y;
}

double CalcPosteriorProb(int x,int y,double priorP,Point* points, int numOfPoints, int xMin, int yMin,
                                                 int xMax, int yMax){
    int xInter = (xMin + xMax) / 2;

    int yInter = (yMin + yMax) / 2;

    double numOfC1 = 0, numOfC2 = 0, numOfC3 = 0, numOfC4 = 0;    

    for(int i = 0; i < numOfPoints; i++)
    {
            int x = points[i].x, y = points[i].y;

            if(checkBounds(x, y, xMin, yInter, xInter, yMax))
                    numOfC1++;
            else  if(checkBounds(x, y, xInter, yInter, xMax, yMax))
                    numOfC2++;
            else  if(checkBounds(x, y, xInter, yMin, xMax, yInter))
                    numOfC3++;
            else
                    numOfC4++;
    }    
    double likelihoodR1 = numOfC1 / numOfPoints;
    double likelihoodR2 = numOfC2 / numOfPoints;
    double likelihoodR3 = numOfC3 / numOfPoints;
    double likelihoodR4 = numOfC4 / numOfPoints;

    if(checkBounds(x, y, xMin, yInter, xInter, yMax))
    	return likelihoodR1*priorP; 	
    else  if(checkBounds(x, y, xInter, yInter, xMax, yMax))
        return likelihoodR2*priorP;
    else  if(checkBounds(x, y, xInter, yMin, xMax, yInter))
        return likelihoodR3*priorP;
    else	
    	return likelihoodR4*priorP;
}

int main(){
    int totalNoOfPoints = 0;

    ifstream inputFile("data_nb4.txt", ios::in);

    if(!inputFile){
            cout<<"Cannot open the file";
            return 1;
    }

    Point points1[TEMP], points2[TEMP];

    int numOfClass1 = 0, numOfClass2 = 0;

    int class1XMin = INT_MAX, class1YMin = INT_MAX;
    int class1XMax = INT_MIN, class1YMax = INT_MIN; 
    int class2XMin = INT_MAX, class2YMin = INT_MAX;
    int class2XMax = INT_MIN, class2YMax = INT_MIN;
    int x = -1, y = -1, c = -1;
    while(!inputFile.eof())
    {        
    	c=-1;
        inputFile>>x>>y>>c;
        
        if(c == -1){                
            continue;
        }
//        cout<<x<<" "<<y<<" "<<c<<endl;        
        if(c == 1)
        {
            points1[numOfClass1].x = x;
            points1[numOfClass1].y = y;

            numOfClass1++;

            if(class1XMin > x)
                    class1XMin = x;
            if(class1XMax < x)
                    class1XMax = x;

            if(class1YMin > y)
                    class1YMin = y;
            if(class1YMax < y)
                     class1XMax = y;
        }
        else
        {
            points2[numOfClass2].x = x;
            points2[numOfClass2].y = y;

            numOfClass2++;

            if(class2XMin > x)
                    class2XMin = x;
            if(class2XMax < x)
                    class2XMax = x;

            if(class2YMin > y)
                    class2YMin = y;
            if(class2YMax < y)
                    class2YMax = y;
        }
    }
    inputFile.close();
    
    totalNoOfPoints = numOfClass1 + numOfClass2;

    double priorP1 = (double)numOfClass1 / totalNoOfPoints;

    double priorP2 = (double)numOfClass2 / totalNoOfPoints;

    cout<<x<<" "<<y<<endl;

    double posteriorP1= CalcPosteriorProb(x,y,priorP1,points1,numOfClass1,class1XMin,class1YMin,class1XMax,class1YMax);
	double posteriorP2= CalcPosteriorProb(x,y,priorP2,points2,numOfClass2,class2XMin,class2YMin,class2XMax,class2YMax);    
	
	cout<<posteriorP1<<" "<<posteriorP2<<endl;
	
	cout<<"Class ";
	if(posteriorP1>posteriorP2)
		cout<<"1\n";
	else
		cout<<"2\n";
    return 0;
}
