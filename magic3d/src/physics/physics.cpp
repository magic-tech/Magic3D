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

Magic3D::Physics* Magic3D::Physics::instance = NULL;

Magic3D::Physics::Physics()
{
    bodiesCount = 0;
    constraintsCount = 0;

    collisionConfiguration = NULL;
    dispatcher = NULL;
    overlappingPairCache = NULL;
    solver = NULL;
    dynamicsWorld = NULL;

    play();
}

Magic3D::Physics::~Physics()
{
    stop();
}

bool Magic3D::Physics::start()
{
    bool result = true;

    if (!instance)
    {
        instance = new Physics();
    }

    if (instance)
    {
        Log::log(eLOG_SUCCESS, "Physics sucessfully started.");
    }
    else
    {
        result = false;
        Log::log(eLOG_FAILURE, "Physics not started.");
    }

    return result;
}

bool Magic3D::Physics::finish()
{
    if (instance)
    {
        delete instance;
        instance = NULL;
    }

    if (!instance)
    {
        Log::log(eLOG_SUCCESS, "Physics sucessfully finished.");
    }
    else
    {
        Log::log(eLOG_FAILURE, "Physics not finished.");
    }

    return !instance;
}

Magic3D::Physics* Magic3D::Physics::getInstance()
{
    return instance;
}

void Magic3D::Physics::createWorld()
{
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher             = new btCollisionDispatcher(collisionConfiguration);
    overlappingPairCache   = new btDbvtBroadphase();
    solver                 = new btSequentialImpulseConstraintSolver;
    dynamicsWorld          = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

    dynamicsWorld->setGravity(btVector3(0,-10,0));
    dynamicsWorld->setDebugDrawer(Renderer::getInstance());
}

void Magic3D::Physics::destroyWorld()
{
    if (dynamicsWorld)
    {
        for (int i = dynamicsWorld->getNumCollisionObjects()-1; i >= 0 ; i--)
        {
            btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
            btRigidBody* body = btRigidBody::upcast(obj);

            if (body && body->getMotionState())
            {
                delete body->getMotionState();
            }

            if (body && body->getCollisionShape())
            {
                delete body->getCollisionShape();
            }

            dynamicsWorld->removeRigidBody(body);
            delete obj;
        }
    }

    if (dynamicsWorld)
    {
        delete dynamicsWorld;
        dynamicsWorld = NULL;
    }

    if (solver)
    {
        delete solver;
        solver = NULL;
    }

    if (overlappingPairCache)
    {
        delete overlappingPairCache;
        overlappingPairCache = NULL;
    }

    if (dispatcher)
    {
        delete dispatcher;
        dispatcher = NULL;
    }

    if (collisionConfiguration)
    {
        delete collisionConfiguration;
        collisionConfiguration = NULL;
    }

    bodiesCount = 0;
    constraintsCount = 0;
}

void Magic3D::Physics::render()
{
    dynamicsWorld->debugDrawWorld();
}

void Magic3D::Physics::play(bool create)
{
    if (create && !dynamicsWorld)
    {
        createWorld();
    }
    playing = true;
}

void Magic3D::Physics::stop(bool destroy)
{
    playing = false;
    if (destroy && dynamicsWorld)
    {
        destroyWorld();
    }
}

bool Magic3D::Physics::isPlaying()
{
    return playing;
}

int Magic3D::Physics::getBodiesCount()
{
    return bodiesCount;
}

int Magic3D::Physics::getConstraintsCount()
{
    return constraintsCount;
}

bool Magic3D::Physics::update()
{
    if (isPlaying() && dynamicsWorld)
    {
        dynamicsWorld->stepSimulation(Magic3D::Magic3D::getInstance()->getElapsedTime(), 3);
        //dynamicsWorld->stepSimulation(1.0f / 60.0f, 5);

        int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
        for (int i=0;i<numManifolds;i++)
        {
            btPersistentManifold* contactManifold =  dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
            const btCollisionObject* obA = contactManifold->getBody0();
            const btCollisionObject* obB = contactManifold->getBody1();

            Object* objectA = static_cast<Object*>(obA->getUserPointer());
            Object* objectB = static_cast<Object*>(obB->getUserPointer());
            int numContacts = contactManifold->getNumContacts();

            for (int j=0;j<numContacts;j++)
            {
                btManifoldPoint& pt = contactManifold->getContactPoint(j);

                const btVector3& ptA = pt.getPositionWorldOnA();
                const btVector3& ptB = pt.getPositionWorldOnB();
                const btVector3& normalOnA = -pt.m_normalWorldOnB;
                const btVector3& normalOnB = pt.m_normalWorldOnB;

                if (objectA->getPhysicsCollision())
                {
                    objectA->getPhysicsCollision()->collide(objectB, Vector3(ptA.x(), ptA.y(), ptA.z()), Vector3(ptB.x(), ptB.y(), ptB.z()), Vector3(normalOnB.x(), normalOnB.y(), normalOnB.z()));
                }
                if (objectA->isScripted())
                {
                    Script::getInstance()->call_collision(objectA, objectB, Vector3(ptA.x(), ptA.y(), ptA.z()), Vector3(ptB.x(), ptB.y(), ptB.z()), Vector3(normalOnB.x(), normalOnB.y(), normalOnB.z()));
                }

                if (objectB->getPhysicsCollision())
                {
                    objectB->getPhysicsCollision()->collide(objectA, Vector3(ptB.x(), ptB.y(), ptB.z()), Vector3(ptA.x(), ptA.y(), ptA.z()), Vector3(normalOnA.x(), normalOnA.y(), normalOnA.z()));
                }
                if (objectB->isScripted())
                {
                    Script::getInstance()->call_collision(objectB, objectA, Vector3(ptB.x(), ptB.y(), ptB.z()), Vector3(ptA.x(), ptA.y(), ptA.z()), Vector3(normalOnA.x(), normalOnA.y(), normalOnA.z()));
                }
            }
        }
    }
    return true;
}

void Magic3D::Physics::add(PhysicsObject* physicsObject)
{
    if (dynamicsWorld && physicsObject)
    {
        if (physicsObject->getRigidBody())
        {
            remove(physicsObject);
        }
        bool isDynamic = (physicsObject->getMass() != 0.0f);

        btCollisionShape* shape = NULL;
        bool shapeOwner = true;
        switch (physicsObject->getShape())
        {
            case ePHYSICS_SHAPE_SPHERE:       shape = new btSphereShape(physicsObject->getRadius()); break;
            case ePHYSICS_SHAPE_CAPSULE:      shape = new btCapsuleShape(physicsObject->getRadius(), physicsObject->getShapeSize().getY()); break;
            case ePHYSICS_SHAPE_CYLINDER:     shape = new btCylinderShape(btVector3(physicsObject->getShapeSize().getX() * 0.5f, physicsObject->getShapeSize().getY() * 0.5f, physicsObject->getShapeSize().getZ() * 0.5f)); break;
            case ePHYSICS_SHAPE_CONE:         shape = new btConeShape(physicsObject->getRadius(), physicsObject->getShapeSize().getY()); break;
            case ePHYSICS_SHAPE_CONVEXHULL:
            {
                if (physicsObject->getType() != eOBJECT_BONE)
                {
                    Object* object = static_cast<Object*>(physicsObject);
                    MeshData* mesh = object->getCollisionMesh();

                    if (!mesh && object->getMeshes()->size() > 0)
                    {
                        mesh = object->getMeshes()->at(0)->getData();
                    }

                    btConvexHullShape* s = NULL;

                    if (mesh && !mesh->getShape())
                    {
                        s = new btConvexHullShape();
                        float* buffer = mesh->mapBuffer();
                        int inc = sizeof(Vertex3D) / sizeof(float);
                        int size = mesh->getVerticesCount();
                        for (int i = 0; i < size; i++)
                        {
                            float* v = buffer + i * inc;

                            float &x = *v; v++;
                            float &y = *v; v++;
                            float &z = *v; v++;

                            s->addPoint(btVector3(x, y, z));
                        }
                        mesh->unmapBuffer();
                        mesh->setShape(s);
                    }
                    else if (mesh)
                    {
                        s = (btConvexHullShape*)mesh->getShape();
                    }

                    shape = s;
                    shapeOwner = false;
                }

                break;
            }
            case ePHYSICS_SHAPE_TRIANGLEMESH:
            {
                if (physicsObject->getType() != eOBJECT_BONE)
                {
                    Object* object = static_cast<Object*>(physicsObject);
                    MeshData* mesh = object->getCollisionMesh();

                    if (!mesh && object->getMeshes()->size() > 0)
                    {
                        mesh = object->getMeshes()->at(0)->getData();
                    }

                    btTriangleMesh* s = NULL;

                    if (mesh && !mesh->getTriangleMesh())
                    {
                        s = new btTriangleMesh();
                        float* buffer = mesh->mapBuffer();
                        vindex* triangles = mesh->mapTriangles();
                        int inc = sizeof(Vertex3D) / sizeof(float);
                        int tri = sizeof(TriangleIndexes) / sizeof(vindex);
                        int size = mesh->getTrianglesCount();
                        for (int i = 0; i < size; i++)
                        {
                            vindex* t = triangles + i * tri;

                            float* v1 = buffer + *t * inc; t++;
                            float* v2 = buffer + *t * inc; t++;
                            float* v3 = buffer + *t * inc; t++;

                            float &x1 = *v1; v1++;
                            float &y1 = *v1; v1++;
                            float &z1 = *v1; v1++;

                            float &x2 = *v2; v2++;
                            float &y2 = *v2; v2++;
                            float &z2 = *v2; v2++;

                            float &x3 = *v3; v3++;
                            float &y3 = *v3; v3++;
                            float &z3 = *v3; v3++;


                            s->addTriangle(btVector3(x1, y1, z1), btVector3(x2, y2, z2), btVector3(x3, y3, z3));
                        }
                        mesh->unmapBuffer();
                        mesh->unmapTriangles();
                        mesh->setTriangleMesh(s);
                    }
                    else if (mesh)
                    {
                        s = mesh->getTriangleMesh();
                    }

                    shape = new btBvhTriangleMeshShape(s, true);
                }
                break;
            }
            default: shape = new btBoxShape(btVector3(physicsObject->getShapeSize().getX() * 0.5f, physicsObject->getShapeSize().getY() * 0.5f, physicsObject->getShapeSize().getZ() * 0.5f)); break;
        }

        if (shape)
        {
            physicsObject->setShapeOwner(shapeOwner);
            shape->setMargin(physicsObject->getMargin());

            btVector3 localInertia(0.0f, 0.0f, 0.0f);
            if (isDynamic)
            {
                shape->calculateLocalInertia(physicsObject->getMass(), localInertia);
            }

            Vector3 center = physicsObject->getPosition();
            Quaternion rotation = physicsObject->getRotation();

            if (physicsObject->getRender() == eRENDER_2D)
            {
                center.setY(-center.getY());
                rotation.setZ(-rotation.getZ());
            }
            btTransform startTransform(btQuaternion(rotation.getX(), rotation.getY(), rotation.getZ(), rotation.getW()),
                                       btVector3(center.getX(), center.getY(), center.getZ()));

            btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

            btRigidBody::btRigidBodyConstructionInfo cInfo(physicsObject->getMass(), myMotionState, shape, localInertia);

            btRigidBody* body = new btRigidBody(cInfo);
            btScalar contactProcessingThreshold(BT_LARGE_FLOAT);
            body->setContactProcessingThreshold(contactProcessingThreshold);

            if (physicsObject->isGhost())
            {
                body->setCollisionFlags( body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
            }

            body->setLinearFactor(btVector3(physicsObject->getShapeTranslation().getX(), physicsObject->getShapeTranslation().getY(), physicsObject->getShapeTranslation().getZ()));
            body->setAngularFactor(btVector3(physicsObject->getShapeRotation().getX(), physicsObject->getShapeRotation().getY(), physicsObject->getShapeRotation().getZ()));


            body->setDamping(physicsObject->getDampingLinear(), physicsObject->getDampingAngular());
            body->setFriction(physicsObject->getFriction());
            body->setRestitution(physicsObject->getRestitution());

            dynamicsWorld->addRigidBody(body);

            body->setActivationState( ACTIVE_TAG );
            body->activate(true);

            body->setUserPointer(physicsObject);
            physicsObject->setRigidBody(body);

            bodiesCount++;
        }
    }
}

void Magic3D::Physics::remove(PhysicsObject* physicsObject)
{
    if (physicsObject)
    {
        btRigidBody* body = physicsObject->getRigidBody();

        if (body && body->getMotionState())
        {
            delete body->getMotionState();
        }

        if (physicsObject->isShapeOwner() && body && body->getCollisionShape())
        {
            delete body->getCollisionShape();
        }

        dynamicsWorld->removeRigidBody(body);
        delete body;

        physicsObject->setRigidBody(NULL);
        bodiesCount--;
    }
}

void Magic3D::Physics::reset(PhysicsObject* physicsObject)
{
    if (physicsObject)
    {
        btRigidBody* body = physicsObject->getRigidBody();
        if (body)
        {
            bool isDynamic = (physicsObject->getMass() != 0.0f);

            body->clearForces();

            Vector3 center = physicsObject->getPosition();
            Quaternion rotation = physicsObject->getRotation();

            if (physicsObject->getRender() == eRENDER_2D)
            {
                center.setY(-center.getY());
                rotation.setZ(-rotation.getZ());
            }
            btTransform transform = btTransform(btQuaternion(rotation.getX(), rotation.getY(), rotation.getZ(), rotation.getW()),
                                                btVector3(center.getX(), center.getY(), center.getZ()));
            body->setWorldTransform(transform);
            body->getMotionState()->setWorldTransform(transform);

            btVector3 localInertia(0.0f, 0.0f, 0.0f);
            if (isDynamic)
            {
                body->getCollisionShape()->calculateLocalInertia(physicsObject->getMass(), localInertia);
                body->setMassProps(physicsObject->getMass(), localInertia);
            }

            body->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
            body->setAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));

            body->setActivationState( ACTIVE_TAG );
            body->activate(true);
        }
    }
}

void Magic3D::Physics::addConstraint(PhysicsConstraint* constraint)
{
    if (constraint && constraint->getConstraint())
    {
        dynamicsWorld->addConstraint(constraint->getConstraint());
        constraintsCount++;
    }
}

void Magic3D::Physics::removeConstraint(PhysicsConstraint* constraint)
{
    if (constraint && constraint->getConstraint())
    {
        dynamicsWorld->removeConstraint(constraint->getConstraint());
        constraintsCount--;
    }
}

float* Magic3D::Physics::mapShape(PhysicsObject* physicsObject)
{
    float* result = NULL;
    if (physicsObject && physicsObject->getRigidBody() && physicsObject->getRigidBody()->getCollisionShape() && physicsObject->getShape() == ePHYSICS_SHAPE_CONVEXHULL)
    {
        btConvexHullShape* shape = (btConvexHullShape*)physicsObject->getRigidBody()->getCollisionShape();
        result = (float*)shape->getUnscaledPoints();
    }

    return result;
}

void Magic3D::Physics::unmapShape(PhysicsObject* physicsObject)
{
    if (physicsObject && physicsObject->getRigidBody() && physicsObject->getRigidBody()->getCollisionShape() && physicsObject->getShape() == ePHYSICS_SHAPE_CONVEXHULL)
    {
        btConvexHullShape* shape = (btConvexHullShape*)physicsObject->getRigidBody()->getCollisionShape();
        shape->recalcLocalAabb();
    }
}

void Magic3D::Physics::setGravity(const Vector3& gravity)
{
    if (dynamicsWorld)
    {
        dynamicsWorld->setGravity(btVector3(gravity.getX(), gravity.getY(), gravity.getZ()));
    }
}

Magic3D::Vector3 Magic3D::Physics::getGravity()
{
    Vector3 result = Vector3(0.0f, 0.0f, 0.0f);
    if (dynamicsWorld)
    {
        result = Vector3(dynamicsWorld->getGravity().getX(), dynamicsWorld->getGravity().getY(), dynamicsWorld->getGravity().getZ());
    }
    return result;
}

Magic3D::RayCastReturn Magic3D::Physics::rayCast(Vector3 start, Vector3 end)
{
    RayCastReturn result;
    result.point = Vector3(0.0f, 0.0f, 0.0f);
    result.normal = Vector3(0.0f, 0.0f, 0.0f);
    result.physicsObject = NULL;

    btVector3 btFrom(start.getX(), start.getY(), start.getZ());
    btVector3 btTo(end.getX(), end.getY(), end.getZ());

    btCollisionWorld::ClosestRayResultCallback RayCallback(btFrom, btTo);
    dynamicsWorld->rayTest(btFrom, btTo, RayCallback);

    if(RayCallback.hasHit())
    {
        result.point = Vector3(RayCallback.m_hitPointWorld.x(), RayCallback.m_hitPointWorld.y(), RayCallback.m_hitPointWorld.z());
        result.normal = Vector3(RayCallback.m_hitNormalWorld.x(), RayCallback.m_hitNormalWorld.y(), RayCallback.m_hitNormalWorld.z());
        result.physicsObject = static_cast<Object*>(RayCallback.m_collisionObject->getUserPointer());
    }

    return result;
}
