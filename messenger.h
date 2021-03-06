#include <pspkernel.h>
#include <oslib/oslib.h>

OSL_IMAGE *messengerbg, *new_message, *cursor;

int InitAdhocMessenger();
void joinServer();
void createServer();

void printInfo();
void clientConnected(struct remotePsp *aPsp);
void doClient();
void serverConnected(struct remotePsp *aPsp);
void doServer();
void activateOSK();
void newMessage();
int messenger();