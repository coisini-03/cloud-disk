#include <iostream>
#include <services/UserService.hpp>

using namespace std;
using namespace services;
using namespace dao;

UserDao userDao("mysql://root:123456@localhost:3306/clouddisk");
services::UserService userService(userDao);
void test_login(){
    SubTask *task = userService.login_user("test","123456",[](bool ok,std::string msg){
        if(ok){
            cout<<"登录成功"<<endl;
        }
        else{
            cout<<"登录失败 :" << msg << endl;
        }
    });
    WFMySQLTask *mysql_task = dynamic_cast<WFMySQLTask *>(task);
    mysql_task->start();
    getchar();
}

void test_register(){
    SubTask *task = userService.register_user("test2","123456",[](bool ok,std::string msg){
        if(ok){
            cout<<"注册成功"<<endl;
        }
        else{
            cout<<"注册失败 :" << msg << endl;
        }
    });
    WFMySQLTask *mysql_task = dynamic_cast<WFMySQLTask *>(task);
    mysql_task->start();
    getchar();
}
int main(int argc,char *arg[]){
    // test_login();
    test_register();
    return 0;
}