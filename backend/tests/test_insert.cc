#include <iostream>
#include "dao/UserDao.hpp"
#include <utils/Hash.hpp>

using namespace std;
using namespace dao;


int main(int argc,char *arg[]){
    UserDao userDao("mysql://root:123456@localhost:3306/clouddisk");
    models::SysUser user = {"test1",utils::Hash::hash_password("123456")};
    
    WFMySQLTask* task = userDao.insert(user,[](bool ok, std::string msg){
        if(ok){
            cout << "insert success" << endl;
        }else{
            cout << "insert failed: " << msg << endl;
        }
    });
    task->start();
    getchar();
    return 0;
}