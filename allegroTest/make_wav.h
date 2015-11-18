/* make_wav.h
 * Fri Jun 18 17:06:02 PDT 2010 Kevin Karplus
 */

#ifndef MAKE_WAV_H
#define MAKE_WAV_H

#define M_PI 3.1459
#define LOW_QUALITY_SOUND 22050
#define HIGH_QUALITY_SOUND 44100
#define GEN_SAMP_RATE HIGH_QUALITY_SOUND

#define BUF_SIZE (GEN_SAMP_RATE*2)	/* 2 second buffer */


#define BIG_SOUND_BUFFER 4096
#define MED_SOUND_BUFFER 2048 
#define SMALL_SOUND_BUFFER 1024 

#define SUBCHUNK1SIZE 16
#define PCM_FORMAT 1

void write_wav(char * filename, unsigned long num_samples, /*short */int * data, int s_rate);
	/* open a file named filename, write signed 16-bit values as a
		monoaural WAV file at the specified sampling rate
		and close the file
	*/
unsigned char* create_wav(unsigned long num_samples, /*short*/ int * data, int s_rate,int & sizeOfWav);

#endif