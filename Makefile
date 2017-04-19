APXS = apxs
SOURCES = mod_reset.c
INCLUDES = /usr/include/php/main /usr/include/php/Zend /usr/include/php/TSRM

main:
	$(APXS) -c $(CFLAGS) -i $(foreach i, $(INCLUDES), -I$i) $(SOURCES)
debug:
	$(APXS) -c $(CFLAGS) -Wc,-O0 -i $(foreach i, $(INCLUDES), -I$i) $(SOURCES)
ruid:
	$(APXS) $(foreach m, MOD_RUID2, -D$m) -c $(CFLAGS) -i $(foreach i, $(INCLUDES), -I$i) $(SOURCES)
nophp:
	$(APXS) $(foreach m, NO_PHP, -D$m) -c $(CFLAGS) -i $(SOURCES)
clean:
	rm mod_reset.o mod_reset.slo mod_reset.lo mod_reset.la
