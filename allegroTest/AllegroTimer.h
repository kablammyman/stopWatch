#ifndef INC_TIMERS_H
#define INC_TIMERS_H


extern volatile unsigned int speed_counter;
extern volatile unsigned int milliSec;
extern volatile int fps;
extern volatile int frame_counter;

void fps_proc(); //this counts frame rate
void increment_speed_counter(); 
void inc_milliSec();

//we can use this to verify the setup was done. right now, no checks are in place
static bool isInit = false;
//to make sure the allegro timer stuff gets done, we will make the user call this method
static void initAllegroTimer()
{
	if(isInit)
		return;
	LOCK_VARIABLE(speed_counter);
	LOCK_VARIABLE(milliSec);  
	LOCK_VARIABLE(fps);
	LOCK_VARIABLE(frame_counter);

	LOCK_FUNCTION(inc_milliSec);
	LOCK_FUNCTION(increment_speed_counter); 
	LOCK_FUNCTION(fps_proc);
	
	install_int(inc_milliSec, 10);// every second    //ever 10 of a sec 
	install_int_ex(increment_speed_counter, BPS_TO_TIMER(60));
	install_int(fps_proc, 1000); 

	isInit = true;
}

class AllegroTimer
{
public:
	int decimals, seconds, minutes;
	int dec_factor, sec_factor, min_factor;
	bool elapsedTimer;
	unsigned int startMilli;
	bool pause;
	bool timeOver;

	AllegroTimer()
	{
		decimals = seconds = minutes = 0;
		dec_factor = sec_factor = min_factor = 0;
		elapsedTimer = true;
		startMilli = milliSec;
	}

	void startElapsedTimer();
	void startCountdown(int min, int sec, int milli);
	void updateElapsedTime();
	void updateCountdown();
	void updateStopWatch();
	bool isTimeUp();
	void convert_to_standard(int time, int &min, int &sec, int &dec);
	int convert_to_milliseconds(int min, int sec,int dec);
	void addToMins(int amt);
	void addToSecs(int amt);
	void addToMils(int amt);
};


#endif //INC_TIMERS_H

