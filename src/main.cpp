#include "utils.h"
#include <ws2tcpip.h>
#include <winsock2.h>

int main()
{
    DeviceEnumerator enumerator;
    std::cout << enumerator << std::endl;
    std::cout << enumerator[3]->name << std::endl;
    return 0;
}
