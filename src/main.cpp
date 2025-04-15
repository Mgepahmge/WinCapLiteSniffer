#include <fstream>

#include "PacketCapture.h"

int main()
{
    std::cout << wcls::DeviceEnumerator::Instance() << std::endl;
    wcls::PacketCapture::Instance().SetDevice(wcls::DeviceEnumerator::Instance()[1]->name);
    wcls::PacketCapture::Instance().Initialize();
    if (const bool ret = wcls::PacketCapture::Instance().StartCapture(); !ret)
    {
        std::cout << "StartCapture failed" << std::endl;
        return -1;
    }
    std::this_thread::sleep_for(std::chrono::seconds(10));
    std::cout << "Captured : " << wcls::PacketCapture::Instance().StopCapture() << std::endl;
    std::ofstream file("result.txt");
    for (auto& packet : wcls::PacketCapture::Instance().getPackets()) {
        file << packet << std::endl;
    }
    file.close();
}
