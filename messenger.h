#include <pspkernel.h>
#include <oslib/oslib.h>

OSL_IMAGE *messengerbg, *new_message, *cursor;
OSL_FONT *pgfFont;

int InitAdhocMessenger();
void joinServer();
void createServer();

char message[500] = "";	

void printInfo();
void clientConnected(struct remotePsp *aPsp);
void doClient();
void serverConnected(struct remotePsp *aPsp);
void doServer();
void activateOSK();
void newMessage();
int messenger();