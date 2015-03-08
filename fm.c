#include "home.h"
#include "game.h"
#include "apollo.h"
#include "fm.h"
#include "clock.h"
#include "gallery.h"
#include "lock.h"
#include "multi.h"
#include "power_menu.h"
#include "include/pgeZip.h"
#include "settingsmenu.h"
#include "screenshot.h"
#include "include/utils.h"

//Functions imported from PRX
void startISO(char* file,int driver);
int launch_pops(char *path);

int folderScan(const char* path )
{
	curScroll = 1;
	sprintf(lastDir, path);

	int i;
	for (i=0; i<=MAX_FILES; i++)	// erase old folders
		dirScan[i].exist = 0;

	int x;
	for (x=0; x<=MAX_FILES; x++) {
		folderIcons[x].active = 0;
	}

	int fd = sceIoDopen(path);

	i = 1;
	
	if (fd) {
		if (!(stricmp(path, "ms0:")==0 || (stricmp(path, "ms0:/")==0))) {

			sceIoDread(fd, &g_dir);		// get rid of '.' and '..'
			sceIoDread(fd, &g_dir);

			// Create our own '..'
			folderIcons[1].active = 1; 
			sprintf(folderIcons[1].filePath, "doesn't matter");
			sprintf(folderIcons[1].name, "..");
			sprintf(folderIcons[1].fileType, "dotdot");

			x = 2;
		} else {
			x = 1;
		}
		while ( sceIoDread(fd, &g_dir) && i<=MAX_FILES ) {
			sprintf( dirScan[i].name, g_dir.d_name );
			sprintf( dirScan[i].path, "%s/%s", path, dirScan[i].name );
			
			//skip . entries
			if (!strcmp(".", g_dir.d_name)) 
			{
				memset(&g_dir, 0, sizeof(SceIoDirent));
				continue;
			};

			if (g_dir.d_stat.st_attr & FIO_SO_IFDIR) {
				dirScan[i].directory = 1;
				dirScan[i].exist = 1;
			} else {
				dirScan[i].directory = 0;
				dirScan[i].exist = 1;
			}

			dirScan[i].size = g_dir.d_stat.st_size;
			i++;
		}
	}

	sceIoDclose(fd);

	for (i=1; i<MAX_FILES; i++) {
		if (dirScan[i].exist == 0) break;
		folderIcons[x].active = 1;
		sprintf(folderIcons[x].filePath, dirScan[i].path);
		sprintf(folderIcons[x].name, dirScan[i].name);

		char *suffix = strrchr(dirScan[i].name, '.');
		
		if (dirScan[i].directory == 1) {      // if it's a directory
			sprintf(folderIcons[x].fileType, "fld");
		} 
		else if ((dirScan[i].directory == 0) && (suffix)) {		// if it's not a directory
			sprintf(folderIcons[x].fileType, "none");
		}
		else if (!(suffix)) {
			sprintf(folderIcons[x].fileType, "none");
		}
		x++;
	}

	return 1;
}

int runFile(const char* path, char* type)
{
	// Folders
	openDir(path, type);
	
	// '..' or 'dotdot'
	if (strcmp(type, "dotdot")==0){
		current = 1;
		dirBack();
	}		
	return 1;
}

int openDir(const char* path, char* type)
{
	if (strcmp(type, "fld")==0) {
		current = 1;
		folderScan(path);
	}
	return 1;
}

/* Build a path, append a directory slash if requested */
void build_path(char *output, const char *root, const char *path, int append)
{
	while(*root != 0)
	{
		*output++ = *root++;
	}

	if(*(root-1) != '/')
	{
		*output++ = '/';
	}

	while(*path != 0)
	{
		*output++ = *path++;
	}
	if(append)
		*output++ = '/';

	*output++ = 0;
}

/* Define a write buffer */
char write_buffer[128*1024];

void write_file(const char *read_loc, const char *write_loc, const char *name)
{
	int fdin;
	int fdout;
	char readpath[256];
	char writepath[256];

	build_path(readpath, read_loc, name, 0);
	build_path(writepath, write_loc, name, 0);
	printf("Writing %s\n", writepath);

	fdin = sceIoOpen(readpath, PSP_O_RDONLY, 0777);
	if(fdin >= 0)
	{
		int bytesRead = 1;
		fdout = sceIoOpen(writepath, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		if(fdout < 0)
		{
			printf("Couldn't open %s\n", writepath);
		}

		bytesRead = sceIoRead(fdin, write_buffer, sizeof(write_buffer));
		while((bytesRead > 0) && (fdout >= 0))
		{
			sceIoWrite(fdout, write_buffer, bytesRead);
			bytesRead = sceIoRead(fdin, write_buffer, sizeof(write_buffer));
		}

		if(fdout >= 0)
		{
			sceIoClose(fdout);
		}

		if(fdin >= 0)
		{
			sceIoClose(fdin);
		}
	}
	else
	{
		printf("Couldn't open %s\n", readpath);
	}
}

void refresh()
{
	folderScan(lastDir);
	current = 1;
	dirBrowse(lastDir);
}

int sceIoCopyFile(const char * srcfile, const char * destfile, int * overwrite) {
	int fdsrc, fddest, bytes_readed = -1;
	char buffer[1024*3];
	int bufsize = sizeof(buffer);
 
	if(fileExists(srcfile) == 0) {
		return 0;
	}
	if((fileExists(destfile) == 1) && (overwrite == 0)) {
		return 0;
	}
 
	fdsrc = sceIoOpen(srcfile, PSP_O_RDONLY, 0777); // Opens the source file for read access permission
	if(fdsrc < 0) return 0; // If 'fdsrc' is not a valid file descriptor, then return 0 and exit function
 
	fddest = sceIoOpen(destfile, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777); // Opens the dest file for write access permission. Creates it if it doesn't exists
	if(fddest < 0) { sceIoClose(fdsrc); return 0; } // If 'fddest' is not a valid file descriptor, then closes the opened file descriptor of the source file and return 0 for exit the function
 
	bytes_readed = sceIoRead(fdsrc, buffer, bufsize);
	while(bytes_readed > 0) {
		sceIoWrite(fddest, buffer, bytes_readed);
		bytes_readed = sceIoRead(fdsrc, buffer, bufsize);
	}
	sceIoClose(fddest);
	sceIoClose(fdsrc);
	return 1;
}
 
int sceIoCopyDirectory(const char * sourcepath, const char * destpath, int * overwrite) {
	int dfdsrc, dfddest, bytes_readed = -1;
 
	dfdsrc = sceIoDopen(sourcepath); // Assign the directory descriptor to the int 'dfd_src'
	if(dfdsrc < 0) return 0; // If 'dfd_src' is not a valid directory descriptor, then returns 0 and exit function
 
	int check = dirExists(destpath); // Assign the directory check to the int 'check'
	if(check == 0) { // If the 'destpath' folder doesn't exists...
		sceIoMkdir(destpath, 0777); // Create a folder
	} else if(check == 1 && overwrite == 0) {
		return 0;
	} else if(check == 1 && overwrite == 1) {
		sceIoRmdir(destpath);
	}
 
	dfddest = sceIoDopen(destpath);
	if(dfddest < 0) {sceIoDclose(dfdsrc); return 0;} // If 'dfddest' is not a valid directory descriptor, then closes the opened directory descriptor of the source directory and return 0 for exit the function
 
	SceIoDirent ent;
	memset(&ent, 0, sizeof(SceIoDirent));
 
	bytes_readed = sceIoDread(dfdsrc, &ent);
 
	while(bytes_readed > 0) {
		sceIoWrite(dfddest, ent.d_private, bytes_readed);
		bytes_readed = sceIoDread(dfdsrc, &ent);
	}
 
	sceIoDclose(dfdsrc);
	sceIoDclose(dfddest);
 
	return 1;
 
}

 /*
// Copy File or Folder
void copy(int flag, File * path)
{	
	// Not found
	if(path == NULL) return;
	
	// Copy File Source
	strcpy(copysource, cwd);
	strcpy(copysource + strlen(copysource), folderIcons[current].name);
	
	// Add Recursive Folder Flag
	if(dirExists(path)) flag |= COPY_FOLDER_RECURSIVE;
	
	// Set Copy Flags
	copymode = flag;
} 

int paste(void)
{
	// No Copy Source
	if(copymode == NOTHING_TO_COPY) return -1;
	
	// Source and Target Folder are identical
	char * lastslash = NULL; int i = 0; for(; i < strlen(copysource); i++) if(copysource[i] == '/') lastslash = copysource + i;
	char backup = lastslash[1];
	lastslash[1] = 0;
	int identical = strcmp(copysource, cwd) == 0;
	lastslash[1] = backup;
	if(identical) return -2;
	
	// Source Filename
	char * filename = lastslash + 1;
	
	// Required Target Path Buffer Size
	int requiredlength = strlen(cwd) + strlen(filename) + 1;
	
	// Allocate Target Path Buffer
	char * copytarget = (char *)malloc(requiredlength);
	
	// Puzzle Target Path
	strcpy(copytarget, cwd);
	strcpy(copytarget + strlen(copytarget), filename);
	
	// Return Result
	int result = -3;
	
	// Recursive Folder Copy
	if((copymode & COPY_FOLDER_RECURSIVE) == COPY_FOLDER_RECURSIVE)
	{
		// Check Files in current Folder
		File * node = files; for(; node != NULL; node = node->next)
		{
			// Found a file matching the name (folder = ok, file = not)
			if(strcmp(filename, node->name) == 0 && !dirExists(node)) //
			{
				// Error out
				return -4;
			}
		}
		
		// Copy Folder recursively
		result = copy_folder_recursive(copysource, copytarget);
		
		// Source Delete
		if(result == 0 && (copymode & COPY_DELETE_ON_FINISH) == COPY_DELETE_ON_FINISH)
		{
			// Append Trailing Slash (for recursion to work)
			copysource[strlen(copysource) + 1] = 0;
			copysource[strlen(copysource)] = '/';
			
			// Delete Source
			delete_folder_recursive(copysource);
		}
	}
	
	// Simple File Copy
	else
	{
		// Copy File
		result = copy_file(copysource, copytarget);
		
		// Source Delete
		if(result == 0 && (copymode & COPY_DELETE_ON_FINISH) == COPY_DELETE_ON_FINISH)
		{
			// Delete File
			sceIoRemove(copysource);
		}
	}
	
	// Paste Success
	if(result == 0)
	{
		// Erase Cache Data
		memset(copysource, 0, sizeof(copysource));
		copymode = NOTHING_TO_COPY;
	}
	
	// Free Target Path Buffer
	free(copytarget);
	
	// Return Result
	return result;
}
*/

void OptionMenu()
{
	action = oslLoadImageFilePNG("system/app/filemanager/actions.png", OSL_IN_RAM, OSL_PF_8888);
	
	if (!action)
		debugDisplay();
	
	while (!osl_quit)
	{
		oslStartDrawing();	
		oslDrawImageXY(action, 98,47);
		oslDrawStringf(120,115,"Press X");
		oslDrawStringf(120,125,"to Copy");
		oslDrawStringf(260,115,"Press Triangle");
		oslDrawStringf(260,125,"to Cut");
	
		oslDrawStringf(120,160,"Press Square");
		oslDrawStringf(120,170,"to Delete");
		oslDrawStringf(260,160,"Press Circle");
		oslDrawStringf(260,170,"to Rename");
		oslDrawStringf(170,215,"Press Select to Cancel");
	
		oslReadKeys();
	
		if (osl_keys->pressed.cross) 
		{
			fcopy(folderIcons[current].filePath);
			oslDeleteImage(action);
			refresh();
		}
		
		else if (osl_keys->pressed.triangle) 
		{
			oslDeleteImage(action);
			refresh();
		}

		else if (osl_keys->pressed.square) 
		{
			DeleteFile(folderIcons[current].filePath);
		}
	
		else if (osl_keys->pressed.circle) 
		{
			oslDeleteImage(action);
			refresh();
		}
	
		else if (osl_keys->pressed.select) 
		{
			oslDeleteImage(action);
			return;
		}
	oslEndDrawing();
	oslSyncFrame();	
	}
}

void newFolder()
{
	int skip = 0;
    char newFolderName[250] = "";

	while (!osl_quit)
	{
		LowMemExit();
	
		if (!skip)
		{
			oslStartDrawing();

			if (oslOskIsActive())
			{
				oslDrawOsk();
				if (oslGetOskStatus() == PSP_UTILITY_DIALOG_NONE)
				{
					if (oslOskGetResult() == OSL_OSK_CANCEL)
					return;
					else
					{
						char userText[100] = "";
						oslOskGetText(userText);
						sprintf(newFolderName, "%s", userText);
						sceIoMkdir(newFolderName,0777);
									
			if (dirExists(newFolderName))
			{
				return;
			}
					}
						oslEndOsk();
				}
			}
			oslEndDrawing();
		}
		
		if (!oslOskIsActive())
		{
			oslReadKeys();
			if (osl_keys->pressed.circle)
			{
				return;
			}
			else
			{
				oslInitOsk("Type Name of Folder", "", 128, 1, -1);
				memset(newFolderName, 0, sizeof(newFolderName));
			}
		oslEndDrawing(); 
		oslEndFrame(); 
		skip = oslSyncFrame();
		}
	}
}

void DeleteFile(const char * path)
{
	deletion = oslLoadImageFilePNG("system/app/filemanager/deletion.png", OSL_IN_RAM, OSL_PF_8888);
	
	if (!deletion)
		debugDisplay();
	
	while (!osl_quit) 
	{
		oslStartDrawing();	
		oslDrawImageXY(deletion, 96,59);
		oslDrawStringf(116,125,"This action cannot be undone. Do you");
		oslDrawStringf(116,135,"want to continue?");
	
		oslDrawStringf(130,180,"Press circle");
		oslDrawStringf(130,190,"to cancel");
		oslDrawStringf(270,180,"Press cross");
		oslDrawStringf(270,190,"to confirm");
		
		oslReadKeys();
		
		if (osl_keys->pressed.cross) 
		{
			if (strcmp(folderIcons[current].fileType, "fld")==0)
			{
				DeleteRecursive(path);
				oslSyncFrame();
				sceKernelDelayThread(3*1000000);
				oslDeleteImage(deletion);
				refresh();
			}
			else
			{
				sceIoRemove(path);
				oslDeleteImage(deletion);
				refresh();
			}
		}
		
		if (osl_keys->pressed.circle) 
		{
			oslDeleteImage(deletion);
			return;
		}
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
}

int DeleteRecursive(char *path) //Thanks Moonchild!
{
  SceUID dfd;

   if ((dfd = sceIoDopen(path)) > 0)
   {
      SceIoDirent dir;

      memset(&dir, 0, sizeof(SceIoDirent));

      while (sceIoDread(dfd, &dir) > 0)
      {
         char filePath[512];

         if(FIO_S_ISDIR(dir.d_stat.st_mode))
         {
            if (dir.d_name[0] != '.')
            {
               sprintf(filePath, "%s%s/", path, dir.d_name);
               DeleteRecursive(filePath);
            }
         }
         else
         {
            SceUID fd;            

            strcpy(filePath, path);
            strcat(filePath, dir.d_name);
            
            //Do something with the files found here     
            sceIoRemove(filePath);
         }
      }
   }

  sceIoDclose(dfd);
  
  //Remove the directories too
  sceIoRmdir(path);
  
  return 1;
}

char * buffer;
long size;

void fcopy(char * source) 
{
	FILE * src;
	src = fopen(source,"r");
	fseek(src,0,SEEK_END);
	size = ftell(src);
	rewind(src);
	buffer = malloc(size);
	fread(buffer,1,size,src);
	fclose(src);
	
	if (osl_keys->pressed.square) 
	{
	fpaste(folderIcons[current].filePath);
	}
}

void fpaste(char * destination) 
{
	FILE * dst;

	dst = fopen(destination,"w");
	fwrite(buffer,1,size,dst);
	fclose(dst);
}

int checkTextFile(char *textfile)
{
	SceUID fd = sceIoOpen(textfile, PSP_O_RDONLY, 0777);
	
	if(fd < 0)
	   return -1;

	sceIoClose(fd);
	
	file = textfile;

	return 0;
}

char *getTextFromFile()
{
	char text_File[1000];

	memset(text_File, 0, sizeof(text_File));
	SceUID fd = sceIoOpen(file, PSP_O_RDONLY, 0777);
	
	sceIoRead(fd, text_File, 255);

	sceIoClose(fd);

	return text_File;
}

void displayTextFromFile()
{
	textview = oslLoadImageFilePNG("system/app/filemanager/textview.png", OSL_IN_RAM, OSL_PF_8888);
	
	if (!textview)
		debugDisplay();

	while (!osl_quit)
	{

		oslStartDrawing();	
		oslReadKeys();
		
		oslClearScreen(RGB(0,0,0));
		oslDrawImageXY(textview,0,19);
		
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,0);
		battery(370,2,1);
		digitaltime(420,4,458);	
		
		oslIntraFontSetStyle(Roboto, 0.5f,BLACK,0,0);
		if(checkTextFile(folderIcons[current].filePath) == -1)
			oslDrawStringf(40,33,"Unable to Open");
			
		oslDrawStringf(40,33,folderIcons[current].name);	
		oslDrawStringf(10,55," \n%s", getTextFromFile());	

		if(osl_keys->pressed.circle)
		{
			oslDeleteImage(textview);
			return;
		}	
				
	oslEndDrawing(); 
	oslEndFrame(); 
	oslSyncFrame();
	}
}
	
void centerText(int centerX, int centerY, char * centerText, int centerLength)
{
	if (strlen(centerText) <= centerLength) 
	{
		int center = ((centerX)-((strlen(centerText)/2)*8));
		oslDrawStringf(center, centerY, centerText);
	}
	else 
	{
		int center = ((centerX)-(centerLength/2)*8);
		char str[255];
		strncpy(str, centerText, centerLength);
		str[centerLength-3] = '.';
		str[centerLength-2] = '.';
		str[centerLength-1] = '.';
		str[centerLength] = '\0';
		oslDrawStringf(center, centerY, str);
	}
}

void dirVars()
{
	sprintf(lastDir, "ms0:/");
	sprintf(returnMe, "blah");
	returnMe[5] = '\0';
	current = 1;
	curScroll = 1;
	timer = 0;
}

void dirUp()
{
	current--; // Subtract a value from current so the ">" goes up
	if ((current <= curScroll-1) && (curScroll > 1)) {
		curScroll--; // To do with how it scrolls
	}
}

void dirDown()
{
	if (folderIcons[current+1].active) current++; // Add a value onto current so the ">" goes down
	if (current >= (MAX_DISPLAY+curScroll)) {
		curScroll++; // To do with how it scrolls
	}
}

void dirUpx5()
{
	current -= 5;  // Subtract a value from current so the ">" goes up
	if ((current <= curScroll-1) && (curScroll > 1)) {
		curScroll -= 5;  // To do with how it scrolls
	}
}

void dirDownx5()
{
	if (folderIcons[current+1].active) current += 5; // Add a value onto current so the ">" goes down
	if (current >= (MAX_DISPLAY+curScroll)) {
		curScroll += 5; // To do with how it scrolls
	}
}

void oslPrintText(int x, int y, float size, char * text, OSL_COLOR color) {
   oslIntraFontSetStyle(Roboto, size, color, RGBA(0,0,0,0), INTRAFONT_ALIGN_LEFT);
   oslDrawStringf(x,y,text);
}

void dirDisplay()
{	
	oslIntraFontSetStyle(Roboto, 0.5f,BLACK,0,0);

	oslDrawImageXY(filemanagerbg, 0, 0);
	oslDrawStringf(86, 30, lastDir,RGB(0,0,0)); // Displays the current directory.
	oslDrawImageXY(bar,0,(current - curScroll)*44+CURR_DISPLAY_Y);
	
	battery(370,2,1);
	digitaltime(420,4,458);

	// Displays the directories, while also incorporating the scrolling
	for(i=curScroll;i<MAX_DISPLAY+curScroll;i++) {
	
		char * ext = strrchr(dirScan[i].name, '.'); //For file extension.
	
		// Handles the cursor and the display to not move past the MAX_DISPLAY.
		// For moving down
		//if ((folderIcons[i].active == 0) && (current >= i-1)) {
	
		if ((folderIcons[i].active == 0) && (current >= i-1)) {
			current = i-1;
			break;
		}
		// For moving up
		if (current <= curScroll-1) {
			current = curScroll-1;
			break;
		}
		
		if (dirScan[i].directory == 0)
		{  
			oslDrawImageXY(unknownicon,45,(i - curScroll)*44+ICON_DISPLAY_Y);
		}
		
		if(((ext) != NULL) && ((strcmp(ext ,".mp3") == 0) || (strcmp(ext ,".mov") == 0) || (strcmp(ext ,".m4a") == 0) || (strcmp(ext ,".wav") == 0) || (strcmp(ext ,".ogg") == 0)) && (dirScan[i].directory == 0))
		{
			//Checks if the file is a music file.
			oslDrawImageXY(mp3icon,45,(i - curScroll)*44+ICON_DISPLAY_Y);
		}
		
		if(((ext) != NULL) && ((strcmp(ext ,".mp4") == 0) || (strcmp(ext ,".mpg") == 0) || (strcmp(ext ,".flv") == 0) || (strcmp(ext ,".mpeg") == 0)) && (dirScan[i].directory == 0)) 
		{
			//Checks if the file is a video.
			oslDrawImageXY(videoicon,45,(i - curScroll)*44+ICON_DISPLAY_Y);
		}
		
		if(((ext) != NULL) && ((strcmp(ext ,".png") == 0) || (strcmp(ext ,".jpg") == 0) || (strcmp(ext ,".jpeg") == 0) || (strcmp(ext ,".gif") == 0)) && (dirScan[i].directory == 0)) 
		{
			//Checks if the file is an image.
			oslDrawImageXY(imageicon,45,(i - curScroll)*44+ICON_DISPLAY_Y);
		}
		
		if(((ext) != NULL) && ((strcmp(ext ,".PBP") == 0) || (strcmp(ext ,".prx") == 0) || (strcmp(ext ,".PRX") == 0) || (strcmp(ext ,".elf") == 0)) && (dirScan[i].directory == 0)) 
		{
			//Checks if the file is a binary file.
			oslDrawImageXY(binaryicon,45,(i - curScroll)*44+ICON_DISPLAY_Y);
		}
		
		if(((ext) != NULL) && ((strcmp(ext ,".txt") == 0) || (strcmp(ext ,".TXT") == 0) || (strcmp(ext ,".log") == 0) || (strcmp(ext ,".prop") == 0) || (strcmp(ext ,".lua") == 0)) && (dirScan[i].directory == 0))
		{
			//Checks if the file is a text document.
			oslDrawImageXY(txticon,45,(i - curScroll)*44+ICON_DISPLAY_Y);
		}
		
		if(((ext) != NULL) && ((strcmp(ext ,".doc") == 0) || (strcmp(ext ,".docx") == 0) || (strcmp(ext ,".pdf") == 0) || (strcmp(ext ,".ppt") == 0)) && (dirScan[i].directory == 0)) 
		{
			//Checks if the file is a document.
			oslDrawImageXY(documenticon,45,(i - curScroll)*44+ICON_DISPLAY_Y);
		}
		
		if(((ext) != NULL) && ((strcmp(ext ,".rar") == 0) || (strcmp(ext ,".zip") == 0) || (strcmp(ext ,".7z") == 0)) && (dirScan[i].directory == 0)) 
		{
			//Checks if the file is an archive.
			oslDrawImageXY(archiveicon,45,(i - curScroll)*44+ICON_DISPLAY_Y);
		}
		
		if (dirScan[i].directory == 1 && (!dirScan[i].directory == 0))
		{      
			// if it's a directory
			oslDrawImageXY(diricon,45,(i - curScroll)*44+ICON_DISPLAY_Y);
		}
		
		// If the currently selected item is active, then display the name
		if (folderIcons[i].active == 1) {
			
			oslDrawStringf(DISPLAY_X, (i - curScroll)*44+DISPLAY_Y, folderIcons[i].name);	// change the X & Y value accordingly if you want to move it (for Y, just change the +10)		
		}
	}
}

void dirControls() //Controls
{
	oslReadKeys();	

	if (pad.Buttons != oldpad.Buttons) {
	
		if ((pad.Buttons & PSP_CTRL_DOWN) && (!(oldpad.Buttons & PSP_CTRL_DOWN))) {
			dirDown();
			timer = 0;
		}
		else if ((pad.Buttons & PSP_CTRL_UP) && (!(oldpad.Buttons & PSP_CTRL_UP))) {
			dirUp();
			timer = 0;
		}
		if ((pad.Buttons & PSP_CTRL_RIGHT) && (!(oldpad.Buttons & PSP_CTRL_RIGHT))) {
			dirDownx5();
			timer = 0;
		}
		else if ((pad.Buttons & PSP_CTRL_LEFT) && (!(oldpad.Buttons & PSP_CTRL_LEFT))) {
			dirUpx5();
			timer = 0;
		}
		if ((pad.Buttons & PSP_CTRL_CROSS) && (!(oldpad.Buttons & PSP_CTRL_CROSS))) {
			runFile(folderIcons[current].filePath, folderIcons[current].fileType);
		}
		if ((pad.Buttons & PSP_CTRL_TRIANGLE) && (!(oldpad.Buttons & PSP_CTRL_TRIANGLE))) {
			if (!(stricmp(lastDir, "ms0:")==0) || (stricmp(lastDir, "ms0:/")==0)) {
				curScroll = 1;
				current = 1;
			}
		}
	}
	
	char * ext = strrchr(folderIcons[current].filePath, '.'); 
	
	if (osl_keys->pressed.select)
	{
			OptionMenu();
	}
	
	if (osl_keys->pressed.circle)
	{		
		if((!strcmp("ms0:/", lastDir)) || (!strcmp("ms0:", lastDir))) //If pressed circle in root folder
		{
			filemanager_unload();
			appdrawer();
		}
	
		else
		{
			dirBack();
		}		
	}
	
	/*
	if (osl_keys->pressed.R)
	{		
		newFolder();
	}
	*/
	
	if (((ext) != NULL) && ((strcmp(ext ,".png") == 0) || (strcmp(ext ,".jpg") == 0) || (strcmp(ext ,".jpeg") == 0) || (strcmp(ext ,".gif") == 0) || (strcmp(ext ,".PNG") == 0) || (strcmp(ext ,".JPG") == 0) || (strcmp(ext ,".JPEG") == 0) || (strcmp(ext ,".GIF") == 0)) && (osl_keys->pressed.cross))
	{
		showImage(folderIcons[current].filePath);
	}
	
	if (((ext) != NULL) && ((strcmp(ext ,".ZIP") == 0) || (strcmp(ext ,".zip") == 0)) && (osl_keys->pressed.cross))
	{
		pgeZip* zipFiles = pgeZipOpen(folderIcons[current].filePath);
		chdir("..");
		pgeZipExtract(zipFiles, NULL);
		pgeZipClose(zipFiles);
		oslSyncFrame();
		sceKernelDelayThread(3*1000000);
		refresh();
	}
	
	if (((ext) != NULL) && ((strcmp(ext ,".PBP") == 0) || (strcmp(ext ,".pbp") == 0)) && (osl_keys->pressed.cross))
	{
		if (kuKernelGetModel() == 4)
			launchEbootEf0(folderIcons[current].filePath);
		else
			launchEbootMs0(folderIcons[current].filePath);
	}
	
	if (((ext) != NULL) && ((strcmp(ext ,".mp3") == 0) || ((strcmp(ext ,".MP3") == 0))) && (osl_keys->pressed.cross))
	{
		MP3Play(folderIcons[current].filePath);
	}
	
	if (((ext) != NULL) && ((strcmp(ext ,".wav") == 0) || ((strcmp(ext ,".WAV") == 0)) || ((strcmp(ext ,".BGM") == 0)) || ((strcmp(ext ,".bgm") == 0)) || ((strcmp(ext ,".MOD") == 0)) || ((strcmp(ext ,".mod") == 0)) || ((strcmp(ext ,".AT3") == 0)) || ((strcmp(ext ,".at3") == 0))) && (osl_keys->pressed.cross))
	{
		soundPlay(folderIcons[current].filePath);
	}
	
	if (((ext) != NULL) && ((strcmp(ext ,".txt") == 0) || ((strcmp(ext ,".TXT") == 0)) || ((strcmp(ext ,".c") == 0)) || ((strcmp(ext ,".h") == 0)) || ((strcmp(ext ,".cpp") == 0))) && (osl_keys->pressed.cross))
	{
		displayTextFromFile(folderIcons[current].filePath);
	}
	
	if (osl_keys->pressed.square)
	{
		powermenu();
	}
		
	if (osl_keys->pressed.L)
	{
		lockscreen();
	}
	
	if (osl_pad.held.R && osl_keys->pressed.triangle)
	{
		screenshot();
	}	
	
	timer++;
	if ((timer > 30) && (pad.Buttons & PSP_CTRL_UP)) {
		dirUp();
		timer = 25;
	} else if ((timer > 30) && (pad.Buttons & PSP_CTRL_DOWN)) {
		dirDown();
		timer = 25;
	}

	if (current < 1) current = 1; // Stop the ">" from moving past the minimum files
	if (current > MAX_FILES) current = MAX_FILES; // Stop the ">" from moving past the max files
}

void dirBack()
{
	int a = 0;
	int b = 0;
	if (strlen(lastDir) > strlen("ms0:/")) {
		for (a=strlen(lastDir);a>=0;a--) {
			if (lastDir[a] == '/') {
				b++;
			}
			lastDir[a] = '\0';
			if (b == 1) {
				break;
			}
		}
		folderScan(lastDir);
	} 
}

// Just call 'path' with whichever path you want it to start off in
char * dirBrowse(const char * path)
{
	folderScan(path);
	dirVars();
	
	while (!osl_quit)
	{		
		LowMemExit();
	
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));	
		oldpad = pad;
		sceCtrlReadBufferPositive(&pad, 1);
		dirDisplay();
		dirControls();
		
		sceDisplayWaitVblankStart();
		
		if (strlen(returnMe) > 4) {
			break;
		}
		oslEndDrawing(); 
        oslEndFrame(); 
		oslSyncFrame();
	}
	return returnMe;
}

void filemanager_unload()
{
	oslDeleteImage(filemanagerbg);
	oslDeleteImage(diricon);
	oslDeleteImage(imageicon);
	oslDeleteImage(mp3icon);
	oslDeleteImage(txticon);
	oslDeleteImage(unknownicon);
	oslDeleteImage(bar);
	oslDeleteImage(documenticon);
	oslDeleteImage(binaryicon);
	oslDeleteImage(videoicon);
	oslDeleteImage(archiveicon);
	oslDeleteFont(Roboto);
}

int filemanage(int argc, char *argv[])
{
	filemanagerbg = oslLoadImageFilePNG("system/app/filemanager/filemanagerbg.png", OSL_IN_RAM, OSL_PF_8888);
	diricon = oslLoadImageFilePNG("system/app/filemanager/dir.png", OSL_IN_RAM, OSL_PF_8888);
	imageicon = oslLoadImageFilePNG("system/app/filemanager/image.png", OSL_IN_RAM, OSL_PF_8888);
	mp3icon = oslLoadImageFilePNG("system/app/filemanager/mp3.png", OSL_IN_RAM, OSL_PF_8888);
	txticon = oslLoadImageFilePNG("system/app/filemanager/txt.png", OSL_IN_RAM, OSL_PF_8888);
	unknownicon = oslLoadImageFilePNG("system/app/filemanager/unknownfile.png", OSL_IN_RAM, OSL_PF_8888);
	bar = oslLoadImageFilePNG("system/app/filemanager/bar.png", OSL_IN_RAM, OSL_PF_8888);
	documenticon = oslLoadImageFilePNG("system/app/filemanager/documenticon.png", OSL_IN_RAM, OSL_PF_8888);
	binaryicon = oslLoadImageFilePNG("system/app/filemanager/binaryicon.png", OSL_IN_RAM, OSL_PF_8888);
	videoicon = oslLoadImageFilePNG("system/app/filemanager/videoicon.png", OSL_IN_RAM, OSL_PF_8888);
	archiveicon = oslLoadImageFilePNG("system/app/filemanager/archiveicon.png", OSL_IN_RAM, OSL_PF_8888);
	
	Roboto = oslLoadFontFile("system/fonts/Roboto.pgf");
	oslIntraFontSetStyle(Roboto, 0.5, RGBA(0,0,0,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_LEFT);
	oslSetFont(Roboto);

	if (!filemanagerbg || !diricon || !imageicon || !mp3icon || !txticon || !unknownicon || !bar || !documenticon || !binaryicon || !videoicon || !archiveicon)
		debugDisplay();

	char * testDirectory = dirBrowse("ms0:");
	
	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));

		centerText(480/2, 272/2, testDirectory, 50);	// Shows the path that 'testDirectory' was supposed to receive from dirBrowse();
		
		oslEndDrawing(); 
        oslEndFrame(); 
		oslSyncFrame();
	}
}



