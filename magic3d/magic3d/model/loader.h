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

#ifndef MAGIC3D_LOADER_H
#define MAGIC3D_LOADER_H

#include <magic3d/xml.h>

namespace Magic3D
{

class Model;
class ModelInfo;

class Loader : public Xml
{
private:
    std::string name;
    ModelInfo* modelInfo;

public:
    Loader();
    virtual ~Loader();

    virtual bool loadModel(Model* model) = 0;
    virtual bool loadModel() = 0;
    virtual bool isLoading() = 0;

    const std::string& getName();
    void addModelInfo(std::string fileName);
    ModelInfo* getModelInfo(std::string fileName);
    ModelInfo* getModelInfo();
};

}

#endif // MAGIC3D_LOADER_H
