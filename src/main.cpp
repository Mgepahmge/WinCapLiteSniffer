#include "PacketCapture.h"

int main()
{
    wcls::DeviceEnumerator enumerator;
    std::cout << enumerator << std::endl;
    wcls::PacketCapture capture(enumerator[5]->name, true, 1000, 65535);
    capture.Initialize();
    capture.StartCapture();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    capture.StopCapture();
}
