#include "Robot.h"

int main()
{
    Robot robot;

    while (robot.isrunning())
    {
        robot.update();

        robot.render();
    }

    return 0;
}