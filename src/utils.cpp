//
// Created by Mgepahmge on 25-4-12.
//
#include "utils.h"

DeviceEnumerator::DeviceEnumerator() : alldevs(NULL), count(0) {
    char errbuf[PCAP_ERRBUF_SIZE];
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        std::cerr << "Error finding devices: " << errbuf << std::endl;
    }
    for (pcap_if_t *d = alldevs; d != NULL; d = d->next) {
        count++;
    }
}

DeviceEnumerator::~DeviceEnumerator() {
    pcap_freealldevs(alldevs);
}

std::map<std::string, std::string> DeviceEnumerator::GetDeviceList() const {
    std::map<std::string, std::string> deviceList;
    for (pcap_if_t *d = alldevs; d != NULL; d = d->next) {
        deviceList[d->name] = d->description;
    }
    return deviceList;
}

pcap_if_t* DeviceEnumerator::operator[](const size_t& index) const {
    if (index >= count) {
        std::cerr << "Invalid index: " << index << std::endl;
        return nullptr;
    }
    auto d = alldevs;
    for (auto i = 0; i < index; i++) {
        d = d->next;
    }
    return d;
}

std::ostream& operator<<(std::ostream& os, const DeviceEnumerator& deviceEnumerator) {
    if (deviceEnumerator.count == 0) {
        os << "No devices found." << std::endl;
        return os;
    }
    auto i = 0;
    for (const auto& [fst, snd] : deviceEnumerator.GetDeviceList()) {
        os << i << ". " << fst << " " << "(" << snd << ")" << std::endl;
        i++;
    }
    return os;
}
