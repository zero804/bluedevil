/***************************************************************************
 *   Copyright (C) 2010  Alex Fiestas <alex@eyeos.org>                     *
 *   Copyright (C) 2010 UFO Coders <info@ufocoders.com>                    *
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


#ifndef WIZARDAGENT_H
#define WIZARDAGENT_H

#include <QtDBus>
#include <QApplication>
#include <QXmlStreamReader>

class WizardAgent : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.bluez.Agent")

public:
    WizardAgent(QApplication* application);
    ~WizardAgent();

    void setPin(const QString& pin);
    QString getPin(const QString& path) const;

//D-Bus interface implementation
public slots:
    void Release();
    void Authorize(QDBusObjectPath device, const QString& uuid, const QDBusMessage &msg);
    QString RequestPinCode(QDBusObjectPath device, const QDBusMessage &msg);
    quint32 RequestPasskey(QDBusObjectPath device, const QDBusMessage &msg);
    void DisplayPasskey(QDBusObjectPath device, quint32 passkey);
    void RequestConfirmation(QDBusObjectPath device, quint32 passkey, const QDBusMessage &msg);
    void ConfirmModeChange(const QString& mode, const QDBusMessage &msg);
    void Cancel();

private:
    QString m_pin;

Q_SIGNALS:
    void pinRequested(const QString&);
    void agentReleased();
};

#endif