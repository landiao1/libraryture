#include "admin.h"
#include <mysql_driver.h>  
#include <mysql_connection.h>  
#include <cppconn/statement.h>  
#include <cppconn/resultset.h>  
#include<cppconn/exception.h>  
#include<iostream>  
#include<string>  
#include<cppconn/prepared_statement.h>
using namespace std;

void admin::addbooks(sql::Connection* con)
{
    try
    {
        string title, author, type, status;
        cout << "����:" << endl;
        cin.ignore();
        getline(cin, title); // Corrected variable name from "tltle" to "title"  

        cout << "����:" << endl;
        cin.ignore();
        getline(cin, author);

        cout << "����:" << endl;
        cin.ignore();
        getline(cin, type);

        cout << "״̬:"<<endl;
        cin.ignore();
        getline(cin, status);

        sql::PreparedStatement* pstmt = con->prepareStatement("INSERT INTO books(title,author,type,status) VALUES(?,?,?,?)");
        pstmt->setString(1, title);
        pstmt->setString(2, author);
        pstmt->setString(3, type);
        pstmt->setString(4, status);
        pstmt->execute();
        cout << "��ӳɹ�";
        delete pstmt;



    }
    catch (const sql::SQLException& e) {  // ��������쳣����
        cout << "���ݿ����: " << e.what() << endl;
        cout << "�������: " << e.getErrorCode() << endl;
        cout << "SQL״̬: " << e.getSQLState() << endl;
    }
    catch (const std::exception& e) {    // ����������׼�쳣
        cout << "��׼�쳣: " << e.what() << endl;
    }
    catch (...) {                        // ��������δ֪�쳣
        cout << "δ֪����" << endl;
    }

}
void admin::showallbooks(sql::Connection* con)
{
    try
    {
        sql::Statement* stmt = con->createStatement();
        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM books");
        cout << "�鼮�б�" << endl;
        while (res->next())
        {
            cout << "id:" << res->getInt("book_id") << endl;
            cout << "����:" << res->getString("title") << endl;
            cout << "����:" << res->getString("author") << endl;
            cout << "����:" << res->getString("type") << endl;
            cout << "״̬:" << res->getString("status") << endl;
        }
        delete stmt;
    }
    catch (const std::exception&)
    {
        cout << "����" << endl;
    }

}
void admin::updatebook(sql::Connection* con)
{
    try
    {
        string book_id;
        string title, author, type, status;


        cout << "����Ҫ���ĵ�id:";
        cin >> book_id;

        cout << "shuming";
        cin.ignore();
        getline(cin, title); // Corrected variable name from "tltle" to "title"  

        cout << "zuozhe";
        cin.ignore();
        getline(cin, author);

        cout << "leixing";
        cin.ignore();
        getline(cin, type);

        cout << "zhuangtai";
        cin.ignore();
        getline(cin, status);

        sql::PreparedStatement* pstmt = con->prepareStatement("UPDATE books SET title = ?,author = ?,type = ?,status = ? WHERE book_id = ?");
        pstmt->setString(1, title);
        pstmt->setString(2, author);
        pstmt->setString(3, type);
        pstmt->setString(4, status);
        pstmt->setString(5, book_id);

        pstmt->execute();
        delete pstmt;
        cout << "�޸ĳɹ�" << endl;
    }
    catch (const std::exception&)
    {
        cout << "�޸�ʧ��" << endl;
    }


}
void admin::deletebook(sql::Connection* con)
{
    int book_id;
    cout << "����Ҫɾ�������Id��" << endl;
    cin >> book_id;

    sql::PreparedStatement* pstmt = con->prepareStatement("DELETE FROM books WHERE book_id = ?");
    pstmt->setInt(1, book_id);

    pstmt->execute();
    cout << "ɾ���ɹ�" << endl;

    delete pstmt;
}
void admin::showallrecord(sql::Connection* con)
{
    try
    {
        string number;
        string decide;
        sql::Statement* satmt = con->createStatement();
        sql::ResultSet* res = satmt->executeQuery("SELECT * FROM borrow_records");

        cout << "���������б�" << endl;
        while (res->next())
        {
            cout << "�������:" << res->getInt("record_id") << endl;
            cout << "�û���:" << res->getString("username") << endl;
            cout << "ͼ��id:" << res->getString("book_id") << endl;
            cout << "��������:" << res->getString("borrow_date") << endl;
            cout << "�黹����:" << res->getString("return_date") << endl;
            cout << "����״̬:" << res->getString("status") << endl;
        }
        delete res;
        delete satmt;

        cout << "����Ҫ�����ļ�¼�����:" << endl;
        cin.ignore();
        getline(cin, number);  // ע�����ﲻ����Ҫignore()

        cout << "�����������(passͨ�� refuse�ܾ�)" << endl;
        getline(cin, decide);

        // ���������ת��Ϊ����
        int recordId = stoi(number);
        string bookId = "";  // ���ڱ���ͼ��ID

        if (decide == "pass")
        {
            // 1. �Ȳ�ѯ��ȡͼ��ID
            sql::PreparedStatement* selectStmt = con->prepareStatement(
                "SELECT book_id FROM borrow_records WHERE record_id = ?");
            selectStmt->setInt(1, recordId);
            sql::ResultSet* bookRes = selectStmt->executeQuery();

            if (bookRes->next()) {
                bookId = bookRes->getString("book_id");
            }
            else {
                cout << "δ�ҵ���Ӧ�Ľ��ļ�¼��" << endl;
                delete selectStmt;
                delete bookRes;
                return;
            }

            delete bookRes;
            delete selectStmt;

            // 2. ��������ȷ������һ����
            con->setAutoCommit(false);  // �����Զ��ύ

            try {
                // ���½��ļ�¼״̬
                sql::PreparedStatement* recordStmt = con->prepareStatement(
                    "UPDATE borrow_records SET status = ? WHERE record_id = ?");
                recordStmt->setString(1, decide);
                recordStmt->setInt(2, recordId);
                recordStmt->executeUpdate();
                delete recordStmt;

                // ����ͼ��״̬Ϊ���
                sql::PreparedStatement* bookStmt = con->prepareStatement(
                    "UPDATE books SET status = 'borrowing' WHERE book_id = ?");
                bookStmt->setString(1, bookId);
                bookStmt->executeUpdate();
                delete bookStmt;

                // �ύ����
                con->commit();
                cout << "�����ɹ���ͼ��״̬�Ѹ���" << endl;
            }
            catch (...) {
                // �ع�����
                con->rollback();
                throw;  // �����׳��쳣
            }

            // �ָ��Զ��ύģʽ
            con->setAutoCommit(true);
        }
        else
        {
            // �ܾ�����ֻ��Ҫ���½��ļ�¼
            sql::PreparedStatement* pstmt = con->prepareStatement(
                "UPDATE borrow_records SET status = ? WHERE record_id = ?");
            pstmt->setString(1, decide);
            pstmt->setInt(2, recordId);
            pstmt->execute();
            delete pstmt;
            cout << "�����ɹ�" << endl;
        }
    }
    catch (const sql::SQLException& e)
    {
        cout << "SQL����: " << e.what() << endl;
        cout << "�������: " << e.getErrorCode() << endl;
        cout << "SQL״̬: " << e.getSQLState() << endl;
    }
    catch (const std::exception& e)
    {
        cout << "����: " << e.what() << endl;
    }
    catch (...)
    {
        cout << "δ֪����" << endl;
    }
}
void admin::adminview(sql::Connection* con)
{
    int choice;


    do {
        // ��ʾ�˵�ѡ��
        cout << "\n===== ����Ա�˵� =====" << endl;
        cout << "1. ����鼮" << endl;
        cout << "2. ��ʾ�����鼮" << endl;
        cout << "3. �����鼮��Ϣ" << endl;
        cout << "4. ɾ���鼮" << endl;
        cout << "5. ��ʾ���н��ļ�¼" << endl;
        cout << "0. �˳�" << endl;
        cout << "���������ѡ��: ";
        cin >> choice;

        // �����û�ѡ��
        switch (choice) {
        case 1:
            system("cls");
            addbooks(con);
            break;
        case 2:
            system("cls");
            showallbooks(con);
            break;
        case 3:
            system("cls");
            updatebook(con);
            break;
        case 4:
            system("cls");
            deletebook(con);
            break;
        case 5:
            system("cls");
            showallrecord(con);
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
