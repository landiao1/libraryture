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

#include <conio.h>  // ���� _kbhit() �� _getch()

void reader_login(sql::Connection* con)
{
    int choice;
    string username, password;
    char ch;
    reader re;  // ��ǰ����reader����

    do {
        system("cls");
        cout << "\n===== ͼ�����ϵͳ��¼ =====" << endl;
        cout << "1. ���ߵ�¼" << endl;
        cout << "2. ����ע��" << endl;
        cout << "��esc����" << endl;
        cout << "������ѡ��: ";

        // ֧��ESC�˳���ѡ������
        choice = -1;
        while ((ch = _getch()) != '\r') {  // ��Enterȷ��
            if (ch == 27) {  // ESC��
                return;  // ������һ���˵�
            }
            else if (isdigit(ch)) {
                choice = ch - '0';  // ת��Ϊ����
                cout << choice;
            }
        }
        cout << endl;

        if (choice == 1) {
            system("cls");
            cout << "���ߵ�¼" << endl;
            cout << "------------------------" << endl;
            cout << "��ESC������һ���˵�" << endl;
            cout << "------------------------" << endl;
            cout << "�������û���: ";

            // ֧��ESC�˳����û�������
            username.clear();
            while ((ch = _getch()) != '\r') {  // ��Enter��������
                if (ch == 27) {  // ESC��
                    break;  // �����û�������ѭ��
                }
                else if (ch == '\b') {  // �˸��
                    if (!username.empty()) {
                        username.pop_back();
                        cout << "\b \b";  // ���˲�����ַ�
                    }
                }
                else {
                    username += ch;
                    cout << ch;
                }
            }
            cout << endl;

            if (ch == 27) continue;  // �������ESC��������������

            cout << "����������: ";

            // ֧��ESC�˳�����������
            password.clear();
            while ((ch = _getch()) != '\r') {  // ��Enter��������
                if (ch == 27) {  // ESC��
                    break;
                }
                else if (ch == '\b') {  // �˸��
                    if (!password.empty()) {
                        password.pop_back();
                        cout << "\b \b";  // ���˲�����ַ�
                    }
                }
                else {
                    password += ch;
                    cout << '*';  // ��ʾ�ǺŴ���ʵ���ַ�
                }
            }
            cout << endl;

            if (ch == 27) continue;  // �������ESC��������¼��֤

            try {
                // ��ѯ���߱���֤�û���������
                sql::PreparedStatement* pstmt = con->prepareStatement(
                    "SELECT username FROM users WHERE username = ? AND password = ?"
                );
                pstmt->setString(1, username);
                pstmt->setString(2, password);
                sql::ResultSet* res = pstmt->executeQuery();

                if (res->next()) {
                    // ��¼�ɹ�����ȡ user_name
                    string user_name1 = res->getString("username");

                    system("cls");
                    cout << "��¼�ɹ�����ӭ���� " << username << endl;
                    re.setUsername(user_name1);
                    re.readerview(con);  // ֱ����ת���߽���
                }
                else {
                    cout << "�û������������" << endl;
                }

                delete res;
                delete pstmt;
            }
            catch (const sql::SQLException& e) {
                cout << "���ݿ����: " << e.what() << endl;
            }

            // �ȴ��û����س����ز˵�
            cin.ignore();
            cout << "���س������ز˵�...";
            cin.get();

        }
        else if (choice == 0) {
            cout << "������һ���˵�..." << endl;
            break;
        }
        else if (choice == 2) {
            re.Register(con);
        }
        else if (choice != -1) {  // �ų�ESC�˳�ʱ����Ч��ʾ
            cout << "��Ч��ѡ������������!" << endl;
            cin.ignore();
            cout << "���س�������...";
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
        cout << "����Ա��¼" << endl;
        cout << "------------------------" << endl;
        cout << "��ESC������һ���˵�" << endl;
        cout << "------------------------" << endl;
        cout << "�������û���: ";

        // ֧��ESC�˳����û�������
        adminname.clear();
        while ((ch = _getch()) != '\r') {  // ��Enter��������
            if (ch == 27) {  // ESC����ASCII����27
                return;  // ֱ�ӷ�����һ���˵�
            }
            else if (ch == '\b') {  // �˸��
                if (!adminname.empty()) {
                    adminname.pop_back();
                    cout << "\b \b";  // ���˲�����ַ�
                }
            }
            else {
                adminname += ch;
                cout << ch;
            }
        }
        cout << endl;

        cout << "����������: ";

        // ֧��ESC�˳����������루���벻��ʾ��
        password.clear();
        while ((ch = _getch()) != '\r') {  // ��Enter��������
            if (ch == 27) {  // ESC��
                return;
            }
            else if (ch == '\b') {  // �˸��
                if (!password.empty()) {
                    password.pop_back();
                    cout << "\b \b";  // ���˲�����ַ�
                }
            }
            else {
                password += ch;
                cout << '*';  // ��ʾ�ǺŴ���ʵ���ַ�
            }
        }
        cout << endl;

        try {
            // ��ѯ����Ա����֤�û���������
            sql::PreparedStatement* pstmt = con->prepareStatement(
                "SELECT adminname FROM admin WHERE adminname = ? AND password = ?"
            );
            pstmt->setString(1, adminname);
            pstmt->setString(2, password);
            sql::ResultSet* res = pstmt->executeQuery();

            if (res->next()) {
                // ��¼�ɹ�
                system("cls");
                cout << "��¼�ɹ�����ӭ����Ա " << adminname << endl;
                ad.adminview(con);  // �������Ա����
                return;  // �ӹ���Ա���淵�غ�ֱ���˳���¼����
            }
            else {
                cout << "�û������������" << endl;
            }

            delete res;
            delete pstmt;
        }
        catch (const sql::SQLException& e) {
            cout << "���ݿ����: " << e.what() << endl;
        }

        // �ȴ��û����س�����
        cout << "���س�������...";
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
        cout << "\n===== ͼ�����ϵͳ =====" << endl;
        cout << "1. ���ߵ�¼:" << endl;
        cout << "2. ����Ա��¼:" << endl;
        cout << "0. �˳�ϵͳ:" << endl;
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
            cout << "�˳�ϵͳ..." << endl;
            break;
        default:
            cout << "��Ч��ѡ������������!" << endl;
            cin.ignore(); // ������뻺����
            cin.get();   // �ȴ��û����س�
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
            cout << "���ݿ�������Ч" << endl;
            login(con);
        }
        else {
            cout << "������ʧЧ" << endl;
        }
        
        delete con;
    }
    catch (sql::SQLException& e)
    {
        cout << "Connection Error: " << e.what() << endl;
        
    }

   return 0;  
}