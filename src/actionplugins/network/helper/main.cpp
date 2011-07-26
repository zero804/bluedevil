/***************************************************************************
 *   Copyright (C) 2010 Alejandro Fiestas Olivares <alex@ufocoders.com>    *
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

#include "networkhelper.h"
#include <KCmdLineArgs>
#include <KApplication>
#include <KAboutData>

int main(int argc, char *argv[])
{
    KAboutData aboutData("bluedevilnetworkhelper", "bluedevil", ki18n("Bluetooth Network Helper"), "0.1", ki18n("Bluetooth Network Helper"),
    KAboutData::License_GPL, ki18n("(c) 2010, UFO Coders"));

    aboutData.addAuthor(ki18n("Alejandro Fiestas Olivares"), ki18n("Developer"), "alex@ufocoders.org",
    "http://www.afiestas.org/");
    aboutData.setProgramIconName("preferences-system-bluetooth");

    KCmdLineArgs::init(argc, argv, &aboutData);
    KCmdLineOptions options;
    options.add("+[ADDRESS]", ki18n("Device to connect"));
    KCmdLineArgs::addCmdLineOptions( options );

    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
    KUrl url;
    if (args->count()) {
        url = args->arg(0);
    }

    KApplication app;
    app.setQuitOnLastWindowClosed(false);

    NetworkHelper *networkHelper = new NetworkHelper(url);

    return app.exec();
}