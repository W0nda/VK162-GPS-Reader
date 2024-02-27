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

void selstr(char* array){
	for (int i=0; i<11; i++){
		array[i]=array[7+i];
	}

	array[11]='\''; // 'N'
	array[12]=array[18];
	array[13]='\'';
	//OK

	for(int i=14; i<27; i++){
		array[i]=array[5+i];
	}

	array[27]='\''; // 'W'
	array[28]=array[32];
	array[29]='\'';

	for(int i=30; i<40; i++){
		array[i]=array[3+i];
	}

	array[40]='\0';
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
		read(fd,&buffer,100);
		if(strstr(buffer, "$GPGLL,")){
			//preparing buffer
			printf("%s\n",buffer);
			selstr(buffer);
			printf("%s\n",buffer);

			//preparing request
			strcat(request, buffer);
			strcat(request,");\n");
			printf("Requests: %s\n",request);

			res = PQexec(dbConn, request); //don't work(prbm de guillemet pour str)
			if (PQresultStatus(res) == PGRES_COMMAND_OK){
				printf("Ooooh yeah!!");
			}else{
				printf("%s\n",PQerrorMessage(dbConn));
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
