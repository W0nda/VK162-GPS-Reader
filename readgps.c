#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

//PostgreSQL lib
#include <libpq-fe.h>

void flush(char* array, int size){
	for(int i=0; i<size; i++){
		array[i] = 0x0;
	}
}

int main(){
	//GPS
	int fd = open("/dev/serial/by-id/usb-u-blox_AG_-_www.u-blox.com_u-blox_7_-_GPS_GNSS_Receiver-if00",O_RDONLY);
	char buffer[100];
	char* substr;

	//DB
	PGconn* dbConn = PQconnectdb("user=chirpstack password=chirpstack dbname=chirpstack");
	if (PQstatus(dbConn) == CONNECTION_OK) {
		printf("Pizzaaa!!\n");
	}
	char request[200]; 

	while(1) {
		strcpy(request,"INSERT INTO coordinates(lat,latcard,lon,loncard,time) VALUES (");
		read(fd,&buffer,100);
		if(strstr(buffer, "$GPGLL,")){
			substr = strtok (buffer,",");
  			for(int i=0;i<6;i++)
  			{
				if(i!=0){
    					printf ("%s\n",substr);
					strcat(request,substr);
					strcat(request,",");
				}
    				substr = strtok (NULL, ",");
  			}
			strcat(request,");\n");
			printf("Requests: %s\n",request);
			flush(request,sizeof(request));
		}
		flush(buffer,sizeof(buffer));
	}

	PQfinish(dbConn);
	close(fd);
	
	return 0;
}
