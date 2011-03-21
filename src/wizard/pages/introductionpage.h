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


#ifndef INTRODUCTIONPAGE_H
#define INTRODUCTIONPAGE_H

#include "ui_introduction.h"
#include <QWizardPage>

namespace BlueDevil {
    class Adapter;
}
using namespace BlueDevil;

class IntroductionPage : public QWizardPage
, public Ui::Introduction
{
Q_OBJECT

public:
    IntroductionPage(QWidget* parent = 0);
    virtual ~IntroductionPage();

    virtual bool isComplete() const;
private Q_SLOTS:
    void adapterAdded();
    void noAdapter( Adapter *);
};

#endif // INTRODUCTIONPAGE_H