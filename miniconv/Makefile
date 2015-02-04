all:
	$(MAKE) cjk
	$(MAKE) eur
	$(MAKE) both
eur:
	$(MAKE) -f Makefile.eur clean
	$(MAKE) -f Makefile.eur
cjk:
	$(MAKE) -f Makefile.cjk clean
	$(MAKE) -f Makefile.cjk

both:
	$(MAKE) -f Makefile.all clean
	$(MAKE) -f Makefile.all

clean:
	rm -f *.bak
	rm -f *.o
	rm -f *.elf
	rm -f *.prx