#BULLET ************************************************************************
HEADERS += \
    $$BULLET_INCLUDE/*.h \
    $$BULLET_INCLUDE/BulletCollision/BroadphaseCollision/*.h \
    $$BULLET_INCLUDE/BulletCollision/CollisionDispatch/*.h \
    $$BULLET_INCLUDE/BulletCollision/CollisionShapes/*.h \
    $$BULLET_INCLUDE/BulletCollision/Gimpact/*.h \
    $$BULLET_INCLUDE/BulletCollision/NarrowPhaseCollision/*.h \
    $$BULLET_INCLUDE/BulletDynamics/Character/*.h \
    $$BULLET_INCLUDE/BulletDynamics/ConstraintSolver/*.h \
    $$BULLET_INCLUDE/BulletDynamics/Dynamics/*.h \
    $$BULLET_INCLUDE/BulletDynamics/Vehicle/*.h \
    $$BULLET_INCLUDE/BulletSoftBody/*.h \
    $$BULLET_INCLUDE/LinearMath/*.h
#********************************************************************************

#VORBIS *************************************************************************
HEADERS += \
    $$VORBIS_SOURCE/*.h \
    $$VORBIS_SOURCE/books/coupled/*.h \
    $$VORBIS_SOURCE/books/floor/*.h \
    $$VORBIS_SOURCE/books/uncoupled/*.h \
    $$VORBIS_SOURCE/modes/*.h \
    $$VORBIS_INCLUDE/*.h
#********************************************************************************

#OGG ****************************************************************************
HEADERS += \
    $$OGG_INCLUDE/*.h
#********************************************************************************

#THEORA *************************************************************************
HEADERS += \
    $$THEORA_SOURCE/*.h \
    $$THEORA_INCLUDE/*.h
#********************************************************************************

#TINYXML ************************************************************************
HEADERS += \
    $$TINYXML_INCLUDE/*.h
#********************************************************************************

#ZLIB ***************************************************************************
HEADERS += \
    $$ZLIB_INCLUDE/*.h
#********************************************************************************

#MINIZIP ***************************************************************************
HEADERS += \
    $$MINIZIP_INCLUDE/crypt.h \
    $$MINIZIP_INCLUDE/ioapi.h \
    $$MINIZIP_INCLUDE/ioapi_buf.h \
    $$MINIZIP_INCLUDE/ioapi_mem.h \
    $$MINIZIP_INCLUDE/unzip.h \
    $$MINIZIP_INCLUDE/zip.h
win32:HEADERS += $$MINIZIP_INCLUDE/iowin32.h
#********************************************************************************

#VECTORMATH *********************************************************************
HEADERS += \    
    $$VECTORMATH_INCLUDE/*.h
#********************************************************************************

#PNG ****************************************************************************
win32|macx: {
HEADERS += \
    $$PNG_INCLUDE/*.h
}
#********************************************************************************

#LUA ****************************************************************************
HEADERS += \
    $$LUA_INCLUDE/*.h
#********************************************************************************

#ENET ***************************************************************************
HEADERS += \
    $$ENET_INCLUDE/*.h
#********************************************************************************

#SDL ***************************************************************************
win32: {
HEADERS += \
    $$SDL_INCLUDE/*.h
}
#********************************************************************************

#IMGUI **************************************************************************
HEADERS += \
    $$IMGUI_INCLUDE/*.h
#********************************************************************************
