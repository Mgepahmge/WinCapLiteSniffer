#include "PacketCapture.h"

int main()
{
    wcls::DeviceEnumerator enumerator;
    std::cout << enumerator << std::endl;
    wcls::PacketCapture::Instance().SetDevice(enumerator[5]->name);
    wcls::PacketCapture::Instance().Initialize();
    wcls::PacketCapture::Instance().StartCapture();
    std::this_thread::sleep_for(std::chrono::seconds(50));
    wcls::PacketCapture::Instance().StopCapture();
}
