//
// Created by Mgepahmge on 25-4-17.
//

#ifndef WINCAPLITESNIFFER_H
#define WINCAPLITESNIFFER_H
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Tabs.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Native_File_Chooser.H>

#include <string>
#include <sstream>
#include <vector>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <atomic>
#include <mutex>

#include "PacketCapture.h"

namespace wcls::ui {
    class PacketSnifferUI {
        // 主窗口
        Fl_Window* window;

        // 设备选择相关
        Fl_Group* deviceGroup;
        Fl_Choice* deviceChoice;
        Fl_Button* refreshDeviceBtn;

        // 抓包控制相关
        Fl_Group* controlGroup;
        Fl_Button* startCaptureBtn;
        Fl_Button* stopCaptureBtn;
        Fl_Check_Button* promiscuousCheck;
        Fl_Int_Input* timeoutInput;
        Fl_Int_Input* maxPacketSizeInput;
        Fl_Input* logPathInput;
        Fl_Button* browsePathBtn;

        // 包显示相关
        Fl_Group* packetDisplayGroup;
        Fl_Text_Display* packetDisplay;
        Fl_Text_Buffer* packetBuffer;

        // 状态栏
        Fl_Box* statusBar;

        // 数据
        std::vector<std::pair<std::string, std::string>> deviceList;
        std::atomic<bool> isCapturing;
        std::thread updateThread;
        std::mutex displayMutex;
        size_t lastProcessedPacketCount;
        std::string captureStartTime;

        // 用于保存日志文件路径
        std::string logFilePath;

    public:
        PacketSnifferUI();

        void show();

        ~PacketSnifferUI();

    private:
        void refreshDeviceList();

        void startCapture();

        void stopCapture();

        void browsePath();

        static void saveLog(const std::string& fileName);

        void startUpdateThread();

        void stopCaptureThread();

        void updatePacketDisplay();

        void updateStatus(const std::string& status);

        void closeWindow();

        static void cb_refreshDevices(Fl_Widget*, void* userdata) {
            auto* ui = static_cast<PacketSnifferUI*>(userdata);
            ui->refreshDeviceList();
        }

        static void cb_startCapture(Fl_Widget*, void* userdata) {
            auto* ui = static_cast<PacketSnifferUI*>(userdata);
            ui->startCapture();
        }

        static void cb_stopCapture(Fl_Widget*, void* userdata) {
            auto* ui = static_cast<PacketSnifferUI*>(userdata);
            ui->stopCapture();
        }

        static void cb_browsePath(Fl_Widget*, void* userdata) {
            auto* ui = static_cast<PacketSnifferUI*>(userdata);
            ui->browsePath();
        }

        static void cb_closeWindow(Fl_Widget*, void* userdata) {
            auto* ui = static_cast<PacketSnifferUI*>(userdata);
            ui->closeWindow();
        }
    };
}

#endif //WINCAPLITESNIFFER_H
