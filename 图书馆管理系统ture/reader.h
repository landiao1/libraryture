#pragma once
#include<string>
#include <mysql_driver.h>  
#include <mysql_connection.h>  
#include <cppconn/statement.h>  
#include <cppconn/resultset.h>  
#include<cppconn/exception.h>  
#include<iostream>  
#include<string>  
#include<cppconn/prepared_statement.h>
using namespace std;
class reader
{
private:
	string user_name;
public:
	
	void setUsername(const string& username) {
		user_name = username;
	}
	string getUsername() const {
		return user_name;
	}
	void readerview(sql::Connection* con);
	void Register(sql::Connection* con);
	void search_book(sql::Connection* con);
	void borrow_book(sql::Connection* con);
	void return_book(sql::Connection* con);
	void showall_book(sql::Connection* con);
	void showall_BorrowingRecord(sql::Connection* con);//仅显示自己的申请记录

};

