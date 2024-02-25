#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

// Prblm de chars non-ASCII à la fin du buffer si trop petit
// soucis d'affichage/lecture si utilisation fopen,fread,fclose

int main(){
	int fd = open("/dev/serial/by-id/usb-u-blox_AG_-_www.u-blox.com_u-blox_7_-_GPS_GNSS_Receiver-if00",O_RDONLY);
	char buffer[100];
	char* substr;

	while(1) {
		read(fd,&buffer,100);
		if(strstr(buffer, "$GPGLL,")){
			substr = strtok (buffer,",");
  			for(int i=0;i<6;i++)
  			{
    				printf ("%s\n",substr);
    				substr = strtok (NULL, ",");
  			}
		}
	}

	close(fd);
	
	return 0;
}
