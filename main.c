#include <math.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#define BUFFER_SIZE 51
#define SQUARE_BUFFER_SIZE 128

const float coeff_lp[BUFFER_SIZE] = {
0.001528*2.5772, 
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
    int16_t sample;
    FILE * file;
    file = fopen("samples.raw","rb"); 

    struct Filter lowPass;
    memcpy(lowPass.coeffBuffer, coeff_lp, BUFFER_SIZE*sizeof(float));
    memset(lowPass.squareBuffer, 0, SQUARE_BUFFER_SIZE*sizeof(float));

	for(int i = 0; i<16000; i++){
        float a = fir(lowPass.coeffBuffer);
        fread(&sample, sizeof(int16_t), 1, file);
        float samplef = ((float)sample)/((float)0xFFFF/2);
		fillBuffer(samplef);
		printf("%i\t%.3f\t%.3f\t%.3f\n", i, samplef, a, rms(a,&lowPass));
	}
	
	return 0;
}

