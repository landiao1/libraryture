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
        cout << "书名:" << endl;
        cin.ignore();
        getline(cin, title); // Corrected variable name from "tltle" to "title"  

        cout << "作者:" << endl;
        cin.ignore();
        getline(cin, author);

        cout << "类型:" << endl;
        cin.ignore();
        getline(cin, type);

        cout << "状态:"<<endl;
        cin.ignore();
        getline(cin, status);

        sql::PreparedStatement* pstmt = con->prepareStatement("INSERT INTO books(title,author,type,status) VALUES(?,?,?,?)");
        pstmt->setString(1, title);
        pstmt->setString(2, author);
        pstmt->setString(3, type);
        pstmt->setString(4, status);
        pstmt->execute();
        cout << "添加成功";
        delete pstmt;



    }
    catch (const sql::SQLException& e) {  // 捕获具体异常类型
        cout << "数据库错误: " << e.what() << endl;
        cout << "错误代码: " << e.getErrorCode() << endl;
        cout << "SQL状态: " << e.getSQLState() << endl;
    }
    catch (const std::exception& e) {    // 捕获其他标准异常
        cout << "标准异常: " << e.what() << endl;
    }
    catch (...) {                        // 捕获所有未知异常
        cout << "未知错误" << endl;
    }

}
void admin::showallbooks(sql::Connection* con)
{
    try
    {
        sql::Statement* stmt = con->createStatement();
        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM books");
        cout << "书籍列表" << endl;
        while (res->next())
        {
            cout << "id:" << res->getInt("book_id") << endl;
            cout << "书名:" << res->getString("title") << endl;
            cout << "作者:" << res->getString("author") << endl;
            cout << "类型:" << res->getString("type") << endl;
            cout << "状态:" << res->getString("status") << endl;
        }
        delete stmt;
    }
    catch (const std::exception&)
    {
        cout << "错误" << endl;
    }

}
void admin::updatebook(sql::Connection* con)
{
    try
    {
        string book_id;
        string title, author, type, status;


        cout << "输入要更改的id:";
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
        cout << "修改成功" << endl;
    }
    catch (const std::exception&)
    {
        cout << "修改失败" << endl;
    }


}
void admin::deletebook(sql::Connection* con)
{
    int book_id;
    cout << "输入要删除的书的Id：" << endl;
    cin >> book_id;

    sql::PreparedStatement* pstmt = con->prepareStatement("DELETE FROM books WHERE book_id = ?");
    pstmt->setInt(1, book_id);

    pstmt->execute();
    cout << "删除成功" << endl;

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

        cout << "借阅申请列表" << endl;
        while (res->next())
        {
            cout << "申请序号:" << res->getInt("record_id") << endl;
            cout << "用户名:" << res->getString("username") << endl;
            cout << "图书id:" << res->getString("book_id") << endl;
            cout << "借阅日期:" << res->getString("borrow_date") << endl;
            cout << "归还日期:" << res->getString("return_date") << endl;
            cout << "审批状态:" << res->getString("status") << endl;
        }
        delete res;
        delete satmt;

        cout << "输入要审批的记录的序号:" << endl;
        cin.ignore();
        getline(cin, number);  // 注意这里不再需要ignore()

        cout << "输入审批结果(pass通过 refuse拒绝)" << endl;
        getline(cin, decide);

        // 将输入序号转换为整数
        int recordId = stoi(number);
        string bookId = "";  // 用于保存图书ID

        if (decide == "pass")
        {
            // 1. 先查询获取图书ID
            sql::PreparedStatement* selectStmt = con->prepareStatement(
                "SELECT book_id FROM borrow_records WHERE record_id = ?");
            selectStmt->setInt(1, recordId);
            sql::ResultSet* bookRes = selectStmt->executeQuery();

            if (bookRes->next()) {
                bookId = bookRes->getString("book_id");
            }
            else {
                cout << "未找到对应的借阅记录！" << endl;
                delete selectStmt;
                delete bookRes;
                return;
            }

            delete bookRes;
            delete selectStmt;

            // 2. 开启事务确保数据一致性
            con->setAutoCommit(false);  // 禁用自动提交

            try {
                // 更新借阅记录状态
                sql::PreparedStatement* recordStmt = con->prepareStatement(
                    "UPDATE borrow_records SET status = ? WHERE record_id = ?");
                recordStmt->setString(1, decide);
                recordStmt->setInt(2, recordId);
                recordStmt->executeUpdate();
                delete recordStmt;

                // 更新图书状态为借出
                sql::PreparedStatement* bookStmt = con->prepareStatement(
                    "UPDATE books SET status = 'borrowing' WHERE book_id = ?");
                bookStmt->setString(1, bookId);
                bookStmt->executeUpdate();
                delete bookStmt;

                // 提交事务
                con->commit();
                cout << "审批成功，图书状态已更新" << endl;
            }
            catch (...) {
                // 回滚事务
                con->rollback();
                throw;  // 重新抛出异常
            }

            // 恢复自动提交模式
            con->setAutoCommit(true);
        }
        else
        {
            // 拒绝操作只需要更新借阅记录
            sql::PreparedStatement* pstmt = con->prepareStatement(
                "UPDATE borrow_records SET status = ? WHERE record_id = ?");
            pstmt->setString(1, decide);
            pstmt->setInt(2, recordId);
            pstmt->execute();
            delete pstmt;
            cout << "审批成功" << endl;
        }
    }
    catch (const sql::SQLException& e)
    {
        cout << "SQL错误: " << e.what() << endl;
        cout << "错误代码: " << e.getErrorCode() << endl;
        cout << "SQL状态: " << e.getSQLState() << endl;
    }
    catch (const std::exception& e)
    {
        cout << "错误: " << e.what() << endl;
    }
    catch (...)
    {
        cout << "未知错误" << endl;
    }
}
void admin::adminview(sql::Connection* con)
{
    int choice;


    do {
        // 显示菜单选项
        cout << "\n===== 管理员菜单 =====" << endl;
        cout << "1. 添加书籍" << endl;
        cout << "2. 显示所有书籍" << endl;
        cout << "3. 更新书籍信息" << endl;
        cout << "4. 删除书籍" << endl;
        cout << "5. 显示所有借阅记录" << endl;
        cout << "0. 退出" << endl;
        cout << "请输入你的选择: ";
        cin >> choice;

        // 处理用户选择
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
            cout << "退出系统..." << endl;
            break;
        default:
            cout << "无效的选择，请重新输入!" << endl;
            cin.ignore(); // 清除输入缓冲区
            cin.get();   // 等待用户按回车
        }
    } while (choice != 0);


}
