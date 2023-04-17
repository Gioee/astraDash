#pragma once

#include <QCanBus>
#include <string>
#include <QVector>
#include <QByteArray>
#include <QVariant>
#include <QTcpSocket>
// #include <QAbstractSocket>
#include "DashLog.hpp"
#include "canbus/ICANBus.hpp"
#include "app/config.hpp"

class SocketCANBus : public ICANBus, public QObject
{
    Q_OBJECT
public:
    SocketCANBus(QObject *parent = 0, QString canInterface = "can0");
    ~SocketCANBus();
    static SocketCANBus *get_instance();
    bool writeFrame(QString frame) override;

private:
    bool socketCANAvailable = false;
    QTcpSocket socket;
    QCanBusDevice *bus;
    int lumws = 10;

signals:

private slots:
    void readyRead();
};
