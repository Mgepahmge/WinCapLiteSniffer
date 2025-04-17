#include "WinCapLiteSniffer.h"

namespace wcls::ui {
    PacketSnifferUI::PacketSnifferUI() :
            isCapturing(false),
            lastProcessedPacketCount(0),
            logFilePath("") {
            window = new Fl_Window(800, 600, "WinCapLiteSniffer");
            window->begin();

            auto* tabs = new Fl_Tabs(10, 10, 780, 550);
            tabs->begin();

            deviceGroup = new Fl_Group(10, 35, 780, 525, "Device Choose");
            deviceGroup->begin();

            deviceChoice = new Fl_Choice(120, 50, 500, 25, "Choose Device");
            deviceChoice->align(FL_ALIGN_LEFT);

            refreshDeviceBtn = new Fl_Button(650, 50, 120, 25, "Refresh");
            refreshDeviceBtn->callback(cb_refreshDevices, this);

            deviceGroup->end();

            controlGroup = new Fl_Group(10, 35, 780, 525, "Capture Settings");
            controlGroup->begin();

            startCaptureBtn = new Fl_Button(50, 60, 120, 40, "Start Capture");
            startCaptureBtn->callback(cb_startCapture, this);

            stopCaptureBtn = new Fl_Button(200, 60, 120, 40, "Stop Capture");
            stopCaptureBtn->callback(cb_stopCapture, this);
            stopCaptureBtn->deactivate();

            promiscuousCheck = new Fl_Check_Button(50, 120, 200, 25, "Promiscuous:");
            promiscuousCheck->value(1);

            timeoutInput = new Fl_Int_Input(150, 160, 100, 25, "Timeout(ms):");
            timeoutInput->value("1000");

            maxPacketSizeInput = new Fl_Int_Input(150, 200, 100, 25, "Max Packet Size:");
            maxPacketSizeInput->value("65535");

            logPathInput = new Fl_Input(150, 240, 450, 25, "Log Path:");
            logPathInput->value("./logs");

            browsePathBtn = new Fl_Button(620, 240, 120, 25, "Browse...");
            browsePathBtn->callback(cb_browsePath, this);

            controlGroup->end();

            packetDisplayGroup = new Fl_Group(10, 35, 780, 525, "Packets");
            packetDisplayGroup->begin();

            packetBuffer = new Fl_Text_Buffer();
            packetDisplay = new Fl_Text_Display(20, 50, 760, 500);
            packetDisplay->buffer(packetBuffer);
            packetDisplay->textfont(FL_COURIER);
            packetBuffer->text("Ready to start...\n");

            packetDisplayGroup->end();

            tabs->end();

            statusBar = new Fl_Box(10, 570, 780, 20, "Status: ready");
            statusBar->box(FL_DOWN_BOX);
            statusBar->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);

            window->end();
            window->callback(cb_closeWindow, this);

            refreshDeviceList();
        }

    void PacketSnifferUI::show() {
        window->show();
    }

    PacketSnifferUI::~PacketSnifferUI() {
        stopCaptureThread();
        delete window;
    }

    void PacketSnifferUI::refreshDeviceList() {
        deviceChoice->clear();
        deviceList = wcls::DeviceEnumerator::Instance().GetDeviceList();

        if (deviceList.empty()) {
            deviceChoice->add("No available device");
            deviceChoice->deactivate();
            startCaptureBtn->deactivate();
            updateStatus("Error: no device found");
        }
        else {
            for (size_t i = 0; i < deviceList.size(); i++) {
                deviceChoice->add(deviceList[i].second.c_str());
            }
            deviceChoice->value(0);
            deviceChoice->activate();
            startCaptureBtn->activate();
            updateStatus("Ready");
        }
    }

    void PacketSnifferUI::startCapture() {
        if (deviceList.empty() || deviceChoice->value() < 0) {
            fl_alert("Please select the network device first");
            return;
        }

        std::string deviceId = deviceList[deviceChoice->value()].first;
        PacketCapture::Instance().SetDevice(deviceId);

        PacketCapture::Instance().SetPromiscuous(promiscuousCheck->value() == 1);
        PacketCapture::Instance().SetTimeout(std::stoi(timeoutInput->value()));
        PacketCapture::Instance().SetMaxPacketSize(std::stoi(maxPacketSizeInput->value()));

        if (!PacketCapture::Instance().Initialize()) {
            fl_alert("Init failed!");
            return;
        }

        auto now = std::chrono::system_clock::now();
        auto now_time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&now_time_t), "%Y%m%d-%H%M%S");
        captureStartTime = ss.str();

        packetBuffer->text("Start capture...\n");

        if (PacketCapture::Instance().StartCapture()) {
            isCapturing = true;
            startCaptureBtn->deactivate();
            stopCaptureBtn->activate();
            deviceChoice->deactivate();
            updateStatus("Capturing...");
            lastProcessedPacketCount = 0;

            startUpdateThread();
        }
        else {
            fl_alert("Start capture failed!");
        }
    }

    void PacketSnifferUI::stopCapture() {
        if (!isCapturing) return;

        size_t packetCount = wcls::PacketCapture::Instance().StopCapture();
        isCapturing = false;
        stopCaptureThread();

        auto now = std::chrono::system_clock::now();
        auto now_time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&now_time_t), "%Y%m%d-%H%M%S");
        std::string captureEndTime = ss.str();

        std::string logFileName;
        if (!logFilePath.empty()) {
            logFileName = logFilePath + "/" + captureStartTime + "-" + captureEndTime + ".log";
        }
        else {
            logFileName = "./logs/" + captureStartTime + "-" + captureEndTime + ".log";
        }

        saveLog(logFileName);

        startCaptureBtn->activate();
        stopCaptureBtn->deactivate();
        deviceChoice->activate();

        std::stringstream statusMsg;
        statusMsg << "Finished, captured " << packetCount << ", the log has been saved to " << logFileName;
        updateStatus(statusMsg.str());
    }

    void PacketSnifferUI::browsePath() {
        Fl_Native_File_Chooser fileChooser;
        fileChooser.title("Select the log saving directory");
        fileChooser.directory(".");
        fileChooser.type(Fl_Native_File_Chooser::BROWSE_DIRECTORY);

        if (fileChooser.show() == 0) {
            logPathInput->value(fileChooser.filename());
            logFilePath = fileChooser.filename();
        }
    }

    void PacketSnifferUI::saveLog(const std::string& fileName) {
        try {
            size_t lastSlash = fileName.find_last_of("/\\");
            if (lastSlash != std::string::npos) {
                std::string dir = fileName.substr(0, lastSlash);
                system(("mkdir -p \"" + dir + "\"").c_str());
            }

            std::ofstream logFile(fileName);
            if (!logFile.is_open()) {
                fl_alert("Unable to open the log for writing!");
                return;
            }

            const auto& packets = wcls::PacketCapture::Instance().getPackets();
            for (const auto& packet : packets) {
                logFile << packet << "\n\n";
            }

            logFile.close();
        }
        catch (const std::exception& e) {
            fl_alert("An error occurred while saving the log: %s", e.what());
        }
    }

    void PacketSnifferUI::startUpdateThread() {
        updateThread = std::thread([this]() {
            while (isCapturing) {
                updatePacketDisplay();
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        });
    }

    void PacketSnifferUI::stopCaptureThread() {
        if (updateThread.joinable()) {
            updateThread.join();
        }
    }

    void PacketSnifferUI::updatePacketDisplay() {
            const auto& packets = wcls::PacketCapture::Instance().getPackets();
            size_t totalPackets = packets.size();

            if (totalPackets > lastProcessedPacketCount) {
                std::lock_guard<std::mutex> lock(displayMutex);

                std::stringstream newContent;
                for (size_t i = lastProcessedPacketCount; i < totalPackets; i++) {
                    newContent << "Packet #" << (i + 1) << " - " << packets[i].GetTime() << "\n";

                    if (packets[i].ethernetEnable) {
                        const auto& eth = packets[i].GetEthernetHeader();
                        newContent << "  Ethernet [";
                        newContent << "Dest: " << wcls::mac_to_string(eth.destMAC)
                            << "  Src: " << wcls::mac_to_string(eth.srcMAC);

                        if (ntohs(eth.etherType) == 0x8100 || ntohs(eth.etherType) == 0x88A8) {
                            newContent << "  VLAN ["
                                << "Type: 0x" << std::hex << std::setw(4)
                                << ntohs(eth.vlanEtherType) << " "
                                << ETHER_TYPE_TO_STR(eth.vlanEtherType)
                                << " | PCP: " << std::dec << ((ntohs(eth.vlanInfo) >> 13) & 0x07)
                                << " DEI: " << ((ntohs(eth.vlanInfo) >> 12) & 0x01)
                                << " VID: " << (ntohs(eth.vlanInfo) & 0x0FFF) << "]";
                        }
                        else {
                            newContent << "  EtherType: 0x" << std::hex << std::setw(4)
                                << ntohs(eth.etherType) << " "
                                << ETHER_TYPE_TO_STR(eth.etherType);
                        }
                        newContent << "]\n";
                    }

                    if (packets[i].ipv4Enable) {
                        const auto& header = packets[i].GetIPv4Header();

                        uint8_t version = (header.version_ihl >> 4) & 0x0F;
                        uint8_t ihl = (header.version_ihl & 0x0F) * 4;
                        uint16_t total_len = ntohs(header.total_length);
                        uint16_t flags_frag = ntohs(header.flags_fragment);

                        bool df_flag = (flags_frag & 0x4000) != 0;
                        bool mf_flag = (flags_frag & 0x2000) != 0;
                        uint16_t frag_offset = flags_frag & 0x1FFF;

                        char src_ip[INET_ADDRSTRLEN], dst_ip[INET_ADDRSTRLEN];
                        inet_ntop(AF_INET, &header.srcIP, src_ip, sizeof(src_ip));
                        inet_ntop(AF_INET, &header.destIP, dst_ip, sizeof(dst_ip));

                        newContent << "  IPv4 ["
                            << "v" << static_cast<int>(version)
                            << " hlen=" << static_cast<int>(ihl)
                            << " tos=0x" << std::hex << static_cast<int>(header.tos)
                            << " len=" << std::dec << total_len
                            << " id=0x" << std::hex << ntohs(header.identification)
                            << " flags=" << (df_flag ? "DF " : "") << (mf_flag ? "MF " : "")
                            << "offset=" << std::dec << frag_offset
                            << " ttl=" << static_cast<int>(header.ttl)
                            << " proto=" << IP_PROTOCOL_TO_STR(header.protocol)
                            << " chksum=0x" << std::hex << ntohs(header.checksum)
                            << " src=" << src_ip
                            << " dst=" << dst_ip
                            << "]\n";
                    }

                    if (packets[i].ipv6Enable) {
                        const auto& header = packets[i].GetIPv6Header();

                        uint32_t ver_class_flow = ntohl(header.version_class_flow);
                        uint16_t payload_len = ntohs(header.payload_length);

                        uint8_t version = (ver_class_flow >> 28) & 0x0F;
                        uint8_t traffic_class = (ver_class_flow >> 20) & 0xFF;
                        uint32_t flow_label = ver_class_flow & 0x000FFFFF;

                        char src_ip[INET6_ADDRSTRLEN], dst_ip[INET6_ADDRSTRLEN];
                        inet_ntop(AF_INET6, header.srcIP, src_ip, sizeof(src_ip));
                        inet_ntop(AF_INET6, header.destIP, dst_ip, sizeof(dst_ip));

                        newContent << "  IPv6 ["
                            << "v" << static_cast<int>(version)
                            << " tc=0x" << std::hex << static_cast<int>(traffic_class)
                            << " flow=0x" << std::setw(5) << std::setfill('0') << flow_label
                            << " plen=" << std::dec << payload_len
                            << " next=" << IP_PROTOCOL_TO_STR(header.next_header)
                            << " hlim=" << static_cast<int>(header.hop_limit)
                            << " src=" << src_ip
                            << " dst=" << dst_ip
                            << "]\n";
                    }

                    if (packets[i].tcpEnable) {
                        const auto& header = packets[i].GetTCPHeader();

                        uint16_t src_port = ntohs(header.src_port);
                        uint16_t dst_port = ntohs(header.dest_port);
                        uint32_t seq_num = ntohl(header.seq_num);
                        uint32_t ack_num = ntohl(header.ack_num);
                        uint16_t flags = ntohs(header.flags_doff);

                        uint8_t data_offset = (flags >> 12) & 0x0F; // 前4位
                        uint8_t flags_bits = flags & 0x1FF; // 低9位

                        bool urg = flags_bits & 0x20; // URG
                        bool ack = flags_bits & 0x10; // ACK
                        bool psh = flags_bits & 0x08; // PSH
                        bool rst = flags_bits & 0x04; // RST
                        bool syn = flags_bits & 0x02; // SYN
                        bool fin = flags_bits & 0x01; // FIN

                        newContent << "  TCP ["
                            << "sport=" << src_port
                            << " dport=" << dst_port
                            << " seq=0x" << std::hex << seq_num
                            << " ack=0x" << ack_num
                            << " flags=";

                        if (urg) newContent << "URG ";
                        if (ack) newContent << "ACK ";
                        if (psh) newContent << "PSH ";
                        if (rst) newContent << "RST ";
                        if (syn) newContent << "SYN ";
                        if (fin) newContent << "FIN ";

                        newContent << "win=" << std::dec << ntohs(header.window_size)
                            << " cksum=0x" << std::hex << ntohs(header.checksum)
                            << " urp=" << std::dec << ntohs(header.urgent_ptr)
                            << " doff=" << static_cast<int>(data_offset) * 4 << "]\n";
                    }

                    if (packets[i].udpEnable) {
                        const auto& header = packets[i].GetUDPHeader();

                        newContent << "  UDP ["
                            << "sport=" << ntohs(header.src_port)
                            << " dport=" << ntohs(header.dest_port)
                            << " len=" << ntohs(header.length)
                            << " cksum=0x" << std::hex << ntohs(header.checksum) << "]\n";
                    }

                    newContent << "  Packet Size: " << packets[i].GetPacketLength() << " bytes\n";
                    newContent << "--------------------------------------------\n";
                }

                Fl::lock();
                packetBuffer->append(newContent.str().c_str());
                packetDisplay->scroll(packetDisplay->count_lines(0, packetBuffer->length(), true), 0);

                std::stringstream statusUpdate;
                statusUpdate << "Status: Captured " << totalPackets << " ";
                statusBar->copy_label(statusUpdate.str().c_str());
                Fl::unlock();
                Fl::awake();

                lastProcessedPacketCount = totalPackets;
            }
        }

    void PacketSnifferUI::updateStatus(const std::string& status) {
        std::string statusText = "Status: " + status;
        statusBar->copy_label(statusText.c_str());
    }

    void PacketSnifferUI::closeWindow() {
        if (isCapturing) {
            if (fl_choice("Capturing, are you sure to quit?", "Cancel", "Yes", nullptr) == 0) {
                return;
            }
            stopCapture();
        }
        stopCaptureThread();
        Fl::delete_widget(window);
    }
}
