/*
    Thats why:
    https://github.com/SambaGodschynski/werckmeister/issues/101
*/

#include "app/os_win_ipc_kill_handler.hpp"
#include <iostream>
#include <windows.h>

int main(int argc, char **argv)
{
    if (argc <= 1)
    {
        std::cout << "missing PID" << std::endl;
        return 1;
    }
    int pid = atoi(argv[1]);
    try
    {
        app::os::InterProcessMessageQueue queue(pid);
        queue.sendSigint();
    }
    catch (const boost::interprocess::interprocess_exception)
    {
        std::cout << "could not access ipc queue on PID " << pid << std::endl;
    }
    catch (const std::exception &ex)
    {
        std::cout << ex.what() << std::endl;
        return 1;
    }
    return 0;
}