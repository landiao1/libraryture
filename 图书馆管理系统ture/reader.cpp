#include "reader.h"
#include <iomanip>  // ����setw��left


void reader::readerview(sql::Connection* con)
{
    int choice;
    do {
        // ��ʾ�˵�ѡ��
        cout << "\n��ӭ����:"<<user_name<< endl;
        cout << "\n===== ���߲˵� =====" << endl;
        cout << "1. �����鼮" << endl;
        cout << "2. �����鼮" << endl;
        cout << "3. �黹�鼮" << endl;
        cout << "4. �鿴�����鼮" << endl;
        cout << "5. ��ʾ���н��ļ�¼" << endl;
        cout << "0. �˳�" << endl;
        cout << "���������ѡ��: ";
        cin >> choice;

        // �����û�ѡ��
        switch (choice) {
        case 1:
            system("cls");
            search_book(con);
            break;
        case 2:
            system("cls");
            borrow_book(con);
            break;
        case 3:
            system("cls");
            return_book(con);
            break;
        case 4:
            system("cls");
            showall_book(con);
            break;
        case 5:
            system("cls");
            showall_BorrowingRecord(con);
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
void reader::Register(sql::Connection* con) {
    try {
        string username, password, yanzhengmima;
        cout << "�û���:" << endl;
        cin.ignore();
        getline(cin, username);

        cout << "����:" << endl;
        getline(cin, password); // �������Ƴ������ cin.ignore()

        cout << "ȷ������:" << endl;
        getline(cin, yanzhengmima);

        if (password != yanzhengmima) {
            cout << "������������벻һ�£����������룡�����س����أ�" << endl;
            cin.ignore();
            cin.get();
        }
        else {
            sql::PreparedStatement* pstmt = con->prepareStatement("INSERT INTO users(username, password) VALUES(?, ?)");
            pstmt->setString(1, username);
            pstmt->setString(2, password);
            pstmt->execute();
            cout << "ע��ɹ������س�����..." << endl; // ��ӻ��з�
            delete pstmt;

            // �ȴ��û����س�ȷ��
            cin.ignore();
            cin.get();
        }
    }
    catch (const sql::SQLException& e) {
        cout << "ע��ʧ��: " << e.what() << endl;
        cin.ignore();
        cin.get();
    }
}
void reader::search_book(sql::Connection* con)
{
    try {
        int book_id = 0;
        string title, author, type, status;
        char choice;

        cout << "\n===== �鼮��ѯ =====" << endl;

        // ��ȡ�û�����
        cout << "�Ƿ�ID��ѯ��(y/n): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            cout << "�������鼮ID: ";
            cin >> book_id;
            cin.ignore();  // ������뻺����
        }

        cout << "�Ƿ�������ѯ��(y/n): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            cout << "����������(�ɲ���ƥ��): ";
            getline(cin >> ws, title);
        }

        cout << "�Ƿ����߲�ѯ��(y/n): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            cout << "����������(�ɲ���ƥ��): ";
            getline(cin >> ws, author);
        }

        cout << "�Ƿ����Ͳ�ѯ��(y/n): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            cout << "����������: ";
            cin >> type;
        }

        cout << "�Ƿ�״̬��ѯ��(y/n): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            cout << "������״̬: ";
            cin >> status;
        }

        // ������ѯ���
        string sql = "SELECT * FROM books WHERE 1=1";
        vector<string> params;  // ֻ�洢����ֵ��������ѭ����̬����

        if (book_id > 0) {
            sql += " AND book_id = ?";
            params.push_back(to_string(book_id));
        }
        if (!title.empty()) {
            sql += " AND title LIKE ?";
            params.push_back("%" + title + "%");
        }
        if (!author.empty()) {
            sql += " AND author LIKE ?";
            params.push_back("%" + author + "%");
        }
        if (!type.empty()) {
            sql += " AND type = ?";
            params.push_back(type);
        }
        if (!status.empty()) {
            sql += " AND status = ?";
            params.push_back(status);
        }

        // �����������ӡ���ɵ�SQL�Ͳ���
        /*cout << "\n������Ϣ:" << endl;
        cout << "���ɵ�SQL: " << sql << endl;
        cout << "��������: " << params.size() << endl;
        for (size_t i = 0; i < params.size(); i++) {
            cout << "���� #" << (i + 1) << ": " << params[i] << endl;
        }*/

        // ִ�в�ѯ
        if (!params.empty()) {
            // ʹ������ָ���Զ�������Դ
            unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(sql));

            // ���ò�����������1��ʼ��
            for (size_t i = 0; i < params.size(); i++) {
                pstmt->setString(i + 1, params[i]);
                cout << "���ò��� #" << (i + 1) << ": " << params[i] << endl;
            }

            // ִ�в�ѯ
            unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

            // ������
            cout << "\n��ѯ�����" << endl;
            cout << left << setw(8) << "ID"
                << setw(20) << "����"
                << setw(20) << "����"
                << setw(15) << "����"
                << "״̬" << endl;
            cout << "----------------------------------------------------------------" << endl;

            while (res->next()) {
                cout << left << setw(8) << res->getInt("book_id")
                    << setw(20) << res->getString("title")
                    << setw(20) << res->getString("author")
                    << setw(15) << res->getString("type")
                    << res->getString("status") << endl;
            }

            cout << "----------------------------------------------------------------" << endl;
        }
        else {
            cout << "������Ҫָ��һ����ѯ������" << endl;
        }
    }
    catch (sql::SQLException& e) {
        cout << "# ERR: SQLException in search_book(): " << e.what() << endl;
        cout << "SQLState: " << e.getSQLState() << ", ErrorCode: " << e.getErrorCode() << endl;
    }
}
void reader::borrow_book(sql::Connection* con)
{

}
void reader::return_book(sql::Connection* con)
{

}
void reader::showall_book(sql::Connection* con)
{
    //�ڲ鿴�����鼮ʱ����
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

            cout << "----------------------------------" << endl;
            

        }
        char choice;
        string book_id;
        string return_time;

        cout << "�Ƿ�Ҫ��������鼮��(y/n): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            cout << "������Ҫ���ĵ��鼮ID: ";
            cin >> book_id;
            cin.ignore();  // ������뻺����

            cout << "������黹����: ";
            getline(cin, return_time);

            sql::PreparedStatement* pstmt = con->prepareStatement("INSERT INTO borrow_records(username,book_id,return_date) VALUES(?,?,?)");
            pstmt->setString(1, user_name);
            pstmt->setString(2, book_id);
            pstmt->setString(3, return_time);
            pstmt->execute();
            delete pstmt;

            sql::PreparedStatement* checkStmt = con->prepareStatement(
                "SELECT status FROM books WHERE book_id = ?"
            );
            checkStmt->setString(1, book_id);
            sql::ResultSet* checkRes = checkStmt->executeQuery();

            if (checkRes->next()) {
                string status = checkRes->getString("status");
                if (status == "borrowing") {
                    cout << "----------------" << endl;
                    cout << "��ͼ���ѱ�����" << endl;
                    cout << "----------------" << endl;
                }
                else {
                    cout << "����ɹ�" << endl;
                }
            }
            
            delete checkStmt;
        }
        
        delete stmt;
    }
    
    catch (const sql::SQLException& e) {  // ��������쳣����
        cout << "���ݿ����: " << e.what() << endl;
        cout << "�������: " << e.getErrorCode() << endl;
        cout << "SQL״̬: " << e.getSQLState() << endl;
    }
    
    
}
void reader::showall_BorrowingRecord(sql::Connection* con)
{
    //�ڲ�ѯ��¼ʱ�黹�鼮
    try
    {
        sql::PreparedStatement* pstmt = con->prepareStatement("SELECT * FROM borrow_records WHERE username = ?");
        pstmt->setString(1, user_name);
        sql::ResultSet* res = pstmt->executeQuery();
        

        cout << "�û�:" << user_name << "���ļ�¼:" << endl;
        if (!res->next())
        {
            cout << user_name << "Ŀǰû�н��ļ�¼" << endl;
        }
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
        delete pstmt;

        char  choice;
        int  linshi_id;
        cout << "�Ƿ�Ҫ����ȡ���������������(y/n): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y') 
        {
            cout << "������Ҫɾ���Ľ��ļ�¼�������:" << endl;
            cin >> linshi_id;
            cin.ignore();

            sql::PreparedStatement* pstmt = con->prepareStatement("DELETE FROM borrow_records WHERE record_id = ?");
            pstmt->setInt(1, linshi_id);

            pstmt->execute();
            cout << "ɾ���ɹ�" << endl;

            delete pstmt;
        }
        

    }
    catch (const sql::SQLException& e) {  // ��������쳣����
        cout << "���ݿ����: " << e.what() << endl;
        cout << "�������: " << e.getErrorCode() << endl;
        cout << "SQL״̬: " << e.getSQLState() << endl;
    }
}
