#include "main.h"
#include "AppDelegate.h"
#include "cocos2d.h"

USING_NS_CC;

int main(int argc, char *argv[])
{
    if (argc < 2) return -1;
    std::string path = argv[1];
    for (std::string::iterator iter = path.begin(); iter != path.end(); ++iter){
        if (*iter == '\\') *iter = '/';
    }
    AppDelegate app(path);
    return Application::getInstance()->run();
}