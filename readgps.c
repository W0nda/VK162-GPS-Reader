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

void selstr(char* array, int start, int n){
	for (int i=0; i<n; i++) {
		array[i] = array[start+i];
	}
	array[n]='\0';
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
	PGresult* res;

	while(1) {
		strcpy(request,"INSERT INTO coordinates(lat,latcard,lon,loncard,time) VALUES (");
		//table a changer dans DB: FLOAT4 -> FLOAT5
		read(fd,&buffer,100);
		if(strstr(buffer, "$GPGLL,")){
			printf("%s\n",buffer);
			selstr(buffer, 7, 36);
			printf("%s\n",buffer);
			strcat(request, buffer);
			strcat(request,");\n");
			printf("Requests: %s\n",request);
			res = PQexec(dbConn, request); //don't work(prbm de guillemet pour str)
			if (PQresultStatus(res) == PGRES_COMMAND_OK){
				printf("Ooooh yeah!!");
			}
			PQclear(res);
			flush(request,sizeof(request));
		}
		flush(buffer,sizeof(buffer));
	}

	PQfinish(dbConn);
	close(fd);
	
	return 0;
}
