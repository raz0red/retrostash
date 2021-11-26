/*
 * Gearboy - Nintendo Game Boy Emulator
 * Copyright (C) 2012  Ignacio Sanchez

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/ 
 * 
 */

#ifndef SOUNDSETTINGS_H
#define SOUNDSETTINGS_H

#include <QSettings>
#include "ui_SoundSettings.h"

class GLFrame;
class Emulator;

class SoundSettings : public QDialog
{
    Q_OBJECT

public:
    SoundSettings(GLFrame* pGLFrame, Emulator* pEmulator);
    ~SoundSettings();
    void SaveSettings(QSettings& settings);
    void LoadSettings(QSettings& settings);

public slots:
    void PressedOK();
    void PressedCancel();

private:
    Ui::SoundSettings widget;
    GLFrame* m_pGLFrame;
    Emulator* m_pEmulator;
    int m_iRate;
    bool m_bEnabled;
};

#endif // SOUNDSETTINGS_H
