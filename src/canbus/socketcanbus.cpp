#include <QTcpSocket>
#include <QStringList>
#include <QString>
#include "canbus/socketcanbus.hpp"

SocketCANBus::SocketCANBus(QString canInterface)
{
    if (QCanBus::instance()->plugins().contains(QStringLiteral("socketcan")))
    {
        DASH_LOG(info) << "[SocketCANBus] 'socketcan' Available";
        socketCANAvailable = true;
        /*QString errorString;
        bus = QCanBus::instance()->createDevice(
            QStringLiteral("socketcan"), canInterface, &errorString);
        if (!bus)
        {
            DASH_LOG(error) << "[SocketCANBus] Error creating CAN device, " << errorString.toStdString();
        }
        else
        {
            DASH_LOG(info) << "[SocketCANBus] Connecting CAN interface " << canInterface.toStdString();
            bus->connectDevice();
            connect(bus, &QCanBusDevice::framesReceived,
                    this, &SocketCANBus::framesAvailable);
        }*/

        this->socket.connectToHost("127.0.0.1", 7070);
        this->socket.waitForConnected();

        if (this->socket.state() == QAbstractSocket::ConnectedState)
        {
            QString allineamento = "CAN USER ALIGN RIGHT\r\n";
            this->socket.write(allineamento.toUtf8());

            QString apertura = "CAN USER OPEN CH1 95K2\r\n";
            this->socket.write(apertura.toUtf8());
            
            QString maschera = "CAN USER MASK CH1 0FFF\r\n";
            this->socket.write(maschera.toUtf8());

            QString filtro10 = "CAN USER FILTER CH1 0 0206\r\n";
            this->socket.write(filtro0.toUtf8());

            QString filtro11 = "CAN USER FILTER CH1 1 0450\r\n";
            this->socket.write(filtro1.toUtf8());

            DASH_LOG(info) << "[SocketCANBus] Connesso a Carberry";
        }
        else
        {
            DASH_LOG(error) << "[SocketCANBus] Errore di connessione a Carberry";
        }
    }
}

SocketCANBus::~SocketCANBus()
{
    DASH_LOG(info) << "[SocketCANBus] Disconnessione da Carberry";
    /* if (bus->state() == QCanBusDevice::ConnectedState)
     {
         bus->disconnectDevice();
     }*/

    this->socket.close();
}

bool SocketCANBus::writeFrame(QCanBusFrame frame)
{
    // return bus->writeFrame(frame);
    if(this->socket.write("p")<0) //maggiore di frame.lenght
        return false;
    return true;
}

SocketCANBus *SocketCANBus::get_instance()
{
    static SocketCANBus bus(Config::get_instance()->get_vehicle_interface());
    return &bus;
}

// QSerialBus readAllFrames introduced in 5.12 - Pi OS is stuck on 5.11, so implement our own
/*QVector<QCanBusFrame> SocketCANBus::readAllFrames(int numFrames)
{
    QVector<QCanBusFrame> frames = QVector<QCanBusFrame>();
    for (int i = 0; i < numFrames; i++)
    {
        frames.append(bus->readFrame());
    }
    return frames;
}

void SocketCANBus::framesAvailable()
{
    int numFrames = bus->framesAvailable();
    if (numFrames > 0)
    {
        QVector<QCanBusFrame> frames = readAllFrames(numFrames);
        for (int i = 0; i < frames.length(); i++)
        {
            QCanBusFrame frame = frames.at(i);
            if (callbacks.find(frame.frameId()) != callbacks.end())
            {
                for (auto callback : callbacks[frame.frameId()])
                {
                    callback(frame.payload());
                }
            }
        }
    }
}*/

void SocketCANBus::registerFrameHandler(int id, std::function<void(QByteArray)> callback)
{

    QStringList list;
    while (this->socket.canReadLine())
    {
        QString data = QString(this->socket.readLine());
        DASH_LOG(info) << data.toStdString();
        list.append(data);
    }

    /*
        callbacks[id].push_back(callback);
        QCanBusDevice::Filter filter;
        filter.frameId = id;
        filter.frameIdMask = 0xFFF;
        filter.format = QCanBusDevice::Filter::MatchBaseAndExtendedFormat;
        filter.type = QCanBusFrame::InvalidFrame;
        filterList.append(filter);
        bus->setConfigurationParameter(QCanBusDevice::RawFilterKey, QVariant::fromValue(filterList));
    */
}
