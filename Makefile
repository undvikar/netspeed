# netspeed status bar module, refreshed by dwmblocks
CC=gcc 
BINPATH=/home/dominik/.local/bin
CACHE=/home/dominik/.cache/netspeed
netspeed: netspeed.c
	$(CC) -o netspeed netspeed.c

clean:
	rm -f netspeed

install:
	mkdir -p $(CACHE)
	touch $(CACHE)/received
	touch $(CACHE)/transmitted
	cp -f netspeed $(BINPATH)
	chmod 755 $(BINPATH)/netspeed
