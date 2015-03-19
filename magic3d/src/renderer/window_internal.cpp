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

#include <magic3d/magic3d.h>
#include <magic3d/renderer/window_internal.h>

Magic3D::WindowInternal* Magic3D::WindowInternal::instance = NULL;

Magic3D::WindowInternal::WindowInternal()
{
    width  = Magic3D::getInstance()->getConfiguration().WINDOW_WIDTH;
    height = Magic3D::getInstance()->getConfiguration().WINDOW_HEIGHT;
    cursorX = 0;
    cursorY = 0;
    orientation = eWINDOW_ORIENTATION_NONE;

    bits   = 32;

    fullscreen = Magic3D::getInstance()->getConfiguration().FULLSCREEN;
    active     = false;
    needUpdateView = false;
}

Magic3D::WindowInternal::~WindowInternal()
{
    instance = NULL;
}

bool Magic3D::WindowInternal::start()
{
    bool result = create();

    if (result)
    {
        Log::log(eLOG_SUCCESS, "OpenGL window sucessfully started.");
    }
    else
    {
        Log::log(eLOG_FAILURE, "OpenGL window not started.");
    }

    return result;
}

bool Magic3D::WindowInternal::finish()
{
    bool result = true;

    Log::log(eLOG_SUCCESS, "OpenGL window sucessfully finished.");

    return result;
}

bool Magic3D::WindowInternal::render()
{
    return true;
}

Magic3D::WindowInternal* Magic3D::WindowInternal::getInstance()
{
    if (!instance)
    {
        instance = new WindowInternal();
    }

    return instance;
}

bool Magic3D::WindowInternal::create()
{
    Renderer::getInstance()->initialize();
    Renderer::getInstance()->resizeWindow(width, height);

    return true;
}
