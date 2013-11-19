#include "headers/controller.h"

int main(int argc, char *argv[])
{
    Controller *controller = new Controller(argc, argv);
    int ret = controller->app->exec();

    //clean up
    controller->telApi->unregisterAccount();
    pjsua_destroy();
    delete controller;

    //thanks for all
    return ret;
}
