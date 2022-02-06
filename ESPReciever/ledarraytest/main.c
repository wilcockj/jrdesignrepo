#include <stdio.h>
#include <stdlib.h>


int main(int argc, char **argv){
  int array[5][5][7][3];
  int address = 0;
  time_t t;
  srand((unsigned) time(&t));
  for(int z = 0;z < 7;z++){
    for(int y = 0;y<5;y++){
      for(int x = 0;x<5;x++){
        for(int rgb = 0; rgb<3; rgb++){
          address = x + y * 5 + z * 25; 
          array[x][y][z][rgb] =   rand() % 256;
        }
        printf("led #%d array[%d][%d][%d] = (%d,%d,%d)\n",address,x,y,z,array[x][y][z][0],array[x][y][z][1],array[x][y][z][2]);
      }
    }
  }
}

