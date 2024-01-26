#include "DbPool.h"
#include "database_config.h"

int main()
{
    DbPool *db_pool = DbPool::getInstance();
    db_pool->init(SQL_HOST, USERNAME, PASSWORD, DATABASE, SQL_PORT, 8);   //创建数据库连接池

    ChatDataBase *db_conn;
    connectionRAII connRAII(db_conn, db_pool);//使用RAII技术从数据库连接池中获取一条数据库连接

    string table = "Chat_Member_Information";
    int userid;
    db_conn->my_database_user_password("111", "111", userid);

    delete db_pool;

    return 0;
}