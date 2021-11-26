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

#include "EmulatorInput.h"
#import "Emulator.h"

EmulatorInput::EmulatorInput(Emulator* pEmulator)
{
    m_pEmulator = pEmulator;
    m_pInputCallbackController = new InputCallback<EmulatorInput> (this, &EmulatorInput::InputController);
    m_pInputCallbackButtons = new InputCallback<EmulatorInput> (this, &EmulatorInput::InputButtons);
}

EmulatorInput::~EmulatorInput()
{
    SafeDelete(m_pInputCallbackController);
    SafeDelete(m_pInputCallbackButtons);
}

void EmulatorInput::Init()
{
    InputManager::Instance().ClearRegionEvents();
    for (int i = 0; i < 4; i++)
        m_bController[i] = false;
    
    CGFloat scale;
    if ([[UIScreen mainScreen] respondsToSelector:@selector(scale)]) {
        scale=[[UIScreen mainScreen] scale];
    } else {
        scale=1; 
    }
    
    BOOL retina;
    retina = (scale != 1);
    
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone)
    {
        if (retina)
        {
            CGRect screenBounds = [[UIScreen mainScreen] bounds];
            
            switch ((int)screenBounds.size.height)
            {
                case 480:
                {
                    // iPhone 4
                    InputManager::Instance().AddCircleRegionEvent(280.0f, 325.0f, 30.0f, m_pInputCallbackButtons, 1, false);
                    InputManager::Instance().AddCircleRegionEvent(233.0f, 345.0f, 30.0f, m_pInputCallbackButtons, 2, false);
                    InputManager::Instance().AddCircleRegionEvent(182.0f, 390.0f, 25.0f, m_pInputCallbackButtons, 3, false);
                    InputManager::Instance().AddCircleRegionEvent(128.0f, 390.0f, 25.0f, m_pInputCallbackButtons, 4, false);
                    InputManager::Instance().AddCircleRegionEvent(57.0f, 342.0f, 50.0f, m_pInputCallbackController, 0, true);
                    break;
                }
                case 568:
                {
                    // iPhone 5
                    InputManager::Instance().AddCircleRegionEvent(257.0f, 354.0f, 35.0f, m_pInputCallbackButtons, 1, false);
                    InputManager::Instance().AddCircleRegionEvent(203.0f, 380.0f, 35.0f, m_pInputCallbackButtons, 2, false);
                    InputManager::Instance().AddCircleRegionEvent(181.0f, 462.0f, 30.0f, m_pInputCallbackButtons, 3, false);
                    InputManager::Instance().AddCircleRegionEvent(129.0f, 462.0f, 30.0f, m_pInputCallbackButtons, 4, false);
                    InputManager::Instance().AddCircleRegionEvent(76.0f, 370.0f, 52.0f, m_pInputCallbackController, 0, true);
                    break;
                }
                case 667:
                {
                    // iPhone 6
                    InputManager::Instance().AddCircleRegionEvent(301.0f, 421.0f, 42.0f, m_pInputCallbackButtons, 1, false);
                    InputManager::Instance().AddCircleRegionEvent(236.0f, 451.0f, 42.0f, m_pInputCallbackButtons, 2, false);
                    InputManager::Instance().AddCircleRegionEvent(214.0f, 550.0f, 35.0f, m_pInputCallbackButtons, 3, false);
                    InputManager::Instance().AddCircleRegionEvent(150.0f, 550.0f, 35.0f, m_pInputCallbackButtons, 4, false);
                    InputManager::Instance().AddCircleRegionEvent(87.0f, 439.0f, 75.0f, m_pInputCallbackController, 0, true);
                    break;
                }
                case 736:
                {
                    // iPhone 6 Plus
                    InputManager::Instance().AddCircleRegionEvent(334.0f, 470.0f, 45.0f, m_pInputCallbackButtons, 1, false);
                    InputManager::Instance().AddCircleRegionEvent(262.0f, 503.0f, 45.0f, m_pInputCallbackButtons, 2, false);
                    InputManager::Instance().AddCircleRegionEvent(235.0f, 613.0f, 40.0f, m_pInputCallbackButtons, 3, false);
                    InputManager::Instance().AddCircleRegionEvent(163.0f, 613.0f, 40.0f, m_pInputCallbackButtons, 4, false);
                    InputManager::Instance().AddCircleRegionEvent(94.0f, 491.0f, 75.0f, m_pInputCallbackController, 0, true);
                    break;
                }
            }
        }
        else
        {
            InputManager::Instance().AddCircleRegionEvent(256.0f, 316.0f, 29.0f, m_pInputCallbackButtons, 1, false);
            InputManager::Instance().AddCircleRegionEvent(213.0f, 337.0f, 29.0f, m_pInputCallbackButtons, 2, false);
            InputManager::Instance().AddCircleRegionEvent(167.0f, 397.0f, 25.0f, m_pInputCallbackButtons, 3, false);
            InputManager::Instance().AddCircleRegionEvent(121.0f, 397.0f, 25.0f, m_pInputCallbackButtons, 4, false);
            InputManager::Instance().AddCircleRegionEvent(80.0f, 331.0f, 50.0f, m_pInputCallbackController, 0, true);
        }
    }
    else
    {
        if (retina)
        {
            InputManager::Instance().AddCircleRegionEvent(674.0f, 660.0f, 78.0f, m_pInputCallbackButtons, 1, false);
            InputManager::Instance().AddCircleRegionEvent(544.0f, 721.0f, 78.0f, m_pInputCallbackButtons, 2, false);
            InputManager::Instance().AddCircleRegionEvent(408.0f, 891.0f, 60.0f, m_pInputCallbackButtons, 3, false);
            InputManager::Instance().AddCircleRegionEvent(276.0f, 891.0f, 60.0f, m_pInputCallbackButtons, 4, false);
            InputManager::Instance().AddCircleRegionEvent(151.0f, 699.0f, 110.0f, m_pInputCallbackController, 0, true);
        }
        else
        {
            InputManager::Instance().AddCircleRegionEvent(614.0f, 623.0f, 64.0f, m_pInputCallbackButtons, 1, false);
            InputManager::Instance().AddCircleRegionEvent(510.0f, 671.0f, 64.0f, m_pInputCallbackButtons, 2, false);
            InputManager::Instance().AddCircleRegionEvent(400.0f, 809.0f, 50.0f, m_pInputCallbackButtons, 3, false);
            InputManager::Instance().AddCircleRegionEvent(293.0f, 809.0f, 50.0f, m_pInputCallbackButtons, 4, false);
            InputManager::Instance().AddCircleRegionEvent(192.0f, 653.0f, 100.0f, m_pInputCallbackController, 0, true);
        }
    }
}

void EmulatorInput::InputController(stInputCallbackParameter parameter, int id)
{
    bool bNewController[4];
    for (int i = 0; i < 4; i++)
        bNewController[i] = false;
    
    if (parameter.type != PRESS_END)
    {
        float length = parameter.vector.length();
        
        float minLength = 25.0f;
        
        if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone)
        {
            minLength = 11.0f;
        }
        
        if (length >= minLength)
        {
            float angle = atan2f(parameter.vector.x, -parameter.vector.y) * 57.29577951f;
            
            bNewController[0] = ((angle >= 35.0f) && (angle <= 145.0f));
            bNewController[1] = ((angle <= -35.0f) && (angle >= -145.0f));
            bNewController[2] = ((angle >= -55.0f) && (angle <= 55.0f));
            bNewController[3] = ((angle >= 125.0f) || (angle <= -125.0f));
        }
    }
    
    for (int i = 0; i < 4; i++)
    {
        if (bNewController[i] != m_bController[i])
        {
            m_bController[i] = bNewController[i];
            
            Gameboy_Keys key = Gameboy_Keys::Select_Key;
            
            switch (i)
            {
                case 0:
                    key = Gameboy_Keys::Right_Key;
                    break;
                case 1:
                    key = Gameboy_Keys::Left_Key;
                    break;
                case 2:
                    key = Gameboy_Keys::Up_Key;
                    break;
                case 3:
                    key = Gameboy_Keys::Down_Key;
                    break;
            }
            
            if (m_bController[i])
            {
                [m_pEmulator keyPressed:key];
            }
            else
            {
                [m_pEmulator keyReleased:key];
            }
        }
    }
}

void EmulatorInput::InputButtons(stInputCallbackParameter parameter, int id)
{
    Gameboy_Keys key;
    
    switch (id) {
        case 1:
            key = Gameboy_Keys::A_Key;
            break;
        case 2:
            key = Gameboy_Keys::B_Key;
            break;
        case 3:
            key = Gameboy_Keys::Start_Key;
            break;
        case 4:
            key = Gameboy_Keys::Select_Key;
            break;
        default:
            return;
    }
    
    if (parameter.type == PRESS_START)
    {
        Log("press %d", id);
        [m_pEmulator keyPressed:key];
    }
    else if (parameter.type == PRESS_END)
    {
        Log("release %d", id);
        [m_pEmulator keyReleased:key];
    }
}
