//
// Created by Mgepahmge on 25-4-12.
//

#ifndef UTILS_H
#define UTILS_H
#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <pcap.h>

namespace wcls {
    class DeviceEnumerator {
    public:
        friend std::ostream &operator<<(std::ostream &os, const DeviceEnumerator &deviceEnumerator);

        DeviceEnumerator(const DeviceEnumerator &deviceEnumerator) = delete;

        DeviceEnumerator &operator=(const DeviceEnumerator &deviceEnumerator) = delete;

        static DeviceEnumerator& Instance();

        ~DeviceEnumerator();

        [[nodiscard]] std::vector<std::pair<std::string, std::string>> GetDeviceList() const;

        pcap_if_t* operator[](const size_t &index) const;

    private:
        pcap_if_t *alldevs;
        size_t count;

        DeviceEnumerator();
    };
}

#endif //UTILS_H
