## netspeed
A small utility to be used with [dwmblocks](https://github.com/torrinfail/dwmblocks) to display down- and upload speed, written in C.

## Installation
Specify the interface, label and units in `config.h`.

Run `make`, then `sudo make install`. After that, add an entry `netspeed` to the blocks.h file with update interval and signal both set to 1. This will run netspeed every second, effectively giving you the down- and upload speed.
![example](example.png)
