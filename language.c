#include "language.h"

/*
	The languages are in the followings order:
	0. English
	1. French
	2. Polish
	3. German
	4. Dutch
	5. Spanish
	6. Portuguese
	7. Brazilian Portuguese
	8. Norwegian
*/

const char lang_welcome[][4][100] = 
{	
	{"Welcome", "Use the analog stick to navigate the cursor and", "press X (cross) to open the application.", "GOT IT"},
	{"Bienvenue", "Utilisez le stick analogique pour déplacer le curseur et", "pressez le bouton X (croix) pour ouvrir l'application.", "OK"},
	{"Witamy", "Użyj analoga by poruszać kursorem", "i przyciśnij X by otworzyć aplikację.", "Zrozumiałem"},
	{"Willkommen", "Benutze den Analog-Stick um den Cursor zu navigieren und", "drücke X (Kreuz) um die Anwendung zu öffnen.", "VERSTANDEN"},
	{"Welkom", "Gebruik de joystick om de aanwijzer te bewegen en", "druk X (kruisje) om de toepassing te openen.", "GOT IT"},
	{"Bienvenido", "Use el stick análogo para navegar con el cursor y", "presione X para abrir la aplicación", "ENTENDIDO"},
	{"Bem vindo", "Use o analogico para navegar o cursor e", "pressione X para abrir a aplicação", "ENTENDIDO"},
	{"Bem-vindo(a)", "Use a manete analógica para mover o cursor e", "aperte X para abrir a aplicação.", "ENTENDIDO"},
	{"Velkommen", "Bruk den analoge spaken for å flytte på pekeren og", "trykk X (kryss) for å åpne applikasjonen.", "SKJØNNER"}
};


const char lang_appDrawer[][10][100] = 
{	
	{"Browser", "Calculator", "Clock", "Manager", "Gallery", "Game", "Messages", "Music", "Settings", "UMD"},
	{"Navigateur", "Calculatrice", "Horloge", "Explorateur", "Galerie", "Jeux", "Messages", "Musique", "Configurations", "UMD"},
	{"Przeglądarka", "Kalkulator", "Zegar", "Manager", "Galeria", "Gry", "Wiadomości", "Muzyka", "Ustawienia", "UMD"},
	{"Browser", "Taschenrechner", "Uhr", "Manager", "Galerie", "Spiel", "Nachrichten", "Musik", "Einstellungen", "UMD"},
	{"Browser", "Rekenmachine", "Klok", " Beheerder", "Gallerij", "Spel", "Berichten", "Muziek", "Instellingen", "UMD"},
	{"Navegador", "Calculadora", "Reloj", "Gestión", "Galería", "Juegos", "Mensajes", "Música", "Ajustes", "UMD"},
	{"Navegador", "Calculadora", "Relógio", "Gerenciador", "Galeria", "Jogo", "Mensagens", "Música", "Opções", "UMD"},
	{"Navegador", "Calculadora", "Relógio", "Gerenciador", "Galeria", "Jogo", "Mensagens", "Música", "Definições", "UMD"},
	{"Nettleser", "Kalkulator", "Klokke", "Filbehandler", "Galleri", "Spill", "Meldinger", "Musikk", "Instillinger", "UMD"}
};

const char lang_settingsMain[][6][100] = 
{	
	{"Wi-Fi", "Security", "Display", "Performance", "Developer Options", "About"},
	{"Wi-Fi", "Sécurité", "Affichage", "Performance", "Options de développeur", "À propos"},
	{"Wi-Fi", "Zabezpieczenia", "Wyświetlacz", "Wydajność", "Opcje developerskie", "O programie"},
	{"Wi-Fi", "Sicherheit", "Bildschirm", "Leistung", "Entwickler Optionen", "Über"},
	{"Wi-Fi", "Beveiliging", "Scherm", "Prestaties", "Opties voor ontwikkelaars", "Over"},
	{"Wi-Fi", "Seguridad", "Pantalla", "Rendimiento", "Opciones de Desarrollador", "Acerca de"},
	{"Wi-Fi", "Segurança", "Tela", "Desempenho", "Opções de desenvolvedor", "Sobre"},
	{"Wi-Fi", "Segurança", "Exibição", "Desempenho", "Opções de Desenvolvedor", "Sobre"},
	{"Wi-Fi", "Sikkerhet", "Skjerm", "Ytelse", "Utviklervalg", "Om"}
};

const char lang_settingsDisplay[][4][100] = 
{	
	{"Themes", "Time", "Language", "Miscellaneous"},
	{"Thèmes", "Heure", "Langue", "Divers"},
	{"Motywy", "Czas", "Język", "Różne"},
	{"Themen", "Uhrzeit", "Sprache", "Allgemein"},
	{"Thema's", "Tijd", "Taal", "Diversen"},
	{"Temas", "Tiempo", "Idioma", "Miscelaneo"},
	{"Temas", "Tempo", "Idioma", "Variados"},
	{"Temas", "Horário", "Idioma", "Miscelâneas"},
	{"Temaer", "Tid", "Diverse"}
};

const char lang_settingsThemes[][4][100] = 
{	
	{"Style", "Wallpaper", "Fonts", "Icon Packs"},
	{"Style", "Fond d'écran", "Polices", "Pack d'icones"},
	{"Wygląd", "Tapeta", "Czcionki", "Paczki ikon"},
	{"Designs", "Hintergründe", "Schriftarten", "Symbole"},
	{"Stijl", "Achtergrond", "Lettertypes", "Pictogrammen"},
	{"Estilo", "Fondo de Pantalla", "Fuentes", "Paquete de Iconos"},
	{"Estilo", "Papel de parede", "Fontes", "Pacotes de ícones"},
	{"Estilo", "Papel de Parede", "Fontes", "Pacotes de Ícones"},
	{"Stil", "Bakgrunnsbilde", "Skrifter", "Ikon-pakker"}
};

const char lang_settingsTime[][1][100] = 
{	
	{"Use 24-hour format"},
	{"Utiliser le format 24 heures"},
	{"Użyj czasu 24-godzinnego"},
	{"24-Stunden format nutzen"},
	{"24-uurs klok gebruiken "},
	{"Usar formato de 24 horas"},
	{"Usar formato de 24 horas"},
	{"Usar formato 24 horas"},
	{"Bruk 24-timers format"}
};

const char lang_settingsMiscellaneous[][4][100] =
{	
	{"Toggle day/night widget", "Toggle expanded desktop", "Toggle boot animation", "Toggle game boot"},
	{"Basculer entre le widget jour/nuit", "Activer/Désactiver le bureau étendu", "Activer/Désactiver l'animation de démarrage", "Activer/Désactiver le l'animation de démarrage de jeu"},
	{"Użyj widgetu day/night", "Użyj pulpitu rozszerzonego", "Użyj animacji bootowania", "Użyj animacji uruchamiania gry"},
	{"Tag/Nacht-Widget ein-/ausschalten", "Erweiterten Desktop ein-/ausschalten", "Startanimation ein-/ausschalten", "Spielstart ein-/ausschalten"},
	{"Schakel tussen dag/nacht widget", "Uitgebreide desktop aan/uit", "Opstart animatie aan/uit", "Game boot aan/uit"},
	{"Usar widget de día/noche", "Usar escritorio expandido","Activar animación de inicio", "Activar inicio del juego"},
	{"Alternar widget dia/noite", "Alternar area de trabalho expandida", "Alternar animação de boot", "Alternar animação de boot de jogo"},
	{"Alternar widget de dia/noite", "Alternar área de trabalho expandida", "Alternar animação de boot", "Alternar boot de jogo"},
	{"Veksle dag/natt modul", "Veksle utvidet skrivebord", "Veklse oppstarts-animasjon", "Veksle spill-oppstart"}
};

const char lang_settingsPerformance[][4][100] = 
{	
	{"Processor", "Memory", "Storage Management", "Battery Management"},
	{"Procceseur", "Memoire vive", "Gestion du stockage", "Gestion de la batterie"},
	{"Procesor", "Pamięć", "Zarządzanie pamięcią", "Zarządzanie baterią"},
	{"Prozessor", "Arbeitsspeicher", "Speicherverwaltung", "Akku"},
	{"Processor", "Geheugen", "Opslag Beheer", "Accu Beheer"},
	{"Procesador", "Memoria", "Gestión de Almacenamiento", "Gestión de la Batería"},
	{"Processador", "Memoria", "Armazenamento", "Bateria"},
	{"Processador", "Memória", "Gestão de Armazenamento", "Gestão de Bateria"},
	{"Prosessor", "Minne", "Administrer lagring", "Administrer batteri"}
};

const char lang_settingsProcessor[][5][100] = 
{	
	{"Current CPU frequency", "CPU over-clock", "Press R to increase frequency and L to decrease frequency.", "Current BUS frequency"},
	{"Fréquence processeur actuelle", "Surcadencement du processeur", "Pressez la touche R pour augmenter la fréquence, et L pour la diminuer.", "Fréquence BUS actuelle"},
	{"Częstotliwość CPU", "Podkręcanie CPU", "Naduś R by zwiększyć, a L by zmniejszyć częstotliwość", "Częstotliwość magistarli"},
	{"Derzeitige CPU frequenz", "CPU übertakten", "Drücke R um die Frequenz zu erhöhen und L um die Frequenz zu senken.", "Derzeitige BUS frequenz"},
	{"Huidige CPU kloksnelheid", "CPU overclock", "Druk R om de kloksnelheid te verhogen en L om het te verlagen.", "Huidige BUS snelheid"},
	{"Frecuencia actual de la CPU", "Over-clock de CPU", "Presione R para aumentar la frecuencia y L para disminuir la frecuencia", "Frecuencia actual de BUS"},
	{"Frenquencia atual da CPU", "Overclock da CPU", "Pressione R para aumentar a frequencia e L para diminui-la.", "Frequencia atual de BUS"},
	{"Atual frequência da CPU", "Overclock da CPU", "Aperte R para aumentar a frequência e L para diminuí-la.", "Atual frequência do BUS"},
	{"Nåværende CPU-frekvens", "CPU over-klokking", "Trykk R for å øke frekvensen og L for å senke frekvensen.", "Nåværende bussfrekvens"}
};

const char lang_settingsRAM[][3][100] = 
{	
	{"RAM available:", "available", "of RAM used"},
	{"RAM disponible :", "disponibles", "de la RAM utilisée"},
	{"Dotępny RAM:", "wolne", "RAM-u użyte"},
	{"RAM verfügbar:", "verfügbar", "des RAMs genutzt"},
	{"RAM vrij:", "beschikbaar", "RAM in gebruik"},
	{"Memoria RAM disponible:", "disponible", "de la memoria RAM utilizada"},
	{"RAM disponível:", "disponível", "de RAM usada"},
	{"RAM disponível:", "disponíveis", "de RAM usada"},
	{"Tilgjengelig RAM:", "tilgjengelig", "av RAM brukt"}
};

const char lang_settingsStorage[][3][100] = 
{	
	{"Press select to toggle USB mass storage", "Total storage capacity:", "Available storage capacity:"},
	{"Appuyez sur Select pour activer/désactiver le mode clé USB", "Capacité de stockage totale :", "Capacité de stockage disponible :"},
	{"Naciśnij Select by podłączyć pamięc USB", "Całkowita ilość pamięci", "Wolna pamieć"},
	{"Drücke Select, um den USB-Speicher ein-/auszuschalten", "Gesamptspeicher kapazität:", "Verfügbar:"},
	{"Druk select om van massaopslag te wisselen", "Totale opslagruimte:", "Beschikbaare opslagruimte:"},
	{"Presione Select para activar el Almacenamiento Masivo USB", "Capacidad de almacenamiento total:", "Capacidad de almacenamiento disponible:"},
	{"Pressione Select para alternar o armazenamento em massa USB", "Capacidade de armazenamento total:", "Capacidade de armazenamento disponível:"},
	{"Aperte select para alternar o dispositivo de armazenmanento USB", "Capacidade total de armazenamento:", "Capacidade de armazenamento disponível:"},
	{"Trykk select for å veksle USB masselagring", "Total lagringskapasitet:", "Tilgjengelig lagringskapasitet:"}
};

const char lang_settingsBattery[][3][60] = 
{	
	{"Powersave", "Balanced", "Performance"},
	{"Economie d'énergie", "Equilibré", "Hautes performances"},
	{"Oszcz. baterii", "Zbalansowane", "Wydajność"},
	{"Energiesparmodus", "Balanced", "Leistung"},
	{"Accubesparing", "Gebalanceerd", "Prestatie"},
	{"Ahorro de Energía", "Balanceado", "Alto Rendimiento"},
	{"Powersave", "Balanciado", "Desempenho"},
	{"Econômico", "Equilibrado", "Desempenhado"},
	{"Strømsparing", "Balansert", "Ytelse"}
};

const char lang_settingsAbout[][7][100] = 
{	
	{"CyanogenPSP updates", "Click for, view or install available updates", "CyanogenPSP version:", "UNOFFICIAL", "Mac Address:", "Kernel version:", "OSLib version:"},
	{"Mises à jour de CyanogenPSP", "Cliquez pour voir et installer les mises à jour disponibles", "Version CyanogenPSP :", "UNOFFICIEL", "Adresse MAC :", "Version du noyau :", "Version de l'OSLib :"},
	{"Aktualizacje CyanogenPSP", "Naciśnij aby sprawdzić/zainstalować aktualizacje", "Wersja CyanogenPSP:", "UNOFFICIAL", "Adres Mac:", "Wersja jądra:", "Wersja OSLib:"},
	{"CyanogenPSP updates", "Click for, view or install available updates", "CyanogenPSP version:", "INOFFIZIELL", "Mac Adresse:", "Kernel version:", "OSLib version:"},
	{"CyanogenPSP updates", "Klik om updates weer te geven/installeren", "CyanogenPSP versie:", "NIET OFFICIEEL", "Mac Adres:", "Kernel versie:", "OSLib versie:"},
	{"Actualizaciones de CyanogenPSP", "Haga click para ver o instalar actualizaciones", "Versión de CyanogenPSP:", "NO OFICIAL", "Dirección MAC:", "Versión del Kernel:", "Versión del OSLib:"},
	{"Atualizações CyanogenPSP", "Clique para, ver ou instalar atualizações disponiveis", "versão CyanogenPSP:", "NÃO-OFICIAL", "Endereço MAC:", "Versão do kernel:", "Versão OSLIB:"},
	{"Atualizações do CyanogenPSP", "Clique para, ver ou instalar atualizações disponíveis", "Versão do CyanogenPSP:", "NÃO-OFICIAL", "Endereço Mac:", "Versão do Kernel:", "Versão do OSLib:"},
	{"CyanogenPSP oppdateringer", "Trykk for, vis eller installer tilgjengelige oppdateringer", "CyanogenPSP versjon:", "UOFFISIELL", "Mac Adresse:", "Kernel versjon:", "OSLib versjon:"}
};

const char lang_settingsUpdates[][1][100] = 
{	
	{"Check for updates"},
	{"Vérifier la présence de mises à jour"},
	{"Sprawdź aktualizacje"},
	{"Auf Updates prüfen"},
	{"Updates zoeken"},
	{"Buscar actualizaciones"},
	{"Procurar por atualizações"},
	{"Checar por atualizações"},
	{"Se etter oppdateringer"}
};

const char lang_settingsSecuirty[][2][100] = 
{	
	{"Password lock", "Pin lock"},
	{"Verrouillage par mot de passe", "Verrouillage par code PIN"},
	{"Zabezp. hasłem", "Zabezpieczenie PIN"},
	{"Passwortsperre", "Pinsperre"},
	{"Wachtwoord vergrendeling", "PIN vergrendeling"},
	{"Bloqueo con contraseña", "Bloqueo con PIN"},
	{"Trava por senha", "Trava por PIN"},
	{"Trava de senha", "Trava de pino"},
	{"Passordlås", "Pin-lås"}
};

const char lang_settingsWifi[][3][100] = 
{	
	{"No WiFi configuration found", "Please turn on the Wlan switch", "Press up or down to navigate through your Wifi configurations"},
	{"Aucune configuration Wi-Fi n'a été trouvée", "Veuillez activer le WLAN à l'aide du commutateur", "Appuyez sur haut ou bas pour vous déplacer à travers vos configurations Wi-Fi"},
	{"Nie znaleziono konfig. Wi-Fi", "Przełącz włącznik WLAN", "Naciśnij w góre/w dół aby wybrać konfigurację"},
	{"Keine WLAN-Einstellungen gefunden", "Bitte schalte den WLAN-Schalter ein", "Drücke Hoch oder Runter, um durch deine WLAN-Einstellungen zu navigieren"},
	{"Geen WiFi configuratie gevonden", "Schakel Wlan knop in alsjeblieft", "Druk omhoog of omlaag om een Wifi-instelling te selecteren"},
	{"No se encontró una configuración de Wi-Fi", "Por favor activa el interruptor del Wi-Fi", "Presiona arriba o abajo para navegar por tus configuraciones de Wi-Fi"},
	{"Nenhuma configuração de WiFi encontrada", "Favor ligar o Wlan", "Pressione cima ou baixo para navegar suas configurações de WiFi"},
	{"Nenhuma configuração de WiFi encontrada", "Por favor, ative o Wlan", "Use os botões direcionais (cima/baixo) para navegar através de suas configurações de Wifi"},
	{"Ingen WiFi konfigurasjon funnet", "Vennligst slå på Wlan bryteren", "Trykk opp eller ned for å navigere dine Wifi konfigurasjoner"}
};

const char lang_settingsDeveloperOptions[][5][100] = 
{	
	{"Toggle Remote Joy Lite", "Displays your PSP screen on your computer via USB", "Press triangle to disable or it may cause the program to crash", "Toggle USB debugging", "Dumping tools"},
	{"Activer/Désactiver Remote Joy Lite", "Affiche l'écran de votre PSP sur votre ordinateur par USB", "Appuyez sur triangle pour le désactiver, ou il risque de faire planter le programme", "Activer/Désactiver le débogage par USB'", "Dumping tools"},
	{"Włącz RemoteJoy Lite", "Wyświetla obraz z PSP na komputerze po USB", "Naciśnij trójkąt by wyłączyć, może powodować crashe", "Włącz debugowanie USB", "Narzędzia do zgrywania"},
	{"Remote Joy Lite ein-/ausschalten", "Zeigt den Bildschirm deiner PSP via USB auf deinem Computer an", "Drücke Dreieck zum deaktivieren, sonst könnte das Programm abstürzen", "USB-Debugging ein-/ausschalten", "Sicherungstools"},
	{"Remote Joy Lite aan/uit", "Geeft je PSP scherm weer op je computer via USB", "Druk driehoek om dit uit te schalelen anders kan dit programma crashen", "USB debugging aan/uit", "Dumping tools"},
	{"Usar Remote Joy Lite", "Muestra la imagen de la pantalla de tu PSP en tu PC vía USB", "Presiona Triangulo para desactivarlo o podria causar que la aplicación se detenga", "Activar depuración de USB", "Opciones de volcado/extracción"},
	{"Alternar Remote Joy Lite", "Exibe a tela do seu PSP no seu PC por um cabo USB", "Pressione triangulo para desabilitar ou pode causar um crash", "Alternar depuração USB", "Ferramentas de dumping"},
	{"Alternar Remote Joy Lite", "Exibir a tela de seu PSP em seu computador via USB", "Aperte triângulo para desabilitar ou isto pode causar o encerramento súbito do aplicativo", "Alternar depuração de USB", "Ferramentas de Dumping"},
	{"Veksle Remote Joy Lite", "Viser PSP skjermen på din PC gjennom USB", "Trykk trekant for å deaktivere, ellers kan det føre til at programmet krasjer", "Veksle USB feilsøking", "Dumping tools"}
};

const char lang_settingsDump[][9][100] = 
{	
	{"Dump Flash 0", "Dump Flash 1", "More", "Dump UMD boot.bin", "Dump Memory", "Insert UMD", "Mounted disc", "Found game", "Dumping 28Megs from"},
	{"Sauvegarder le Flash 0", "Sauvegarder le Flash 1", "Plus", "Sauvegarder le boot.bin de l'UMD", "Sauvegarder la mémoire", "Insérer un UMD", "Disque monté", "Jeu trouvé", "Sauvegarde de 28 Mo de"},
	{"Zgraj Flash 0", "Zgraj Flash 1", "Więcej", "Zgraj UMD boot.bin", "Zgraj Memory", "Włóż UMD", "Włożony dysk", "Znaleziono game", "Zgrywanie 28Mega z"},
	{"Flash 0 sichern", "Flash 1 sichern", "Mehr", "UMD boot.bin sichern", "Speicher sichern", "UMD einlegen", "Eingehängte Disc", "Spiel gefunden", "Sichere 28MB von"},
	{"Dump Flash 0", "Dump Flash 1", "Meer", "Dump UMD boot.bin", "Dump Geheugen", "Plaats UMD", "Geplaatste schijf", "Spel gevonden", "Dumping 28Megs van"},
	{"Extraer Flash 0", "Extraer Flash 1", "Más", "Extraer boot.bin del UMD", "Volcar Memoria", "Insertar UMD", "Disco Leído", "Se ha encontrado un juego", "Extrayendo 28Megas de"},
	{"Dump Flash 0", "Dump Flash 1", "Mais", "Dump UMD boot.bin", "Memoria Dump", "Insira um UMD", "Disco montado", "Jogo encontrado", "Dumping 28Megs de"},
	{"Despejar Flash 0", "Despejar Flash 1", "Mais", "Despejar boot.bin do UMD", "Despejar Memória", "Insira o UMD", "Disco montado", "Jogo encontrado", "Despejando 28Megs de"},
	{"Dump Flash 0", "Dump Flash 1", "Mer", "Dump UMD boot.bin", "Dump Minne", "Sett inn UMD", "Montert plate", "Spill funnet", "Dumper 28Megs fra"}
};

const char lang_quickSettings[][5][100] = 
{	
	{"Wi-Fi", "Power save", "Balanced", "Performance", "Lockscreen"},
	{"Wi-Fi", "Économie de batterie", "Équilibrée", "Performance", "Écran de verrouillage"},
	{"Wi-Fi", "Oszcz. energii", "Zbalansowane", "Wydajność", "Ekran blokady"},
	{"WLAN", "Energiesparmodus", "Ausgeglichen", "Leistung", "Bildschirmsperre"},
	{"Wi-Fi", "Accubesparing", "Gebalanceerd", "Prestatie", "Vergrendelscherm"},
	{"Wi-Fi", "Ahorro de Energía", "Balanceado", "Alto Rendimiento", "Pantalla de Bloqueo"},
	{"Wi-Fi", "Power save", "Balanceado", "Desempenho", "Tela de lock"},
	{"Wi-Fi", "Modo econômico", "Equilibrado", "Desempenhado", "Bloqueio de tela"},
	{"Wi-Fi", "Strømsparing", "Balansert", "Ytelse", "Låseskjerm"}
};