#define configFile "system/build.prop"
#define Address "www.google.com"
#define MAX 8

char backgroundPath[100];
int benchmarkDebugActivate;
void onlineUpdater();
void switchStatus(int n);
void loadConfig();
void saveConfig();
void changer(int set);
int getCpuClock();
int getBusClock();
void pspGetCpuBus();
void pspGetModel(int x, int y);
void aboutMenu();
void easterEgg();
void updatesMenu();
void performanceMenu();
void wait_release(unsigned int buttons);
unsigned int wait_press(unsigned int buttons);
void set_cpu_clock(int n);
void processorMenu();
void ramMenu();
void displayMenu();
void wifiMenu();
void DumpBootBin(void);
void dump_filesystem(const char *root, const char *write_loc);
void dump_memory(void);
void dumpMenu();
void dumpMenuMore();
void developerMenu();
void settingsHighlight();
void settingsDeleteResources();
void settingsMenu();

