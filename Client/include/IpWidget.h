#ifndef IPWIDGET_H
#define IPWIDGET_H

#include "TableModel.h"
#include "client.h"

#include <QItemSelection>
#include <QTabWidget>
#include <vector>

class QSortFilterProxyModel;
class QItemSelectionModel;

class IpWidget : public QTabWidget
{
    Q_OBJECT

public:
    IpWidget(QWidget *parent = 0);
    void addEntry(QString name, QString address);
    void updateUserList(std::vector<std::string> const &requestVector);
    Client *getClient();
    void setClient(Client *client);
    QItemSelectionModel *getSelectionModel();

public slots:
    void call();
    void hangup();

signals:
    void selectionChanged (const QItemSelection &selected);

private:
    void setupTabs();

    TableModel *table;
    QSortFilterProxyModel *proxyModel;
    Client *cl;
    QAction *callAct;
    QAction *hangupAct;
};

#endif // IPWIDGET_H
