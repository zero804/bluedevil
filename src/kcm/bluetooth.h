/**
 * SPDX-FileCopyrightText: 2020 Nicolas Fella <nicolas.fella@gmx.de>
 * SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <KQuickAddons/ConfigModule>
#include <QObject>

class Bluetooth : public KQuickAddons::ConfigModule
{
    Q_OBJECT

public:
    Bluetooth(QObject *parent, const QVariantList &args);

    Q_INVOKABLE void runWizard();
    Q_INVOKABLE void runSendFile(const QString &ubi);

    Q_INVOKABLE void checkNetworkConnection(const QStringList &uuids, const QString &address);
    Q_INVOKABLE void setupNetworkConnection(const QString &service, const QString &address, const QString &deviceName);

Q_SIGNALS:
    void networkAvailable(const QString &service, bool available);

private:
    void checkNetworkInternal(const QString &service, const QString &address);
};

#endif // BLUETOOTHKCM_H
