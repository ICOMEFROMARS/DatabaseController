#include <QApplication>
#include "MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv); // UI için QCoreApplication yerine QApplication kullanılmalı.

    MainWindow w;
    w.show(); // Ana pencereyi göster.

    return app.exec(); // Qt uygulamasını başlat.
}
