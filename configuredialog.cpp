// Copyright (C) 2020  Gabriele Salvato

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "configuredialog.h"

#include <QGridLayout>
#include <QDebug>
#include <QMessageBox>

#include <sys/types.h>
#include <signal.h>


ConfigureDialog::ConfigureDialog(QWidget *parent)
    : QDialog(parent)
{
    usernameLabel.setText("Username:");
    usernameLabel.setAlignment(Qt::AlignRight);
    mailServerLabel.setText("Mail Server:");
    mailServerLabel.setAlignment(Qt::AlignRight);
    toLabel.setText("To:");
    toLabel.setAlignment(Qt::AlignRight);
    ccLabel.setText("Cc:");
    ccLabel.setAlignment(Qt::AlignRight);
    cc1Label.setText("Cc1:");
    cc1Label.setAlignment(Qt::AlignRight);
    maxTemperatureLabel.setText("Alarm Threshold");
    maxTemperatureLabel.setAlignment(Qt::AlignRight);
    passwordLabel.setText("Password:");
    passwordLabel.setAlignment(Qt::AlignRight);
    passwordEdit.setEchoMode(QLineEdit::Password);

    textLabel.setText("Message to Send:");

    // Setup the QLineEdit styles
    sNormalStyle = usernameEdit.styleSheet();
    sErrorStyle  = "QLineEdit { ";
    sErrorStyle += "color: rgb(255, 255, 255);";
    sErrorStyle += "background: rgb(255, 0, 0);";
    sErrorStyle += "selection-background-color: rgb(128, 128, 255);";
    sErrorStyle += "}";

    getSettings();
    initLayout();
    bCanClose = true;
}


int
ConfigureDialog::exec() {
    getSettings();
    return QDialog::exec();
}


void
ConfigureDialog::getSettings() {
    restoreGeometry(settings.value(QString("Configuration Dialog")).toByteArray());

    usernameEdit.setText(settings.value(usernameLabel.text(), "upsgenerale").toString());
    passwordEdit.setText(settings.value(passwordLabel.text(), "").toString());
    mailServerEdit.setText(settings.value(mailServerLabel.text(), "posta.ipcf.cnr.it").toString());
    toEdit.setText(settings.value(toLabel.text(), "").toString());
    ccEdit.setText(settings.value(ccLabel.text(), "").toString());
    cc1Edit.setText(settings.value(cc1Label.text(), "").toString());
    maxTemperatureEdit.setText(settings.value(maxTemperatureLabel.text(), "28.0").toString());
    QString sMessageText = settings.value(textLabel.text(), "").toString();
    textMessage.clear();
    textMessage.appendPlainText(sMessageText);
}


void
ConfigureDialog::saveSettings() {
    settings.setValue(QString("Configuration Dialog"), saveGeometry());

    settings.setValue(usernameLabel.text(),       usernameEdit.text());
    settings.setValue(passwordLabel.text(),       passwordEdit.text());
    settings.setValue(mailServerLabel.text(),     mailServerEdit.text());
    settings.setValue(toLabel.text(),             toEdit.text());
    settings.setValue(ccLabel.text(),             ccEdit.text());
    settings.setValue(cc1Label.text(),            cc1Edit.text());
    //qDebug() << "max Temp=" << maxTemperatureEdit.text();
    settings.setValue(maxTemperatureLabel.text(), maxTemperatureEdit.text());
    QString sMessageText = textMessage.toPlainText();
    settings.setValue(textLabel.text(), sMessageText);
}


void
ConfigureDialog::initLayout() {
    // Create the Dialog Layout
    QGridLayout* pLayout = new QGridLayout();

    pLayout->addWidget(&usernameLabel,   0, 0, 1, 1);
    pLayout->addWidget(&usernameEdit,    0, 1, 1, 1);
    pLayout->addWidget(&passwordLabel,   0, 2, 1, 1);
    pLayout->addWidget(&passwordEdit,    0, 3, 1, 1);

    pLayout->addWidget(&mailServerLabel, 1, 0, 1, 1);
    pLayout->addWidget(&mailServerEdit,  1, 1, 1, 1);

    pLayout->addWidget(&toLabel,         2, 0, 1, 1);
    pLayout->addWidget(&toEdit,          2, 1, 1, 1);
    pLayout->addWidget(&ccLabel,         2, 2, 1, 1);
    pLayout->addWidget(&ccEdit,          2, 3, 1, 1);

    pLayout->addWidget(&cc1Label,            3, 0, 1, 1);
    pLayout->addWidget(&cc1Edit,             3, 1, 1, 1);
    pLayout->addWidget(&maxTemperatureLabel, 3, 2, 1, 1);
    pLayout->addWidget(&maxTemperatureEdit,  3, 3, 1, 1);

    pLayout->addWidget(&textLabel,       4, 0, 1, 1);
    pLayout->addWidget(&textMessage,     5, 0, 2, 4);

    pButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok |
                                      QDialogButtonBox::Cancel);
    pLayout->addWidget(pButtonBox, 7, 0, 1, 4);

    // Set the Layout
    setLayout(pLayout);
    sNormalStyle = mailServerLabel.styleSheet();

    sErrorStyle  = "QLineEdit { ";
    sErrorStyle += "color: rgb(255, 255, 255);";
    sErrorStyle += "background: rgb(255, 0, 0);";
    sErrorStyle += "selection-background-color: rgb(128, 128, 255);";
    sErrorStyle += "}";

    setToolTips();
    connectSignals();
}


void
ConfigureDialog::connectSignals() {
    // Button Box
    connect(pButtonBox, SIGNAL(accepted()),
            this, SLOT(onOk()));
    connect(pButtonBox, SIGNAL(rejected()),
            this, SLOT(onCancel()));

    // QLineEdit
    connect(&maxTemperatureEdit, SIGNAL(textChanged(const QString)),
            this, SLOT(onMaxTemperatureEdit_textChanged(const QString)));
}


void
ConfigureDialog::onCancel() {
    getSettings();
    reject();
}


void
ConfigureDialog::onOk() {
    if(!bCanClose)
        return;
    saveSettings();
    QMessageBox::warning(this,
                         QString("Configuration Changed"),
                         QString("Warning:\n") +
                         QString("The service upsAlarm MUST be Restarted with:\n") +
                         QString("sudo systemctl restart upsAlarm.service\n") +
                         QString("for the new settings to take effect !"),
                         QMessageBox::Ok);
    accept();
}


void
ConfigureDialog::onMaxTemperatureEdit_textChanged(const QString &arg1) {
    double dTemp = arg1.toDouble();
    bCanClose = (dTemp > 0) && (dTemp < 30);
    if(bCanClose) {
        maxTemperatureEdit.setStyleSheet(sNormalStyle);
    }
    else {
        maxTemperatureEdit.setStyleSheet(sErrorStyle);
    }
}


double
ConfigureDialog::getMaxTemperature() {
    return maxTemperatureEdit.text().toDouble();
}


void
ConfigureDialog::setToolTips() {
    maxTemperatureEdit.setToolTip("Enter a value >0.0 and <30.0");
}


QString
ConfigureDialog::getUsername() {
    return usernameEdit.text();
}


QString
ConfigureDialog::getMailServer() {
    return mailServerEdit.text();
}


QString
ConfigureDialog::getPassword() {
    return passwordEdit.text();
}


QString
ConfigureDialog::getToDestination() {
   return toEdit.text();
}


QString
ConfigureDialog::getCcDestination() {
    return ccEdit.text();
}


QString
ConfigureDialog::getCc1Destination() {
    return cc1Edit.text();
}


QString
ConfigureDialog::getMessage() {
    return textMessage.toPlainText();
}

