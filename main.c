#include <math.h>
#include <stdio.h>
#define BUFFER_SIZE 9

float sampleBuffer[BUFFER_SIZE];
float coeffBuffer[BUFFER_SIZE] = {0.001528, 
0.015556, 
0.046380, 
0.081717, 
0.097656, 
0.081717, 
0.046380, 
0.015556, 0.001528};


void fillBuffer(float v){
	
	for(int i = BUFFER_SIZE-1; i>0; i--){
		sampleBuffer[i] = sampleBuffer[i-1];
	}

	sampleBuffer[0] = v;

}


float fir(){
	float res = 0;
	for(int i = 0; i<BUFFER_SIZE; i++){
		res = res + sampleBuffer[i]*2.5772*coeffBuffer[i];
	}
	return res;
}

float generateSample(float x){
	return sin(x)+sin(15*x)+sin(10*x);
}

int main(){
	for(int i = 0; i<100; i++){

		fillBuffer(generateSample(i/4.0f));
		printf("%i\t%.3f\t%.3f\n", i, generateSample(i/4.0f), fir());
	}
	
	return 0;
}

