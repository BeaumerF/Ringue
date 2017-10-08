#include "StartupDialog.h"

#include <QtWidgets>

StartupDialog::StartupDialog(QWidget *parent)
    : QDialog(parent)
{
    nameLabel = new QLabel("User name");
    ipLabel = new QLabel("IP Address");

    portLabel = new QLabel("Port");
    okButton = new QPushButton("OK");
    cancelButton = new QPushButton("Cancel");

    nameText = new QLineEdit;
    ipText = new QLineEdit;
    portText = new QLineEdit;

    QGridLayout *gLayout = new QGridLayout;
    //gLayout->setColumnStretch(1, 2);

    gLayout->addWidget(nameLabel, 0, 0, Qt::AlignLeft|Qt::AlignTop);
    gLayout->addWidget(ipLabel, 1, 0, Qt::AlignLeft);
    gLayout->addWidget(portLabel, 2, 0, Qt::AlignLeft);

    gLayout->addWidget(nameText, 0, 1, Qt::AlignRight|Qt::AlignTop);
    gLayout->addWidget(ipText, 1, 1, Qt::AlignRight);
    gLayout->addWidget(portText, 2, 1, Qt::AlignRight);

    okButton->setEnabled(false);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    gLayout->addLayout(buttonLayout, 3, 1, Qt::AlignCenter);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(gLayout);
    setLayout(mainLayout);

    connect(nameText, &QLineEdit::textChanged, this, &StartupDialog::onTextChanged);
    connect(ipText, &QLineEdit::textChanged, this, &StartupDialog::onTextChanged);
    connect(portText, &QLineEdit::textChanged, this, &StartupDialog::onTextChanged);

    connect(okButton, &QAbstractButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);

    setWindowTitle(tr("Log in"));
}

void StartupDialog::onTextChanged()
{
    if (nameText->text().isEmpty() || ipText->text().isEmpty() || portText->text().isEmpty())
    {
        okButton->setEnabled(false);
    }
    else
    {
        okButton->setEnabled(true);
    }
}
