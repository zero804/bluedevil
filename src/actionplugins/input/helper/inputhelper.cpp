/*
    Copyright (C) 2010 UFO Coders <info@ufocoders.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "inputhelper.h"
#include "../../actionplugin.h"

#include <QApplication>
#include <QDebug>

#include <klocalizedstring.h>
#include <kservicetypetrader.h>

#include <bluedevil/bluedevil.h>

using namespace BlueDevil;

InputHelper::InputHelper(const KUrl& address) {
    if (!BlueDevil::Manager::self()->defaultAdapter()) {
        qDebug() << "No Adapters found";
        qApp->exit();
    }

    Device *device = Manager::self()->defaultAdapter()->deviceForAddress(address.host().replace("-", ":"));

    if(device->isPaired()) {
        QString constraing("'00001124-0000-1000-8000-00805F9B34FB' in [X-BlueDevil-UUIDS]");
;
        KPluginFactory *factory = KPluginLoader(
            KServiceTypeTrader::self()->query("BlueDevil/ActionPlugin", constraing).first().data()->library()
        ).factory();

        ActionPlugin *plugin = factory->create<ActionPlugin>(this);
        connect(plugin, SIGNAL(finished()), qApp, SLOT(quit()));

        plugin->setDevice(device);
        plugin->startAction();
    }
}