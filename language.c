#include "language.h"

/*
	The languages are in the followings order:
	0. English
	1. French
	2. Polish
	3. German
	4. Dutch
	5. Spanish
	6. Russian
*/

const char lang_welcome[][4][100] = 
{	
	{"Welcome", "Use the analog stick to navigate the cursor and", "press X (cross) to open the application.", "GOT IT"},
	{"Bienvenue", "Utilisez le stick analogique pour déplacer le curseur et", "pressez le bouton X (croix) pour ouvrir l'application.", "OK"},
	{"Witamy", "Użyj analoga by poruszać kursorem", "i przyciśnij X by otworzyć aplikację.", "Zrozumiałem"},
	{"Willkommen", "Benutze den Analog-Stick um den Cursor zu navigieren und", "drücke X (Kreuz) um die Anwendung zu öffnen.", "VERSTANDEN"},
	{"Welkom", "Gebruik de joystick om de aanwijzer te bewegen en", "druk X (kruisje) om de toepassing te openen.", "GOT IT"},
	{"Welcome", "Use the analog stick to navigate the cursor and", "press X (cross) to open the application.", "GOT IT"},
	{"Welcome", "Use the analog stick to navigate the cursor and", "press X (cross) to open the application.", "GOT IT"}
};


const char lang_appDrawer[][10][100] = 
{	
	{"Browser", "Calculator", "Clock", "Manager", "Gallery", "Game", "Messages", "Music", "Settings", "UMD"},
	{"Navigateur", "Calculatrice", "Horloge", "Explorateur", "Galerie", "Jeux", "Messages", "Musique", "Configurations", "UMD"},
	{"Przeglądarka", "Kalkulator", "Zegar", "Manager", "Galeria", "Gry", "Wiadomości", "Muzyka", "Ustawienia", "UMD"},
	{"Browser", "Taschenrechner", "Uhr", "Manager", "Galerie", "Spiel", "Nachrichten", "Musik", "Einstellungen", "UMD"},
	{"Browser", "Rekenmachine", "Klok", " Beheerder", "Gallerij", "Spel", "Berichten", "Muziek", "Instellingen", "UMD"},
	{"Navegador", "Calculadora", "Reloj", "Explorador", "galería", "Juegos", "Mensajes", "música", "Opciones", "UMD"},
	{"Ð±Ñ€Ð°ÑƒÐ·ÐµÑ€", "ÐºÐ°Ð»ÑŒÐºÑƒÐ»ÑÑ‚Ð¾Ñ€", "Ñ‡Ð°ÑÑ‹", "Ð¼ÐµÐ½ÐµÐ´Ð¶ÐµÑ€", " Ð³Ð°Ð»ÐµÑ€ÐµÑ", "Ð¸Ð³Ñ€Ð°", "ÑÐ¾Ð¾Ð±Ñ‰ÐµÐ½Ð¸Ñ", "Ð¼ÑƒÐ·Ñ‹ÐºÐ°", " Ð½Ð°ÑÑ‚Ñ€Ð¾Ð¹ÐºÐ¸", "UMD"}
};

const char lang_settingsMain[][6][100] = 
{	
	{"Wi-Fi", "Security", "Display", "Performance", "Developer Options", "About"},
	{"Wi-Fi", "Sécurité", "Affichage", "Performance", "Options de développeur", "À propos"},
	{"Wi-Fi", "Zabezpieczenia", "Wyświetlacz", "Wydajność", "Opcje developerskie", "O programie"},
	{"Wi-Fi", "Sicherheit", "Bildschirm", "Leistung", "Entwickler Optionen", "Über"},
	{"Wi-Fi", "Beveiliging", "Scherm", "Prestaties", "Opties voor ontwikkelaars", "Over"},
	{"Wi-Fi", "Security", "Display", "Performance", "Developer Options", "About"},
	{"Wi-Fi", "Security", "Display", "Performance", "Developer Options", "About"}
};

const char lang_settingsDisplay[][3][100] = 
{	
	{"Themes", "Time", "Miscellaneous"},
	{"Thèmes", "Heure", "Divers"},
	{"Motywy", "Czas", "Różne"},
	{"Themen", "Uhrzeit", "Allgemein"},
	{"Thema's", "Tijd", "Diversen"},
	{"Themes", "Time", "Miscellaneous"},
	{"Themes", "Time", "Miscellaneous"}
};

const char lang_settingsThemes[][4][100] = 
{	
	{"Style", "Wallpaper", "Fonts", "Icon Packs"},
	{"Style", "Fond d'écran", "Polices", "Pack d'icones"},
	{"Wygląd", "Tapeta", "Czcionki", "Paczki ikon"},
	{"Designs", "Hintergründe", "Schriftarten", "Symbole"},
	{"Stijl", "Achtergrond", "Lettertypes", "Pictogrammen"},
	{"Style", "Wallpaper", "Fonts", "Icon Packs"},
	{"Style", "Wallpaper", "Fonts", "Icon Packs"}
};

const char lang_settingsTime[][1][100] = 
{	
	{"Use 24-hour format"},
	{"Utiliser le format 24 heures"},
	{"Użyj czasu 24-godzinnego"},
	{"24-Stunden format nutzen"},
	{"24-uurs klok gebruiken "},
	{"Use 24-hour format"},
	{"Use 24-hour format"}
};

const char lang_settingsMiscellaneous[][4][100] =
{	
	{"Toggle day/night widget", "Toggle expanded desktop", "Toggle boot animation", "Toggle game boot"},
	{"Basculer entre le widget jour/nuit", "Activer/Désactiver le bureau étendu", "Activer/Désactiver l'animation de démarrage", "Activer/Désactiver le l'animation de démarrage de jeu"},
	{"Użyj widgetu day/night", "Użyj pulpitu rozszerzonego", "Użyj animacji bootowania", "Użyj animacji uruchamiania gry"},
	{"Tag/Nacht-Widget ein-/ausschalten", "Erweiterten Desktop ein-/ausschalten", "Startanimation ein-/ausschalten", "Spielstart ein-/ausschalten"},
	{"Schakel tussen dag/nacht widget", "Uitgebreide desktop aan/uit", "Opstart animatie aan/uit", "Game boot aan/uit"},
	{"Toggle day/night widget", "Toggle expanded desktop", "Toggle boot animation", "Toggle game boot"},
	{"Toggle day/night widget", "Toggle expanded desktop", "Toggle boot animation", "Toggle game boot"}
};

const char lang_settingsPerformance[][4][100] = 
{	
	{"Processor", "Memory", "Storage Management", "Battery Management"},
	{"Procceseur", "Memoire vive", "Gestion du stockage", "Gestion de la batterie"},
	{"Procesor", "Pamięć", "Zarządzanie pamięcią", "Zarządzanie baterią"},
	{"Prozessor", "Arbeitsspeicher", "Speicherverwaltung", "Akku"},
	{"Processor", "Geheugen", "Opslag Beheer", "Accu Beheer"},
	{"Processor", "Memory", "Storage Management", "Battery Management"},
	{"Processor", "Memory", "Storage Management", "Battery Management"}
};

const char lang_settingsProcessor[][5][100] = 
{	
	{"Current CPU frequency", "CPU over-clock", "Press R to increase frequency and L to decrease frequency.", "Current BUS frequency"},
	{"Fréquence processeur actuelle", "Surcadencement du processeur", "Pressez la touche R pour augmenter la fréquence, et L pour la diminuer.", "Fréquence BUS actuelle"},
	{"Częstotliwość CPU", "Podkręcanie CPU", "Naduś R by zwiększyć, a L by zmniejszyć częstotliwość", "Częstotliwość magistarli"},
	{"Derzeitige CPU frequenz", "CPU übertakten", "Drücke R um die Frequenz zu erhöhen und L um die Frequenz zu senken.", "Derzeitige BUS frequenz"},
	{"Huidige CPU kloksnelheid", "CPU overclock", "Druk R om de kloksnelheid te verhogen en L om het te verlagen.", "Huidige BUS snelheid"},
	{"Current CPU frequency", "CPU over-clock", "Press R to increase frequency and L to decrease frequency.", "Current BUS frequency"},
	{"Current CPU frequency", "CPU over-clock", "Press R to increase frequency and L to decrease frequency.", "Current BUS frequency"}
};

const char lang_settingsRAM[][3][100] = 
{	
	{"RAM available:", "available", "of RAM used"},
	{"RAM disponible :", "disponibles", "de la RAM utilisée"},
	{"Dotępny RAM:", "wolne", "RAM-u użyte"},
	{"RAM verfügbar:", "verfügbar", "des RAMs genutzt"},
	{"RAM vrij:", "beschikbaar", "RAM in gebruik"},
	{"RAM available:", "available", "of RAM used"},
	{"RAM available:", "available", "of RAM used"}
};

const char lang_settingsStorage[][3][60] = 
{	
	{"Press select to toggle USB mass storage", "Total storage capacity:", "Available storage capacity:"},
	{"Appuyez sur Select pour activer/désactiver le mode clé USB", "Capacité de stockage totale :", "Capacité de stockage disponible :"},
	{"Naciśnij Select by podłączyć pamięc USB", "Całkowita ilość pamięci", "Wolna pamieć"},
	{"Drücke Select, um den USB-Speicher ein-/auszuschalten", "Gesamptspeicher kapazität:", "Verfügbar:"},
	{"Druk select om van massaopslag te wisselen", "Totale opslagruimte:", "Beschikbaare opslagruimte:"},
	{"Press select to toggle USB mass storage", "Total storage capacity:", "Available storage capacity:"},
	{"Press select to toggle USB mass storage", "Total storage capacity:", "Available storage capacity:"}
};

const char lang_settingsBattery[][3][60] = 
{	
	{"Powersave", "Balanced", "Performance"},
	{"Economie d'énergie", "Equilibré", "Hautes performances"},
	{"Oszcz. baterii", "Zbalansowane", "Wydajność"},
	{"Energiesparmodus", "Balanced", "Leistung"},
	{"Accubesparing", "Gebalanceerd", "Prestatie"},
	{"Powersave", "Balanced", "Performance"},
	{"Powersave", "Balanced", "Performance"}
};

const char lang_settingsAbout[][7][100] = 
{	
	{"CyanogenPSP updates", "Click for, view or install available updates", "CyanogenPSP version:", "UNOFFICIAL", "Mac Address:", "Kernel version:", "OSLib version:"},
	{"Mises à jour de CyanogenPSP", "Cliquez pour voir et installer les mises à jour disponibles", "Version CyanogenPSP :", "UNOFFICIEL", "Adresse MAC :", "Version du noyau :", "Version de l'OSLib :"},
	{"Aktualizacje CyanogenPSP", "Naciśnij aby sprawdzić/zainstalować aktualizacje", "Wersja CyanogenPSP:", "UNOFFICIAL", "Adres Mac:", "Wersja jądra:", "Wersja OSLib:"},
	{"CyanogenPSP updates", "Click for, view or install available updates", "CyanogenPSP version:", "INOFFIZIELL", "Mac Adresse:", "Kernel version:", "OSLib version:"},
	{"CyanogenPSP updates", "Klik om updates weer te geven/installeren", "CyanogenPSP versie:", "NIET OFFICIEEL", "Mac Adres:", "Kernel versie:", "OSLib versie:"},
	{"CyanogenPSP updates", "Click for, view or install available updates", "CyanogenPSP version:", "UNOFFICIAL", "Mac Address:", "Kernel version:", "OSLib version:"},
	{"CyanogenPSP updates", "Click for, view or install available updates", "CyanogenPSP version:", "UNOFFICIAL", "Mac Address:", "Kernel version:", "OSLib version:"}
};

const char lang_settingsUpdates[][1][100] = 
{	
	{"Check for updates"},
	{"Vérifier la présence de mises à jour"},
	{"Sprawdź aktualizacje"},
	{"Auf Updates prüfen"},
	{"Updates zoeken"},
	{"Check for updates"},
	{"Check for updates"}
};

const char lang_settingsSecuirty[][2][100] = 
{	
	{"Password lock", "Pin lock"},
	{"Verrouillage par mot de passe", "Verrouillage par code PIN"},
	{"Zabezp. hasłem", "Zabezpieczenie PIN"},
	{"Passwortsperre", "Pinsperre"},
	{"Wachtwoord vergrendeling", "PIN vergrendeling"},
	{"Password lock", "Pin lock"},
	{"Password lock", "Pin lock"}
};

const char lang_settingsWifi[][3][100] = 
{	
	{"No WiFi configuration found", "Please turn on the Wlan switch", "Press up or down to navigate through your Wifi configurations"},
	{"Aucune configuration Wi-Fi n'a été trouvée", "Veuillez activer le WLAN à l'aide du commutateur", "Appuyez sur haut ou bas pour vous déplacer à travers vos configurations Wi-Fi"},
	{"Nie znaleziono konfig. Wi-Fi", "Przełącz włącznik WLAN", "Naciśnij w góre/w dół aby wybrać konfigurację"},
	{"Keine WLAN-Einstellungen gefunden", "Bitte schalte den WLAN-Schalter ein", "Drücke Hoch oder Runter, um durch deine WLAN-Einstellungen zu navigieren"},
	{"Geen WiFi configuratie gevonden", "Schakel Wlan knop in alsjeblieft", "Druk omhoog of omlaag om een Wifi-instelling te selecteren"},
	{"No WiFi configuration found", "Please turn on the Wlan switch", "Press up or down to navigate through your Wifi configurations"},
	{"No WiFi configuration found", "Please turn on the Wlan switch", "Press up or down to navigate through your Wifi configurations"}
};

const char lang_settingsDeveloperOptions[][5][100] = 
{	
	{"Toggle Remote Joy Lite", "Displays your PSP screen on your computer via USB", "Press triangle to disable or it may cause the program to crash", "Toggle USB debugging", "Dumping tools"},
	{"Activer/Désactiver Remote Joy Lite", "Affiche l'écran de votre PSP sur votre ordinateur par USB", "Appuyez sur triangle pour le désactiver, ou il risque de faire planter le programme", "Activer/Désactiver le débogage par USB'", "Dumping tools"},
	{"Włącz RemoteJoy Lite", "Wyświetla obraz z PSP na komputerze po USB", "Naciśnij trójkąt by wyłączyć, może powodować crashe", "Włącz debugowanie USB", "Narzędzia do zgrywania"},
	{"Remote Joy Lite ein-/ausschalten", "Zeigt den Bildschirm deiner PSP via USB auf deinem Computer an", "Drücke Dreieck zum deaktivieren, sonst könnte das Programm abstürzen", "USB-Debugging ein-/ausschalten", "Sicherungstools"},
	{"Remote Joy Lite aan/uit", "Geeft je PSP scherm weer op je computer via USB", "Druk driehoek om dit uit te schalelen anders kan dit programma crashen", "USB debugging aan/uit", "Dumping tools"},
	{"Toggle Remote Joy Lite", "Displays your PSP screen on your computer via USB", "Press triangle to disable or it may cause the program to crash", "Toggle USB debugging", "Dumping tools"},
	{"Toggle Remote Joy Lite", "Displays your PSP screen on your computer via USB", "Press triangle to disable or it may cause the program to crash", "Toggle USB debugging", "Dumping tools"}
};

const char lang_settingsDump[][9][100] = 
{	
	{"Dump Flash 0", "Dump Flash 1", "More", "Dump UMD boot.bin", "Dump Memory", "Insert UMD", "Mounted disc", "Found game", "Dumping 28Megs from"},
	{"Sauvegarder le Flash 0", "Sauvegarder le Flash 1", "Plus", "Sauvegarder le boot.bin de l'UMD", "Sauvegarder la mémoire", "Insérer un UMD", "Disque monté", "Jeu trouvé", "Sauvegarde de 28 Mo de"},
	{"Zgraj Flash 0", "Zgraj Flash 1", "Więcej", "Zgraj UMD boot.bin", "Zgraj Memory", "Włóż UMD", "Włożony dysk", "Znaleziono game", "Zgrywanie 28Mega z"},
	{"Flash 0 sichern", "Flash 1 sichern", "Mehr", "UMD boot.bin sichern", "Speicher sichern", "UMD einlegen", "Eingehängte Disc", "Spiel gefunden", "Sichere 28MB von"},
	{"Dump Flash 0", "Dump Flash 1", "Meer", "Dump UMD boot.bin", "Dump Geheugen", "Plaats UMD", "Geplaatste schijf", "Spel gevonden", "Dumping 28Megs van"},
	{"Dump Flash 0", "Dump Flash 1", "More", "Dump UMD boot.bin", "Dump Memory", "Insert UMD", "Mounted disc", "Found game", "Dumping 28Megs from"},
	{"Dump Flash 0", "Dump Flash 1", "More", "Dump UMD boot.bin", "Dump Memory", "Insert UMD", "Mounted disc", "Found game", "Dumping 28Megs from"}
};

const char lang_quickSettings[][5][100] = 
{	
	{"Wi-Fi", "Power save", "Balanced", "Performance", "Lockscreen"},
	{"Wi-Fi", "Économie de batterie", "Équilibrée", "Performance", "Écran de verrouillage"},
	{"Wi-Fi", "Oszcz. energii", "Zbalansowane", "Wydajność", "Ekran blokady"},
	{"WLAN", "Energiesparmodus", "Ausgeglichen", "Leistung", "Bildschirmsperre"},
	{"Wi-Fi", "Accubesparing", "Gebalanceerd", "Prestatie", "Vergrendelscherm"},
	{"Wi-Fi", "Power save", "Balanced", "Performance", "Lockscreen"},
	{"Wi-Fi", "Power save", "Balanced", "Performance", "Lockscreen"}
};
