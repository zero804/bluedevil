/***************************************************************************
 *   Copyright (C) 2010 Alejandro Fiestas Olivares <alex@eyeos.org>        *
 *   Copyright (C) 2010 Eduardo Robles Elvira <edulix@gmail.com>           *
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

#include "requestconfirmation.h"

#include <QTimer>
#include <QCoreApplication>
#include <QIcon>

#include <KNotification>
#include <KLocalizedString>

RequestConfirmation::RequestConfirmation() : QObject()
{
    KNotification *notification = new KNotification(QStringLiteral("bluedevilRequestConfirmation"),
                                                    KNotification::Persistent, this);

    notification->setText(i18nc(
        "The text is shown in a notification to know if the PIN is correct, %1 is the remote bluetooth device and %2 is the pin",
        "%1 is asking if the PIN is correct: %2", qApp->arguments().at(1), qApp->arguments().at(2))
    );

    QStringList actions;
    actions.append(i18nc("Notification button to know if the pin is correct or not", "PIN correct"));
    actions.append(i18nc("Notification button to say that the PIN is wrong", "PIN incorrect"));

    notification->setActions(actions);

    connect(notification, &KNotification::action1Activated, this, &RequestConfirmation::pinCorrect);
    connect(notification, &KNotification::action2Activated, this, &RequestConfirmation::pinWrong);
    connect(notification, &KNotification::closed, this, &RequestConfirmation::pinWrong);
    connect(notification, &KNotification::ignored, this, &RequestConfirmation::pinWrong);

    // We're using persistent notifications so we have to use our own timeout (10s)
    QTimer::singleShot(10000, notification, &KNotification::close);
    notification->setPixmap(QIcon::fromTheme(QStringLiteral("preferences-system-bluetooth")).pixmap(42));
    notification->sendEvent();
}

void RequestConfirmation::pinCorrect()
{
    qApp->exit(0);
}

void RequestConfirmation::pinWrong()
{
    qApp->exit(1);
}

