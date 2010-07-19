/*
 *    Copyright (C) 2010 Alejandro Fiestas Olivares  <alex@ufocoders.com>
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "monolitic.h"
#include <kmenu.h>
#include <kaction.h>
#include <kprocess.h>
#include <klocalizedstring.h>

#include <QDebug>
#include <bluedevil/bluedevil.h>

using namespace BlueDevil;
Monolitic::Monolitic(QObject* parent): KStatusNotifierItem(parent)
{
    setCategory(KStatusNotifierItem::Hardware);
    setIconByName("preferences-system-bluetooth");

    onlineMode();
    if (!Manager::self()->defaultAdapter()) {
        offlineMode();
    }

    connect(Manager::self(), SIGNAL(adapterAdded(Adapter*)), this,
        SLOT(adapterAdded()));
    connect(Manager::self(), SIGNAL(defaultAdapterChanged(Adapter*)), this,
        SLOT(noAdapters(Adapter*)));
}

void Monolitic::noAdapters(Adapter* adapter)
{
    if (!adapter) {
        offlineMode();
    }
}

void Monolitic::adapterAdded()
{
    if (status() != KStatusNotifierItem::Active) {
        onlineMode();
    }
}

void Monolitic::onlineMode()
{
    setStatus(KStatusNotifierItem::Active);

    KMenu *menu = contextMenu();

    KAction *addDevice = new KAction(KIcon("edit-find-project"), i18n("Add Device"), menu);
    connect(addDevice, SIGNAL(triggered(Qt::MouseButtons,Qt::KeyboardModifiers)), this, SLOT(addDevice()));
    menu->addAction(addDevice);

    KAction *configReceive = new KAction(KIcon("folder-tar"),i18n("Receive files"), menu);
    connect(configReceive, SIGNAL(triggered(Qt::MouseButtons,Qt::KeyboardModifiers)), this, SLOT(configReceive()));
    menu->addAction(configReceive);

    KAction *deviceManager = new KAction(KIcon("input-mouse"), i18n("Manage Devices"), menu);
    connect(deviceManager, SIGNAL(triggered(Qt::MouseButtons,Qt::KeyboardModifiers)), this, SLOT(deviceManager()));
    menu->addAction(deviceManager);

    KAction *configAdapter = new KAction(KIcon("audio-card"), i18n("Configure Adapters"), menu);
    connect(configAdapter, SIGNAL(triggered(bool)), this, SLOT(configAdapter()));
    menu->addAction(configAdapter);

    setContextMenu(menu);
    setStandardActionsEnabled(true);
}

void Monolitic::addDevice()
{
    KProcess process;
    process.setProgram("bluedevil-wizard");
    process.startDetached();
}

void Monolitic::configReceive()
{
    KProcess process;
    process.startDetached("kcmshell4", QStringList("bluedeviltransfer"));
}

void Monolitic::deviceManager()
{
    KProcess process;
    process.startDetached("kcmshell4", QStringList("bluedevildevices"));
}

void Monolitic::configAdapter()
{
    KProcess process;
    process.startDetached("kcmshell4", QStringList("bluedeviladapters"));
}


void Monolitic::offlineMode()
{
    setStatus(KStatusNotifierItem::Passive);
    contextMenu()->clear();
}