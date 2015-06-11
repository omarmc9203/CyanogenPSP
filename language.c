#include "language.h"

/*
	The languages are in the followings order:
	English
	French
	German
	Spanish
	Russian
*/

const char lang_appDrawer[][10][25] = 
{	
	{"Browser", "Calculator", "Clock", "Manager", "Gallery", "Game", "Messages", "Music", "Settings", "UMD"},
	{"Navigateur", "Calculatrice", "Horloge", "Explorateur", "Galerie", "Jeux", "Messages", "Musique", "Configurations", "UMD"},
	{"Browser", "Rechner", "Uhr", "Forscher", "Galerie", " Spiele", "Nachrichten", "Musik", "Einstellungen", "UMD"},
	{"Navegador", "Calculadora", "Reloj", "Explorador", "galer�a", "Juegos", "Mensajes", "m�sica", "Opciones", "UMD"},
	{"браузер", "калькулятор", "часы", "менеджер", " галерея", "игра", "сообщения", "музыка", " настройки", "UMD"}
};

const char lang_settingsMain[][6][25] = 
{	
	{"Wi-Fi", "Security", "Display", "Performance", "Developer Options", "About"},
	{"Wi-Fi", "Security", "Display", "Performance", "Developer Options", "About"},
	{"Wi-Fi", "Security", "Display", "Performance", "Developer Options", "About"},
	{"Wi-Fi", "Security", "Display", "Performance", "Developer Options", "About"},
	{"Wi-Fi", "Security", "Display", "Performance", "Developer Options", "About"}
};

const char lang_settingsDisplay[][3][25] = 
{	
	{"Themes", "Time", "Miscellaneous"},
	{"Themes", "Time", "Miscellaneous"},
	{"Themes", "Time", "Miscellaneous"},
	{"Themes", "Time", "Miscellaneous"},
	{"Themes", "Time", "Miscellaneous"}
};

const char lang_settingsThemes[][2][20] = 
{	
	{"Wallpaper", "Fonts"},
	{"Wallpaper", "Fonts"},
	{"Wallpaper", "Fonts"},
	{"Wallpaper", "Fonts"},
	{"Wallpaper", "Fonts"}
};

const char lang_settingsTime[][1][20] = 
{	
	{"Use 24-hour format"},
	{"Use 24-hour format"},
	{"Use 24-hour format"},
	{"Use 24-hour format"},
	{"Use 24-hour format"}
};

const char lang_settingsMiscellaneous[][4][35] = 
{	
	{"Toggle day/night widget", "Toggle expanded desktop", "Toggle boot animation", "Toggle game boot"},
	{"Toggle day/night widget", "Toggle expanded desktop", "Toggle boot animation", "Toggle game boot"},
	{"Toggle day/night widget", "Toggle expanded desktop", "Toggle boot animation", "Toggle game boot"},
	{"Toggle day/night widget", "Toggle expanded desktop", "Toggle boot animation", "Toggle game boot"},
	{"Toggle day/night widget", "Toggle expanded desktop", "Toggle boot animation", "Toggle game boot"}
};

const char lang_settingsPerformance[][3][35] = 
{	
	{"Processor", "RAM Management", "Storage Management"},
	{"Processor", "RAM Management", "Storage Management"},
	{"Processor", "RAM Management", "Storage Management"},
	{"Processor", "RAM Management", "Storage Management"},
	{"Processor", "RAM Management", "Storage Management"}
};

const char lang_settingsProcessor[][5][80] = 
{	
	{"Current CPU frequency", "CPU over-clock", "Press R to increase frequency and L to decrease frequency.", "Current BUS frequency"},
	{"Current CPU frequency", "CPU over-clock", "Press R to increase frequency and L to decrease frequency.", "Current BUS frequency"},
	{"Current CPU frequency", "CPU over-clock", "Press R to increase frequency and L to decrease frequency.", "Current BUS frequency"},
	{"Current CPU frequency", "CPU over-clock", "Press R to increase frequency and L to decrease frequency.", "Current BUS frequency"},
	{"Current CPU frequency", "CPU over-clock", "Press R to increase frequency and L to decrease frequency.", "Current BUS frequency"}
};

const char lang_settingsRAM[][2][20] = 
{	
	{"RAM available:", "MB available"},
	{"RAM available:", "MB available"},
	{"RAM available:", "MB available"},
	{"RAM available:", "MB available"},
	{"RAM available:", "MB available"}
};

const char lang_settingsStorage[][3][60] = 
{	
	{"Press select to toggle USB mass storage", "Total storage capacity:", "Available storage capacity:"},
	{"Press select to toggle USB mass storage", "Total storage capacity:", "Available storage capacity:"},
	{"Press select to toggle USB mass storage", "Total storage capacity:", "Available storage capacity:"},
	{"Press select to toggle USB mass storage", "Total storage capacity:", "Available storage capacity:"},
	{"Press select to toggle USB mass storage", "Total storage capacity:", "Available storage capacity:"}
};

const char lang_settingsAbout[][7][80] = 
{	
	{"CyanogenPSP updates", "Click for, view or install available updates", "CyanogenPSP version:", "OFFICIAL", "Mac Address:", "Kernel version:", "OSLib version:"},
	{"CyanogenPSP updates", "Click for, view or install available updates", "CyanogenPSP version:", "OFFICIAL", "Mac Address:", "Kernel version:", "OSLib version:"},
	{"CyanogenPSP updates", "Click for, view or install available updates", "CyanogenPSP version:", "OFFICIAL", "Mac Address:", "Kernel version:", "OSLib version:"},
	{"CyanogenPSP updates", "Click for, view or install available updates", "CyanogenPSP version:", "OFFICIAL", "Mac Address:", "Kernel version:", "OSLib version:"},
	{"CyanogenPSP updates", "Click for, view or install available updates", "CyanogenPSP version:", "OFFICIAL", "Mac Address:", "Kernel version:", "OSLib version:"}
};

const char lang_settingsUpdates[][1][20] = 
{	
	{"Check for updates"},
	{"Check for updates"},
	{"Check for updates"},
	{"Check for updates"},
	{"Check for updates"}
};

const char lang_settingsSecuirty[][2][20] = 
{	
	{"Password lock", "Pin lock"},
	{"Password lock", "Pin lock"},
	{"Password lock", "Pin lock"},
	{"Password lock", "Pin lock"},
	{"Password lock", "Pin lock"}
};

const char lang_settingsWifi[][3][80] = 
{	
	{"No WiFi configuration found", "Please turn on the Wlan switch", "Press up or down to navigate through your Wifi configurations"},
	{"No WiFi configuration found", "Please turn on the Wlan switch", "Press up or down to navigate through your Wifi configurations"},
	{"No WiFi configuration found", "Please turn on the Wlan switch", "Press up or down to navigate through your Wifi configurations"},
	{"No WiFi configuration found", "Please turn on the Wlan switch", "Press up or down to navigate through your Wifi configurations"},
	{"No WiFi configuration found", "Please turn on the Wlan switch", "Press up or down to navigate through your Wifi configurations"}
};

const char lang_settingsDeveloperOptions[][5][80] = 
{	
	{"Toggle Remote Joy Lite", "Displays your PSP screen on your computer via USB", "Press triangle to disable or it may cause the program to crash", "Toggle USB debugging", "Dumping tools"},
	{"Toggle Remote Joy Lite", "Displays your PSP screen on your computer via USB", "Press triangle to disable or it may cause the program to crash", "Toggle USB debugging", "Dumping tools"},
	{"Toggle Remote Joy Lite", "Displays your PSP screen on your computer via USB", "Press triangle to disable or it may cause the program to crash", "Toggle USB debugging", "Dumping tools"},
	{"Toggle Remote Joy Lite", "Displays your PSP screen on your computer via USB", "Press triangle to disable or it may cause the program to crash", "Toggle USB debugging", "Dumping tools"},
	{"Toggle Remote Joy Lite", "Displays your PSP screen on your computer via USB", "Press triangle to disable or it may cause the program to crash", "Toggle USB debugging", "Dumping tools"}
};

const char lang_settingsDump[][9][20] = 
{	
	{"Dump Flash 0", "Dump Flash 1", "More", "Dump UMD boot.bin", "Dump Memory", "Insert UMD", "Mounted disc", "Found game", "Dumping 28Megs from"},
	{"Dump Flash 0", "Dump Flash 1", "More", "Dump UMD boot.bin", "Dump Memory", "Insert UMD", "Mounted disc", "Found game", "Dumping 28Megs from"},
	{"Dump Flash 0", "Dump Flash 1", "More", "Dump UMD boot.bin", "Dump Memory", "Insert UMD", "Mounted disc", "Found game", "Dumping 28Megs from"},
	{"Dump Flash 0", "Dump Flash 1", "More", "Dump UMD boot.bin", "Dump Memory", "Insert UMD", "Mounted disc", "Found game", "Dumping 28Megs from"},
	{"Dump Flash 0", "Dump Flash 1", "More", "Dump UMD boot.bin", "Dump Memory", "Insert UMD", "Mounted disc", "Found game", "Dumping 28Megs from"}
};

const char lang_quickSettings[][5][20] = 
{	
	{"Wi-Fi", "Power save", "Balanced", "Performance", "Lockscreen"},
	{"Wi-Fi", "Power save", "Balanced", "Performance", "Lockscreen"},
	{"Wi-Fi", "Power save", "Balanced", "Performance", "Lockscreen"},
	{"Wi-Fi", "Power save", "Balanced", "Performance", "Lockscreen"},
	{"Wi-Fi", "Power save", "Balanced", "Performance", "Lockscreen"}
};