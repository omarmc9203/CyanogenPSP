#include "homeMenu.h"
#include "gameLauncher.h"
#include "appDrawer.h"
#include "musicPlayer.h"
#include "fileManager.h"
#include "clock.h"
#include "gallery.h"
#include "lockScreen.h"
#include "recentsMenu.h"
#include "powerMenu.h"
#include "include/pgeZip.h"
#include "settingsMenu.h"
#include "screenshot.h"
#include "include/utils.h"

static unsigned copy_progress;

int copy_bytes(SceUID source, SceUID destination, unsigned bytes)
{
	char buffer[0x8000];
	
	while(bytes)
	{
		//calculate the read/write size
		int copy_size = bytes > sizeof(buffer)? sizeof(buffer): bytes;
		
		//read bytes to buffer
		if(sceIoRead(source, buffer, copy_size) < copy_size)
			break;
		
		//write bytes to file
		if(sceIoWrite(destination, buffer, copy_size) < copy_size)
			break;
		
		bytes -= copy_size;
		copy_progress += copy_size;
	};
	
	return bytes == 0;
}

int copy_folder( char * source,  char * destination)
{
	if(!strcmp(source, destination) || !strcmp(destination, "ms0:/PSP/GAME/"))
		return 0;
	
	if(!strncmp(source, destination, strlen(source))) //avoid inception
		return 0;
	
	//get a pointer to the folder name
	char * new_folder = strrchr(source, '/') - 1;
	while(* new_folder != '/')
		new_folder--;
	new_folder++;

	size_t len = strlen(destination);
	
	char * new_destination = malloc(len + 256);
	
	//create destination folder if it doesnt exist
	sceIoMkdir(destination,0777);
	
	//create new dir on destination
	strcpy(new_destination, destination);
	strcat(new_destination, new_folder);
	
	int ret = copy_folder_recursive(source, new_destination);
	
	free(new_destination);
	
	return ret;
}

int copymode = NOTHING_TO_COPY;

static char copysource[1024];
char oldLocation[250];
char newLocation[250];
	
int copyData = 0;
int position = 0;
int filecount = 0;

// File List
File * findindex(int index);
File * files = NULL;

File * findindex(int index)
{
	// File Iterator Variable
	int i = 0;
	
	// Find File Item
	File * file = files; for(; file != NULL && i != index; file = file->next) i++;
	
	// Return File
	return file;
}

// Copy File or Folder
void copy(int flag)
{
	// Find File
	File * file = findindex(position);
	
	// Not found
	if(file == NULL) return;
	
	// Copy File Source
	strcpy(copysource, cwd);
	strcpy(copysource + strlen(copysource), file->name);
	
	// Add Recursive Folder Flag
	if(file->isFolder) flag |= COPY_FOLDER_RECURSIVE;
	
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
			if(strcmp(filename, node->name) == 0 && !node->isFolder)
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
			deleteRecursive(copysource);
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

// Copy File from A to B
int copy_file(char * a, char * b)
{
	// Chunk Size
	int chunksize = 512 * 1024;
	
	// Reading Buffer
	char * buffer = (char *)malloc(chunksize);
	
	// Accumulated Writing
	int totalwrite = 0;
	
	// Accumulated Reading
	int totalread = 0;
	
	// Result
	int result = 0;
	
	// Open File for Reading
	int in = sceIoOpen(a, PSP_O_RDONLY, 0777);
	
	// Opened File for Reading
	if(in >= 0)
	{
		// Delete Output File (if existing)
		sceIoRemove(b);
		
		// Open File for Writing
		int out = sceIoOpen(b, PSP_O_WRONLY | PSP_O_CREAT, 0777);
		
		// Opened File for Writing
		if(out >= 0)
		{
			// Read Byte Count
			int read = 0;
			
			// Copy Loop (512KB at a time)
			while((read = sceIoRead(in, buffer, chunksize)) > 0)
			{
				// Accumulate Read Data
				totalread += read;
				
				// Write Data
				totalwrite += sceIoWrite(out, buffer, read);
			}
			
			// Close Output File
			sceIoClose(out);
			
			// Insufficient Copy
			if(totalread != totalwrite) result = -3;
		}
		
		// Output Open Error
		else result = -2;
		
		// Close Input File
		sceIoClose(in);
	}
	
	// Input Open Error
	else result = -1;
	
	// Free Memory
	free(buffer);
	
	// Return Result
	return result;
}

// Copy Folder from A to B
int copy_folder_recursive(char * a, char * b)
{
	// Open Working Directory
	int directory = sceIoDopen(a);
	
	// Opened Directory
	if(directory >= 0)
	{
		// Create Output Directory (is allowed to fail, we can merge folders after all)
		sceIoMkdir(b, 0777);
		
		// File Info Read Result
		int dreadresult = 1;
		
		// Iterate Files
		while(dreadresult > 0)
		{
			// File Info
			SceIoDirent info;
			
			// Clear Memory
			memset(&info, 0, sizeof(info));
			
			// Read File Data
			dreadresult = sceIoDread(directory, &info);
			
			// Read Success
			if(dreadresult >= 0)
			{
				// Valid Filename
				if(strlen(info.d_name) > 0)
				{
					// Calculate Buffer Size
					int insize = strlen(a) + strlen(info.d_name) + 2;
					int outsize = strlen(b) + strlen(info.d_name) + 2;
					
					// Allocate Buffer
					char * inbuffer = (char *)malloc(insize);
					char * outbuffer = (char *)malloc(outsize);
					
					// Puzzle Input Path
					strcpy(inbuffer, a);
					inbuffer[strlen(inbuffer) + 1] = 0;
					inbuffer[strlen(inbuffer)] = '/';
					strcpy(inbuffer + strlen(inbuffer), info.d_name);
					
					// Puzzle Output Path
					strcpy(outbuffer, b);
					outbuffer[strlen(outbuffer) + 1] = 0;
					outbuffer[strlen(outbuffer)] = '/';
					strcpy(outbuffer + strlen(outbuffer), info.d_name);
					
					// Another Folder
					if(FIO_S_ISDIR(info.d_stat.st_mode))
					{
						// Copy Folder (via recursion)
						copy_folder_recursive(inbuffer, outbuffer);
					}
					
					// Simple File
					else
					{
						// Copy File
						copy_file(inbuffer, outbuffer);
					}
					
					// Free Buffer
					free(inbuffer);
					free(outbuffer);
				}
			}
		}
		
		// Close Directory
		sceIoDclose(directory);
		
		// Return Success
		return 0;
	}
	
	// Open Error
	else return -1;
}

int fileExists(const char* path)
{
	SceUID dir = sceIoOpen(path, PSP_O_RDONLY, 0777);
	if (dir >= 0)
	{
		sceIoClose(dir);
		return 1;
	}
	else
	{
		return 0;
	}
}

int dirExists(const char* path)
{
	SceUID dir = sceIoDopen(path);
	if (dir >= 0)
	{
		sceIoDclose(dir);
		return 1;
	}
	else 
	{
		return 0;
	}
}

long getFileSize(const char * fileName) 
{ 
	struct stat file; 
    if(!stat(fileName, &file)) 
	{ 
		return file.st_size; 
    } 
    return 0; 
} 

SceUID dirId;
int dirStatus = 1;
char * fileName;

int IsNextDir()
{
	return FIO_S_ISDIR(g_dir.d_stat.st_mode);
}

char* GetNextFileName()
{
	if (dirStatus > 0)
	{
		if(dirStatus >= 0)strcpy(fileName, g_dir.d_name);
	}
	return fileName;
}

int ChangeDir(const char* path)
{
	return sceIoChdir(path);
}

int folderScan(const char* path )
{
	memset(&g_dir, 0, sizeof(SceIoDirent));

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
	
	if (fd) 
	{
		if (!(stricmp(path, "ms0:")==0 || (stricmp(path, "ms0:/")==0))) 
		{
			sceIoDread(fd, &g_dir);		// get rid of '.' and '..'
			sceIoDread(fd, &g_dir);

			// Create our own '..'
			folderIcons[1].active = 1; 
			sprintf(folderIcons[1].filePath, "doesn't matter");
			sprintf(folderIcons[1].name, "..");
			sprintf(folderIcons[1].fileType, "dotdot");

			x = 2;
		} 
		else 
		{
			x = 1;
		}
		while ( sceIoDread(fd, &g_dir) && i<=MAX_FILES ) 
		{
			sprintf( dirScan[i].name, g_dir.d_name );
			sprintf( dirScan[i].path, "%s/%s", path, dirScan[i].name );

			//get rid of . and .. entries
			if ((!strcmp(".", g_dir.d_name)) || (!strcmp("..", g_dir.d_name)))  
			{
				memset(&g_dir, 0, sizeof(SceIoDirent));
				continue;
			}

			if (g_dir.d_stat.st_attr & FIO_SO_IFDIR) 
			{
				dirScan[i].directory = 1;
				dirScan[i].exist = 1;
			} 
			else 
			{
			
				dirScan[i].directory = 0;
				dirScan[i].exist = 1;
			}

			dirScan[i].size = g_dir.d_stat.st_size;
			i++;
		}
	}

	sceIoDclose(fd);

	for (i=1; i<MAX_FILES; i++) 
	{
		if (dirScan[i].exist == 0) break;
		folderIcons[x].active = 1;
		sprintf(folderIcons[x].filePath, dirScan[i].path);
		sprintf(folderIcons[x].name, dirScan[i].name);

		char *suffix = strrchr(dirScan[i].name, '.');
		
		if (dirScan[i].directory == 1) 
		{      // if it's a directory
			sprintf(folderIcons[x].fileType, "fld");
		} 
		else if ((dirScan[i].directory == 0) && (suffix)) 
		{		// if it's not a directory
			sprintf(folderIcons[x].fileType, "none");
		}
		else if (!(suffix)) 
		{
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
	if (strcmp(type, "dotdot")==0)
	{
		current = 1;
		dirBack(0);
	}		
	return 1;
}

int openDir(const char* path, char* type)
{
	if (strcmp(type, "fld")==0)
	{
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
		if (copyData == 1)
		{
			oslDrawStringf(120,125,"to Paste");
		}
		else
		{
			oslDrawStringf(120,125,"to Copy");
		}
		oslDrawStringf(260,115,"Press Triangle");
		oslDrawStringf(260,125,"to Cut");
	
		oslDrawStringf(120,160,"Press Square");
		oslDrawStringf(120,170,"to Delete");
		oslDrawStringf(260,160,"Press Circle");
		oslDrawStringf(260,170,"to Rename");
		oslDrawStringf(170,215,"Press Select to Cancel");
	
		oslReadKeys();
	
		if(osl_keys->pressed.cross) 
		{
			if(copyData == 0)
			{
				strcpy(oldLocation, folderIcons[current].filePath); 
				copyData = 1;
				oslPlaySound(KeypressStandard, 1);  
				oslDeleteImage(action);
				refresh();
			}
			if (copyData == 1)
			{
				copy_folder(oldLocation, folderIcons[current].filePath);
				copyData = 0;
				oslPlaySound(KeypressStandard, 1);  
				oslDeleteImage(action);
				refresh();
			}
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
			renameFile();
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

void renameFile()
{
	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();
		
		openOSK("Enter File Name", folderIcons[current].filePath, 250, -1);
		
		sceIoRename(folderIcons[current].filePath, tempData);
		
		refresh();
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();
	}
}

void newFolder()
{
	char newDir[250] = "";

	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();
		
		openOSK("Enter Folder Name", "", 250, -1);
		
		strcpy(newDir, lastDir); //Copy current directory to newDir
        strcat(newDir, "/"); //Add a '/' to newDir to denote the file path
        strcat(newDir, tempData); //Add data returned from the OSK to newDir
		
		if (dirExists(newDir))
		{
			return;
		}
		
		sceIoMkdir(newDir,0777); //Create the new dir in the current directory.
		
		refresh();
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();
	}
}

void DeleteFile(char path[])
{
	deletion = oslLoadImageFilePNG("system/app/filemanager/deletion.png", OSL_IN_RAM, OSL_PF_8888);
	
	if (!deletion)
		debugDisplay();
	
	while (!osl_quit) 
	{
		oslStartDrawing();	
		oslDrawImageXY(deletion, 84,48);
		oslDrawStringf(116,125,"This action cannot be undone. Do you");
		oslDrawStringf(116,135,"want to continue?");
	
		oslDrawStringf(130,190,"Press circle");
		oslDrawStringf(130,200,"to cancel");
		oslDrawStringf(270,190,"Press cross");
		oslDrawStringf(270,200,"to confirm");
		
		oslReadKeys();
		
		if (osl_keys->pressed.cross) 
		{
			oslPlaySound(KeypressStandard, 1);  
			if (strcmp(folderIcons[current].fileType, "fld")==0)
			{
				deleteRecursive(path);
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

int deleteRecursive(char path[]) //Thanks Moonchild!
{
  SceUID dfd;

   if ((dfd = sceIoDopen(path)) > 0)
   {
      memset(&g_dir, 0, sizeof(SceIoDirent));

      while (sceIoDread(dfd, &g_dir) > 0)
      {
         char filePath[512];

         if(FIO_S_ISDIR(g_dir.d_stat.st_mode))
         {
            if (g_dir.d_name[0] != '.')
            {
               sprintf(filePath, "%s%s/", path, g_dir.d_name);
               deleteRecursive(filePath);
            }
         }
         else
         {     
            strcpy(filePath, path);
            strcat(filePath, g_dir.d_name);
            
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

char * readTextFromFile(char *path)
{
	memset(read_buffer, 0, sizeof(read_buffer));
	SceUID fd = sceIoOpen(path, PSP_O_RDONLY, 0777);
	
	if(fd < 0)
	   return NULL;
	
	int endOfFile = sceIoLseek(fd, 0, PSP_SEEK_END);
	sceIoLseek(fd, 0, PSP_SEEK_SET);
	sceIoRead(fd, read_buffer, endOfFile);

	sceIoClose(fd);

	return read_buffer;
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
		oslDrawImageXY(textview,0,0);
		
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,0);
		battery(370,2,1);
		digitaltime(420,4,0,hrTime);	
		volumeController();
		
		oslIntraFontSetStyle(Roboto, 0.5f,BLACK,0,0);	
			
		oslDrawStringf(40,40, folderIcons[current].name);	
		oslDrawStringf(10,66," \n%s", readTextFromFile(folderIcons[current].filePath));

		if(osl_keys->pressed.circle)
		{
			free(read_buffer);
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

void dirDisplay()
{	
	oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,0);

	oslDrawImageXY(filemanagerbg, 0, 0);
	oslDrawStringf(86, 30, "%.34s",lastDir); // Displays the current directory.
	oslDrawImageXY(bar,0,(current - curScroll)*44+CURR_DISPLAY_Y);
	
	battery(370,2,1);
	digitaltime(420,4,0,hrTime);
	
	oslIntraFontSetStyle(Roboto, 0.5f,BLACK,0,0);

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
			
			oslDrawStringf(DISPLAY_X, (i - curScroll)*44+DISPLAY_Y, "%.52s", folderIcons[i].name);	// change the X & Y value accordingly if you want to move it (for Y, just change the +10)		
		}
	}
}

void dirControls() //Controls
{
	oslReadKeys();	

	if (pad.Buttons != oldpad.Buttons) 
	{
		if (osl_keys->pressed.down) 
		{
			dirDown();
			timer = 0;
		}
		else if (osl_keys->pressed.up) 
		{
			dirUp();
			timer = 0;
		}	
	
		if (osl_keys->pressed.right) 
		{
			dirDownx5();
			timer = 0;
		}
		else if (osl_keys->pressed.left) 
		{
			dirUpx5();
			timer = 0;
		}
		
		if (osl_keys->pressed.triangle) 
		{
			curScroll = 1;
			current = 1;
		}
	
		if (osl_keys->pressed.cross) 
		{
			oslPlaySound(KeypressStandard, 1);  
			runFile(folderIcons[current].filePath, folderIcons[current].fileType);
		}
	}
	
	volumeController();
	
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
			dirBack(0);
		}		
	}

	if (osl_keys->pressed.R)
	{		
		newFolder();
	}
	
	if (((ext) != NULL) && ((strcmp(ext ,".png") == 0) || (strcmp(ext ,".jpg") == 0) || (strcmp(ext ,".jpeg") == 0) || (strcmp(ext ,".gif") == 0) || (strcmp(ext ,".PNG") == 0) || (strcmp(ext ,".JPG") == 0) || (strcmp(ext ,".JPEG") == 0) || (strcmp(ext ,".GIF") == 0)) && (osl_keys->pressed.cross))
	{
		oslPlaySound(KeypressStandard, 1);  
		showImage(folderIcons[current].filePath, 0);
	}
	
	if (((ext) != NULL) && ((strcmp(ext ,".ZIP") == 0) || (strcmp(ext ,".zip") == 0)) && (osl_keys->pressed.cross))
	{
		oslPlaySound(KeypressStandard, 1);  
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
		oslPlaySound(KeypressStandard, 1);  
		if (kuKernelGetModel() == 4)
			launchEbootEf0(folderIcons[current].filePath);
		else
			launchEbootMs0(folderIcons[current].filePath);
	}
	
	if (((ext) != NULL) && ((strcmp(ext ,".mp3") == 0) || ((strcmp(ext ,".MP3") == 0))) && (osl_keys->pressed.cross))
	{
		oslPlaySound(KeypressStandard, 1);  
		MP3Play(folderIcons[current].filePath);
	}
	
	if (((ext) != NULL) && ((strcmp(ext ,".wav") == 0) || ((strcmp(ext ,".WAV") == 0)) || ((strcmp(ext ,".BGM") == 0)) || ((strcmp(ext ,".bgm") == 0)) || ((strcmp(ext ,".MOD") == 0)) || ((strcmp(ext ,".mod") == 0)) || ((strcmp(ext ,".AT3") == 0)) || ((strcmp(ext ,".at3") == 0))) && (osl_keys->pressed.cross))
	{
		oslPlaySound(KeypressStandard, 1);  
		soundPlay(folderIcons[current].filePath);
	}
	
	if (((ext) != NULL) && ((strcmp(ext ,".txt") == 0) || ((strcmp(ext ,".TXT") == 0)) || ((strcmp(ext ,".c") == 0)) || ((strcmp(ext ,".h") == 0)) || ((strcmp(ext ,".cpp") == 0)) || ((strcmp(ext ,".bin") == 0))) && (osl_keys->pressed.cross))
	{
		oslPlaySound(KeypressStandard, 1);  
		displayTextFromFile();
	}
	
	if (osl_keys->pressed.square)
	{
		powermenu();
	}
		
	if (osl_keys->pressed.L)
	{
		oslPlaySound(Lock, 1);  
		lockscreen();
	}
	
	captureScreenshot();
	
	timer++;
	if ((timer > 30) && (pad.Buttons & PSP_CTRL_UP))
	{
		dirUp();
		timer = 25;
	} 
	else if ((timer > 30) && (pad.Buttons & PSP_CTRL_DOWN))
	{
		dirDown();
		timer = 25;
	}

	if (current < 1) 
		current = 1;
	if (current > MAX_FILES) 
		current = MAX_FILES;
}

char* pspFileGetParentDirectory(const char *path)
{
  char *pos = strrchr(path, '/');

  if (!pos) return NULL;

  char *parent = (char*)malloc(sizeof(char) * (pos - path + 2));
  strncpy(parent, path, pos - path + 1);
  parent[pos - path + 1] = '\0';

  return parent;
}

void dirBack(int n)
{
	int a = 0;
	int b = 0;
	const char * str = NULL;
	
	switch(n)
	{
		case 0:
			str = "ms0:/";
			break;
	
		case 1:
			str = "ms0:/PSP/GAME/";
			break;
		
		case 2:
			str = "ms0:/ISO/";
			break;
		
		case 3:
			str = "ms0:/MUSIC/";
			break;
		
		case 4:
			str = "ms0:/PSP/MUSIC/";
			break;
		
		case 5:
			str = "ms0:/PSP/GAME/CyanogenPSP/downloads/";
			break;
		
		case 6:
			str = "ms0:/PICTURE";
			break;
	
		case 7:
			str = "ms0:/PSP/PHOTO";
			break;
		
		case 8:
			str = "ms0:/PSP/GAME/CyanogenMod/screenshots";
			break;
	}
	
	if (strlen(lastDir) > strlen(str)) 
	{
		for (a=strlen(lastDir);a>=0;a--) 
		{
			if (lastDir[a] == '/') 
			{
				b++;
			}
			lastDir[a] = '\0';
			if (b == 1) 
			{
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
		
		if (strlen(returnMe) > 4) 
			break;	
			
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
}

int filemanage(int argc, char *argv[])
{
	filemanagerbg = oslLoadImageFilePNG(fmBgPath, OSL_IN_RAM, OSL_PF_8888);
	diricon = oslLoadImageFilePNG(diriconPath, OSL_IN_RAM, OSL_PF_8888);
	imageicon = oslLoadImageFilePNG("system/app/filemanager/image.png", OSL_IN_RAM, OSL_PF_8888);
	mp3icon = oslLoadImageFilePNG("system/app/filemanager/mp3.png", OSL_IN_RAM, OSL_PF_8888);
	txticon = oslLoadImageFilePNG("system/app/filemanager/txt.png", OSL_IN_RAM, OSL_PF_8888);
	unknownicon = oslLoadImageFilePNG("system/app/filemanager/unknownfile.png", OSL_IN_RAM, OSL_PF_8888);
	bar = oslLoadImageFilePNG("system/app/filemanager/bar.png", OSL_IN_RAM, OSL_PF_8888);
	documenticon = oslLoadImageFilePNG("system/app/filemanager/documenticon.png", OSL_IN_RAM, OSL_PF_8888);
	binaryicon = oslLoadImageFilePNG("system/app/filemanager/binaryicon.png", OSL_IN_RAM, OSL_PF_8888);
	videoicon = oslLoadImageFilePNG("system/app/filemanager/videoicon.png", OSL_IN_RAM, OSL_PF_8888);
	archiveicon = oslLoadImageFilePNG("system/app/filemanager/archiveicon.png", OSL_IN_RAM, OSL_PF_8888);
	
	oslSetFont(Roboto);
	oslIntraFontSetStyle(Roboto, 0.5f,BLACK,0,0);

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
	return 0;
}