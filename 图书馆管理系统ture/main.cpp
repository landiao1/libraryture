#include <mysql_driver.h>  
#include <mysql_connection.h>  
#include <cppconn/statement.h>  
#include <cppconn/resultset.h>  
#include<cppconn/exception.h>  
#include<iostream>  
#include<string>  
#include<cppconn/prepared_statement.h>
#include <cstdlib>
#include "admin.h"
#include "reader.h"
#include <conio.h>
using namespace std;  
admin ad;
reader re;

#include <conio.h>  // 用于 _kbhit() 和 _getch()

void reader_login(sql::Connection* con)
{
    int choice;
    string username, password;
    char ch;
    reader re;  // 提前声明reader对象

    do {
        system("cls");
        cout << "\n===== 图书管理系统登录 =====" << endl;
        cout << "1. 读者登录" << endl;
        cout << "2. 读者注册" << endl;
        cout << "按esc返回" << endl;
        cout << "请输入选择: ";

        // 支持ESC退出的选择输入
        choice = -1;
        while ((ch = _getch()) != '\r') {  // 按Enter确认
            if (ch == 27) {  // ESC键
                return;  // 返回上一级菜单
            }
            else if (isdigit(ch)) {
                choice = ch - '0';  // 转换为数字
                cout << choice;
            }
        }
        cout << endl;

        if (choice == 1) {
            system("cls");
            cout << "读者登录" << endl;
            cout << "------------------------" << endl;
            cout << "按ESC返回上一级菜单" << endl;
            cout << "------------------------" << endl;
            cout << "请输入用户名: ";

            // 支持ESC退出的用户名输入
            username.clear();
            while ((ch = _getch()) != '\r') {  // 按Enter结束输入
                if (ch == 27) {  // ESC键
                    break;  // 跳出用户名输入循环
                }
                else if (ch == '\b') {  // 退格键
                    if (!username.empty()) {
                        username.pop_back();
                        cout << "\b \b";  // 回退并清除字符
                    }
                }
                else {
                    username += ch;
                    cout << ch;
                }
            }
            cout << endl;

            if (ch == 27) continue;  // 如果按了ESC，跳过密码输入

            cout << "请输入密码: ";

            // 支持ESC退出的密码输入
            password.clear();
            while ((ch = _getch()) != '\r') {  // 按Enter结束输入
                if (ch == 27) {  // ESC键
                    break;
                }
                else if (ch == '\b') {  // 退格键
                    if (!password.empty()) {
                        password.pop_back();
                        cout << "\b \b";  // 回退并清除字符
                    }
                }
                else {
                    password += ch;
                    cout << '*';  // 显示星号代替实际字符
                }
            }
            cout << endl;

            if (ch == 27) continue;  // 如果按了ESC，跳过登录验证

            try {
                // 查询读者表验证用户名和密码
                sql::PreparedStatement* pstmt = con->prepareStatement(
                    "SELECT username FROM users WHERE username = ? AND password = ?"
                );
                pstmt->setString(1, username);
                pstmt->setString(2, password);
                sql::ResultSet* res = pstmt->executeQuery();

                if (res->next()) {
                    // 登录成功，获取 user_name
                    string user_name1 = res->getString("username");

                    system("cls");
                    cout << "登录成功！欢迎读者 " << username << endl;
                    re.setUsername(user_name1);
                    re.readerview(con);  // 直接跳转读者界面
                }
                else {
                    cout << "用户名或密码错误！" << endl;
                }

                delete res;
                delete pstmt;
            }
            catch (const sql::SQLException& e) {
                cout << "数据库错误: " << e.what() << endl;
            }

            // 等待用户按回车返回菜单
            cin.ignore();
            cout << "按回车键返回菜单...";
            cin.get();

        }
        else if (choice == 0) {
            cout << "返回上一级菜单..." << endl;
            break;
        }
        else if (choice == 2) {
            re.Register(con);
        }
        else if (choice != -1) {  // 排除ESC退出时的无效提示
            cout << "无效的选择，请重新输入!" << endl;
            cin.ignore();
            cout << "按回车键继续...";
            cin.get();
        }
    } while (true);
}
void admin_login(sql::Connection* con)
{
    string adminname, password;
    char ch;

    do {
        system("cls");
        cout << "管理员登录" << endl;
        cout << "------------------------" << endl;
        cout << "按ESC返回上一级菜单" << endl;
        cout << "------------------------" << endl;
        cout << "请输入用户名: ";

        // 支持ESC退出的用户名输入
        adminname.clear();
        while ((ch = _getch()) != '\r') {  // 按Enter结束输入
            if (ch == 27) {  // ESC键的ASCII码是27
                return;  // 直接返回上一级菜单
            }
            else if (ch == '\b') {  // 退格键
                if (!adminname.empty()) {
                    adminname.pop_back();
                    cout << "\b \b";  // 回退并清除字符
                }
            }
            else {
                adminname += ch;
                cout << ch;
            }
        }
        cout << endl;

        cout << "请输入密码: ";

        // 支持ESC退出的密码输入（密码不显示）
        password.clear();
        while ((ch = _getch()) != '\r') {  // 按Enter结束输入
            if (ch == 27) {  // ESC键
                return;
            }
            else if (ch == '\b') {  // 退格键
                if (!password.empty()) {
                    password.pop_back();
                    cout << "\b \b";  // 回退并清除字符
                }
            }
            else {
                password += ch;
                cout << '*';  // 显示星号代替实际字符
            }
        }
        cout << endl;

        try {
            // 查询管理员表验证用户名和密码
            sql::PreparedStatement* pstmt = con->prepareStatement(
                "SELECT adminname FROM admin WHERE adminname = ? AND password = ?"
            );
            pstmt->setString(1, adminname);
            pstmt->setString(2, password);
            sql::ResultSet* res = pstmt->executeQuery();

            if (res->next()) {
                // 登录成功
                system("cls");
                cout << "登录成功！欢迎管理员 " << adminname << endl;
                ad.adminview(con);  // 进入管理员界面
                return;  // 从管理员界面返回后直接退出登录函数
            }
            else {
                cout << "用户名或密码错误！" << endl;
            }

            delete res;
            delete pstmt;
        }
        catch (const sql::SQLException& e) {
            cout << "数据库错误: " << e.what() << endl;
        }

        // 等待用户按回车继续
        cout << "按回车键继续...";
        cin.ignore();
        cin.get();
    } while (true);
}
void login(sql::Connection* con)
{
    int choice;
    do
    {
        system("cls");
        cout << "\n===== 图书管理系统 =====" << endl;
        cout << "1. 读者登录:" << endl;
        cout << "2. 管理员登录:" << endl;
        cout << "0. 退出系统:" << endl;
        cin >> choice;

        switch (choice) {
        case 1:
            system("cls");
            reader_login(con);
            break;
        case 2:
            system("cls");
            admin_login(con);
            break;
        case 0:
            cout << "退出系统..." << endl;
            break;
        default:
            cout << "无效的选择，请重新输入!" << endl;
            cin.ignore(); // 清除输入缓冲区
            cin.get();   // 等待用户按回车
        }
    } while (choice != 0);


}
int main()  
{  
    try
    {
        sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
        sql::Connection* con = driver->connect("tcp://localhost:3306", "root", "123456");
        con->setSchema("123321");
        if (con->isValid()) {
            cout << "数据库连接有效" << endl;
            login(con);
        }
        else {
            cout << "连接已失效" << endl;
        }
        
        delete con;
    }
    catch (sql::SQLException& e)
    {
        cout << "Connection Error: " << e.what() << endl;
        
    }

   return 0;  
}