
#include <allegro.h>
#include <winalleg.h>
#include <algorithm>

#include <ctime> 
#include <string>

#include "button.h"
#include "main.h"
#include <sys/stat.h>


using namespace std;


int main(int argc, char* argv[])
{
	time_t rawtime;
	struct tm* timeinfo = NULL;

	struct tm* creationTime = NULL;     

	string temp = argv[0];

	int found = temp.find_last_of("/\\");
	
	bool mouseClicked = false;
	srand((unsigned)time(0));
	long ltime = time(NULL);
	int stime = (unsigned) ltime/2;

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	allegro_init();
	install_keyboard();
	install_mouse();
	enable_hardware_cursor();

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


	string filePathBase = temp.substr(0,found);
	string sndFile = filePathBase + "\\snd.wav";

	replace( sndFile.begin(), sndFile.end(), '\\', '/' );


	 FONT *myFont;
     PALETTE palette;
	 string fontFile = filePathBase + "\\gothicHeavy.pcx";
      myFont = load_font(fontFile.c_str(), palette, NULL);
      if (!myFont)
	  {
         allegro_message("couldnt load font", "couldnt set it", "Using defaults.","&Continue", NULL, 'c', 0);
		 myFont = font;
	  }
	SAMPLE *snd = load_sample(sndFile.c_str());

	//use same color depth as current desktop color depth
	int depth =  desktop_color_depth();
	if (depth != 0) 
         set_color_depth(depth);
	else
		alert("bad color depth", "couldnt set it", "Using defaults.","&Continue", NULL, 'c', 0);

	if(set_gfx_mode(GFX_AUTODETECT_WINDOWED,screenWidth,screenHeight,0,0)!=0)
	{
		allegro_message(allegro_error);
		return 0;
	}
	//let the program run in the background
	set_display_switch_mode(SWITCH_BACKGROUND);
	//Sets your window title
    set_window_title("stop watch");



	if(install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,NULL) != 0)
	{ 
		printf("Error setting up Sound, so the game will be silent!\n%s\n", allegro_error);     
	}

	screenBuffer = create_bitmap(screenWidth, screenHeight);
	clear(screenBuffer);
	
	Button button[NUM_BUTTONS];
	{
		int x = 200, y =250;
		int w = 64, h =32;
		int spacing = 80;
		
		workoutTimer.elapsedTimer = true;

		button[0].init(x,y,w,h, "m_up","m_up");
		button[0].buttonFunctionI = &addMin;
		
		button[1].init(x,y+(h+2),w,h, "m_dn","m_dn");
		button[1].buttonFunctionI = &subMin;

		button[2].buttonFunctionI = &addSec;
		button[2].init(x+spacing,y,w,h, "s_up","s_up");
		
		button[3].buttonFunctionI = &subSec;
		button[3].init(x+spacing,y+(h+2),w,h, "s_dn","s_dn");

		button[4].init(x+(spacing*2),y,w,h, "n_up","n_up");
		button[4].buttonFunctionI = &addMil;

		button[5].init(x+(spacing*2),y+(h+2),w,h, "n_dn","n_dn");
		button[5].buttonFunctionI = &subMil;

		button[6].init(x,y+100,w,h, "start","pause");
		button[6].buttonFunctionI = &start;
		button[6].isToggleable = true;

		button[7].init(x+spacing,y+100,w,h, "reset","reset");
		button[7].buttonFunctionI = &reset;
		
		button[8].init((x+spacing*2),y+100,w,h, "cntDn","cntUp");
		button[8].buttonFunctionI = &countType;
		button[8].isToggleable = true;

		workoutTimer.elapsedTimer = true;
	}

	workoutTimer.pause = true;

	do//  all the looping begins here, duh
	{                                 
		rest(0);
		while (speed_counter > 0)//this makes sure the game runs at a constant rate on any machine
		{
			
			poll_mouse();
			if(mouse_b & 1)
				mouseClicked = true;
			else
				mouseClicked = false;

			for(int i = 0; i < NUM_BUTTONS; i++)
			{
				button[i].buttonLogic(mouse_x,mouse_y,mouseClicked);
			}

			if (key[KEY_ESC])  //this will exit you from the game...and the program as of now
				done = true;
				
			if(key[KEY_RIGHT])
			{
				while (key[KEY_RIGHT]) {}//only do action once key is released
			}
			else if(key[KEY_LEFT])
			{
				while (key[KEY_LEFT]) {}//only do action once key is released
			}
			
		
			else if(key[KEY_UP])
			{
				while (key[KEY_UP]) {}//only do action once key is released
				workoutTimer.startElapsedTimer();
				timeOver = false;
			}
			else if(key[KEY_DOWN])
			{
				while (key[KEY_DOWN]) {}
				workoutTimer.startCountdown(mins,sec,mil);
				timeOver = false;
			}
		
			else if(key[KEY_SPACE])
			{
				while (key[KEY_SPACE]) {}//only do action once key is released
				workoutTimer.pause = !workoutTimer.pause;
			}
			time ( &rawtime );
			timeinfo = localtime ( &rawtime );
			
			workoutTimer.updateStopWatch();

			if (snd && workoutTimer.timeOver == true && timeOver == false)
			{
				play_sample(snd, 255, 128, 1000, 0);
				timeOver = true;
				button[6].resetButton();
			}

			speed_counter--;   //for constant rate

		}//end logic while loop
			
///////////////////////////////////////////////////				

		clear(screenBuffer);
		textprintf_ex(screenBuffer, font, 10, 30,  makecol(255,255,255), 0, "fps: %d",fps);
		textprintf_ex(screenBuffer, font, 10, 50,  makecol(255,255,255), 0,"%s", asctime (timeinfo));
	
		textprintf_ex(screenBuffer, myFont, timerX, timerY,  makecol(255,255,255), 0,"%d:%d:%d", workoutTimer.minutes,workoutTimer.seconds,workoutTimer.decimals);
		
		for(int i = 0; i < NUM_BUTTONS; i++)
			button[i].drawButton(screenBuffer);
		
		show_mouse(screenBuffer);
		blit(screenBuffer, screen, 0, 0, 0, 0, screenWidth, screenHeight); //blit finial product to the screen, makes it slow but smooth
		frame_counter++;  //update the fps
	} while (!done);

	remove_mouse();
	destroy_bitmap(screenBuffer);
	return 0;
}
END_OF_MAIN()