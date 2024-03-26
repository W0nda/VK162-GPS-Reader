#include <stdio.h>
#include <string.h>

void frame2datas(char* frame, char datas[][20]){
        int save=7;
        int j=0;
        for (int i=0; i<5; i++){
                while (frame[save+j] != ',') {
                        datas[i][j] = frame[save+j];
                        j++;
                }
                datas[i][j]='\0';
                j++;
                save=save+j;
                j=0;
        }
}

int main(){

        FILE* gps = fopen("/dev/serial/by-id/usb-u-blox_AG_-_www.u-blox.com_u-blox_7_-_GPS_GNSS_Receiver-if00", "r");
        char frame[100];
        char datas[5][20];

        for(int i=0; i<100; i++){
                fgets(frame, sizeof(frame), gps);
                if (strstr(frame,"$GPGLL")) {
                        frame2datas(frame,datas);
                        for (int j=0; j<5; j++) {
                                printf("%s\n",datas[j]);
                        }
                }
        }

        fclose(gps);
        return 0;
}
