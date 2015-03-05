#include "main.h"
#include "AppDelegate.h"
#include "cocos2d.h"

USING_NS_CC;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    std::wstring cmdline(lpCmdLine);
    std::string path;
    for (auto iter : cmdline){
        if (iter == '\\')
            iter = '/';

        path.push_back(iter);
    }
    // create the application instance
    AppDelegate app(path);
    return Application::getInstance()->run();
}
