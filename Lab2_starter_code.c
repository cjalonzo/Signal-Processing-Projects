#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <sndfile.h>
#include <math.h>

#define PI 3.14159265

int main(int argc, char *argv[])
{
	int ii;

	//Require 2 arguments: input file and output file
	if(argc < 2)
	{
		printf("Not enough arguments \n");
		return -1;
	}

	SF_INFO sndInfo;
	SNDFILE *sndFile = sf_open(argv[1], SFM_READ, &sndInfo);
	if (sndFile == NULL) {
		fprintf(stderr, "Error reading source file '%s': %s\n", argv[1], sf_strerror(sndFile));
		return 1;
	}

	SF_INFO sndInfoOut = sndInfo;
	sndInfoOut.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
	sndInfoOut.channels = 1;
	sndInfoOut.samplerate = sndInfo.samplerate;
	SNDFILE *sndFileOut = sf_open(argv[2], SFM_WRITE, &sndInfoOut);

	// Check format - 16bit PCM
	if (sndInfo.format != (SF_FORMAT_WAV | SF_FORMAT_PCM_16)) {
		fprintf(stderr, "Input should be 16bit Wav\n");
		sf_close(sndFile);
		return 1;
	}

	// Check channels - mono
	if (sndInfo.channels != 1) {
		fprintf(stderr, "Wrong number of channels\n");
		sf_close(sndFile);
		return 1;
	}

	// The following code is a only a template, please rewrite

	// Allocate memory
	float *sig_in = malloc (sndInfo.frames*sizeof(float));
	float *sig_out = malloc(sndInfo.frames*sizeof(float));
	float *buffer = malloc( sndInfo.frames*sizeof(float));

	if (sig_in == NULL) {
		fprintf(stderr, "Could not allocate memory for file\n");
		sf_close(sndFile);
	 	return 1;
	}

	// Read data
	// ...
	for (int x = 0; x< sndInfo.frames; x++){
		sf_readf_float(sndFile, buffer, 1);
		sig_in[x] =* buffer;
	}
	// ...
	
	// Your implementation
	
	
	//int nq = sizeof(sig_in);// adjust
	int nq = 960000;
	printf("length: %d", nq);
	
	float delay[24000] = {0};
	float buff[24000+960000]={0}; //nq =100000
	float y[100000]= {0};
	printf("%d",buff[25000]);
	
	//float *x = sig_in;
	//float *b = buff;
	int i;

	for (i =0; i<24000; i++){
		buff[i] = delay[i];
	}
	for (i=24000; i< nq; i++){
		buff[i] = sig_in[i];
	}
	for (i =0; i < 960000-1; i++){
		*(sig_out+i) = 0.7*(*(sig_in+i))+0.3*(*(buff+i));
	}
	printf("%f", sig_out[10]);

	// Write data
	// ...
	for (int i =0; i < 960000; i++){
		sf_writef_float(sndFileOut, (sig_out+i), 1);
	}
	// ...
	
	sf_close(sndFile);
	sf_write_sync(sndFileOut);
	sf_close(sndFileOut);
	free(sig_in);
	//free(x);
	free(sig_out);
	

	return 1;
}
