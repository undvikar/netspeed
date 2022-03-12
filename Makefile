# netspeed status bar module, refreshed by dwmblocks
CC=gcc 
BINPATH=/usr/bin
CACHE=/var/lib/netspeed
all: netspeed

netspeed: netspeed.c
	$(CC) -Wall -o netspeed netspeed.c

clean:
	rm -f netspeed

install:
	mkdir -p $(CACHE)
	touch $(CACHE)/received
	touch $(CACHE)/transmitted
	chmod 666 $(CACHE)/received $(CACHE)/transmitted
	echo "0\n" > $(CACHE)/received
	echo "0\n" > $(CACHE)/transmitted
	cp -f netspeed $(BINPATH)
	chmod 755 $(BINPATH)/netspeed
