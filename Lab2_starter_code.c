#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <sndfile.h>
#include <math.h>

#define PI 3.14159265
#define ECHO_DELAY_SEC 0.5f   // Echo delay in seconds
#define ECHO_DRY  0.7f        // Gain for original signal
#define ECHO_WET  0.3f        // Gain for delayed signal

/* ------------------------------------------------------------
 * min_max_normalize
 * Normalizes an array in-place to the range [-1, 1].
 * ------------------------------------------------------------ */
void min_max_normalize(float *sig, int len) {
    float min_val =  FLT_MAX;
    float max_val = -FLT_MAX;

    // Find min and max
    for (int i = 0; i < len; i++) {
        if (sig[i] < min_val) min_val = sig[i];
        if (sig[i] > max_val) max_val = sig[i];
    }

    float range = max_val - min_val;
    if (range == 0.0f) return; // Avoid division by zero (silent signal)

    // Scale to [-1, 1]
    for (int i = 0; i < len; i++) {
        sig[i] = 2.0f * (sig[i] - min_val) / range - 1.0f;
    }
}

/* ------------------------------------------------------------
 * main
 * Usage: ./task6_echo <input.wav> <output.wav>
 * ------------------------------------------------------------ */
int main(int argc, char *argv[])
{
    // Require input and output file arguments
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <input.wav> <output.wav>\n", argv[0]);
        return -1;
    }

    // ----------------------------------------------------------
    // Open input file
    // ----------------------------------------------------------
    SF_INFO sndInfo;
    SNDFILE *sndFile = sf_open(argv[1], SFM_READ, &sndInfo);
    if (sndFile == NULL) {
        fprintf(stderr, "Error reading source file '%s': %s\n", argv[1], sf_strerror(sndFile));
        return 1;
    }

    // Check format — must be 16-bit PCM WAV
    if (sndInfo.format != (SF_FORMAT_WAV | SF_FORMAT_PCM_16)) {
        fprintf(stderr, "Input should be a 16-bit PCM WAV file\n");
        sf_close(sndFile);
        return 1;
    }

    // Check channels — must be mono
    if (sndInfo.channels != 1) {
        fprintf(stderr, "Input must be mono (1 channel)\n");
        sf_close(sndFile);
        return 1;
    }

    // ----------------------------------------------------------
    // Open output file
    // ----------------------------------------------------------
    SF_INFO sndInfoOut  = sndInfo;
    sndInfoOut.format   = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
    sndInfoOut.channels = 1;

    SNDFILE *sndFileOut = sf_open(argv[2], SFM_WRITE, &sndInfoOut);
    if (sndFileOut == NULL) {
        fprintf(stderr, "Error opening output file '%s': %s\n", argv[2], sf_strerror(sndFileOut));
        sf_close(sndFile);
        return 1;
    }

    // ----------------------------------------------------------
    // Allocate memory
    // ----------------------------------------------------------
    int N = (int)sndInfo.frames;

    float *sig_in  = malloc(N * sizeof(float));
    float *sig_out = malloc(N * sizeof(float));

    if (sig_in == NULL || sig_out == NULL) {
        fprintf(stderr, "Could not allocate memory\n");
        sf_close(sndFile);
        sf_close(sndFileOut);
        return 1;
    }

    // ----------------------------------------------------------
    // Read input audio
    // ----------------------------------------------------------
    sf_readf_float(sndFile, sig_in, N);

    // ----------------------------------------------------------
    // Min-max normalize to [-1, 1]
    // ----------------------------------------------------------
    min_max_normalize(sig_in, N);

    // ----------------------------------------------------------
    // Apply echo effect
    //
    //   y[n] = 0.7 * x[n] + 0.3 * x[n - Ne]
    //
    // where Ne = Fs * 0.5 (0.5 second delay)
    // For n < Ne, x[n - Ne] is treated as 0 (zero-padding)
    // ----------------------------------------------------------
    int Ne = (int)(sndInfo.samplerate * ECHO_DELAY_SEC);
    printf("Sample rate : %d Hz\n", sndInfo.samplerate);
    printf("Echo delay  : %d samples (%.1f s)\n", Ne, ECHO_DELAY_SEC);
    printf("Total frames: %d\n", N);

    for (int i = 0; i < N; i++) {
        float delayed = (i >= Ne) ? sig_in[i - Ne] : 0.0f;
        sig_out[i] = ECHO_DRY * sig_in[i] + ECHO_WET * delayed;
    }

    // ----------------------------------------------------------
    // Write output audio
    // ----------------------------------------------------------
    sf_writef_float(sndFileOut, sig_out, N);

    // ----------------------------------------------------------
    // Cleanup
    // ----------------------------------------------------------
    sf_write_sync(sndFileOut);
    sf_close(sndFile);
    sf_close(sndFileOut);
    free(sig_in);
    free(sig_out);

    printf("Done! Echo output written to %s\n", argv[2]);
    return 0;
}
