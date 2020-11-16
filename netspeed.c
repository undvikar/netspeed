/*Based on https://dwm.suckless.org/status_monitor/dwmstatus-netusage.c.
  To be used with dwmblocks.
*/

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define INTERFACES "/proc/net/dev"
#define RECFILE "/tmp/netspeed/received"
#define TRANSFILE "/tmp/netspeed/transmitted"

void
get_new_total (unsigned long long int *received, unsigned long long int *transmitted){
    static int bufsize = 255;
    char buf[bufsize];
    FILE *devfd;
    char *start;
    unsigned long long int sum_received = 0;
    unsigned long long int sum_transmitted = 0;

    devfd = fopen(INTERFACES, "r");

    //ignore first two lines
    fgets(buf, bufsize, devfd);
    fgets(buf, bufsize, devfd);

    while(fgets(buf, bufsize, devfd)){
        unsigned long long int in;
        unsigned long long int out;
        //ignore loopback interface
        if (strstr(buf, "lo:") == NULL){
            start = strstr(buf,":");
            sscanf(start + 1, "%llu  %*d     %*d  %*d  %*d  %*d   %*d        %*d       %llu", &in, &out);
            sum_received += in;
            sum_transmitted += out;
        }
    }
    *received = sum_received;
    *transmitted = sum_transmitted;
    fclose(devfd);
}

void
calculate_speed(char *speedstr, unsigned long long int *newval, unsigned long long int *oldval){
    double speed;
    speed = (*newval - *oldval) / 1024.0; //result in KB/s
    if (speed > 1024.0) { 
        speed /= 1024.0;    //result in MB/s
        sprintf (speedstr, "%.2f MB/s", speed);
    } else {
        sprintf (speedstr, "%.2f KB/s", speed);
    }
}




int
main(){
    char downspeedstr[15], upspeedstr[15];
    
    unsigned long long int received = 0;
    unsigned long long int transmitted = 0;
    unsigned long long int old_tm = 0;
    unsigned long long int old_rc = 0;

    FILE *rec = fopen(RECFILE, "r+");
    FILE *transm = fopen(TRANSFILE, "r+");
    
    fscanf(rec, "%llu\n", &old_rc);
    fscanf(transm, "%llu\n", &old_tm);
    /*move both streams back to the start of the file*/
    rewind(rec);
    rewind(transm);

    get_new_total (&received, &transmitted);
    calculate_speed (downspeedstr, &received, &old_rc);
    calculate_speed (upspeedstr, &transmitted, &old_tm);
    fprintf(rec, "%llu\n", received);
    fprintf(transm, "%llu\n", transmitted);

    printf("DOWN: %s UP: %s \n", downspeedstr, upspeedstr);
    fclose(rec);
    fclose(transm);
}
