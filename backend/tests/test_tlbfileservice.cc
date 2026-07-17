
#include <iostream>
#include <services/TlbFileService.hpp>
#include <workflow/SubTask.h>

using namespace std;

void test_select_all(){
    dao::TlbFileDao dao("mysql://root:123456@localhost:3306/clouddisk");
    services::TlbFileService service(dao);

    SubTask *task = nullptr;
    task = service.list_all(19,[](bool ok, std::string msg, std::vector<models::TlbFile> files){
        if(ok){
            cout << "list all success" << endl;
            if(files.size() > 0){
                for (auto &file : files)
                {
                    cout << file << endl;
                }
            }

        }else{
            cout << "list all failed: " << msg << endl;
        }
    });
    dynamic_cast<WFMySQLTask *>(task)->start();
    getchar();
}

int main(int argc,char *arg[]){
    test_select_all();
    return 0;
}