#pragma once
#include <mysql_driver.h>  
#include <mysql_connection.h>  
#include <cppconn/statement.h>  
#include <cppconn/resultset.h>  
#include<cppconn/exception.h>  
#include<iostream>  
#include<string>  
#include<cppconn/prepared_statement.h>
class admin
{
private:

public:
	void adminview(sql::Connection* con);
	void addbooks(sql::Connection* con);
	void showallbooks(sql::Connection* con);
	void updatebook(sql::Connection* con);
    void deletebook(sql::Connection* con);
	
	void showallrecord(sql::Connection* con);//�ڲ鿴��¼ʱ���� ��ʾ�������е������¼



};

