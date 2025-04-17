#include "WinCapLiteSniffer.h"



int main() {
    wcls::ui::PacketSnifferUI app;
    app.show();
    return Fl::run();
}
