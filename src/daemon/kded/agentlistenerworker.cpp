/***************************************************************************
 *   Copyright (C) 2010 Alejandro Fiestas Olivares <alex@eyeos.org>        *
 *   Copyright (C) 2010 Eduardo Robles Elvira <edulix@gmail.com>           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA            *
 ***************************************************************************/

#include "agentlistenerworker.h"
#include <QDBusConnection>
#include <QDebug>
#include <KProcess>
#include <solid/control/bluetoothmanager.h>

AgentListenerWorker::AgentListenerWorker(QObject *parent) :  QDBusAbstractAdaptor(parent)
{
    const QString agentPath = "/blueDevil_agent";

    if(!QDBusConnection::systemBus().registerObject(agentPath, parent)){
        qDebug() << "The dbus object can't be registered";
        return;
    }

    Solid::Control::BluetoothManager &man = Solid::Control::BluetoothManager::self();
    m_adapter = new Solid::Control::BluetoothInterface(man.defaultInterface());
    m_adapter->registerAgent(agentPath, "DisplayYesNo");
    qDebug() << "Agent registered";
}

void AgentListenerWorker::unregister()
{
    const QString agentPath = "/blueDevil_agent";

    qDebug() << "Unregistering object";
    QDBusConnection::systemBus().unregisterObject(agentPath);
}

void AgentListenerWorker::Release()
{
    qDebug() << "Agent Release";
    emit agentReleased();
}

void AgentListenerWorker::Authorize(QDBusObjectPath device, const QString& uuid, const QDBusMessage &msg)
{
    Q_UNUSED(uuid)
    qDebug() << "Authorize called";

    Solid::Control::BluetoothRemoteDevice *remote = m_adapter->findBluetoothRemoteDeviceUBI(device.path());

    QStringList list;
    list.append(remote->name());
    list.append(device.path());

    int result = KProcess::execute("bluedevil-authorize",list);
    if (result == 0) {
        qDebug() << "Go on camarada!";
        return;
    }
    sendBluezError(QString("Authorize"),msg);
}

QString AgentListenerWorker::RequestPinCode(QDBusObjectPath device, const QDBusMessage &msg)
{
    qDebug() << "AGENT-RequestPinCode " << device.path();
    Solid::Control::BluetoothRemoteDevice *remote = m_adapter->findBluetoothRemoteDeviceUBI(device.path());

    QStringList list(remote->name());

    KProcess process;
    process.setOutputChannelMode(KProcess::OnlyStdoutChannel);
    process.setProgram("bluedevil-requestpin",list);
    process.start();

    if (process.waitForFinished(-1)) {
        return QString(process.readAllStandardOutput());
    }

    qDebug() << "Timeout men!";
    QDBusMessage error = msg.createErrorReply("org.bluez.Error.Canceled", "Pincode request failed");
    QDBusConnection::systemBus().send(error);
    return QString();
}

quint32 AgentListenerWorker::RequestPasskey(QDBusObjectPath device, const QDBusMessage &msg)
{
    qDebug() << "AGENT-RequestPasskey " << device.path();
    QString ret = RequestPinCode(device, msg);
    return ret.toInt();
}

void AgentListenerWorker::DisplayPasskey(QDBusObjectPath device, quint32 passkey)
{
    qDebug() << "AGENT-DisplayPasskey " << device.path() << ", " << QString::number(passkey);
}

void AgentListenerWorker::RequestConfirmation(QDBusObjectPath device, quint32 passkey, const QDBusMessage &msg)
{
    qDebug() << "AGENT-RequestConfirmation " << device.path() << ", " << QString::number(passkey);

    Solid::Control::BluetoothRemoteDevice *remote = m_adapter->findBluetoothRemoteDeviceUBI(device.path());

    QStringList list;
    list.append(remote->name());
    list.append(device.path());

    int result = KProcess::execute("bluedevil-requestconfirmation",list);
    if (result == 0) {
        qDebug() << "Go on camarada!";
        return;
    }
    sendBluezError(QString("RequestConfirmation"),msg);
}

void AgentListenerWorker::ConfirmModeChange(const QString& mode, const QDBusMessage &msg)
{
    qDebug() << "AGENT-ConfirmModechange " << mode;
    QStringList list;
    list.append(mode);

    int result = KProcess::execute("bluedevil-confirmchangemode",list);
    if (result == 0) {
        qDebug() << "Go on camarada!";
        return;
    }
    sendBluezError(QString("ConfirmModechange"),msg);
}

void AgentListenerWorker::Cancel()
{
    qDebug() << "AGENT-Cancel";
}


void AgentListenerWorker::sendBluezError(const QString &helper, const QDBusMessage &msg)
{
    qDebug() << "Sending canceled msg to bluetooth" << helper;
    QDBusMessage error = msg.createErrorReply("org.bluez.Error.Canceled", "Authorization canceled");
    QDBusConnection::systemBus().send(error);
}