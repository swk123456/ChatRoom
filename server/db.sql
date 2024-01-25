drop database if exists ChatRoom;
create database ChatRoom;
use ChatRoom;

drop table if exists Chat_Member_Information;
create table Chat_Member_Information/*用户信息*/
(
	Member_ID bigint(20) not null AUTO_INCREMENT,/*自增ID*/
	Member_Password varchar(64) not null,/*用户密码*/
	Member_Nickname varchar(64) not null,/*用户昵称*/
    Member_UserID bigint(20) not null,/*用户ID*/
	Member_Photo varchar(64) default null,
    Member_Signature varchar(64) default null,
	constraint PK_Member_Information primary key(Member_ID),
    constraint UK_Member_Information unique key(Member_UserID)
);

drop table if exists Chat_Room_Information;
create table Chat_Room_Information/*群聊信息*/
(
	Room_ID bigint(20) not null AUTO_INCREMENT,/*群聊ID*/
	Room_Nickname varchar(64) not null,/*群聊名*/
	Room_OwnerID bigint(20) default null,/*群主ID*/
	constraint PK_Room_Information primary key(Room_ID),
    constraint FK_OwnerID_UserID foreign key(Room_OwnerID) references Chat_Member_Information(Member_UserID)
);

drop table if exists Chat_Room_Member;
create table Chat_Room_Member/*群聊成员信息*/
(
	Room_ID bigint(20) not null,/*群聊ID*/
	Member_UserID bigint(20) not null,/*群成员ID*/
	Msg_Num bigint(20) default 0,/*消息数量*/
    constraint PK_Room_Member primary key(Room_ID, Member_UserID),
	constraint FK_Room_Member_Room_ID foreign key(Room_ID) references Chat_Room_Information(Room_ID) on delete cascade,
	constraint FK_Room_Member_Member_ID foreign key(Member_UserID) references Chat_Member_Information(Member_UserID) on delete no action
);

drop table if exists Chat_Group_Message;
create table Chat_Group_Message/*群聊内容*/
(
	Message_ID bigint(20) not null AUTO_INCREMENT,/*消息编号*/
	Room_ID bigint(20) not null,/*群聊ID*/
	Member_UserID bigint(20) not null,/*群成员ID*/
	Message_Time datetime,/*发送时间*/
	Message_Information varchar(128) default null,/*发送内容*/
    Message_Type bigint(20) default null,/*消息类型*/
	constraint PK_Chat_Group_Message primary key(Message_ID),
	constraint FK_Group_Message_Room_ID foreign key(Room_ID) references Chat_Room_Information(Room_ID) on delete cascade,
	constraint FK_Group_Message_Member_ID foreign key(Member_UserID) references Chat_Member_Information(Member_UserID)on delete no action
);

drop table if exists Chat_Friend_Notification;
create table Chat_Friend_Notification/*好友申请表*/
(
	Member_UserID bigint(20) not null,/*申请人ID*/
	Friend_UserID bigint(20) not null,/*另一人ID*/
	Agree_or_Disagree int default 0,/*是否通过申请*/
	constraint FK_Friend_Notif_Member_UserID foreign key(Member_UserID) references Chat_Member_Information(Member_UserID)on delete cascade,
    constraint FK_Friend_Notif_Friend_ID foreign key(Friend_UserID) references Chat_Member_Information(Member_UserID)on delete cascade
);

drop table if exists Chat_Friend;
create table Chat_Friend/*好友列表*/
(
	Member_UserID bigint(20) not null,/*本人ID*/
	Friend_UserID bigint(20) not null,/*好友ID*/
    Msg_Num bigint(20) default 0,
	constraint FK_Friend_Member_UserID foreign key(Member_UserID) references Chat_Member_Information(Member_UserID)on delete cascade,
    constraint FK_Friend_Friend_UserID foreign key(Friend_UserID) references Chat_Member_Information(Member_UserID)on delete cascade
);

drop table if exists Chat_Message;
create table Chat_Message/*聊天内容*/
(
	Message_ID bigint(20) not null AUTO_INCREMENT,/*消息编号*/
	Sender_UserID bigint(20) not null,/*发送者ID*/
	Target_UserID bigint(20) not null,/*接受者ID*/
	Message_Time datetime,/*发送时间*/
	Message_Information varchar(128) default null,/*发送内容*/
    Message_Type bigint(20) default null,/*消息类型*/
	constraint PK_Chat_Message primary key(Message_ID),
	constraint FK_Message_Sender_UserID foreign key(Sender_UserID) references Chat_Member_Information(Member_UserID) on delete no action,
	constraint FK_Message_Target_UserID foreign key(Target_UserID) references Chat_Member_Information(Member_UserID)on delete no action
);