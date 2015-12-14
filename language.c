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
	9. Romanian 
	10. Italian
	11. Russian
	12. Ukrainian
*/

const char lang_welcome[][4][120] = 
{	
	{"Welcome", "Use the analog stick to navigate the cursor and", "press X (cross) to open the application.", "GOT IT"},
	{"Bienvenue", "Utilisez le stick analogique pour déplacer le curseur et", "pressez le bouton X (croix) pour ouvrir l'application.", "OK"},
	{"Witamy", "Uzyj analoga by poruszac kursorem", "i przycisnij X by otworzyc aplikacje.", "Zrozumialem"},
	{"Willkommen", "Benutze den Analog-Stick um den Cursor zu navigieren \nund", "\n\ndrücke X (Kreuz) um die Anwendung zu öffnen.", "OK"},
	{"Welkom", "Gebruik de joystick om de aanwijzer te bewegen en", "druk X (kruisje) om de toepassing te openen.", "GOT IT"},
	{"Bienvenido", "Use el stick análogo para navegar con el cursor y", "presione X para abrir la aplicación", "ENTENDIDO"},
	{"Bem vindo", "Use o analogico para navegar o cursor e", "pressione X para abrir a aplicação", "ENTENDIDO"},
	{"Bem-vindo(a)", "Use a manete analógica para mover o cursor e", "aperte X para abrir a aplicação.", "ENTENDIDO"},
	{"Velkommen", "Bruk den analoge spaken for å flytte på pekeren og", "trykk X (kryss) for å åpne applikasjonen.", "SKJØNNER"},
	{"Bine ai venit", "Folosește joystickul pentru a naviga și", "apasă X pentru a deschide o aplicație", "AM ÎNȚELES"},
	{"Benvenuto", "Usa lo stick analogico per muovere il cursore e", "premi X (croce) per aprire l'applicazione.", "HO CAPITO"},
	{"Приветствую", "Используйте аналоговый стик для управления курсором и", "нажмите X (крест) для открытия программы.", "Я понял"},
	{"Вітаю", "Використовуйте аналоговий стік для керування курсором і", "натисніть X (хрест) для відкриття програми.", "Я зрозумів"}
};


const char lang_appDrawer[][10][100] = 
{	
	{"Browser", "Calculator", "Clock", "Manager", "Gallery", "Game", "Messages", "Music", "Settings", "UMD"},
	{"Navigateur", "Calc", "Horloge", "Explorateur", "Galerie", "Jeux", "Messages", "Musique", "Paramètres", "UMD"},
	{"Internet", "Kalkulator", "Zegar", "Pliki", "Galeria", "Gry", "Wiadomosci", "Muzyka", "Ustawienia", "UMD"},
	{"Browser", "Taschenrechner", "Uhr", "Manager", "Galerie", "Spiel", "Nachrichten", "Musik", "Einstellungen", "UMD"},
	{"Browser", "Rekenmachine", "Klok", " Beheerder", "Gallerij", "Spel", "Berichten", "Muziek", "Instellingen", "UMD"},
	{"Navegador", "Calculadora", "Reloj", "Gestión", "Galería", "Juegos", "Mensajes", "Música", "Ajustes", "UMD"},
	{"Navegador", "Calculadora", "Relógio", "Gerenciador", "Galeria", "Jogo", "Mensagens", "Música", "Opções", "UMD"},
	{"Navegador", "Calculadora", "Relógio", "Gerenciador", "Galeria", "Jogo", "Mensagens", "Música", "Definições", "UMD"},
	{"Nettleser", "Kalkulator", "Klokke", "Filbehandler", "Galleri", "Spill", "Meldinger", "Musikk", "Instillinger", "UMD"},
	{"Internet", "Calculator", "Ceas", "Fișiere", "Galerie", "Joc", "Mesaje", "Muzică", "Setări", "UMD"},
	{"Browser", "Calcolatrice", "Orologio", "Manager", "Galleria", "Giochi", "Messaggi", "Musica", "Impostazioni", "UMD"},
	{"Браузер", "Калькулятор", "Часы", "Проводник", "Галерея", "Игра", "Сообщения", "Музыка", "Настройки", "UMD"},
	{"Браузер", "Калькулятор", "Час", "Провідник", "Галерея", "Ігри", "SMS", "Музика", "Настройки", "UMD"}
};

const char lang_settingsMain[][6][100] = 
{	
	{"Wi-Fi", "Security", "Display", "Performance", "Developer Options", "About"},
	{"Wi-Fi", "Sécurité", "Affichage", "Performance", "Options de développeur", "À propos"},
	{"Wi-Fi", "Zabezpieczenia", "Wyswietlacz", "Wydajnosc", "Opcje developerskie", "O programie"},
	{"Wi-Fi", "Sicherheit", "Bildschirm", "Leistung", "Entwickler Optionen", "Über"},
	{"Wi-Fi", "Beveiliging", "Scherm", "Prestaties", "Opties voor ontwikkelaars", "Over"},
	{"Wi-Fi", "Seguridad", "Pantalla", "Rendimiento", "Opciones de Desarrollador", "Acerca de"},
	{"Wi-Fi", "Segurança", "Tela", "Desempenho", "Opções de desenvolvedor", "Sobre"},
	{"Wi-Fi", "Segurança", "Exibição", "Desempenho", "Opções de Desenvolvedor", "Sobre"},
	{"Wi-Fi", "Sikkerhet", "Skjerm", "Ytelse", "Utviklervalg", "Om"},
	{"Wi-Fi", "Securitate", "Ecran", "Performanță", "Opțiuni dezvoltator", "Despre consolă"},
	{"Wi-Fi", "Sicurezza", "Display", "Performance", "Opzione sviluppatore", "Informazioni"},
	{"Wi-Fi", "Безопасность", "Дисплей", "Производительность", "Настройки разработчика", "О программе"},
	{"Wi-Fi", "Безпека", "Дисплей", "Продуктивність", "Спец. настройки", "Про програму"}
};

const char lang_settingsDisplay[][4][100] = 
{	
	{"Themes", "Time", "Language", "Miscellaneous"},
	{"Thèmes", "Heure", "Langue", "Divers"},
	{"Motywy", "Czas", "Jezyk", "Rózne"},
	{"Themen", "Uhrzeit", "Sprache", "Allgemein"},
	{"Thema's", "Tijd", "Taal", "Diversen"},
	{"Temas", "Tiempo", "Idioma", "Miscelaneo"},
	{"Temas", "Tempo", "Idioma", "Variados"},
	{"Temas", "Horário", "Idioma", "Miscelâneas"},
	{"Temaer", "Tid", "språk", "Diverse"},
	{"Teme", "Ceas", "Limbă", "Diverse"},
	{"Temi", "Ora", "lingua", "Varie"},
	{"Темы", "Время", "Язык﻿", "Разное"},
	{"Теми", "Час", "Мови﻿", "Різне"}
};

const char lang_settingsThemes[][4][100] = 
{	
	{"Style", "Wallpaper", "Fonts", "Icon Packs"},
	{"Style", "Fond d'écran", "Polices", "Pack d'icones"},
	{"Wyglad", "Tapeta", "Czcionki", "Paczki ikon"},
	{"Designs", "Hintergründe", "Schriftarten", "Symbole"},
	{"Stijl", "Achtergrond", "Lettertypes", "Pictogrammen"},
	{"Estilo", "Fondo de Pantalla", "Fuentes", "Paquete de Iconos"},
	{"Estilo", "Papel de parede", "Fontes", "Pacotes de ícones"},
	{"Estilo", "Papel de Parede", "Fontes", "Pacotes de Ícones"},
	{"Stil", "Bakgrunnsbilde", "Skrifter", "Ikon-pakker"},
	{"Stil", "Fundal", "Fonturi", "Pachete de pictograme"},
	{"Stile", "Sfondo", "Fonts", "Pacchetti di icone"},
	{"Стиль", "Обои", "Шрифты", "Наборы иконок"},
	{"Стиль", "Шпалери", "Шрифт", "Пакети іконок"}
};

const char lang_settingsFonts[][5][100] = 
{	
	{"Font Style", "Font Size", "Small", "Medium", "Large"},
	{"Le style de police", "Taille de police", "Petit", "Moyen", "Grand"},
	{"Krój czcionki", "Rozmiar czcionki", "Mały", "Średni", "duże"},
	{"Schriftstil", "Schriftgröße", "Klein", "Medium", "große"},
	{"Lettertype", "Letter grootte", "Klein", "Medium", "Groot"},
	{"Estilo de fuente", "Tamaño de fuente", "Pequeña", "Medio", "Gran"},
	{"Estilo da fonte", "Tamanho da fonte", "Pequena", "Médio", "Grande"},
	{"Estilo da fonte", "Tamanho da fonte", "Pequena", "Médio", "Grande"},
	{"Skriftstil", "Font-størrelse", "Liten", "Medium", "Stor"},
	{"Stilul fontului", "Marimea fontului", "Mic", "Mediu", "Mare"},
	{"Stile carattere", "Dimensione del carattere", "Piccolo", "Media", "Di grandi dimensioni"},
	{"Стиль шрифта", "Размер шрифта", "Малый", "Средний", "Большой"},
	{"Стиль шрифту", "Розмір шрифту", "Малий", "Середній", "Великий"}
};

const char lang_settingsTime[][1][100] = 
{	
	{"Use 24-hour format"},
	{"Utiliser le format 24 heures"},
	{"Uzyj czasu 24-godzinnego"},
	{"24-Stunden format nutzen"},
	{"24-uurs klok gebruiken "},
	{"Usar formato de 24 horas"},
	{"Usar formato de 24 horas"},
	{"Usar formato 24 horas"},
	{"Bruk 24-timers format"},
	{"Folosește formatul de 24 de ore"},
	{"Usa il formato 24 ore"},
	{"24-часовой формат"},
	{"24-годинний формат"}
};

const char lang_settingsMiscellaneous[][4][100] =
{	
	{"Toggle day/night widget", "Toggle expanded desktop", "Toggle boot animation", "Toggle game boot"},
	{"Basculer entre le widget jour/nuit", "Activer/Désactiver le bureau étendu", "Activer/Désactiver l'animation de démarrage", "Activer/Désactiver le l'animation de démarrage de jeu"},
	{"Uzyj widgetu dzien/noc", "Uzyj pulpitu rozszerzonego", "Uzyj animacji bootowania", "Animacja przy uruchamianiu gry"},
	{"Tag/Nacht-Widget ein-/ausschalten", "Erweiterten Desktop ein-/ausschalten", "Startanimation ein-/ausschalten", "Spielstart ein-/ausschalten"},
	{"Schakel tussen dag/nacht widget", "Uitgebreide desktop aan/uit", "Opstart animatie aan/uit", "Game boot aan/uit"},
	{"Usar widget de día/noche", "Usar escritorio expandido","Activar animación de inicio", "Activar inicio del juego"},
	{"Alternar widget dia/noite", "Alternar area de trabalho expandida", "Alternar animação de boot", "Alternar animação de boot de jogo"},
	{"Alternar widget de dia/noite", "Alternar área de trabalho expandida", "Alternar animação de boot", "Alternar boot de jogo"},
	{"Veksle dag/natt modul", "Veksle utvidet skrivebord", "Veklse oppstarts-animasjon", "Veksle spill-oppstart"},
	{"Activează widgetul de zi/noapte", "Activează desktop extins", "Activează animația de pornire", "Activează pornirea în joc"},
	{"Attiva il widget giorno/notte", "Attiva il desktop espanso", "Attiva animazione di avvio", "Attiva il boot del gioco"},
	{"Вкл. виджет дня/ночи", "Вкл. расширенный рабочий стол", "Вкл. анимацию загрузки", "Вкл. анимацию при запуске игры"},
	{"Увм. віджет дня/ночі", "Увм. розширений робочий стіл", "Увм. анімацію загрузки", "Увм. анімацію при запуску гри"}
};

const char lang_settingsPerformance[][4][100] = 
{	
	{"Processor", "Memory", "Storage Management", "Battery Management"},
	{"Procceseur", "Memoire vive", "Gestion du stockage", "Gestion de la batterie"},
	{"Procesor", "Pamiec", "Zarzadzanie pamiecia", "Zarzadzanie bateria"},
	{"Prozessor", "Arbeitsspeicher", "Speicherverwaltung", "Akku"},
	{"Processor", "Geheugen", "Opslag Beheer", "Accu Beheer"},
	{"Procesador", "Memoria", "Gestión de Almacenamiento", "Gestión de la Batería"},
	{"Processador", "Memoria", "Armazenamento", "Bateria"},
	{"Processador", "Memória", "Gestão de Armazenamento", "Gestão de Bateria"},
	{"Prosessor", "Minne", "Administrer lagring", "Administrer batteri"},
	{"Procesor", "Memorie", "Gestionarea memoriei", "Gestionarea bateriei"},
	{"Processore", "Memoria", "Gestione dello spazio", "Gestione della batteria"},
	{"Процессор", "Память", "Управление памятью", "Управление электропитанием"},
	{"Процесор", "Пам'ять", "Керування памя'ттю", "Керування батареєю"}
};

const char lang_settingsProcessor[][5][100] = 
{	
	{"Current CPU frequency", "CPU over-clock", "Press R to increase frequency and L to decrease frequency.", "Current BUS frequency"},
	{"Fréquence processeur actuelle", "Surcadencement du processeur", "Pressez la touche R pour augmenter la fréquence, et L pour la diminuer.", "Fréquence BUS actuelle"},
	{"Czestotliwosc CPU", "Podkrecanie CPU", "Nacisnij R by zwiekszyc, a L by zmniejszyc czestotliwosc", "Czestotliwosc magistarli"},
	{"Derzeitige CPU frequenz", "CPU übertakten", "Drücke R um die Frequenz zu erhöhen und L um die Frequenz zu senken.", "Derzeitige BUS frequenz"},
	{"Huidige CPU kloksnelheid", "CPU overclock", "Druk R om de kloksnelheid te verhogen en L om het te verlagen.", "Huidige BUS snelheid"},
	{"Frecuencia actual de la CPU", "Over-clock de CPU", "Presione R para aumentar la frecuencia y L para disminuir la frecuencia", "Frecuencia actual de BUS"},
	{"Frenquencia atual da CPU", "Overclock da CPU", "Pressione R para aumentar a frequencia e L para diminui-la.", "Frequencia atual de BUS"},
	{"Atual frequência da CPU", "Overclock da CPU", "Aperte R para aumentar a frequência e L para diminuí-la.", "Atual frequência do BUS"},
	{"Nåværende CPU-frekvens", "CPU over-klokking", "Trykk R for å øke frekvensen og L for å senke frekvensen.", "Nåværende bussfrekvens"},
	{"Frecvența actuală a CPU", "Creșterea frecvenței CPU", "Apasă R pentru a crește frecvența și L pentru a descrește frecvența.", "Frecvența actuală a BUS"},
	{"Frequenza CPU attuale", "Over-clock CPU", "Premi R per incrementare la frequenza e L per diminuire la frequenza.", "Frequenza attuale BUS"},
	{"Текущая частота ЦПУ", "Разгон ЦПУ", "Нажмите R для увеличения частоты и L для уменьшения.", "Текущая частота шины"},
	{"Поточна частота ЦП", "Розгін ЦП", "Натисніть R для збільшення частоти та L для зменшення.", "Поточна частота шини"}
};

const char lang_settingsRAM[][3][100] = 
{	
	{"RAM available:", "available", "of RAM used"},
	{"RAM disponible :", "disponibles", "de la RAM utilisée"},
	{"Dotepny RAM:", "wolne", "RAM-u uzyte"},
	{"RAM verfügbar:", "verfügbar", "des RAMs genutzt"},
	{"RAM vrij:", "beschikbaar", "RAM in gebruik"},
	{"Memoria RAM disponible:", "disponible", "de la memoria RAM utilizada"},
	{"RAM disponível:", "disponível", "de RAM usada"},
	{"RAM disponível:", "disponíveis", "de RAM usada"},
	{"Tilgjengelig RAM:", "tilgjengelig", "av RAM brukt"},
	{"RAM disponibil:", "disponibil", "de RAM utilizat"},
	{"RAM disponibile:", "disponibile", "di RAM usata"},
	{"Доступно ОЗУ:", "доступно", "ОЗУ используется"},
	{"Доступно RAM:", "доступно", "RAM використовується"}
};

const char lang_settingsStorage[][3][100] = 
{	
	{"Press select to toggle USB mass storage", "Total storage capacity:", "Available storage capacity:"},
	{"Appuyez sur Select pour activer/désactiver le mode clé USB", "Capacité de stockage totale :", "Capacité de stockage disponible :"},
	{"Nacisnij Select by podlaczyc pamiec USB", "Calkowita ilosc pamieci", "Wolna pamiec"},
	{"Drücke Select, um den USB-Speicher ein-/auszuschalten", "Gesamptspeicher kapazität:", "Verfügbar:"},
	{"Druk select om van massaopslag te wisselen", "Totale opslagruimte:", "Beschikbaare opslagruimte:"},
	{"Presione Select para activar el Almacenamiento Masivo USB", "Capacidad de almacenamiento total:", "Capacidad de almacenamiento disponible:"},
	{"Pressione Select para alternar o armazenamento em massa USB", "Capacidade de armazenamento total:", "Capacidade de armazenamento disponível:"},
	{"Aperte select para alternar o dispositivo de armazenmanento USB", "Capacidade total de armazenamento:", "Capacidade de armazenamento disponível:"},
	{"Trykk select for å veksle USB masselagring", "Total lagringskapasitet:", "Tilgjengelig lagringskapasitet:"},
	{"Apasă SELECT pentru a activa stocarea USB", "Capacitate de stocare totală:", "Capacitate de stocare disponibilă:"},
	{"Premi select per attivare memoria di massa USB", "Capacità totale della memoria:", "Capacità di archiviazione disponibile"},
	{"Нажмите SELECT для переключения на USB накопитель", "Полный объём памяти:", "Доступный объём памяти:"},
	{"Натисніть SELECT для переключення на USB накопичувач", "Повний об'єм пам'яті:", "Доступний об'єм пам'яті:"}
};

const char lang_settingsBattery[][3][60] = 
{	
	{"Powersave", "Balanced", "Performance"},
	{"Economie d'énergie", "Equilibré", "Hautes performances"},
	{"Oszczedzanie baterii", "Zbalansowane", "Wydajnosc"},
	{"Energiesparmodus", "Balanced", "Leistung"},
	{"Accubesparing", "Gebalanceerd", "Prestatie"},
	{"Ahorro de Energía", "Balanceado", "Alto Rendimiento"},
	{"Powersave", "Balanciado", "Desempenho"},
	{"Econômico", "Equilibrado", "Desempenhado"},
	{"Strømsparing", "Balansert", "Ytelse"},
	{"Economisire", "Echilibrat", "Performanță"},
	{"Risparmio energetico", "Bilanciato", "Prestazioni"},
	{"Энергосберегающий", "Сбаланированный", "Производительность"},
	{"Енергозберігаючий", "Збалансований", "Продуктивність"}
};

const char lang_settingsAbout[][7][100] = 
{	
	{"CyanogenPSP updates", "Click for, view or install available updates", "CyanogenPSP version:", "OFFICIAL", "Mac Address:", "Kernel version:", "OSLib version:"},
	{"Mises à jour de CyanogenPSP", "Cliquez pour voir et installer les mises à jour disponibles", "Version CyanogenPSP :", "OFFICIEL", "Adresse MAC :", "Version du noyau :", "Version de l'OSLib :"},
	{"Aktualizacje CyanogenPSP", "Nacisnij aby sprawdzic/zainstalowac aktualizacje", "Wersja CyanogenPSP:", "OFFICIAL", "Adres Mac:", "Wersja jadra:", "Wersja OSLib:"},
	{"CyanogenPSP updates", "Click for, view or install available updates", "CyanogenPSP version:", "OFFIZIELL", "Mac Adresse:", "Kernel version:", "OSLib version:"},
	{"CyanogenPSP updates", "Klik om updates weer te geven/installeren", "CyanogenPSP versie:", "OFFICIEEL", "Mac Adres:", "Kernel versie:", "OSLib versie:"},
	{"Actualizaciones de CyanogenPSP", "Haga click para ver o instalar actualizaciones", "Versión de CyanogenPSP:", "OFICIAL", "Dirección MAC:", "Versión del Kernel:", "Versión del OSLib:"},
	{"Atualizações CyanogenPSP", "Clique para, ver ou instalar atualizações disponiveis", "versão CyanogenPSP:", "OFICIAL", "Endereço MAC:", "Versão do kernel:", "Versão OSLIB:"},
	{"Atualizações do CyanogenPSP", "Clique para, ver ou instalar atualizações disponíveis", "Versão do CyanogenPSP:", "OFICIAL", "Endereço Mac:", "Versão do Kernel:", "Versão do OSLib:"},
	{"CyanogenPSP oppdateringer", "Trykk for, vis eller installer tilgjengelige oppdateringer", "CyanogenPSP versjon:", "OFFISIELL", "Mac Adresse:", "Kernel versjon:", "OSLib versjon:"},
	{"Actualizări CyanogenPSP", "Apasă pentru a vedea și instala actualizări disponibile", "Versiune CyanogenPSP:", "OFICIAL", "Adresă Mac:", "Versiune de kernel:", "Versiune de OSLib:"},
	{"Aggiornamenti CyanogenPSP", "Clicca per, vedere o installare gli aggiornamenti disponibili", "Versione CyanogenPSP:", "UFFICIALE", "Indirizzo MAC:", "Versione kernel:", "Versione OSLib:"},
	{"Обновления CyanogenPSP", "Нажмите для проверки или установки обновлений", "Версия CyanogenPSP:", "OFFICIAL", "MAC-адрес:", "Версия ядра:", "Версия OSLib:"},
	{"Оновлення CyanogenPSP", "Натисніть, щоб перевірити і встановити оновлення", "Версія CyanogenPSP:", "OFFICIAL", "MAC-адрес:", "Версія ядра:", "Версія OSLib:"}
};

const char lang_settingsUpdates[][1][100] = 
{	
	{"Check for updates"},
	{"Vérifier la présence de mises à jour"},
	{"Sprawdz aktualizacje"},
	{"Auf Updates prüfen"},
	{"Updates zoeken"},
	{"Buscar actualizaciones"},
	{"Procurar por atualizações"},
	{"Checar por atualizações"},
	{"Se etter oppdateringer"},
	{"Caută actualizări"},
	{"Ricerca aggiornamenti"},
	{"Проверить обновления"},
	{"Перевірити оновлення"}
};

const char lang_settingsSecuirty[][3][100] = 
{	
	{"Password lock", "Pin lock", "Swipe"},
	{"Verrouillage par mot de passe", "Verrouillage par code PIN", "Chiper"},
	{"Zabezpieczenie haslem", "Zabezpieczenie kodem PIN", "Trzepnięcie"},
	{"Passwortsperre", "Pinsperre", "Stibitzen"},
	{"Wachtwoord vergrendeling", "PIN vergrendeling", "Slaan"},
	{"Bloqueo con contraseña", "Bloqueo con PIN", "Apandar"},
	{"Trava por senha", "Trava por PIN", "Pancada forte"},
	{"Trava de senha", "Trava de pino", "Pancada forte"},
	{"Passordlås", "Pin-lås", "Sveip"},
	{"Blocare cu parolă", "Blocare cu PIN", "Bețivan"},
	{"Blocco con password", "Blocco con PIN", "Swipe"},
	{"Защита паролем", "Защита PIN-кодом", "Жест"},
	{"Захист паролем", "Захист PIN-кодом", "Жест"}
};

const char lang_settingsWifi[][3][100] = 
{	
	{"No WiFi configuration found", "Please turn on the Wlan switch", "Press up or down to navigate through your Wifi configurations"},
	{"Aucune configuration Wi-Fi n'a été trouvée", "Veuillez activer le WLAN à l'aide du commutateur", "Appuyez sur haut ou bas pour vous déplacer à travers vos configurations Wi-Fi"},
	{"Nie znaleziono konfiguracji Wi-Fi", "Przelacz wlacznik WLAN", "Nacisnij w gore/w doł aby wybrac konfiguracje"},
	{"Keine WLAN-Einstellungen gefunden", "Bitte schalte den WLAN-Schalter ein", "Drücke Hoch oder Runter, um durch deine WLAN-Einstellungen zu navigieren"},
	{"Geen WiFi configuratie gevonden", "Schakel Wlan knop in alsjeblieft", "Druk omhoog of omlaag om een Wifi-instelling te selecteren"},
	{"No se encontró una configuración de Wi-Fi", "Por favor activa el interruptor del Wi-Fi", "Presiona arriba o abajo para navegar por tus configuraciones de Wi-Fi"},
	{"Nenhuma configuração de WiFi encontrada", "Favor ligar o Wlan", "Pressione cima ou baixo para navegar suas configurações de WiFi"},
	{"Nenhuma configuração de WiFi encontrada", "Por favor, ative o Wlan", "Use os botões direcionais (cima/baixo) para navegar através de suas configurações de Wifi"},
	{"Ingen WiFi konfigurasjon funnet", "Vennligst slå på Wlan bryteren", "Trykk opp eller ned for å navigere dine Wifi konfigurasjoner"},
	{"Nicio configurație WiFi găsită", "Glisează butonul de WLAN", "Apasă sus sau jos pentru a naviga configurațiile de WiFi"},
	{"Nessuna configurazione WiFi trovata", "Prego, accendere l'interuttore Wlan", "Premi su o giù per navigare tra le tue impostazioni WiFi"},
	{"Не найдено конфигураций Wi-Fi", "Пожалуйста, вкл. WLAN переключатель", "Нажмите вверх/вниз для выбора конфигурации Wi-Fi"},
	{"Не знайдено конфігурацій Wi-Fi", "Будь ласка, вкл. WLAN перемикач", "Натисніть вверх/вниз для вибору конфігурацій Wi-Fi"}
};

const char lang_settingsDeveloperOptions[][4][150] = 
{	
	{"Toggle Remote Joy Lite", "Displays your PSP screen on your computer via USB", "Toggle experimental features", "Dark theme"},
	{"Activer/Désactiver Remote Joy Lite", "Affiche l'écran de votre PSP sur votre ordinateur par USB", "Activer/Désactiver caractéristiques expérimentales", "Thème noir"},
	{"Wlącz Remote Joy Lite", "Wyswietla obraz z PSP na komputerze poprzez USB", "Wlacz funkcje eksperymentalne", "Dark theme"},
	{"Remote Joy Lite ein-/ausschalten", "Zeigt den Bildschirm deiner PSP via USB auf deinem Computer an", "Experimentelle Funktionen ein-/ausschalten", "Dunkles thema"},
	{"Remote Joy Lite aan/uit", "Geeft je PSP scherm weer op je computer via USB", "Experimentele functies aan/uit", "Donkere thema"},
	{"Usar Remote Joy Lite", "Muestra la imagen de la pantalla de tu PSP en tu PC vía USB", "Activar características experimentales", "Tema oscuro"},
	{"Alternar Remote Joy Lite", "Exibe a tela do seu PSP no seu PC por um cabo USB", "Alternar recursos experimentais", "Tema escuro"},
	{"Alternar Remote Joy Lite", "Exibir a tela de seu PSP em seu computador via USB", "Alternar recursos experimentais", "Tema escuro"},
	{"Veksle Remote Joy Lite", "Viser PSP skjermen på din PC gjennom USB", "Veksle eksperimentelle funksjoner", "Mørkt tema"},
	{"Activează Remote Joy Lite", "Afișează ecranul PSPului pe calculator prin USB", "Activează caracteristici experimentale", "Temă întuneric"},
	{"Attiva Remote Joy Lite", "Visualizza la tua schermata PSP sul tuo computer tramite USB", "Attiva funzioni sperimentali", "Tema scuro"},
	{"Вкл. Remote Joy Lite", "Отображение изображения с PSP на компьютер через USB", "Включить экспериментальные возможности", "Ночной режим"},
	{"Увм. Remote Joy Lite", "Відображення зображення з PSP на комп'ютер через USB", "Ввімкнути експериментальні можливості", "Нічний режим"}
};

const char lang_quickSettings[][5][100] = 
{	
	{"Wi-Fi", "Power save", "Balanced", "Performance", "Lockscreen"},
	{"Wi-Fi", "Économie de batterie", "Équilibrée", "Performance", "Écran de verrouillage"},
	{"Wi-Fi", "Oszczedzanie baterii", "Zbalansowane", "Wydajnosc", "Ekran blokady"},
	{"WLAN", "Energiesparmodus", "Ausgeglichen", "Leistung", "Bildschirmsperre"},
	{"Wi-Fi", "Accubesparing", "Gebalanceerd", "Prestatie", "Vergrendelscherm"},
	{"Wi-Fi", "Ahorro de Energía", "Balanceado", "Alto Rendimiento", "Pantalla de Bloqueo"},
	{"Wi-Fi", "Power save", "Balanceado", "Desempenho", "Tela de lock"},
	{"Wi-Fi", "Modo econômico", "Equilibrado", "Desempenhado", "Bloqueio de tela"},
	{"Wi-Fi", "Strømsparing", "Balansert", "Ytelse", "Låseskjerm"},
	{"Wi-Fi", "Economisire", "Echilibrat", "Performanță", "Ecran de pornire"},
	{"Wi-Fi", "Risparmio energetico", "Bilanciato", "Prestazioni", "Blocca schermo"},
	{"Wi-Fi", "Энергосберегающий", "Сбалансированный", "Производительность", "Блокировка экрана"},
	{"Wi-Fi", "Енергозберігаючий", "Збалансований", "Продуктивність", "Блокування екрану"}
};