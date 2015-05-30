#include <pspkernel.h>
#include <pspdebug.h>
#include <oslib/oslib.h>
#include <pspctrl.h>
#include <psprtc.h>
#include <psputils.h>

OSL_IMAGE *clockBg, *timeBg, *cursor, *stopWatchBg, *timerBg, *timerPause, *timerPlay, *reset;

void startTimer();
void resetTimer();
int getTimer();
void getDayOfWeek(int x, int y, int n); //Outputs the Day of the Week
void getMonthOfYear(int x, int y); //Outputs the Month of the Year
void digitaltime(int x, int y, int color);
void centerClock(int n);
void startCounter();
void stopWatch();
int pspTimer();
int pspStopWatch();
void loadBg();
int pspclock();