#include <QGuiApplication>
#include <QProcess>
#include <QScreen>
#include <QString>

#include "x.hpp"

X::X()
{
    this->screen = QGuiApplication::primaryScreen();
}

bool X::supported()
{
    return true;
}

uint8_t X::priority()
{
    return 2;
}

void X::set(int brightness)
{
    if (this->screen != nullptr) {
        QProcess process(this);
        QString stampa = QStringLiteral("[LUM] BARRA = %1 RISULTATO %2 \n").arg(brightness).arg((int)(brightness / 25.5));
        //qDebug() << stampa.toStdString();
        process.start(QString("/home/gioele/RPi-USB-Brightness/64/lite/Raspi_USB_Backlight_nogui -b %1").arg((int)(brightness / 25.5)));
        process.waitForFinished();
    }
}
