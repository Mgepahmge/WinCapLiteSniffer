#include "PacketCapture.h"

int main()
{
    std::cout << wcls::DeviceEnumerator::Instance() << std::endl;
    wcls::PacketCapture::Instance().SetDevice(wcls::DeviceEnumerator::Instance()[5]->name);
    wcls::PacketCapture::Instance().Initialize();
    std::cout << wcls::PacketCapture::Instance().StartCapture() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(50));
    wcls::PacketCapture::Instance().StopCapture();
}
