/******************************************************************************
 @Copyright    Copyright (C) 2008 - 2016 by MagicTech.

 @Platform     ANSI compatible
******************************************************************************/
/*
Magic3D Engine
Copyright (c) 2008-2016
Thiago C. Moraes
http://www.magictech.com.br

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software.
   If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef MAGIC3D_INPUT_H
#define MAGIC3D_INPUT_H

#include <magic3d/magic3d.h>

namespace Magic3D
{

class EventListener;
class EventListener_Keyboard;
class EventListener_Mouse;
class EventListener_Joystick;
class EventListener_Touch;

class Input
{
private:
    std::vector<EventListener*> events_keyboard;
    std::vector<EventListener*> events_mouse;
    std::vector<EventListener*> events_joystick;
    std::vector<EventListener*> events_touch;
    std::vector<EventListener*> events_accelerometer;
    std::vector<EventListener*> events_gyroscope;

    static Input* instance;

    Input();
    ~Input();

public:
    static bool start();
    static bool finish();

    static Input* getInstance();

    void addEventListener(EventListener* listener);
    void removeEventListener(EventListener* listener);

    void dispatchEvent(INPUT input, EVENT event, int value);
    void dispatchEvent(INPUT input, EVENT event, float value);
    void dispatchEvent(INPUT input, EVENT event, int x, int y, int button);
    void dispatchEvent(INPUT input, EVENT event, float x, float y, float z);
    void dispatchEvent(INPUT input, EVENT event, float x, float y, float z, float w);
};

}

#endif // MAGIC3D_INPUT_H
