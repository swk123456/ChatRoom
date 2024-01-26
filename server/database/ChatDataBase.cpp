#include <vector>
#include "ChatDataBase.h"
#include "database_config.h"
ChatDataBase::ChatDataBase()
{
    /*my_database_connect("tengxun");*/
    mysql = mysql_init(NULL);
    if(mysql == NULL)
    {
        cout << "Error:" << mysql_error(mysql);
        exit(1);
    }
}

ChatDataBase::~ChatDataBase()
{
    /*mysql_close(mysql);*/
    if(mysql != NULL)  //关闭数据连接
    {
        mysql_close(mysql);
    }
}

void ChatDataBase::my_database_connect(const char *name)
{
    mysql = mysql_init(NULL);
    cout<<"111"<<endl;
    mysql = mysql_real_connect(mysql, SQL_HOST, USERNAME, PASSWORD, DATABASE, SQL_PORT, NULL, 0);\
    cout<<"111"<<endl;
    if (NULL == mysql)
    {
        cout << "connect database failure" << endl;
    } else{
        cout<<"connect database success"<<endl;
    }

    if (mysql_query(mysql, "set names utf8;") != 0)
    {
        cout << "mysql_query error" << endl;
    }
    else
    {
        cout << "set name utf8 success and mysql_query success" <<endl;
    }

}

int ChatDataBase::my_database_get_group_name(string *s)
{
    if (mysql_query(mysql, "show tables;") != 0)
    {
        cout << "mysql_query error" << endl;
    }

    MYSQL_RES *res = mysql_store_result(mysql);
    if (NULL == res)
    {
        cout << "mysql_store_result" << endl;
    }

    int count = 0;
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        s[count] += row[0];
        count++;
    }

    return count;
}

void ChatDataBase::my_database_get_group_member(string name, string &s)
{
    char sql[1024] = {0};
    sprintf(sql, "select member from %s;", name.c_str());
    if (mysql_query(mysql, sql) != 0)
    {
        cout << "mysql_query error" << endl;
    }

    MYSQL_RES *res = mysql_store_result(mysql);
    if (NULL == res)
    {
        cout << "mysql_store_result error" << endl;
    }

    MYSQL_ROW row = mysql_fetch_row(res);
    if (NULL == row[0])
    {
        return;
    }
    s += row[0];
}




void ChatDataBase::my_database_user_password(string name, string password,int &userid)
{
    char sql[1024] = {0};
    sprintf(sql, "insert into Chat_Member_Information (Member_Nickname,Member_Password,Member_Status,Member_Photo,Member_Signature) values('%s','%s','%d','%s','%s');", name.c_str(),password.c_str(),0,":/media/person/media/person/2.jpg","happy");
    if (mysql_query(mysql,sql))
    {
        cout<<"Query Error: "<<mysql_error(mysql);
        return ;
    }
    memset(sql, 0, sizeof(sql));
    sprintf(sql, "SELECT max(Member_UserID) from Chat_Member_Information ;");
    if (mysql_query(mysql,sql))
    {
        cout<<"Query Error: "<<mysql_error(mysql);
        return ;
    }
    MYSQL_RES *res = mysql_store_result(mysql);
    MYSQL_ROW row = mysql_fetch_row(res);
    userid = stoi(row[0]);
}

bool ChatDataBase::my_database_password_correct(int id, string password)
{
    char sql[1024] = {0};
    sprintf(sql, "select Member_Password from Chat_Member_Information where Member_UserID='%d';",id);
    if (mysql_query(mysql, sql) != 0)
    {
        cout << "mysql_query error" << endl;
    }
    MYSQL_RES *res = mysql_store_result(mysql);
    MYSQL_ROW row = mysql_fetch_row(res);
    if (row && password == row[0])
    {
        cout<<"password true"<<endl;
        memset(sql,0,sizeof(sql));
        sprintf(sql,"UPDATE Chat_Member_Information SET Member_Status=1 where Member_UserID='%d';",id);
        if (mysql_query(mysql, sql) != 0) {
            cout << "update user online information error" << endl;
            return false;
        }else
        {
            cout<<"update user online information success" <<endl;
            return true;
        }
    }
    else
    {
        cout<<"user no exist or password false"<<endl;
        return false;
    }
}



bool ChatDataBase::my_database_is_friend(int n1, int n2)
{
    char sql[1024] = {0};
    sprintf(sql, "select Friend_UserID from Chat_Friend where Member_UserID='%d';", n1);
    if (mysql_query(mysql, sql) != 0)
    {
        cout << "mysql_query error" << endl;
    }
    else
    {
        cout<< "mysql_query correct" <<endl;
    }
    MYSQL_RES *res = mysql_store_result(mysql);
    MYSQL_ROW row;
    string temp = to_string(n2);
    while((row = mysql_fetch_row(res)) != NULL)
    {
        cout<<row[0]<<endl;
        if(temp == row[0]) {
            cout << "yes,friend" << endl;
            return true;
        }
        else {
            cout << "no" << endl;
        }
    }
    return false;
}

void ChatDataBase::my_database_add_new_friend_notification(int n1, int n2)
{
    if(my_database_is_friend(n1, n2))
    {
        cout<<"friend already"<<endl;
        return ;
    }
    char sql[1024] = {0};
    //n1 发送 好友请求  n2 接受
    sprintf(sql,"insert into Chat_Friend_Notification (Member_UserID,Friend_UserID) values('%d','%d')",n1,n2);
    if (mysql_query(mysql, sql) != 0)
    {
        cout << "insert notification error" << endl;
    } else{
        cout << "insert notification  success" << endl;
    }
}

bool ChatDataBase::my_database_group_exist(string group_name)
{

    char sql[1024] = {0};
    sprintf(sql, "select Room_Nickname like '%s' from Chat_Room_Information;", group_name.c_str());
    if (mysql_query(mysql, sql) != 0)
    {
        cout << "mysql_query error" << endl;
    }

    MYSQL_RES *res = mysql_store_result(mysql);
    MYSQL_ROW row = mysql_fetch_row(res);
    if (NULL == row)      //用户不存在
    {
        cout<<"No group name:"<<group_name<<endl;
        return false;
    }
    else                  //用户存在
    {
        cout<<"find group name:"<<group_name<<endl;
        return true;
    }
}

void ChatDataBase::my_database_add_new_group(string group_name, int ownerid,int &group_id)
{
    char sql[1024] = {0};
    sprintf(sql, "insert into Chat_Room_Information(Room_Nickname,Room_OwnerID) values('%s','%d');", group_name.c_str(),ownerid);
    if (mysql_query(mysql, sql) != 0)
    {
        cout << "mysql_query insert error" << endl;
    }
    else
    {
        cout<< "mysql_query insert success" <<endl;
    }

    memset(sql, 0, sizeof(sql));
    sprintf(sql, "select max(Room_ID) from Chat_Room_Information;");
    if (mysql_query(mysql, sql) != 0)
    {
        cout << "mysql_query error" << endl;
    }
    else
    {
        cout<< "mysql_query success" <<endl;
    }
    MYSQL_RES *res = mysql_store_result(mysql);
    MYSQL_ROW row = mysql_fetch_row(res);
    cout<<row[0]<<endl;
    string temp = row[0];
    group_id=stoi(temp);
    memset(sql, 0, sizeof(sql));
    sprintf(sql, "insert into Chat_Room_Member(Room_ID,Member_UserID) values('%d','%d');", std::stoi(temp),ownerid);
    if (mysql_query(mysql, sql) != 0)
    {
        cout << "mysql_query insert error" << endl;
    }
    else
    {
        cout<< "mysql_query insert success" <<endl;
    }
}

void ChatDataBase::my_database_user_add_group_notification(int user_id,int group_id)
{
    if(!my_database_group_id_exist(group_id)) return;
    char sql[1024] = {0};
    sprintf(sql, "select Room_OwnerID from Chat_Room_Information where Room_ID='%d';", group_id);
    if (mysql_query(mysql, sql) != 0)
    {
        cout << "mysql_query error" << endl;
    }
    else
    {
        cout << "mysql_query success" << endl;
    }
    MYSQL_RES *res = mysql_store_result(mysql);
    MYSQL_ROW row = mysql_fetch_row(res);
    string temp_group_owner_id = row[0];
    cout<<temp_group_owner_id<<endl;//群主 的id
    memset(sql, 0, sizeof(sql));
    sprintf(sql, "insert into T_GROUP_NOTIFICATION(f_group_owner_id,f_user_id,f_type,f_group_id)values('%d','%d','%s','%d');", std::stoi(temp_group_owner_id), user_id,"add",group_id);
    if (mysql_query(mysql, sql) != 0)
    {
        cout << "mysql insert notification error" << endl;
        return ;
    }
    else
    {
        cout << "mysql insert notification success" << endl;
    }

}


void ChatDataBase::my_database_friend_list(vector<User> &friendlist,vector<int> &friend_message, int userid) {
    char sql[1024] = {0};
    sprintf(sql, "select Friend_UserID,Member_Nickname,Member_Status,Member_Photo,Member_Signature,Msg_Num from Chat_Friend, Chat_Member_Information where Chat_Friend.Member_UserID = '%d' and Chat_Friend.Friend_UserID = Chat_Member_Information.Member_UserID", userid);
    if (mysql_query(mysql, sql) != 0)
    {
        cout << "mysql_query error" << endl;
        return;
    }
    else
    {
        cout << "mysql_query success" <<endl;
    }
    auto res = mysql_store_result(mysql);
    auto numField = mysql_num_fields(res);
    MYSQL_ROW row;
    cout<<numField<<endl;
    /*for(int i=0;i<numField;i++)
    {
        cout<<mysql_fetch_field(res)->name<<endl;
    }*/
    while((row=mysql_fetch_row(res))!=NULL)
    {
        User temp;
        for(unsigned int i=0;i<numField;i++)
        {
            if(i==0)
                temp.userId=stoi(row[i]);
            if(i==1)
                temp.userName=row[i];
            if(i==2)
                temp.online=stoi(row[i]);
            if(i==3)
                temp.iconStr=row[i];
            if(i==4)
                temp.desc=row[i];
        }
        friendlist.push_back(temp);
        friend_message.push_back(stoi(row[5]));
    }
}

bool ChatDataBase::my_database_user_id_exist(int userid) {
    char sql[1024] = {0};
    sprintf(sql, "select Member_UserID from Chat_Member_Information;");
    if (mysql_query(mysql,sql))
    {
        cout<<"Query Error: "<<mysql_error(mysql);
        return false;
    }
    MYSQL_RES *res = mysql_store_result(mysql);
    MYSQL_ROW row;
    while((row = mysql_fetch_row(res)) != NULL)
    {
        if(row[0]==to_string(userid))
        {
            cout<<"find userid:"<<userid<<endl;
            return true;
        }
    }
    cout<<"can't find userid:"<<userid<<endl;
    return false;
}

bool ChatDataBase::my_database_group_id_exist(int groupid) {
    char sql[1024] = {0};
    sprintf(sql, "select Room_ID from Chat_Room_Information;");
    if (mysql_query(mysql, sql) != 0)
    {
        cout << "mysql_query error" << endl;
        return false;
    }

    MYSQL_RES *res = mysql_store_result(mysql);
    MYSQL_ROW row;
    while((row = mysql_fetch_row(res)) != NULL)
    {
        if(row[0]==to_string(groupid))
        {
            cout<<"find groupid:"<<groupid<<endl;
            return true;
        }
        cout<<row[0]<<endl;
    }
    cout<<"can't find grouprid:"<<groupid<<endl;
    return false;
}

void ChatDataBase::my_database_user_information(User &ur, int userid) {
    char sql[1024] = {0};
    sprintf(sql, "select Member_UserID,Member_Nickname,Member_Status,Member_Photo,Member_Signature from Chat_Member_Information where Member_UserID='%d'", userid);
    if (mysql_query(mysql,sql))
    {
        cout<<"Query Error: "<<mysql_error(mysql);
        return ;
    }
    auto res = mysql_store_result(mysql);
    auto numField = mysql_num_fields(res);
    MYSQL_ROW row;
    while((row=mysql_fetch_row(res))!=NULL)
    {
        for(unsigned int i=0;i<numField;i++)
        {
            if(i==0)
                ur.userId=stoi(row[i]);
            if(i==1)
                ur.userName=row[i];
            if(i==2)
                ur.online=stoi(row[i]);
            if(i==3)
                ur.iconStr=row[i];
            if(i==4)
                ur.desc=row[i];
        }
    }
}

void ChatDataBase::my_database_chat(Message ms) {
    char sql[1024] = {0};
    sprintf(sql, "INSERT into Chat_Message(Sender_UserID,Target_UserID,Message_Information,Message_Time,Message_Type) VALUES('%d','%d','%s','%s','%d');", ms.sendId,ms.receiveId,ms.content.c_str(),ms.time.c_str(),ms.type);
    if (mysql_query(mysql, sql) != 0)
    {
        cout << "mysql_insert error" << endl;
        return;
    }
    else
    {
        cout << "mysql_insert success" <<endl;
    }
    bool temp = my_database_user_state(ms.receiveId);
    if(temp == false)
    {
        cout<<ms.receiveId<<"not online"<<endl;
        memset(sql,0,sizeof(sql));
        sprintf(sql,"update Chat_Friend SET Msg_Num=Msg_Num+1 WHERE Member_UserID='%d' and Friend_UserID='%d';",ms.receiveId,ms.sendId);
        if (mysql_query(mysql, sql) != 0)
        {
            cout<<"error"<<endl;
        }
    }

}

void ChatDataBase::my_database_chat_search(vector<Message> &messageList, int senderid, int targetid) {
    char sql[1024] = {0};
    sprintf(sql, "select Sender_UserID,Target_UserID,Message_Information,Message_Time,Message_Type from Chat_Message where (Sender_UserID='%d' and Target_UserID='%d')or(Sender_UserID='%d' and Target_UserID='%d')", senderid,targetid,targetid,senderid);
    if (mysql_query(mysql, sql) != 0)
    {
        cout << "mysql_query error" << endl;
        return;
    }
    else
    {
        cout << "mysql_query success" <<endl;
    }
    auto res = mysql_store_result(mysql);
    auto numField = mysql_num_fields(res);
    MYSQL_ROW row;
    Message temp;
    while((row=mysql_fetch_row(res))!=NULL)
    {
        for(unsigned int i=0;i<numField;i++) {
            cout << i<<":"<<row[i] << endl;
            if(i==0){temp.sendId=stoi(row[i]);}
            if(i==1){temp.receiveId=stoi(row[i]);}
            if(i==2){temp.content=row[i];}
            if(i==3){temp.time=row[i];}
            if(i==4){temp.type=stoi(row[i]);}
        }
        messageList.push_back(temp);
    }
    memset(sql,0,sizeof(sql));
    sprintf(sql,"update Chat_Friend SET Msg_Num=0 WHERE Member_UserID='%d' and Friend_UserID='%d';",targetid,senderid);
    if (mysql_query(mysql, sql) != 0)
    {
        cout << "mysql_query error" << endl;
        return;
    }
}

bool ChatDataBase::search_user(int userid, User &ur) {
    if(my_database_user_id_exist(userid))
    {
        my_database_user_information(ur,userid);
        return true;
    }
    else{
        return false;
    }
}

void ChatDataBase::my_database_add_new_friend(int sender_id, int receive_id) {
    char sql[1024] = {0};
    memset(sql, 0, sizeof(sql));
    sprintf(sql,"select Member_UserID,Friend_UserID from Chat_Friend_Notification where Member_UserID='%d' and Friend_UserID='%d';",sender_id,receive_id);
    if (mysql_query(mysql, sql) != 0)
    {
        cout << "insert friend relationship information error" << endl;
    }
    else
    {
        cout << "insert friend relationship information success" << endl;
    }
    MYSQL_RES *res = mysql_store_result(mysql);
    MYSQL_ROW row = mysql_fetch_row(res);
    if(row!=NULL){
        memset(sql, 0, sizeof(sql));
        sprintf(sql,"insert Chat_Friend(Member_UserID,Friend_UserID)values('%d','%d');",sender_id,receive_id);
        if(mysql_query(mysql,sql)!=0){
            cout<<"insert error"<<endl;
        }
        memset(sql, 0, sizeof(sql));
        sprintf(sql,"insert Chat_Friend(Member_UserID,Friend_UserID)values('%d','%d');",receive_id,sender_id);
        if(mysql_query(mysql,sql)!=0){
            cout<<"insert error"<<endl;
        }
        memset(sql, 0, sizeof(sql));
        sprintf(sql,"delete from Chat_Friend_Notification where Member_UserID='%d' and Friend_UserID='%d';",sender_id,receive_id);
        if(mysql_query(mysql,sql)!=0){
            cout<<"delete error"<<endl;
        }
    }
}

void ChatDataBase::get_all_add_friend_notification(int userId, vector<User> &users)
{
    char sql[1024] = {0};
    sprintf(sql, "select Friend_UserID,Member_Nickname,Member_Status,Member_Photo,Member_Signature from Chat_Friend_Notification, Chat_Member_Information where Chat_Friend_Notification.Member_UserID = '%d' and Chat_Friend_Notification.Friend_UserID = Chat_Member_Information.Member_UserID", userId);
    if (mysql_query(mysql, sql) != 0)
    {
        cout << "mysql_query error" << endl;
        return;
    }
    else
    {
        cout << "mysql_query success" <<endl;
    }
    auto res = mysql_store_result(mysql);
    auto numField = mysql_num_fields(res);
    MYSQL_ROW row;
    cout<<numField<<endl;
    /*for(int i=0;i<numField;i++)
    {
        cout<<mysql_fetch_field(res)->name<<endl;
    }*/
    while((row=mysql_fetch_row(res))!=NULL)
    {
        User temp;
        for(unsigned int i=0;i<numField;i++)
        {
            if(i==0)
                temp.userId=stoi(row[i]);
            if(i==1)
                temp.userName=row[i];
            if(i==2)
                temp.online=stoi(row[i]);
            if(i==3)
                temp.iconStr=row[i];
            if(i==4)
                temp.desc=row[i];
        }
        users.push_back(temp);
    }
}

void ChatDataBase::my_database_search_notification(int userid, User &ur) {
    char sql[1024] = {0};
    memset(sql, 0, sizeof(sql));
    sprintf(sql,"select Friend_UserID FROM Chat_Friend_Notification where Member_UserID='%d';",userid);
    if (mysql_query(mysql, sql) != 0)
    {
        cout << "select error" << endl;
    }
    else
    {
        cout << "select success" << endl;
    }
    auto res = mysql_store_result(mysql);
    MYSQL_ROW row = mysql_fetch_row(res);
    //cout<<row[0]<<endl;
    auto temp = row[0];
    int userid2 = stoi(temp);
    search_user(userid2,ur);
}

void ChatDataBase::my_database_offline(int userid) {
    char sql[1024];
    memset(sql,0,sizeof(sql));
    sprintf(sql,"UPDATE Chat_Member_Information SET Member_Status=0 where Member_UserID='%d';",userid);
    if (mysql_query(mysql, sql) != 0) {
        cout << "update user offline information error" << endl;
        return ;
    }else
    {
        cout<<"update user offline information success" <<endl;
    }
}

void ChatDataBase::my_database_search_group_notification(int groupownerid, vector<User> &userList) {
    char sql[1024];
    memset(sql,0,sizeof(sql));
    sprintf(sql,"SELECT f_user_id from T_GROUP_NOTIFICATION where f_type='add' and f_group_owner_id='%d';",groupownerid);
    if(mysql_query(mysql,sql) != 0)
    {
        cout<<"get users of group failed"<<endl;
    }
    else{
        cout<<"get users of group success"<<endl;
    }
    MYSQL_RES *res = mysql_store_result(mysql);
    MYSQL_ROW row;
    User temp;
    while((row=mysql_fetch_row(res))!=NULL)
    {
        cout<<row[0]<<endl;
        my_database_user_information(temp,stoi(row[0]));
        userList.push_back(temp);
    }
}

void ChatDataBase::my_database_group_owner_add_user(int groupid, int userid) {
    char sql[1024];
    memset(sql,0,sizeof(sql));
    sprintf(sql,"insert into Chat_Room_Member(Room_ID,Member_UserID) values('%d','%d')",groupid,userid);
    if(mysql_query(mysql,sql) !=0)
    {
        cout<<"insert failed"<<endl;
    }
    else{
        cout<<"insert success"<<endl;
    }
}

void ChatDataBase::my_database_search_group(Group &gp, int groupid)
{
    char sql[1024] = {0};
    sprintf(sql, "select Room_ID,Room_Nickname,Room_OwnerID from Chat_Room_Information where Room_ID='%d'", groupid);
    if (mysql_query(mysql,sql))
    {
        cout<<"Query Error: "<<mysql_error(mysql);
        return ;
    }
    auto res = mysql_store_result(mysql);
    auto numField = mysql_num_fields(res);
    MYSQL_ROW row;
    while((row=mysql_fetch_row(res))!=NULL)
    {
        for(unsigned int i=0;i<numField;i++)
        {
            if(i==0)
                gp.groupId=stoi(row[i]);
            if(i==1)
                gp.name=row[i];
            if(i==2)
                gp.ownerId=stoi(row[i]);
        }
    }
}

bool ChatDataBase::my_database_in_group(int userId, int groupId)
{
    char sql[1024] = {0};
    sprintf(sql, "select Member_UserID from Chat_Room_Member where Room_ID='%d';", groupId);
    if (mysql_query(mysql, sql) != 0)
    {
        cout << "mysql_query error" << endl;
    }
    else
    {
        cout<< "mysql_query correct" <<endl;
    }
    MYSQL_RES *res = mysql_store_result(mysql);
    MYSQL_ROW row;
    string temp = to_string(userId);
    while((row = mysql_fetch_row(res)) != NULL)
    {
        cout<<row[0]<<endl;
        if(temp == row[0]) {
            cout << "yes" << endl;
            return true;
        }
        else {
            cout << "no" << endl;
        }
    }
    return false;
}

void ChatDataBase::my_database_get_group(vector<Group> &vgp, vector<int> &group_message, int userid)
{
    char sql[1024];
    memset(sql,0,sizeof(sql));
    sprintf(sql,"SELECT Room_ID,Msg_Num from Chat_Room_Member where Member_UserID='%d';",userid);
    if(mysql_query(mysql,sql) != 0)
    {
        cout<<"get group failed"<<endl;
    }
    else{
        cout<<"get group success"<<endl;
    }
    MYSQL_RES *res = mysql_store_result(mysql);
    MYSQL_ROW row;
    Group temp;
    while((row=mysql_fetch_row(res))!=NULL)
    {
        group_message.push_back(stoi(row[1]));
        my_database_search_group(temp,stoi(row[0]));
        vgp.push_back(temp);
    }
}

void ChatDataBase::my_database_get_group_user(int groupid, vector<User> &ur)
{
    char sql[1024] = {0};
    sprintf(sql, "select Chat_Room_Member.Member_UserID,Member_Nickname,Member_Status,Member_Photo,Member_Signature from Chat_Room_Member, Chat_Member_Information where Chat_Room_Member.Room_ID = '%d' and Chat_Room_Member.Member_UserID = Chat_Member_Information.Member_UserID", groupid);
    if (mysql_query(mysql, sql) != 0)
    {
        cout << "mysql_query error" << endl;
        return;
    }
    else
    {
        cout << "mysql_query success" <<endl;
    }
    auto res = mysql_store_result(mysql);
    auto numField = mysql_num_fields(res);
    MYSQL_ROW row;
    cout<<numField<<endl;
    /*for(int i=0;i<numField;i++)
    {
        cout<<mysql_fetch_field(res)->name<<endl;
    }*/
    while((row=mysql_fetch_row(res))!=NULL)
    {
        User temp;
        for(unsigned int i=0;i<numField;i++)
        {
            if(i==0)
                temp.userId=stoi(row[i]);
            if(i==1)
                temp.userName=row[i];
            if(i==2)
                temp.online=stoi(row[i]);
            if(i==3)
                temp.iconStr=row[i];
            if(i==4)
                temp.desc=row[i];
        }
        ur.push_back(temp);
    }
}

void ChatDataBase::my_database_group_msg_insert(Message ms)
{
    char sql[1024] = {0};
    sprintf(sql, "INSERT into Chat_Group_Message(Sender_UserID,Room_ID,Message_Information,Message_Time,Message_Type) VALUES('%d','%d','%s','%s','%d');", ms.sendId,ms.receiveId,ms.content.c_str(),ms.time.c_str(),ms.type);
    if (mysql_query(mysql, sql) != 0)
    {
        cout << "mysql_insert error" << endl;
        return;
    }
    else
    {
        cout << "mysql_insert success" <<endl;
    }
    vector<User> user;
    my_database_get_group_user(ms.receiveId, user);
    memset(sql,0,sizeof(sql));
    for(auto tmp : user)
    {
        bool temp = my_database_user_state(tmp.userId);
        if(temp == false)
        {
            cout<<ms.receiveId<<"not online"<<endl;
            memset(sql,0,sizeof(sql));
            sprintf(sql,"update Chat_Room_Member SET Msg_Num=Msg_Num+1 WHERE Member_UserID='%d' and Room_ID='%d';",tmp.userId,ms.receiveId);
            if (mysql_query(mysql, sql) != 0)
            {
                cout<<"error"<<endl;
            }
        }
    }
}

void ChatDataBase::my_database_get_group_chat_msg(int groupid,int userid,vector<Message> &ms)
{
    char sql[1024] = {0};
    sprintf(sql, "select Member_UserID,Room_ID,Message_Information,Message_Time,Message_Type from Chat_Group_Message where Member_UserID='%d' and Room_ID='%d'",userid,groupid);
    if (mysql_query(mysql, sql) != 0)
    {
        cout << "mysql_query error" << endl;
        return;
    }
    else
    {
        cout << "mysql_query success" <<endl;
    }
    auto res = mysql_store_result(mysql);
    auto numField = mysql_num_fields(res);
    MYSQL_ROW row;
    Message temp;
    while((row=mysql_fetch_row(res))!=NULL)
    {
        for(unsigned int i=0;i<numField;i++) {
            cout << i<<":"<<row[i] << endl;
            if(i==0){temp.sendId=stoi(row[i]);}
            if(i==1){temp.receiveId=stoi(row[i]);}
            if(i==2){temp.content=row[i];}
            if(i==3){temp.time=row[i];}
            if(i==4){temp.type=stoi(row[i]);}
        }
        ms.push_back(temp);
    }
    memset(sql,0,sizeof(sql));
    sprintf(sql,"update Chat_Room_Member SET Msg_Num=0 WHERE Member_UserID='%d' and Room_ID='%d';",userid,groupid);
    if (mysql_query(mysql, sql) != 0)
    {
        cout << "mysql_query error" << endl;
        return;
    }
}

void ChatDataBase::my_database_delete_friend(int userid, int friendid) {
    char sql[1024];
    memset(sql,0,sizeof(sql));
    sprintf(sql,"delete from Chat_Friend where Member_UserID='%d' and Friend_UserID='%d';",userid,friendid);
    if(mysql_query(mysql,sql)!=0)
    {
        cout<<"query error"<<endl;
        return ;
    }
    else{
        cout<<"query success" <<endl;
    }
    memset(sql,0,sizeof(sql));
    sprintf(sql,"delete from Chat_Friend where Member_UserID='%d' and Friend_UserID='%d';",friendid,userid);
    if(mysql_query(mysql,sql)!=0)
    {
        cout<<"query error"<<endl;
        return ;
    }
    else{
        cout<<"query success" <<endl;
    }
}

bool ChatDataBase::my_database_user_state(int userid) {
    char sql[1024];
    memset(sql,0,sizeof(sql));
    sprintf(sql,"select Chat_Member_Information.Member_Status from Chat_Member_Information where Member_UserID='%d';",userid);
    if (mysql_query(mysql,sql))
    {
        cout<<"Query Error: "<<mysql_error(mysql);
        return false; //抛出异常还是 返回false
    }
    MYSQL_RES *res = mysql_store_result(mysql);
    MYSQL_ROW row = mysql_fetch_row(res);
    if(stoi(row[0])==1)
        return true;
    else{
        return false;
    }
}

bool ChatDataBase::initDb(string host, string user, string passwd, string db_name, int port) {
    mysql = mysql_real_connect(mysql, host.c_str(), user.c_str(), passwd.c_str(), db_name.c_str(), port, NULL, 0);
    if(mysql == NULL)
    {
        cout << "Error: " << mysql_error(mysql);
        exit(1);
    }
    return true;
}

bool ChatDataBase::exeSQL(string sql) {
    if (mysql_query(mysql,sql.c_str()))
    {
        cout<<"Query Error: "<<mysql_error(mysql);
        return false;
    }
    else // 查询成功
    {
        result = mysql_store_result(mysql);  //获取结果集
        if (result)  // 返回了结果集
        {
            int  num_fields = mysql_num_fields(result);   //获取结果集中总共的字段数，即列数
            int  num_rows=mysql_num_rows(result);       //获取结果集中总共的行数
            for(int i=0;i<num_rows;i++) //输出每一行
            {
                //获取下一行数据
                row=mysql_fetch_row(result);
                if(row != NULL) break;

                for(int j=0;j<num_fields;j++)  //输出每一字段
                {
                    cout<<row[j]<<"\t\t";
                }
                cout<<endl;
            }

        }
        else  // result==NULL
        {
            if(mysql_field_count(mysql) == 0)   //代表执行的是update,insert,delete类的非查询语句
            {
                // (it was not a SELECT)
                int num_rows = mysql_affected_rows(mysql);  //返回update,insert,delete影响的行数
                printf("%s execute successed! affect %d rows\n", sql.c_str(), num_rows);
            }
            else // error
            {
                cout<<"Get result error: "<<mysql_error(mysql);
                return false;
            }
        }
    }
    return true;
}