#include "MainWindow.h"
#include "client.h"

#include <QAction>
#include <QFileDialog>
#include <QMenuBar>
#include <QInputDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QCoreApplication>

MainWindow::MainWindow() : client(new Client())
{
    ipWidget = new IpWidget;
    ch = std::make_shared<CmdHandler>();
    sd = std::make_shared<StartupDialog>();
    setCentralWidget(ipWidget);
    createMenus();
    setWindowTitle(tr("Babel"));
    getUserInfos();
    //updateUsers();
}

void MainWindow::getUserInfos()
{
    QString t;

    if (sd->exec()) {
           username = sd->nameText->text();
           ip = sd->ipText->text();
           port = sd->portText->text();
    }

   if (!username.isEmpty() && !ip.isEmpty() && !port.isEmpty())
   {
        ipWidget->setClient(client);
        client->connect_to_serv(ip, port.toInt());
        t = "CONNECT " + username + "\r\n";
        client->send_texte(t.toUtf8().constData());
   }
 }

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));

    refreshAct = new QAction(tr("&Refresh"), this);
    fileMenu->addAction(refreshAct);
    connect(refreshAct, &QAction::triggered, this, &MainWindow::updateUsers);

    fileMenu->addSeparator();

    exitAct = new QAction(tr("E&xit"), this);
    fileMenu->addAction(exitAct);
    connect(exitAct, &QAction::triggered, this, &QWidget::close);

    toolMenu = menuBar()->addMenu(tr("&Tools"));

    callAct = new QAction(tr("&Call contact"), this);
    callAct->setEnabled(false);
    toolMenu->addAction(callAct);
    connect(callAct, &QAction::triggered, ipWidget, &IpWidget::call);

    hangupAct = new QAction(tr("&Hangup contact"), this);
    hangupAct->setEnabled(false);
    toolMenu->addAction(hangupAct);
    connect(hangupAct, &QAction::triggered, ipWidget, &IpWidget::hangup);

    connect(ipWidget, &IpWidget::selectionChanged, this, &MainWindow::updateActions);
}

void MainWindow::updateActions(const QItemSelection &selection)
{
    QModelIndexList indexes = selection.indexes();

    if (!indexes.isEmpty()) {
        callAct->setEnabled(true);
        hangupAct->setEnabled(true);
    } else {
        callAct->setEnabled(false);
        hangupAct->setEnabled(false);
    }
}

void MainWindow::updateUsers()
{
    client->send_texte("GETADDRESSES\r\n");
    std::string request = client->getLigne();

    std::cout << request;
    std::vector<std::string> users = ch->splitRequest(request);

    if (users.front() == UPDT_USR_LIST)
      ipWidget->updateUserList(users);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_F5)
        updateUsers();
}
