#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "config.h"

#define RECFILE "/var/lib/netspeed/received"
#define TRANSFILE "/var/lib/netspeed/transmitted"

void
get_new_total(unsigned long long int *received, unsigned long long int *transmitted, char *rx_path, char *tx_path) {
    unsigned long long int rx_bytes = 0;
    unsigned long long int tx_bytes = 0;
    FILE *rx_fd;
    FILE *tx_fd;

    rx_fd = fopen(rx_path, "r");
    tx_fd = fopen(tx_path, "r");
    fscanf(rx_fd, "%llu", &rx_bytes);
    fscanf(tx_fd, "%llu", &tx_bytes);
    *received = rx_bytes;
    *transmitted = tx_bytes;
    fclose(rx_fd);
    fclose(tx_fd);
}

void
calculate_speed(char *speedstr, unsigned long long int *newval, unsigned long long int *oldval) {
    double speed;
    if (units == 1) {
        speed = (*newval - *oldval) * 8 / 1000.0;
        if (speed > 1000.0) { 
            speed /= 1000.0;
            sprintf (speedstr, "%.2f Mb/s", speed);
        } else {
            sprintf (speedstr, "%.2f Kb/s", speed);
        }
    } else {
        speed = (*newval - *oldval) / 1024.0;
        if (speed > 1024.0) { 
            speed /= 1024.0;
            sprintf (speedstr, "%.2f MB/s", speed);
        } else {
            sprintf (speedstr, "%.2f KB/s", speed);
        }
    }
}

int
main(){
    // should be enough space for the additional interface name
    // and subsequent files
    char rx_path[60] = "/sys/class/net/";
    char tx_path[60] = "/sys/class/net/";
    char downspeedstr[15], upspeedstr[15];
    unsigned long long int received = 0;
    unsigned long long int transmitted = 0;
    unsigned long long int old_tm = 0;
    unsigned long long int old_rc = 0;

    strncat(rx_path, interface, 10);
    strncat(rx_path, "/statistics/rx_bytes", 21);
    strncat(tx_path, interface, 10);
    strncat(tx_path, "/statistics/tx_bytes", 21);

    FILE *rec = fopen(RECFILE, "r+");
    FILE *transm = fopen(TRANSFILE, "r+");
    
    fscanf(rec, "%llu", &old_rc);
    fscanf(transm, "%llu", &old_tm);
    /*move both streams back to the start of the file*/
    rewind(rec);
    rewind(transm);

    get_new_total(&received, &transmitted, rx_path, tx_path);
    calculate_speed(downspeedstr, &received, &old_rc);
    calculate_speed(upspeedstr, &transmitted, &old_tm);
    rec = freopen(RECFILE, "w", rec);
    transm = freopen(TRANSFILE, "w", transm);
    fprintf(rec, "%llu", received);
    fprintf(transm, "%llu", transmitted);
    printf("%s  %11s  %11s \n", label, downspeedstr, upspeedstr);
    fclose(rec);
    fclose(transm);
}
