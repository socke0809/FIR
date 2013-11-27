#include <math.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#define BUFFER_SIZE 9
#define SQUARE_BUFFER_SIZE 12

const float coeff_lp[BUFFER_SIZE] = {0.001528*2.5772, 
0.015556*2.5772, 
0.046380*2.5772, 
0.081717*2.5772, 
0.097656*2.5772, 
0.081717*2.5772, 
0.046380*2.5772, 
0.015556*2.5772,
0.001528*2.5772};

float sampleBuffer[BUFFER_SIZE];

struct Filter{ 
    float squareBuffer[SQUARE_BUFFER_SIZE];
    float coeffBuffer[BUFFER_SIZE];
    float sum;
};


void fillBuffer(float v){
	
	for(int i = BUFFER_SIZE-1; i>0; i--){
		sampleBuffer[i] = sampleBuffer[i-1];
	}

	sampleBuffer[0] = v;

}

float rms(float v, struct Filter* filter){
	filter->sum = filter->sum + v*v - filter->squareBuffer[SQUARE_BUFFER_SIZE -1];
	
	for(int i = SQUARE_BUFFER_SIZE-1; i>0; i--){
		filter->squareBuffer[i] = filter->squareBuffer[i-1];
	}

	filter->squareBuffer[0] = v*v;
	
	return sqrt(filter->sum/SQUARE_BUFFER_SIZE);

}

float fir(float filter[]){
	float res = 0;
	for(int i = 0; i<BUFFER_SIZE; i++){
		res = res + sampleBuffer[i]*filter[i];
	}
	
	return res;
}

float generateSample(float x){
	return 2*sin(x)+sin(15*x)+sin(10*x);
}
	
	

int main(void){
    int32_t sample;
    char sampleB[5];
    FILE * file;
    file = fopen("samples.raw","r"); 

    struct Filter lowPass;
    memcpy(lowPass.coeffBuffer, coeff_lp, BUFFER_SIZE*sizeof(float));
    memset(lowPass.squareBuffer, 0, SQUARE_BUFFER_SIZE*sizeof(float));

	for(int i = 0; i<40000; i++){
        float a = fir(lowPass.coeffBuffer);
        fgets(sampleB, 4, file);
        sample = sampleB[3] | sampleB[2]<<8 | sampleB[1]<<16 | sampleB[0]<<24;
		fillBuffer((float)sample);
		printf("%i\t%i\t%.3f\t%.3f\n", i, sample, a, rms(a,&lowPass));
	}
	
	return 0;
}

