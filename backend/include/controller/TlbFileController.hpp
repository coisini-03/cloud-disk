#pragma once

#include <wfrest/BluePrint.h>
#include <services/TlbFileService.hpp>
namespace controller
{

    class TlbFileController
    {
    private:
        services::TlbFileService &tlbFileService_;
        void uploadHandler(const wfrest::HttpReq *req, wfrest::HttpResp *resp,SeriesWork *sw);
        void listAllHandler(const wfrest::HttpReq *req, wfrest::HttpResp *resp,SeriesWork *sw);
    public:
        TlbFileController(services::TlbFileService &tlbFileService);
        ~TlbFileController();
        void registerRoutes(wfrest::BluePrint &bp);
    };

}
