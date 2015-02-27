#include <pspkernel.h>
#include <pspdebug.h>
#include <oslib/oslib.h>
#include <pspctrl.h>
#include <psprtc.h>

OSL_IMAGE *clockBg, *timeBg, *cursor, *stop_watch;
OSL_FONT *Roboto;

void getDayOfWeek(int x, int y, int n); //Outputs the Day of the Week
void getMonthOfYear(int x, int y); //Outputs the Month of the Year
void digitaltime(int x, int y, int x2);
void startCounter();
void stopWatch();
int pspclock();