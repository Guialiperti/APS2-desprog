#include <stdio.h>
#include <math.h>


int main(){

double s[] = [1,2,3,4,5,6,7,8];
int size = 8;
double complex sp[size]; 
  double complex si[size];
  double complex tp[size];
  double complex ti[size]; 

  //divisão entre pares e impares
  int i_i = 0;
  int i_p = 0;
  for(int z = 0;

for(int z = 0; z < n - 1; z++){
  if(z % 2 == 0){
    sp[i_p] = s[z];
    i_p++;
  }
  else
  {
    si[i_i] = s[z];
    i_i++;
  }
  }
printf(s);
printf("\n");
printf(sp);
printf("\n");
printf(si);
printf("\n");

return 0;
}