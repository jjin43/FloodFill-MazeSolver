/*
Name: Justin Jin
Date: 6/10/2002
Description: Path finder
Usage: ...
*/

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>

using namespace std;

void floodFill(int** array, int width, int height, int x, int y);
void PrintEnvironment(int** array, int width, int height);
int findMin(int** arr, int width, int height, int x, int y);
void findPath(int** map, char** charMap,int startX,int startY,int goalX,int goalY,int width,int height);
void PrintCharEnvironment(char** array, int width, int height);

int main(int argc, char** argv)
{
  int width = 0;
  int height = 0;
  
  int obstacles = 0;
  
  cout << "Welcome to CSE WaveFront Pather" << endl;
  cout << "Enter grid informations." << endl;

  do
    {
      cout << "Enter the width (at least 10):";
      cin >> width;
      if(width < 10){
	cout << "Invalid Input, width must be at least 10. Try Again." << endl;
      }

    } while(width < 10);

  do
    {
      cout << "Enter the height (at least 10):";
      cin >> height;
      if(width < 10){
	cout << "Invalid Input, height must be at least 10. Try Again." << endl;
      }

    } while(height < 10);

  char choice;
  do
    {
      cout << "Enter the percent of impassible terrain (0 - 100):";
      cin >> obstacles;
      choice = 'y';
      if(obstacles > 100 || obstacles < 0){
	cout << "Invalid Input, percentage must be between 0 - 100. Try Again." << endl;
      }
      else if(obstacles > 35) {
	cout << "Having a value greater than 35% might create poor results, do you wish to continue? (y/n):";
	cin >> choice;
      }
      

    } while(obstacles > 100 || obstacles < 0 || choice == 'n');

  int numObs = (width * height) * (float)(obstacles/100.0);
  
  // Create Array
  int** map = new int*[height];
  for(int i=0; i<width; ++i){
    map[i] = new int[width];
  }
  
  for(int i=0; i<height; ++i){
    for(int j=0; j<width; ++j){
      map[j][i]=0;
    }
  }
  
  // Add obstacles (-1)
  for(int i=0; i<numObs; ++i){
    int randX = rand() % (width);
    int randY = rand() % (height);
    
    if(map[randX][randY] == 0){
      map[randX][randY] = -1;
    }
    else{
      --i;
    }  
  }

  PrintEnvironment(map, width, height);

  
  // Set Goal Pos
  int goalX = 0;
  int goalY = 0;
  bool goalDone = false;
  do
    {
      printf("Map Size is: [%d][%d]  (Starting at index 0)\n",(width-1),(height-1));
      cout << "Please enter the Goal Position X:" ;
      cin >> goalX;
      cout << "Please enter the Goal Position Y:";
      cin >> goalY;
      if((goalX >= 0 && goalX < width) && (goalY >= 0 && goalY < height)){
	if(map[goalX][goalY] != 0){
	  cout << "Sorry, that position is inside an obstacle, select new position" << endl;
	}
	else{
	  goalDone = true;
	  
	}
      }
      else {
	cout << "Invalid Position, please enter a coordinate on the map" << endl;
      }
    } while(!goalDone);
  
  floodFill(map, width, height, goalX, goalY);
  
  //END of Floodfill Map

  //Start of Transfering Map to charMap
  
  // creating charArray
  char** charMap = new char*[height];
  for(int i=0; i<width; ++i){
    charMap[i] = new char[width];
  }
  
  for(int i=0; i<height; ++i){
    for(int j=0; j<width; ++j){
      charMap[j][i]= ' ';
    }
  }
  

  //getting starting pos
  int startX = 0;
  int startY = 0;
  bool startDone = false;
  do
    {
      printf("Map Size is: [%d][%d]  (Starting at index 0)\n",(width-1),(height-1));
      cout << "Please enter the Start Position X:" ;
      cin >> startX;
      cout << "Please enter the Start Position Y:";
      cin >> startY;
      if((startX >= 0 && startX < width) && (startY >= 0 && startY < height)){
	if(map[startX][startY] == -1){
	  cout << "Sorry, that position is inside an obstacle, select new position" << endl;
	}
	else if(map[startX][startY] == 1){
	  cout << "Sorry, that position is already the Goal, select new position" << endl;
	}
	else{
	  startDone = true;
	}
      }
      else {
	cout << "Invalid Position, please enter a coordinate on the map" << endl;
      }
    } while(!startDone);
  

  //print to console
  findPath(map,charMap,startX,startY,goalX,goalY,width,height);
  PrintEnvironment(map, width, height);
  printf("\n\n");
  PrintCharEnvironment(charMap,width,height);
  
  //print to File
  ofstream outfile;
  outfile.open("wave.txt");
  for(int i=0; i<height; ++i){
    for(int j=0; j<width; ++j){
      outfile << map[j][i] << "\t"; 
    }
    outfile << endl;
  }
  
  outfile<< "\n" << endl;

  for(int i=0; i<height; ++i){
    for(int j=0; j<width; ++j){
      outfile << charMap[j][i] << " "; 
    }
    outfile << endl;
  }
  outfile.close();
  
  // Clean Up (end of program)
  for(int i=0; i<height; ++i){
    delete [] map[i];
  }
  delete [] map;

  for(int i=0; i<height; ++i){
    delete [] charMap[i];
  }
  delete [] charMap;

  return 0;
}

// Each call first fills surrounding spaces, then recursive call

void floodFill(int** arr, int width, int height, int x, int y){
  

  
  if(x >= width || x < 0)
    return;
  if(y >= height || y < 0)
    return;
  if(arr [x][y] != 0)
    return;
  
  arr[x][y] = 1;
  
  // first scan - in to out
  //right of x
  for(int i=x;i<width;i++){
    for(int j=y; j<height;j++){
      if(arr[i][j]==0){
	arr[i][j] = findMin(arr,width,height,i,j) +1;
      }
    }
    for(int j=y-1; j>=0;j--)
      if(arr[i][j]==0){
	arr[i][j] = findMin(arr,width,height,i,j) +1;
      }
  }

  //left of x
  for(int i=x-1;i>=0;i--){
    for(int j=y; j<height;j++)
      if(arr[i][j]==0)
	arr[i][j] = findMin(arr,width,height,i,j) +1;
    for(int j=y-1; j>=0;j--)
      if(arr[i][j]==0)
	arr[i][j] = findMin(arr,width,height,i,j) +1;
  }

  // second scan - out to in
  for(int i=width-1;i>=x;i--){
    for(int j=y; j<height;j++){
      if(arr[i][j]==0){
	arr[i][j] = findMin(arr,width,height,i,j) +1;
      }
    }
    for(int j=y-1; j>=0;j--)
      if(arr[i][j]==0){
	arr[i][j] = findMin(arr,width,height,i,j) +1;
      }
  }

  //left of x
  for(int i=0;i<x;i++){
    for(int j=y; j<height;j++)
      if(arr[i][j]==0)
	arr[i][j] = findMin(arr,width,height,i,j) +1;
    for(int j=y-1; j>=0;j--)
      if(arr[i][j]==0)
	arr[i][j] = findMin(arr,width,height,i,j) +1;
  }
  
  
}

//floodFill supplement
int findMin(int** arr, int width, int height, int x, int y){
  int min = 1000;
  
  if(y+1<height){
    if(arr[x][y+1] < min && arr[x][y+1]>0)
      min = arr[x][y+1];
  }
  
  if(y-1>=0){
    if(arr[x][y-1] < min && arr[x][y-1]>0)
      min = arr[x][y-1];
  }
  
  if(x-1 >= 0){
    if(arr[x-1][y]<min && arr[x-1][y]>0)
      min = arr[x-1][y];
    if(y-1>=0){
      if(arr[x-1][y-1] < min && arr[x-1][y-1]>0)
	min = arr[x-1][y-1];
    }
      
    if(y+1<height){
      if(arr[x-1][y+1] < min && arr[x-1][y+1]>0)
	min = arr[x-1][y+1];
    }
  }
  if(x+1 < width){
    if(arr[x+1][y]<min && arr[x+1][y]>0)
      min = arr[x+1][y];
      
    if(y-1>=0){
      if(arr[x+1][y-1] < min && arr[x+1][y-1]>0)
	min = arr[x+1][y-1];
    }
    
    if(y+1<height){
      if(arr[x+1][y+1] < min && arr[x+1][y+1]>0)
	min = arr[x+1][y+1];
    }
  }
  if(min==1000)
    return -1;
  else
    return min;
}

void findPath(int** arr, char** charMap,int startX,int startY,int goalX,int goalY,int width, int height){
  
  int x = startX;
  int y = startY;
  int min =  arr[startX][startY];
  int minX = 0;
  int minY = 0;
  
  //find obstacles
  for(int i=0; i<height; ++i){
    for(int j=0; j<width; ++j){
      if(arr[j][i]==-1){
	charMap[j][i] = '#';
      }
    }
  }
  
  //find Path
  while(min!=1){
    if(y+1<height){
      if(arr[x][y+1] < min && arr[x][y+1]>0){
	minX = x;
	minY = y+1;
	min = arr[minX][minY];
      }
    }
  
    if(y-1>=0){
      if(arr[x][y-1] < min && arr[x][y-1]>0){
	minX = x;
	minY = y-1;
	min = arr[minX][minY];
      }
    }
  
    if(x-1 >= 0){
      if(arr[x-1][y]<min && arr[x-1][y]>0){
	minX = x-1;
	minY = y;
	min = arr[minX][minY];
      }
      if(y-1>=0){
	if(arr[x-1][y-1] < min && arr[x-1][y-1]>0){
	  minX = x-1;
	  minY = y-1;
	  min = arr[minX][minY];
	}
      }
      
      if(y+1<height){
	if(arr[x-1][y+1] < min && arr[x-1][y+1]>0){
	  minX = x-1;
	  minY = y+1;
	  min = arr[minX][minY];
	}
      }
    }
    if(x+1 < width){
      if(arr[x+1][y]<min && arr[x+1][y]>0){
	minX = x+1;
	minY = y;
	min = arr[minX][minY];
      }
      if(y-1>=0){
	if(arr[x+1][y-1] < min && arr[x+1][y-1]>0){
	  minX = x+1;
	  minY = y-1;
	  min = arr[minX][minY];
	}
      }
    
      if(y+1<height){
	if(arr[x+1][y+1] < min && arr[x+1][y+1]>0){
	  minX = x+1;
	  minY = y+1;
	  min = arr[minX][minY];
	}
      }
    }
    
    if(minX == x && minY == y){
      cout << "No Path can be found" << endl;
      break;
    }
    charMap[minX][minY] = '*';
    x=minX;
    y=minY;
    min = arr[minX][minY];
  }

  charMap[startX][startY] = '@';
  charMap[goalX][goalY] = '$';
}

void PrintEnvironment(int** array, int width, int height){
  for(int i=0; i<height; ++i){
    for(int j=0; j<width; ++j){
      printf("%d\t",array[j][i]);
    }
    printf("\n");
  }
}

void PrintCharEnvironment(char** array, int width, int height){
  for(int i=0; i<height; ++i){
    for(int j=0; j<width; ++j){
      printf("%c ",array[j][i]);
    }
    printf("\n");
  }
}

