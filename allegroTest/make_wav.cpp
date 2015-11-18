/* make_wav.c
 * Creates a WAV file from an array of ints.
 * Output is monophonic, signed 16-bit samples
 * copyright
 * Fri Jun 18 16:36:23 PDT 2010 Kevin Karplus
 * Creative Commons license Attribution-NonCommercial
 *	http://creativecommons.org/licenses/by-nc/3.0/
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <cstring>//memmove

#include "make_wav.h"



void write_little_endian(unsigned int word, int num_bytes, FILE *wav_file)
{
    unsigned buf;
    while(num_bytes>0)
    {   buf = word & 0xff;
	//size_t fwrite ( const void * ptr, size_t size, size_t count, FILE * stream );
    	fwrite(&buf, 1,1, wav_file);
        num_bytes--;
	word >>= 8;
    }
}

unsigned char* write_to_buffer_little_endian(unsigned int word, int num_bytes)
{
    unsigned buf;
	unsigned char *buffer;
	int offset = 0;
	buffer = (unsigned char*) malloc(num_bytes);
    while(num_bytes>0)
    {   buf = word & 0xff;
		//void * memmove ( void * destination, const void * source, size_t num );
	//needs an offset so mem is overwritten
    	memmove(buffer+offset,&buf, 1);
        num_bytes--;
		offset++;
	word >>= 8;
    }
	return buffer;
}

/* information about the WAV file format from
	http://ccrma.stanford.edu/courses/422/projects/WaveFormat/
 */

void write_wav(char * filename, unsigned long num_samples, /*short*/ int * data, int s_rate)
{
    FILE* wav_file;
    unsigned int sample_rate;
    unsigned int num_channels;
    unsigned int bytes_per_sample;
    unsigned int byte_rate;
    unsigned long i;	/* counter for samples */

    num_channels = 1;	/* monoaural */
    bytes_per_sample = 2;

    if (s_rate<=0) sample_rate = 44100;
    else sample_rate = (unsigned int) s_rate;

    byte_rate = sample_rate*num_channels*bytes_per_sample;

    wav_file = fopen(filename, "w");
    assert(wav_file);	/* make sure it opened */

    /* write RIFF header */
    fwrite("RIFF", 1, 4, wav_file);
    write_little_endian(36 + bytes_per_sample* num_samples*num_channels, 4, wav_file);
    fwrite("WAVE", 1, 4, wav_file);

    /* write fmt  subchunk */
    fwrite("fmt ", 1, 4, wav_file);
    write_little_endian(SUBCHUNK1SIZE, 4, wav_file);	/* SubChunk1Size is 16 */
    write_little_endian(PCM_FORMAT, 2, wav_file);	/* PCM is format 1 */
    write_little_endian(num_channels, 2, wav_file);
    write_little_endian(sample_rate, 4, wav_file);
    write_little_endian(byte_rate, 4, wav_file);
    write_little_endian(num_channels*bytes_per_sample, 2, wav_file);  /* block align */
    write_little_endian(8*bytes_per_sample, 2, wav_file);  /* bits/sample */

    /* write data subchunk */
    fwrite("data", 1, 4, wav_file);
    write_little_endian(bytes_per_sample* num_samples*num_channels, 4, wav_file);
    for (i=0; i< num_samples; i++)
    { 	write_little_endian((unsigned int)(data[i]),bytes_per_sample, wav_file);
    }

    fclose(wav_file);
}


void appendToWave(unsigned char *dest, unsigned char *src, int numBytes, int & offset)
{
//	char *buffer = (char *)memcpy (dest+offset,src,numBytes);
	//memmove (dest+offset,src,numBytes);
	for(int i = 0; i < numBytes; i++)
		dest[i + offset] = src[i];

	offset += numBytes;
	//return buffer;
	
}
unsigned char *create_wav(unsigned long num_samples, /*short*/ int * data, int s_rate, int & sizeOfWav)
{
	int blockCount = 0;// when we realloc, we willnned a running count on current size
	unsigned char *wav_file;//final result
    unsigned int sample_rate;
    unsigned int num_channels;
    unsigned int bytes_per_sample;
    unsigned int byte_rate;
    unsigned long i;	/* counter for samples */

    num_channels = 1;	/* monoaural */
    bytes_per_sample = 2;

    if (s_rate<=0) sample_rate = 44100;
    else sample_rate = (unsigned int) s_rate;

    byte_rate = sample_rate*num_channels*bytes_per_sample;

//--------------------------------------
    /* write RIFF header */
    unsigned char riffHeader[5] = "RIFF";
    unsigned char *riffData = write_to_buffer_little_endian(36 + bytes_per_sample* num_samples*num_channels, 4);
    unsigned char waveHeader[5] = "WAVE";

    /* write fmt  subchunk */
    unsigned char fmtSubchunk[5] = "fmt ";
    unsigned char *subChunk = write_to_buffer_little_endian(SUBCHUNK1SIZE, 4);	/* SubChunk1Size is 16 */
    unsigned char *format =write_to_buffer_little_endian(PCM_FORMAT, 2);	/* PCM is format 1 */
    unsigned char *channels = write_to_buffer_little_endian(num_channels, 2);
    unsigned char *sampRate =write_to_buffer_little_endian(sample_rate, 4);
    unsigned char *byteRate =write_to_buffer_little_endian(byte_rate, 4);
    unsigned char *align =write_to_buffer_little_endian(num_channels*bytes_per_sample, 2);  /* block align */
    unsigned char *bitsPerSample =write_to_buffer_little_endian(8*bytes_per_sample, 2);  /* bits/sample */

    /* write data subchunk */
    unsigned char dataSubchunk[5] = "data";
    unsigned char *dataChunk =write_to_buffer_little_endian(bytes_per_sample* num_samples*num_channels, 4);
//-------------------------------------
	wav_file = (unsigned char*) malloc(45);//add up the previous mem allocations to get 45
	 memset (wav_file,'-',44); 
	//piece together wav broiler plate stuff here
	appendToWave(wav_file, riffHeader, 4, blockCount);
	appendToWave(wav_file, riffData, 4, blockCount);
	appendToWave(wav_file, waveHeader, 4, blockCount);
	appendToWave(wav_file, fmtSubchunk, 4, blockCount);

	appendToWave(wav_file, subChunk, 4, blockCount);
	appendToWave(wav_file, format, 2, blockCount);
	appendToWave(wav_file, channels, 2, blockCount);
	appendToWave(wav_file, sampRate, 4, blockCount);
	appendToWave(wav_file, byteRate, 4, blockCount);
	appendToWave(wav_file, align, 2, blockCount);
	appendToWave(wav_file, bitsPerSample, 2, blockCount);
	appendToWave(wav_file, dataSubchunk, 4, blockCount);
	appendToWave(wav_file, dataChunk, 4, blockCount);
	
	//now add sound data that was passed in from param
    for (i=0; i< num_samples; i++)
	{
     	
		unsigned char *temp = write_to_buffer_little_endian((unsigned int)(data[i]),bytes_per_sample);
		wav_file = (unsigned char*) realloc (wav_file, blockCount+bytes_per_sample * sizeof(unsigned char*));
		appendToWave(wav_file, temp, bytes_per_sample, blockCount);

	}
	sizeOfWav = blockCount;
	return wav_file;
	
}