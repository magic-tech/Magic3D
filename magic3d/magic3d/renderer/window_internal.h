/******************************************************************************
 @Copyright    Copyright (C) 2008 - 2015 by MagicTech.

 @Platform     ANSI compatible
******************************************************************************/
/*
Magic3D Engine
Copyright (c) 2008-2015
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

#ifndef MAGIC3D_RENDERER_WINDOW_INTERNAL_H
#define MAGIC3D_RENDERER_WINDOW_INTERNAL_H

#include <magic3d/renderer/renderer.h>

namespace Magic3D
{

class WindowInternal : public Window
{
private:
    static WindowInternal* instance;

    bool create();

    WindowInternal();
    ~WindowInternal();
public:    
    virtual bool start();
    virtual bool finish();
    virtual bool render();

    virtual void* getWindowHandle();
    virtual void showCursor(bool show);        
    virtual void setClipboardText(const char* text);
    virtual const char* getClipboardText();

    virtual void grabInput(bool grabbed);
    virtual bool hasGrabbedInput();
    virtual bool hasInputFocus();
    virtual bool hasMouseFocus();

    virtual void mapGUIKeys(void* gui);
    virtual int getMouseState(int* x, int* y, bool* left, bool* right, bool* middle);

    static WindowInternal* getInstance();
};

}

#endif // MAGIC3D_RENDERER_WINDOW_INTERNAL_H
