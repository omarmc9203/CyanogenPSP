#include "language.h"

/*
	The languages are in the followings order:
	1. English
	2. French
	3. Polish
	4. German
	5. Spanish
	6. Russian
*/

const char lang_welcome[][4][100] = 
{	
	{"Welcome", "Use the analog stick to navigate the cursor and", "press X (cross) to open the application.", "GOT IT"},
	{"Bienvenue", "Utilisez le stick analogique pour déplacer le curseur et", "pressez le bouton X (croix) pour ouvrir l'application.", "OK"},
	{"Witamy", "Użyj analoga by poruszać kursorem", "i przyciśnij X by otworzyć aplikację.", "Zrozumiałem"},
	{"Welcome", "Use the analog stick to navigate the cursor and", "press X (cross) to open the application.", "GOT IT"},
	{"Welcome", "Use the analog stick to navigate the cursor and", "press X (cross) to open the application.", "GOT IT"},
	{"Welcome", "Use the analog stick to navigate the cursor and", "press X (cross) to open the application.", "GOT IT"}
};


const char lang_appDrawer[][10][100] = 
{	
	{"Browser", "Calculator", "Clock", "Manager", "Gallery", "Game", "Messages", "Music", "Settings", "UMD"},
	{"Navigateur", "Calculatrice", "Horloge", "Explorateur", "Galerie", "Jeux", "Messages", "Musique", "Configurations", "UMD"},
	{"Przeglądarka", "Kalkulator", "Zegar", "Manager", "Galeria", "Gry", "Wiadomości", "Muzyka", "Ustawienia", "UMD"},
	{"Browser", "Rechner", "Uhr", "Forscher", "Galerie", " Spiele", "Nachrichten", "Musik", "Einstellungen", "UMD"},
	{"Navegador", "Calculadora", "Reloj", "Explorador", "galería", "Juegos", "Mensajes", "música", "Opciones", "UMD"},
	{"Ð±Ñ€Ð°ÑƒÐ·ÐµÑ€", "ÐºÐ°Ð»ÑŒÐºÑƒÐ»ÑÑ‚Ð¾Ñ€", "Ñ‡Ð°ÑÑ‹", "Ð¼ÐµÐ½ÐµÐ´Ð¶ÐµÑ€", " Ð³Ð°Ð»ÐµÑ€ÐµÑ", "Ð¸Ð³Ñ€Ð°", "ÑÐ¾Ð¾Ð±Ñ‰ÐµÐ½Ð¸Ñ", "Ð¼ÑƒÐ·Ñ‹ÐºÐ°", " Ð½Ð°ÑÑ‚Ñ€Ð¾Ð¹ÐºÐ¸", "UMD"}
};

const char lang_settingsMain[][6][100] = 
{	
	{"Wi-Fi", "Security", "Display", "Performance", "Developer Options", "About"},
	{"Wi-Fi", "Sécurité", "Affichage", "Performance", "Options de développeur", "À propos"},
	{"Wi-Fi", "Zabezpieczenia", "Wyświetlacz", "Wydajność", "Opcje developerskie", "O programie"},
	{"Wi-Fi", "Security", "Display", "Performance", "Developer Options", "About"},
	{"Wi-Fi", "Security", "Display", "Performance", "Developer Options", "About"},
	{"Wi-Fi", "Security", "Display", "Performance", "Developer Options", "About"}
};

const char lang_settingsDisplay[][3][100] = 
{	
	{"Themes", "Time", "Miscellaneous"},
	{"Thèmes", "Heure", "Divers"},
	{"Motywy", "Czas", "Różne"},
	{"Themes", "Time", "Miscellaneous"},
	{"Themes", "Time", "Miscellaneous"},
	{"Themes", "Time", "Miscellaneous"}
};

const char lang_settingsThemes[][4][100] = 
{	
	{"Style", "Wallpaper", "Fonts", "Icon Packs"},
	{"Style", "Fond d'écran", "Polices", "Icon Packs"},
	{"Wygląd", "Tapeta", "Czcionki", "Paczki ikon"},
	{"Style", "Wallpaper", "Fonts", "Icon Packs"},
	{"Style", "Wallpaper", "Fonts", "Icon Packs"},
	{"Style", "Wallpaper", "Fonts", "Icon Packs"}
};

const char lang_settingsTime[][1][100] = 
{	
	{"Use 24-hour format"},
	{"Utiliser le format 24 heures"},
	{"Użyj czasu 24-godzinnego"},
	{"Use 24-hour format"},
	{"Use 24-hour format"},
	{"Use 24-hour format"}
};

const char lang_settingsMiscellaneous[][4][100] = 
{	
	{"Toggle day/night widget", "Toggle expanded desktop", "Toggle boot animation", "Toggle game boot"},
	{"Basculer entre le widget jour/nuit", "Activer/Désactiver le bureau étendu", "Activer/Désactiver l'animation de démarrage", "Activer/Désactiver le l'animation de démarrage de jeu"},
	{"Użyj widgetu day/night", "Użyj pulpitu rozszerzonego", "Użyj animacji bootowania", "Użyj animacji uruchamiania gry"},
	{"Toggle day/night widget", "Toggle expanded desktop", "Toggle boot animation", "Toggle game boot"},
	{"Toggle day/night widget", "Toggle expanded desktop", "Toggle boot animation", "Toggle game boot"},
	{"Toggle day/night widget", "Toggle expanded desktop", "Toggle boot animation", "Toggle game boot"}
};

const char lang_settingsPerformance[][4][100] = 
{	
	{"Processor", "Memory", "Storage Management", "Battery Management"},
	{"Processeur", "Memory", "Gestion de l'espace de stockage", "Battery Management"},
	{"Procesor", "Pamięć", "Zarządzanie pamięcią", "Zarządzanie baterią"},
	{"Processor", "Memory", "Storage Management", "Battery Management"},
	{"Processor", "Memory", "Storage Management", "Battery Management"},
	{"Processor", "Memory", "Storage Management", "Battery Management"}
};

const char lang_settingsProcessor[][5][100] = 
{	
	{"Current CPU frequency", "CPU over-clock", "Press R to increase frequency and L to decrease frequency.", "Current BUS frequency"},
	{"Fréquence processeur actuelle", "Surcadencement du processeur", "Pressez la touche R pour augmenter la fréquence, et L pour la diminuer.", "Fréquence BUS actuelle"},
	{"Częstotliwość CPU", "Podkręcanie CPU", "Naduś R by zwiększyć, a L by zmniejszyć częstotliwość", "Częstotliwość magistarli"},
	{"Current CPU frequency", "CPU over-clock", "Press R to increase frequency and L to decrease frequency.", "Current BUS frequency"},
	{"Current CPU frequency", "CPU over-clock", "Press R to increase frequency and L to decrease frequency.", "Current BUS frequency"},
	{"Current CPU frequency", "CPU over-clock", "Press R to increase frequency and L to decrease frequency.", "Current BUS frequency"}
};

const char lang_settingsRAM[][3][100] = 
{	
	{"RAM available:", "available", "of RAM used"},
	{"RAM disponible :", "disponibles", "of RAM used"},
	{"Dotępny RAM:", "wolne", "RAM-u użyte"},
	{"RAM available:", "available", "of RAM used"},
	{"RAM available:", "available", "of RAM used"},
	{"RAM available:", "available", "of RAM used"}
};

const char lang_settingsStorage[][3][60] = 
{	
	{"Press select to toggle USB mass storage", "Total storage capacity:", "Available storage capacity:"},
	{"Appuyez sur Select pour activer/désactiver le mode clé USB", "Capacité de stockage totale :", "Capacité de stockage disponible :"},
	{"Naciśnij Select by podłączyć pamięc USB", "Całkowita ilość pamięci", "Wolna pamieć"},
	{"Press select to toggle USB mass storage", "Total storage capacity:", "Available storage capacity:"},
	{"Press select to toggle USB mass storage", "Total storage capacity:", "Available storage capacity:"},
	{"Press select to toggle USB mass storage", "Total storage capacity:", "Available storage capacity:"}
};

const char lang_settingsBattery[][3][60] = 
{	
	{"Powersave", "Balanced", "Performance"},
	{"Powersave", "Balanced", "Performance"},
	{"Oszcz. baterii", "Zbalansowane", "Wydajność"},
	{"Powersave", "Balanced", "Performance"},
	{"Powersave", "Balanced", "Performance"},
	{"Powersave", "Balanced", "Performance"}
};

const char lang_settingsAbout[][7][100] = 
{	
	{"CyanogenPSP updates", "Click for, view or install available updates", "CyanogenPSP version:", "UNOFFICIAL", "Mac Address:", "Kernel version:", "OSLib version:"},
	{"Mises à jour de CyanogenPSP", "Cliquez pour voir et installer les mises à jour disponibles", "Version CyanogenPSP :", "UNOFFICIEL", "Adresse MAC :", "Version du noyau :", "Version de l'OSLib :"},
	{"Aktualizacje CyanogenPSP", "Naciśnij aby sprawdzić/zainstalować aktualizacje", "Wersja CyanogenPSP:", "UNOFFICIAL", "Adres Mac:", "Wersja jądra:", "Wersja OSLib:"},
	{"CyanogenPSP updates", "Click for, view or install available updates", "CyanogenPSP version:", "UNOFFICIAL", "Mac Address:", "Kernel version:", "OSLib version:"},
	{"CyanogenPSP updates", "Click for, view or install available updates", "CyanogenPSP version:", "UNOFFICIAL", "Mac Address:", "Kernel version:", "OSLib version:"},
	{"CyanogenPSP updates", "Click for, view or install available updates", "CyanogenPSP version:", "UNOFFICIAL", "Mac Address:", "Kernel version:", "OSLib version:"}
};

const char lang_settingsUpdates[][1][100] = 
{	
	{"Check for updates"},
	{"Vérifier la présence de mises à jour"},
	{"Sprawdź aktualizacje"},
	{"Check for updates"},
	{"Check for updates"},
	{"Check for updates"}
};

const char lang_settingsSecuirty[][2][100] = 
{	
	{"Password lock", "Pin lock"},
	{"Verrouillage par mot de passe", "Verrouillage par code PIN"},
	{"Zabezp. hasłem", "Zabezpieczenie PIN"},
	{"Password lock", "Pin lock"},
	{"Password lock", "Pin lock"},
	{"Password lock", "Pin lock"}
};

const char lang_settingsWifi[][3][100] = 
{	
	{"No WiFi configuration found", "Please turn on the Wlan switch", "Press up or down to navigate through your Wifi configurations"},
	{"Aucune configuration Wi-Fi n'a été trouvée", "Veuillez activer le WLAN à l'aide du commutateur", "Appuyez sur haut ou bas pour vous déplacer à travers vos configurations Wi-Fi"},
	{"Nie znaleziono konfig. Wi-Fi", "Przełącz włącznik WLAN", "Naciśnij w góre/w dół aby wybrać konfigurację"},
	{"No WiFi configuration found", "Please turn on the Wlan switch", "Press up or down to navigate through your Wifi configurations"},
	{"No WiFi configuration found", "Please turn on the Wlan switch", "Press up or down to navigate through your Wifi configurations"},
	{"No WiFi configuration found", "Please turn on the Wlan switch", "Press up or down to navigate through your Wifi configurations"}
};

const char lang_settingsDeveloperOptions[][5][100] = 
{	
	{"Toggle Remote Joy Lite", "Displays your PSP screen on your computer via USB", "Press triangle to disable or it may cause the program to crash", "Toggle USB debugging", "Dumping tools"},
	{"Activer/Désactiver Remote Joy Lite", "Affiche l'écran de votre PSP sur votre ordinateur par USB", "Appuyez sur triangle pour le désactiver, ou il risque de faire planter le programme", "Activer/Désactiver le débogage par USB'", "Dumping tools"},
	{"Włącz RemoteJoy Lite", "Wyświetla obraz z PSP na komputerze po USB", "Naciśnij trójkąt by wyłączyć, może powodować crashe", "Włącz debugowanie USB", "Narzędzia do zgrywania"},
	{"Toggle Remote Joy Lite", "Displays your PSP screen on your computer via USB", "Press triangle to disable or it may cause the program to crash", "Toggle USB debugging", "Dumping tools"},
	{"Toggle Remote Joy Lite", "Displays your PSP screen on your computer via USB", "Press triangle to disable or it may cause the program to crash", "Toggle USB debugging", "Dumping tools"},
	{"Toggle Remote Joy Lite", "Displays your PSP screen on your computer via USB", "Press triangle to disable or it may cause the program to crash", "Toggle USB debugging", "Dumping tools"}
};

const char lang_settingsDump[][9][100] = 
{	
	{"Dump Flash 0", "Dump Flash 1", "More", "Dump UMD boot.bin", "Dump Memory", "Insert UMD", "Mounted disc", "Found game", "Dumping 28Megs from"},
	{"Sauvegarder le Flash 0", "Sauvegarder le Flash 1", "Plus", "Sauvegarder le boot.bin de l'UMD", "Sauvegarder la mémoire", "Insérer un UMD", "Disque monté", "Jeu trouvé", "Sauvegarde de 28 Mo de"},
	{"Zgraj Flash 0", "Zgraj Flash 1", "Więcej", "Zgraj UMD boot.bin", "Zgraj Memory", "Włóż UMD", "Włożony dysk", "Znaleziono game", "Zgrywanie 28Mega z"},
	{"Dump Flash 0", "Dump Flash 1", "More", "Dump UMD boot.bin", "Dump Memory", "Insert UMD", "Mounted disc", "Found game", "Dumping 28Megs from"},
	{"Dump Flash 0", "Dump Flash 1", "More", "Dump UMD boot.bin", "Dump Memory", "Insert UMD", "Mounted disc", "Found game", "Dumping 28Megs from"},
	{"Dump Flash 0", "Dump Flash 1", "More", "Dump UMD boot.bin", "Dump Memory", "Insert UMD", "Mounted disc", "Found game", "Dumping 28Megs from"}
};

const char lang_quickSettings[][5][100] = 
{	
	{"Wi-Fi", "Power save", "Balanced", "Performance", "Lockscreen"},
	{"Wi-Fi", "Économie de batterie", "Équilibrée", "Performance", "Écran de verrouillage"},
	{"Wi-Fi", "Oszcz. energii", "Zbalansowane", "Wydajność", "Ekran blokady"},
	{"Wi-Fi", "Power save", "Balanced", "Performance", "Lockscreen"},
	{"Wi-Fi", "Power save", "Balanced", "Performance", "Lockscreen"},
	{"Wi-Fi", "Power save", "Balanced", "Performance", "Lockscreen"}
};
