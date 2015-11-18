//create wave stuff
#include <math.h>
#include "AllegroTimer.h"
#include "make_wav.h"

#define NUM_BUTTONS 9

using namespace std;

bool done = false;
	
bool timeOver = false;//used to know when to play time over sound

int buffer[BUF_SIZE];
int sizeOfWav;
int  screenWidth = 640, screenHeight = 480;
int timerX = (screenWidth/2), timerY = (screenHeight/4);
int mins = 2, sec = 5,mil = 0;

BITMAP*screenBuffer = NULL;
	
AllegroTimer workoutTimer;

unsigned char *doSoundCreateTest()
{
    float amplitude = 32000;//volume
    float freq_Hz = 261.626;//this is middle C //240;//note
    float phase=0;

    float freq_radians_per_sample = freq_Hz*2*M_PI/GEN_SAMP_RATE;

    /* fill buffer with a sine wave */
    for (int i=0; i<BUF_SIZE; i++)
    {
        phase += freq_radians_per_sample;
		buffer[i] = (int)(amplitude * sin(phase));
    }

    return create_wav(BUF_SIZE, buffer, GEN_SAMP_RATE ,sizeOfWav);
}


int getRandomNum(int min, int max)
{
	//if(rand()%10 > 5)
	{
		int diff = ((max - min) + 1);
		return ((diff * rand())/RAND_MAX) + min;
	}
	//return rand()%max + min;
}



double convertSecondsToDays(double secs)
{
	//return secs/86400;
	double t = secs/86400;
	return t;
	//1.15741
}
void addMin(int x)
{
	mins+=x;
	workoutTimer.minutes = mins;
}
void subMin(int x)
{
	if(mins > 0)
	{
		mins-=x;
		workoutTimer.minutes = mins;
	}
}
void addSec(int x)
{
	sec+=x;
	workoutTimer.seconds = sec;
}
void subSec(int x)
{
	if(sec > 0)
	{
		sec-=x;
		workoutTimer.seconds = sec;
	}
}
void addMil(int x)
{
	mil+=x;
	workoutTimer.decimals = mil;
}
void subMil(int x)
{
	if(mil > 0)
	{
		mil-=x;
		workoutTimer.decimals = mil;
	}
}
void start(int x)
{
	workoutTimer.pause = !workoutTimer.pause;
	if(workoutTimer.decimals == 0)
	{
		if(workoutTimer.seconds > 1)
			workoutTimer.seconds--;
		else if(workoutTimer.minutes > 1)
		{
			workoutTimer.minutes--;
			workoutTimer.seconds = 59;
		}
	}
}


void reset(int x)
{
	if(!workoutTimer.elapsedTimer)
	{
		workoutTimer.minutes = mins;
		workoutTimer.seconds = sec;
		workoutTimer.decimals = mil;
	}
	else
	{
		workoutTimer.minutes = 0;
		workoutTimer.seconds = 0;
		workoutTimer.decimals = 0;
	}
	workoutTimer.pause = true;
	workoutTimer.timeOver = false;
	timeOver = false;
}
void countType(int x)
{
	workoutTimer.elapsedTimer = !workoutTimer.elapsedTimer;
	reset(1);
}
