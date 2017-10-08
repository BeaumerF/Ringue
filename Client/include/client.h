#ifndef CLIENT_H
#define CLIENT_H

#include <QDebug>
#include <QObject>
#include <QTcpSocket>
#include <QCoreApplication>
#include <iostream>
#include "audio.h"
#include "clientudp.h"

class Client : public QObject
{
    Q_OBJECT
    public :
        Client();
        ~Client();
        void    connect_to_serv(QString IP2, int port_s);
        void    parse(std::string tmp);
    public slots :
        void send_texte(QByteArray t); // en provenance de l'IHM et écrit sur la socket
        void lecture();       // en provenance de la socket, lit la socket, émet un signal vers l'IHM
        std::string getLigne();
        void call(QString ip);
        void hangup(QString ip);
    private slots :
        void connexion_OK();  // en provenance de la socket et émet un signal vers l'IHM
    signals :
        void vers_IHM_connexion_OK();
        void vers_IHM_texte(QString);
    private :
        QString IP;
        int port;
        QTcpSocket soc;
        QString ligne;
        Audio *audio;
        ClientUDP *clientudp;
};

#endif // CLIENT_H
