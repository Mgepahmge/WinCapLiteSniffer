#include "PacketCapture.h"

int main()
{
    std::cout << wcls::DeviceEnumerator::Instance() << std::endl;
    wcls::PacketCapture::Instance().SetDevice(wcls::DeviceEnumerator::Instance()[5]->name);
    wcls::PacketCapture::Instance().Initialize();
    if (const bool ret = wcls::PacketCapture::Instance().StartCapture(); !ret)
    {
        std::cout << "StartCapture failed" << std::endl;
        return -1;
    }
    std::this_thread::sleep_for(std::chrono::seconds(50));
    wcls::PacketCapture::Instance().StopCapture();
}
