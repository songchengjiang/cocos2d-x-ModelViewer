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

    // create the application instance
    std::wstring str(lpCmdLine);
    std::string filePath;
    for (auto iter : str)
    {
		if (iter == '\\') iter = '/';
        filePath.push_back(iter);
    }

    if (filePath.empty()) return -1;

    AppDelegate app(filePath);
    return Application::getInstance()->run();
}
