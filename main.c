#include <math.h>
#include <stdio.h>
#define BUFFER_SIZE 9
#define SQUARE_BUFFER_SIZE 12

float sampleBuffer[BUFFER_SIZE];
float squareBUFFER[SQUARE_BUFFER_SIZE];
float coeffBuffer[BUFFER_SIZE] = {0.001528, 
0.015556, 
0.046380, 
0.081717, 
0.097656, 
0.081717, 
0.046380, 
0.015556, 0.001528};
float sum = 0;


void fillSampleBuffer(float v){
	
	for(int i = BUFFER_SIZE-1; i>0; i--){
		sampleBuffer[i] = sampleBuffer[i-1];
	}

	sampleBuffer[0] = v;

}

float rms(float v){
	sum = sum + v - squareBuffer[SQUARE_BUFFER_SIZE -1];
	
	for(int i = SQUARE_BUFFER_SIZE-1; i>0; i--){
		squareBuffer[i] = squareBuffer[i+1];
	}

	squareBuffer[0] = v;
	
	return sqrt(sum/SQUARE_BUFFER_SIZE);

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
		printf("%i\t%.3f\t%.3f\t%.3f\n", i, generateSample(i/4.0f), fir(), rms(fir()*fir()));
	}
	
	return 0;
}

