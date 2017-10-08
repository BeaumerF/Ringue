#include "client.h"
#include <QLineEdit>
#include <QFormLayout>

Client::Client() : audio(new Audio())
{
    soc.open(QIODevice::ReadWrite);
    //connectTcp();
}

Client::~Client()
{
    soc.close();
}

void Client::connect_to_serv(QString IP2, int port_s)
{
    IP=IP2;
    port=port_s;

    qDebug() << IP2;
    qDebug() << port;
    soc.connectToHost(IP, port);
    QObject::connect(&soc, SIGNAL(connected()), this, SLOT(connexion_OK()));
    QObject::connect(&soc, SIGNAL(readyRead()), this, SLOT(lecture()));
}

void Client::send_texte(QByteArray t)
{
    qDebug() << "Envoi: " << t;
    soc.write(t);
}

void Client::connexion_OK()
{
    qDebug() << "Connexion OK";
    emit vers_IHM_connexion_OK();
}

void Client::lecture()
{
    QString ligne_r;
    QString user_list;

    while(soc.canReadLine())
    {
        ligne_r = soc.readLine();
        user_list += ligne_r;
    }
    qDebug() << "Lecture :" << user_list;
    ligne = user_list;

    std::string tmp = ligne.toUtf8().constData();
    parse(tmp);
}

void Client::parse(std::string tmp)
{
    if (tmp.substr(0, 4) == "CALL")
    {
        size_t posa = tmp.find_first_of(' ');
        size_t posb = tmp.find_first_of('\r');
        std::string clIP = tmp.substr(posa + 1, posb);
        this->clientudp = new ClientUDP(QString::fromStdString(clIP));
    }
    else if (tmp.substr(0, 4) == "STOP")
    {
        size_t posa = tmp.find_first_of(' ');
        size_t posb = tmp.find_first_of('\r');
        std::string clIP = tmp.substr(posa + 1, posb);
        delete this->clientudp;
    }
}

std::string Client::getLigne()
{
    return (ligne.toStdString());
}

void Client::call(QString ip)
{
    QString msg;

    msg = "CALL " + ip + "\r\n";
    this->send_texte(msg.toUtf8().constData());
    this->clientudp = new ClientUDP(ip);
}

void Client::hangup(QString ip)
{
    QString msg = "STOP " + ip + "\r\n";
    this->send_texte(msg.toLocal8Bit().constData());
    delete this->clientudp;
}

/*
#include <iostream>
void Client::connectTcp()
{
    QByteArray data; // <-- fill with data

    receive = new QTcpSocket(); // <-- needs to be a member variable: QTcpSocket * _pSocket;
    connect(receive, SIGNAL(readyRead()), SLOT(readTcpData()));

    receive->connectToHost(IP, port);
    if(receive->waitForConnected() ) {
       receive->write( data );
       std::cout << data.data() << std::endl;
       //this->clientudp = new ClientUDP(ip);
    }
}

void Client::readTcpData()
{
    QByteArray data = receive->readAll();
}*/
