#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "IpWidget.h"
#include "CmdHandler.h"
#include "client.h"
#include "StartupDialog.h"

#include <QMainWindow>
#include <memory>

#define UPDT_USR_LIST "101 Address IP:"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private slots:
    void updateUsers();
    void updateActions(const QItemSelection &selection);

private:
    void createMenus();
    void getUserInfos();
    void keyPressEvent(QKeyEvent *k);

    IpWidget *ipWidget;
    QMenu *fileMenu;
    QMenu *toolMenu;
    QAction *refreshAct;
    QAction *exitAct;
    QAction *callAct;
    QAction *hangupAct;
    Client  *client;
    std::shared_ptr<CmdHandler> ch;
    std::shared_ptr<StartupDialog> sd;
    QString username;
    QString ip;
    QString port;
};

#endif // MAINWINDOW_H
