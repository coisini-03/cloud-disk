
#include <iostream>
#include <services/TlbFileService.hpp>
#include <workflow/SubTask.h>

using namespace std;

dao::TlbFileDao filedao("mysql://root:123456@localhost:3306/clouddisk");
services::TlbFileService service(filedao);
void test_select_all(){
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

void test_download(){
    SubTask *task = nullptr;
    task = service.download(1,[](bool ok, std::string msg, std::string data){
        if(ok){
            cout << "download success" << endl;
            cout << data << endl;
        }else{
            cout << "download failed: " << msg << endl;
        }
    });
    dynamic_cast<WFMySQLTask *>(task)->start();
    getchar();
}
int main(int argc,char *arg[]){
    // test_select_all();
    test_download();
    return 0;
}