#include "IpWidget.h"

#include <QtWidgets>
#include <iostream>
#include "client.h"

IpWidget::IpWidget(QWidget *parent)
    : QTabWidget(parent)
{
    table = new TableModel(this);

    setupTabs();
}

void IpWidget::setupTabs()
{
    QStringList groups;
    groups << "ABC" << "DEF" << "GHI" << "JKL" << "MNO" << "PQR" << "STU" << "VW" << "XYZ";

    for (int i = 0; i < groups.size(); ++i) {
        QString str = groups.at(i);
        QString regExp = QString("^[%1].*").arg(str);

        proxyModel = new QSortFilterProxyModel(this);
        proxyModel->setSourceModel(table);
        proxyModel->setFilterRegExp(QRegExp(regExp, Qt::CaseInsensitive));
        proxyModel->setFilterKeyColumn(0);

        QTableView *tableView = new QTableView;
        tableView->setModel(proxyModel);

        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView->horizontalHeader()->setStretchLastSection(true);
        tableView->verticalHeader()->hide();
        tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableView->setSelectionMode(QAbstractItemView::SingleSelection);

        tableView->setSortingEnabled(true);

        connect(tableView->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this, &IpWidget::selectionChanged);

        addTab(tableView, str);
    }
}

void IpWidget::addEntry(QString name, QString ip)
{
    QList<QPair<QString, QString>> list = table->getList();
    QPair<QString, QString> pair(name, ip);

    table->insertRows(0, 1, QModelIndex());

    QModelIndex index = table->index(0, 0, QModelIndex());
    table->setData(index, name, Qt::EditRole);

    index = table->index(0, 1, QModelIndex());
    table->setData(index, ip, Qt::EditRole);
}

void IpWidget::updateUserList(std::vector<std::string> const &requestVector)
{
    size_t pos;
    std::string ip, name;

    if (table->rowCount(QModelIndex()))
        table->removeRows(0, table->rowCount(QModelIndex()));

    for (size_t i = 1; i < requestVector.size(); ++i)
    {
        if (requestVector[i] != "101 End\r")
        {
            pos = requestVector[i].find_first_of(' ');

            ip = requestVector[i].substr(0, pos);
            name = requestVector[i].substr(pos + 1);


            addEntry(QString::fromStdString(name), QString::fromStdString(ip));
        }
    }
}

void IpWidget::call()
{
    QTableView *temp = static_cast<QTableView*>(currentWidget());
    QSortFilterProxyModel *proxy = static_cast<QSortFilterProxyModel*>(temp->model());
    QItemSelectionModel *selectionModel = temp->selectionModel();

    QModelIndexList indexes = selectionModel->selectedRows();
    QString ip;
    int row = -1;

    foreach (QModelIndex index, indexes) {
        row = proxy->mapToSource(index).row();

        QModelIndex ipIndex = table->index(row, 1, QModelIndex());
        QVariant varAddr = table->data(ipIndex, Qt::DisplayRole);
        ip = varAddr.toString();
    }
    std::cout << "Calling IP " << ip.toLocal8Bit().constData() << std::endl;
    cl->call(ip);
}

void IpWidget::hangup()
{
    QTableView *temp = static_cast<QTableView*>(currentWidget());
    QSortFilterProxyModel *proxy = static_cast<QSortFilterProxyModel*>(temp->model());
    QItemSelectionModel *selectionModel = temp->selectionModel();

    QModelIndexList indexes = selectionModel->selectedRows();
    QString ip;
    int row = -1;

    foreach (QModelIndex index, indexes) {
        row = proxy->mapToSource(index).row();

        QModelIndex ipIndex = table->index(row, 1, QModelIndex());
        QVariant varAddr = table->data(ipIndex, Qt::DisplayRole);
        ip = varAddr.toString();
    }
    std::cout << "Hangup on IP " << ip.toLocal8Bit().constData() << std::endl;
    cl->hangup(ip);
}

Client *IpWidget::getClient()
{
    return (cl);
}

void IpWidget::setClient(Client *client)
{
    cl = client;
}
