#include "clientudp.h"

ClientUDP::ClientUDP(QString ip_)
{
//   socket = new QUdpSocket(this);

//    //We need to bind the UDP socket to an address and a port
//    socket->bind(QHostAddress::LocalHost, 1234);         //ex. Address localhost, port 1234

//    connect(socket, SIGNAL(server()), this, SLOT(server()));
    this->ip = ip_;
    this->port = 5199;
    call();
    server();
}

ClientUDP::~ClientUDP()
{
    soc->close();
}

void ClientUDP::server()
{
//    QByteArray Buffer;
//    Buffer.resize(socket->pendingDatagramSize());

//    QHostAddress sender;
//    quint16 senderPort;
//    socket->readDatagram(Buffer.data(), Buffer.size(), &sender, &senderPort);
//    qDebug() << sender.toString() << "call you";
    soc = new QUdpSocket();
    soc->bind(port);
    QAudioFormat format;
    format.setSampleRate(128000);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());


    if (!info.isFormatSupported(format))
        format = info.nearestFormat(format);

    output = new QAudioOutput(format);
    device = output->start();
    connect(soc, SIGNAL(readyRead()), this, SLOT(playData()));
}

void ClientUDP::playData()
{
    //You need to read datagrams from the udp socket
    while (soc->hasPendingDatagrams())
    {
        QByteArray data;
        data.resize(soc->pendingDatagramSize());
        soc->readDatagram(data.data(), data.size());
        device->write(data.data(), data.size());
    }
}

void ClientUDP::call()
{/*
    QByteArray Data;
     Data.append("Hello from UDP land");

     socket->writeDatagram(Data,QHostAddress::LocalHost,1234);*/
    QAudioFormat format;
    format.setSampleRate(128000);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    //If format isn't supported find the nearest supported
    QAudioDeviceInfo info(QAudioDeviceInfo::defaultInputDevice());
    if (!info.isFormatSupported(format))
        format = info.nearestFormat(format);

    QAudioInput* input = new QAudioInput(format);
    QUdpSocket* socket = new QUdpSocket();
    socket->connectToHost(ip.toUtf8().constData(), port);
    input->start(socket);
}
