APXS = apxs
SOURCES = mod_reset.c
INCLUDES = /usr/include/php/main /usr/include/php/Zend /usr/include/php/TSRM

main:
	$(APXS) -c -i $(foreach i, $(INCLUDES), -I$i) $(SOURCES)
clean:
	rm mod_reset.o mod_reset.slo mod_reset.lo mod_reset.la
