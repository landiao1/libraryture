#include "reader.h"
#include <iomanip>  // 用于setw和left


void reader::readerview(sql::Connection* con)
{
    int choice;
    do {
        // 显示菜单选项
        cout << "\n欢迎读者:"<<user_name<< endl;
        cout << "\n===== 读者菜单 =====" << endl;
        cout << "1. 查找书籍" << endl;
        cout << "2. 借阅书籍" << endl;
        cout << "3. 归还书籍" << endl;
        cout << "4. 查看所有书籍" << endl;
        cout << "5. 显示所有借阅记录" << endl;
        cout << "0. 退出" << endl;
        cout << "请输入你的选择: ";
        cin >> choice;

        // 处理用户选择
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
            cout << "退出系统..." << endl;
            break;
        default:
            cout << "无效的选择，请重新输入!" << endl;
            cin.ignore(); // 清除输入缓冲区
            cin.get();   // 等待用户按回车
        }
    } while (choice != 0);


}
void reader::Register(sql::Connection* con) {
    try {
        string username, password, yanzhengmima;
        cout << "用户名:" << endl;
        cin.ignore();
        getline(cin, username);

        cout << "密码:" << endl;
        getline(cin, password); // 修正：移除多余的 cin.ignore()

        cout << "确认密码:" << endl;
        getline(cin, yanzhengmima);

        if (password != yanzhengmima) {
            cout << "两次输入的密码不一致，请重新输入！（按回车返回）" << endl;
            cin.ignore();
            cin.get();
        }
        else {
            sql::PreparedStatement* pstmt = con->prepareStatement("INSERT INTO users(username, password) VALUES(?, ?)");
            pstmt->setString(1, username);
            pstmt->setString(2, password);
            pstmt->execute();
            cout << "注册成功！按回车返回..." << endl; // 添加换行符
            delete pstmt;

            // 等待用户按回车确认
            cin.ignore();
            cin.get();
        }
    }
    catch (const sql::SQLException& e) {
        cout << "注册失败: " << e.what() << endl;
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

        cout << "\n===== 书籍查询 =====" << endl;

        // 获取用户输入
        cout << "是否按ID查询？(y/n): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            cout << "请输入书籍ID: ";
            cin >> book_id;
            cin.ignore();  // 清除输入缓冲区
        }

        cout << "是否按书名查询？(y/n): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            cout << "请输入书名(可部分匹配): ";
            getline(cin >> ws, title);
        }

        cout << "是否按作者查询？(y/n): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            cout << "请输入作者(可部分匹配): ";
            getline(cin >> ws, author);
        }

        cout << "是否按类型查询？(y/n): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            cout << "请输入类型: ";
            cin >> type;
        }

        cout << "是否按状态查询？(y/n): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            cout << "请输入状态: ";
            cin >> status;
        }

        // 构建查询语句
        string sql = "SELECT * FROM books WHERE 1=1";
        vector<string> params;  // 只存储参数值，索引由循环动态生成

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

        // 调试输出：打印生成的SQL和参数
        /*cout << "\n调试信息:" << endl;
        cout << "生成的SQL: " << sql << endl;
        cout << "参数数量: " << params.size() << endl;
        for (size_t i = 0; i < params.size(); i++) {
            cout << "参数 #" << (i + 1) << ": " << params[i] << endl;
        }*/

        // 执行查询
        if (!params.empty()) {
            // 使用智能指针自动管理资源
            unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(sql));

            // 设置参数（索引从1开始）
            for (size_t i = 0; i < params.size(); i++) {
                pstmt->setString(i + 1, params[i]);
                cout << "设置参数 #" << (i + 1) << ": " << params[i] << endl;
            }

            // 执行查询
            unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

            // 输出结果
            cout << "\n查询结果：" << endl;
            cout << left << setw(8) << "ID"
                << setw(20) << "书名"
                << setw(20) << "作者"
                << setw(15) << "类型"
                << "状态" << endl;
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
            cout << "至少需要指定一个查询条件！" << endl;
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
    //在查看所有书籍时借阅
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

            cout << "----------------------------------" << endl;
            

        }
        char choice;
        string book_id;
        string return_time;

        cout << "是否要申请借阅书籍？(y/n): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            cout << "请输入要借阅的书籍ID: ";
            cin >> book_id;
            cin.ignore();  // 清除输入缓冲区

            cout << "请输入归还日期: ";
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
                    cout << "该图书已被借阅" << endl;
                    cout << "----------------" << endl;
                }
                else {
                    cout << "申请成功" << endl;
                }
            }
            
            delete checkStmt;
        }
        
        delete stmt;
    }
    
    catch (const sql::SQLException& e) {  // 捕获具体异常类型
        cout << "数据库错误: " << e.what() << endl;
        cout << "错误代码: " << e.getErrorCode() << endl;
        cout << "SQL状态: " << e.getSQLState() << endl;
    }
    
    
}
void reader::showall_BorrowingRecord(sql::Connection* con)
{
    //在查询记录时归还书籍
    try
    {
        sql::PreparedStatement* pstmt = con->prepareStatement("SELECT * FROM borrow_records WHERE username = ?");
        pstmt->setString(1, user_name);
        sql::ResultSet* res = pstmt->executeQuery();
        

        cout << "用户:" << user_name << "借阅记录:" << endl;
        if (!res->next())
        {
            cout << user_name << "目前没有借阅记录" << endl;
        }
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
        delete pstmt;

        char  choice;
        int  linshi_id;
        cout << "是否要进行取消借阅申请操作？(y/n): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y') 
        {
            cout << "请输入要删除的借阅记录申请序号:" << endl;
            cin >> linshi_id;
            cin.ignore();

            sql::PreparedStatement* pstmt = con->prepareStatement("DELETE FROM borrow_records WHERE record_id = ?");
            pstmt->setInt(1, linshi_id);

            pstmt->execute();
            cout << "删除成功" << endl;

            delete pstmt;
        }
        

    }
    catch (const sql::SQLException& e) {  // 捕获具体异常类型
        cout << "数据库错误: " << e.what() << endl;
        cout << "错误代码: " << e.getErrorCode() << endl;
        cout << "SQL状态: " << e.getSQLState() << endl;
    }
}
