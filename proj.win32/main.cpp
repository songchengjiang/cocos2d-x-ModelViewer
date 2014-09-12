#include "main.h"
#include "AppDelegate.h"
#include "cocos2d.h"

USING_NS_CC;

int main(int argc, char *argv[])
{
    if (argc < 2) return -1;
    AppDelegate app(argv[1]);
    return Application::getInstance()->run();
}