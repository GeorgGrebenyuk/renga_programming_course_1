#include <iostream>
#import "RengaCOMAPI.tlb"
#include "Renga/CreateApplication.hpp"

int main()
{
    ::CoInitialize(nullptr);
    Renga::IApplicationPtr r_app = Renga::CreateApplication(CLSCTX_LOCAL_SERVER);

    HRESULT status_open = r_app->OpenProject("G:\\Soft\\Renga\\Samples\\Cottage2.rnp");
    if (status_open == S_OK)std::cout << "file_path = " << r_app->Project->FilePath << std::endl;
    else std::cout << "status = " << status_open << std::endl;
    

    printf("End!");
    return 0;

}
