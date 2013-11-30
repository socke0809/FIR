#include <math.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#define BUFFER_SIZE 51
#define SQUARE_BUFFER_SIZE 128

const float coeff_lp[BUFFER_SIZE] = {
//0.001528*2.5772, 
//0.015556*2.5772, 
//0.046380*2.5772, 
//0.081717*2.5772, 
//0.097656*2.5772, 
//0.081717*2.5772, 
//0.046380*2.5772, 
//0.015556*2.5772,
//0.001528*2.5772};

0.00066, -0.00008, -0.00099, -0.00162, -0.00137, 0.00016, 0.00257, 0.00436, 0.00368, -0.00034, -0.00625, -0.01032, -0.00850, 0.00056, 0.01325, 0.02173, 0.01791, -0.00077, -0.02739, -0.04648, -0.04047, 0.00092, 0.07334, 0.15700, 0.22379, 0.24926, 0.22379, 0.15700, 0.07334, 0.00092, -0.04047, -0.04648, -0.02739, -0.00077, 0.01791, 0.02173, 0.01325, 0.00056, -0.00850, -0.01032, -0.00625, -0.00034, 0.00368, 0.00436, 0.00257, 0.00016, -0.00137, -0.00162, -0.00099, -0.00008, 0.00066};

//0.0010121, 0.0010233, 0.0009973, 0.0008614, 0.0005111, -0.0001634, -0.0012440, -0.0027478, -0.0045949, -0.0065879, -0.0084093, -0.0096393, -0.0097956, -0.0083909, -0.0050010, 0.0006643, 0.0086992, 0.0189587, 0.0310414, 0.0443045, 0.0579106, 0.0709016, 0.0822938, 0.0911805, 0.0968302, 0.0987682, 0.0968302, 0.0911805, 0.0822938, 0.0709016, 0.0579106, 0.0443045, 0.0310414, 0.0189587, 0.0086992, 0.0006643, -0.0050010, -0.0083909, -0.0097956, -0.0096393, -0.0084093, -0.0065879, -0.0045949, -0.0027478, -0.0012440, -0.0001634, 0.0005111, 0.0008614, 0.0009973, 0.0010233, 0.0010121};


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

	for(int i = 0; i<160; i++){
        float a = fir(lowPass.coeffBuffer);
        for(int j=0; j<10; j++){
            fread(&sample, sizeof(int16_t), 1, file);
        }
        float samplef = ((float)sample)/((float)0xFFFF/2);
		fillBuffer(samplef);
		printf("%i\t%.3f\t%.3f\t%.3f\n", i, samplef, a, rms(a,&lowPass));
	}
	
	return 0;
}

