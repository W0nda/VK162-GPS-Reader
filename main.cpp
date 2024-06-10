#include <cstddef>
#include <mysql/mysql.h>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

using namespace std;

int main(){
	// mysql_library_init & mysql_library_end can be avoided in nonmultithreadedenvironment
	// (called mysql_init/close if needed)
	MYSQL* conn = mysql_init(NULL);

	fstream gps("/dev/ttyACM0",ios_base::in);
	string gpsFrame;


	//TEMPS
	//Mettre dans la boucle & Trouver plus propre
	chrono::system_clock::time_point now = chrono::system_clock::now();
	time_t cdate = chrono::system_clock::to_time_t(now);
	tm* realdate = localtime(&cdate);

	//CONNEXION A LA BDD
	if( mysql_real_connect(conn,"192.168.12.22","remote_user","bonjour","bdd",3306,NULL,0) == NULL ){
			printf("Connexion error: %s\n",mysql_error(conn));
			mysql_close(conn);
			return 1;
	}

	//LECTURE DE LA SORTIE DU GPS
	cout << "Searching";
	do{
		getline(gps,gpsFrame);
		cout << ".";
	}
	while(gpsFrame.find("$GPGLL") == string::npos);
	cout << endl;
	cout << gpsFrame << endl;

	//FORMATAGE POUR FABRICATION DE LA REQUETE
	float lat = stof(gpsFrame.substr(7,10))/100;
	float lon = stof(gpsFrame.substr(20,10))/100;
	cout << lat << "; " << lon << "; " << endl;
	if (gpsFrame.substr(7,10)[0] == '0') {
		lat = lat / (-1);
	}
	if (gpsFrame.substr(20,10)[0] == '0') {
		lon = lon / (-1);
	}
	cout << lat << "; " << lon << "; " << endl;

	//FABRICATION DE LA REQUETE
	string request("INSERT INTO datas (Date,Heures,Latitude,Longitude) VALUES ");
	request += "('" + to_string(realdate->tm_year+1900) + "-" + "0" + to_string(realdate->tm_mon + 1 ) + "-" + to_string(realdate->tm_mday) + "','";
	request += to_string(realdate->tm_hour) + ":" + to_string(realdate->tm_min) + ":" + to_string(realdate->tm_sec) + "','";
	request += to_string(lat) + "','" + to_string(lon) + "');";
	cout << request << endl;

	//ENVOI DE LA REQUETE
	if( mysql_query(conn, request.c_str()) ){
		cout << "Query error: " << mysql_error(conn) << endl;;
		mysql_close(conn);
		return 1;
	}

	MYSQL_RES* result = mysql_store_result(conn);

	cout << "Query result:" << result << endl;

	//DESTRUCTION DES OBJETS
	gps.close();
	mysql_free_result(result);
	mysql_close(conn);

	return 0;
}
