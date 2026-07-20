#include <iostream>
#include <dao/TlbFileDao.hpp>
#include "TlbFile.hpp"
#include "Hash.hpp"
using namespace std;

int main(int argc,char *arg[]){
    dao::TlbFileDao dao("mysql://root:123456@localhost:3306/clouddisk");
    models::TlbFile tlbf;
    tlbf.uid = 1;
    tlbf.filename = "test.txt";
    const unsigned char *data = reinterpret_cast<const unsigned char *>("test.txt"); 
    tlbf.hashcode = utils::Hash::generate_hashcode(data,9);
    tlbf.size = 1024;
    
    WFMySQLTask *task = dao.insert(tlbf,[](bool ok, std::string msg){
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