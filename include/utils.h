//
// Created by Mgepahmge on 25-4-12.
//

#ifndef UTILS_H
#define UTILS_H
#include <iostream>
#include <vector>
#include <map>
#include <pcap.h>

class DeviceEnumerator {
public:
    friend std::ostream &operator<<(std::ostream &os, const DeviceEnumerator &deviceEnumerator);

    DeviceEnumerator();

    ~DeviceEnumerator();

    [[nodiscard]] std::vector<std::pair<std::string, std::string>> GetDeviceList() const;

    pcap_if_t* operator[](const size_t &index) const;

private:
    pcap_if_t *alldevs;
    size_t count;
};

#endif //UTILS_H
