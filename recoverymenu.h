#define PWD "password.TXT"
#define selector_xDistance 0 //The distance between the X axis of the two selections
#define selector_yDistance 10 //The distance between the Y axis of the two selections

int selector_image_x; //Determines the starting x position of the selection
int selector_image_y; //Determines the starting y position of the selection

void ShowPage5();
void ShowPage4();
void backupPassword();
void ShowPage3();
void ShowPage2();
void ShowPage1();
void ShowVersionTxt();
int ConfigurationMenu();
int ShowAdvancedCnfMenu();
int ShowCnfMenu();
int ShowSystemMenu();
int ShowBatteryMenu();
void unloadRecoveryMenuRes();
int mainRecoveryMenu();