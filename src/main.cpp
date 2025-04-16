#include <fstream>

#include "PacketCapture.h"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>


void button_callback(Fl_Widget* widget, void* data) {
    auto* box = static_cast<Fl_Box*>(data);
    static int count = 0;
    count++;
    const std::string buffer = std::to_string(count);
    box->label(buffer.c_str());
    box->redraw();
}

int main()
{
    auto* window = new Fl_Window(400, 300, "FLTK");

    auto* box = new Fl_Box(100, 50, 200, 50, "FLTK");
    box->box(FL_UP_BOX);
    box->labelfont(FL_BOLD);
    box->labelsize(18);
    box->labeltype(FL_SHADOW_LABEL);

    auto* button = new Fl_Button(150, 150, 100, 40, "click me");
    button->callback(button_callback, box);

    window->end();
    window->show();

    return Fl::run();
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
