#include <stdio.h>
#include <string.h>

void frame2datas(char* frame, char datas[][20]){
	int j=0; //put 7
	printf("%s\n",frame);
	for (int i=0; i<6; i++){
		while (frame[j] != ',') {
			datas[i][j] = frame[j];
			j++;
		}
		datas[i][j+1]='\n';
		j++;
		printf("frame2datas %s\n",datas[i]);
	}
}

int main(){

	FILE* gps = fopen("/dev/serial/by-id/usb-u-blox_AG_-_www.u-blox.com_u-blox_7_-_GPS_GNSS_Receiver-if00", "r");
	char frame[100];
	char datas[6][20];

	for(int i=0; i<100; i++){
		fgets(frame, sizeof(frame), gps);
		if (strstr(frame,"$GPGLL")) {
			printf("%s",frame);
			frame2datas(frame,datas);
			for (int j=0; j<6; j++) {
				printf("%d\n",j);
				printf("%s",datas[j]);
			}
			printf("%c",'\n');
		}
	}

	fclose(gps);
	return 0;
}
