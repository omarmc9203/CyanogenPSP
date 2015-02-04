make
DEL *.o
DEL *.ELF
bin2c kuman.prx kuman.h kuman
cp kuman.h ../modules/kuman.h
#psp-build-exports -s exports.exp
pause