#define rootdir "ms0:/"

#define MAX_FILES			256 // max amount of files needed to load.
#define MAX_DISPLAY			5 // max amount of files displayed on-screen.
#define DISPLAY_X			85 // X value of where the filebrowser is displayed.
#define DISPLAY_Y			70 // Y value of the filebrowser is displayed.
#define ICON_DISPLAY_Y      56 
#define CURR_DISPLAY_Y     	52
#define MP3DISPLAY_X        20

/* Globals */

typedef struct fileIcon {

	int		active;

	char	name[256];
	char	filePath[256];
	char	fileType[256];

	int		x;
	int		y;

} fileIcon;

fileIcon folderIcons[MAX_FILES];

typedef struct File {

	int exist;

	char path[256];
	char name[256];

	int size;
	int directory;

} File;

File dirScan[MAX_FILES];


SceIoDirent g_dir;

char *file;
int i;
int current;
int curScroll;
char lastDir[512];
int timer;
char returnMe[512];
SceCtrlData pad, oldpad;

int folderScan(const char* path);
int runFile(const char* path, char* type );
int openDir(const char* path, char* type);
void build_path(char *output, const char *root, const char *path, int append);
void write_file(const char *read_loc, const char *write_loc, const char *name);
void refresh();
void OptionMenu();
void DeleteFile(const char * path);
void recursiveDelete(const char *dir);
void fcopy(char * source);
void fpaste(char * destination);
int checkTextFile(char *textfile);
char *getTextFromFile();
void displayTextFromFile();
void dirUpx5();
void dirDownx5();
void oslPrintText(int x, int y, float size, char * text, OSL_COLOR color);
void dirDisplay();
void centerText(int centerX, int centerY, char * centerText, int centerLength);
void dirVars();
void dirBack();
void dirUp();
void dirDown();
void dirControls();
char * dirBrowse(const char * path);
void filemanager_unload();
int filemanage();