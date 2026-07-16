#include <iostream>
#include <services/UserService.hpp>

using namespace std;
using namespace services;
using namespace dao;

UserDao userDao("mysql://root:123456@localhost:3306/clouddisk");
services::UserService userService(userDao);
// void test_login(){
//     SubTask *task = userService.login_user("test","123456",[](bool ok,std::string msg){
//         if(ok){
//             cout<<"登录成功"<<endl;
//         }
//         else{
//             cout<<"登录失败 :" << msg << endl;
//         }
//     });
//     WFMySQLTask *mysql_task = dynamic_cast<WFMySQLTask *>(task);
//     mysql_task->start();
//     getchar();
// }

// void test_register(){
//     SubTask *task = userService.register_user("test2","123456",[](bool ok,std::string msg){
//         if(ok){
//             cout<<"注册成功"<<endl;
//         }
//         else{
//             cout<<"注册失败 :" << msg << endl;
//         }
//     });
//     WFMySQLTask *mysql_task = dynamic_cast<WFMySQLTask *>(task);
//     mysql_task->start();
//     getchar();
// }

// void test_select_by_id(){
//     WFMySQLTask *task = userService.get_user_info("1",[](bool ok,std::string msg,models::SysUser user){
//         if(ok){
//             cout<<"查询成功,用户ID:"<<user.id<<",用户名:"<<user.user_name<<"创建时间:"<<user.create_time<<endl;
//         }
//         else{
//             cout<<"查询失败 :" << msg << endl;
//         }
//     });
//     task->start();
//     getchar();
// }

int main(int argc,char *arg[]){
    // test_login();
    // test_register();
    // test_select_by_id();
    return 0;
}