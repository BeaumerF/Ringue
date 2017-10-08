#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>

class QLabel;
class QPushButton;
class QTextEdit;
class QLineEdit;

class StartupDialog : public QDialog
{
    Q_OBJECT

public:
    StartupDialog(QWidget *parent = 0);
    void onTextChanged();

    QLineEdit *nameText;
    QLineEdit *ipText;
    QLineEdit *portText;

private:
    QLabel *nameLabel;
    QLabel *ipLabel;
    QLabel *portLabel;
    QPushButton *okButton;
    QPushButton *cancelButton;
};

#endif // ADDDIALOG_H
