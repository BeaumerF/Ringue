#include <QUdpSocket>
#include <qaudioinput.h>
#include <qaudiooutput.h>

class ClientUDP : public QObject
{
  Q_OBJECT

  public:
      ClientUDP(QString ip);
      ~ClientUDP();
      void call();

  private:
      QString ip;
      int port;
      QAudioOutput *output;
      QUdpSocket *soc;
      QIODevice *device;

  signals:

  public slots:
      void server();
      void playData();
};
