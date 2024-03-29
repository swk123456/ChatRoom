#pragma once

#include <mysql/mysql.h>
#include <iostream>
#include <cstdio>
#include <cstring>
#include "../../utility/common.h"
#include "../../utility/locker.h"
using namespace std;
struct userInfo{
    int id;
    string name;
    string self_signature;
    string picture;
};

class ChatDataBase {
private:
    MYSQL *mysql;
    MYSQL_RES *result;    //指向查询结果的指针
    MYSQL_ROW row;
public:
    ChatDataBase();
    ~ChatDataBase();
    bool initDb(string host,string user,string passwd,string db_name, int port);
    bool exeSQL(string sql);
    // 数据库连接和 去连接
    void my_database_connect(const char *name);

    //用户注册
    void my_database_user_password(string name, string password,int&userid);
    //判断用户是不是在线
    bool my_database_user_state(int userid);
    //获取用户信息
    void my_database_user_information(User&ur,int);

    //用户登录判断
    bool my_database_password_correct(int id, string password);
    //好友关系 用户是否存在  两个用户是否为好友关系  加好友  返回好友列表

    // 搜索用户
    bool search_user(int,User& ur);
    //判断用户是否存在
    bool my_database_user_id_exist(int userid);

    bool my_database_is_friend(int,int);
    void my_database_add_new_friend_notification(int, int);
    void my_database_add_new_friend(int,int);
    void my_database_delete_friend(int userid,int friendid);
    void my_database_search_notification(int userid,User& ur);

    //void add_friend(int userid,vector<User> &ur);


    void my_database_friend_list(std::vector<User> &friendlist,vector<int>&friend_message,int);
    // 用户下线
    void my_database_offline(int);
    //用户消息
    void my_database_chat(Message ms);
    void my_database_chat_search(vector<Message> &messageList,int senderid,int targetid);
    //群组关系 群是否存在 群组创建  群添加用户
    bool my_database_group_exist(string group_name);
    bool my_database_group_id_exist(int);

    void my_database_user_add_group_notification(int,int);//================================
    void my_database_group_owner_add_user(int groupid,int userid);
    void my_database_search_group_notification(int,vector<User>& userList);//==========================

    int my_database_get_group_name(string *);//====================================
    void my_database_get_group_member(string, string &);//==========================

    //获取所有未读的添加好友通知
    void get_all_add_friend_notification(int userId, vector<User> &users);//ok and not test

    //通过群id查找群
    void my_database_search_group(Group &gp, int groupid);//ok and not test

    //创建一个新的群
    void my_database_add_new_group(string group_name, int ownerid,int &group_id);

    //通过groupid得到群里所有成员的信息
    void my_database_get_group_user(int groupid, vector<User> &ur);//ok and not test

    //通过groupid获得该群的聊天记录
    void my_database_get_group_chat_msg(int groupid,int userid,vector<Message> &ms);//ok and not test

    //在群消息记录表中增加一条消息记录
    void my_database_group_msg_insert(Message ms);//ok and not test

    //通过一个用户id获取该用户加入的所有群聊，并获取相应的历史消息数
    void my_database_get_group(vector<Group> &vgp, vector<int> &group_message, int userid);//ok and not test

    bool my_database_in_group(int userId, int groupId);//ok and not test
};