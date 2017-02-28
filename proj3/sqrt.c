#include <stdio.h>
#include <math.h>

float sqrt7(float x)
 {
   unsigned int i = *(unsigned int*) &x; 
   // adjust bias
   i  += 127 << 23;
   // approximation of square root
   i >>= 1; 
   return *(float*) &i;
 }   

int main(){
	float i=0;
	for(i = 0; i<=1000;i++){
		printf("%fthe result is %f,%f\n",i,sqrt7(i),sqrt(i));
	}
	return 0;
}



