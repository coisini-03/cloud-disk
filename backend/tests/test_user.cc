#include <iostream>
#include "dao/UserDao.hpp"
#include "Hash.hpp"

using namespace std;
using namespace dao;

UserDao userDao("mysql://root:123456@localhost:3306/clouddisk");
void test_insert()
{

    models::SysUser user = {"test2", utils::Hash::hash_password("123456")};

    WFMySQLTask *task = userDao.insert(user, [](bool ok, std::string msg)
                                       {
        if(ok){
            cout << "insert success" << endl;
        }else{
            cout << "insert failed: " << msg << endl;
        } });
    task->start();
    getchar();
}

void test_select_by_username()
{
    models::SysUser user = {"test0", utils::Hash::hash_password("123456")};

    WFMySQLTask *task = userDao.select_by_username("test", [](bool ok, std::string msg, models::SysUser user)
                                       {
        if(ok){
            cout << "select_by_username success" << endl;
            cout << user << endl;
        }else{
            cout << "select_by_username failed: " << msg << endl;
        } });
    task->start();
    getchar();

}

void test_select_by_id(){
    WFMySQLTask *task = userDao.select_by_id(1, [](bool ok, std::string msg, models::SysUser user)
                                       {
        if(ok){
            cout << "select_by_id success" << endl;
            cout << user << endl;
        }else{
            cout << "select_by_id failed: " << msg << endl;
        } });
    task->start();
    getchar();
}

int main(int argc, char *arg[])
{

    // test_insert();
    // test_select_by_username();
    test_select_by_id();
    return 0;
}