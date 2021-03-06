#include <mysql.h>
#include <iostream>
#include <map>
#include <vector>

#ifndef mysql_H_INCLUDED_
#define mysql_H_INCLUDED_

using namespace std;


class MySQL {
	
	private:
	string server;
	string user;
	string password;
	string database;
	
	MYSQL* mysqlConnection;
	
	public:
	MySQL(string server, string user, string password, string database);
	MySQL();
	~MySQL();

	map<string, vector<string>> query( string );

	bool connect();

	//TODO: Put some getters and setters
	void setConnectionDetails( string server, string user, string password, string database );
	void close();
};

#endif
