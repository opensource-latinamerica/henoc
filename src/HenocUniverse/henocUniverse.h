/*
 * TT102 HENOC 
 * 2008 @ ESCOM-IPN
 *
 */
#ifndef _HENOC_HENOCUNIVERSE_H
#define _HENOC_HENOCUNIVERSE_H

#include <ode/ode.h>
#include "aabb.h"
#include "vector.h"
#include "enums.h"
#include "intersection.h"
#include <cassert>
#include <stack>

// Forward declarations from included headers, now properly scoped.
namespace HenocUniverse {
    struct vec2;
    class aabb;
    struct Shape;
}

// Make types available for the declarations in this file
using HenocUniverse::vec2;
using HenocUniverse::aabb;
using HenocUniverse::Shape;


// Colisiones en 2D
namespace HenocUniverse{
    class ContactList;
    class Object;

    typedef unsigned int Mask;
    typedef dBodyID Body;
    typedef void (*Callback)(ContactList &contacts);

    // Clase abstracta para la deteccion de colisiones.
    class Geometry{
        public:
            Geometry();
            virtual ~Geometry() { if (geom) dGeomDestroy(geom); }
            virtual void UpdateBounds() = 0;
            virtual void SetCenter(const vec2 &center);
            vec2 Center() const { return center; }
            virtual void SetAxis(const vec2 &axis);
            void Rotate(dReal theta);
            void Rotate(const vec2 &xform);
            vec2 Axis() const { return axis; }
            vec2 Axis(int i) const { return i ? axis.perp() : axis; }
            virtual void SetMass(Body, dReal) const {}
            virtual Shape GetShape() const = 0;
            const aabb &GetBounds() const { return bounds; }
            dGeomID GetGeomID() const { return geom; }
        protected:
            dGeomID geom;
            aabb bounds;
            vec2 axis;
            vec2 center;
    };

    // Propiedades Fisicas.
    struct ObjectProperties{
        dReal density;
        dReal friction;
        dReal bounceFactor;
        dReal bounceVelocity;
        Mask collisionMask;
        Mask frictionMask;
        Callback callback;
    };

    //Clase abstracta base para todos los objetos, ya sean dinamicos o estaticos
    class Object{
        public:
            Object();
            virtual ~Object() {}
            virtual Geometry &GetGeometry() = 0;
            virtual const Geometry &GetGeometry() const = 0;
            virtual bool IsDynamic() const = 0;
            virtual Body GetBody() = 0;
            virtual void SetMass(dReal density) = 0;
            void Move();
            void Rotate(dReal theta);
            void SetCenter(const vec2 &center);
        public:
            ObjectProperties &Property() { return properties; }
            const ObjectProperties &Property() const { return properties; }
            static ObjectProperties &Default() { return defaults; }
            static void PushProperties() { defaultStack.push(defaults); }
            static void PopProperties() { defaults = defaultStack.top(); defaultStack.pop(); }
        private:
            ObjectProperties properties;
            static ObjectProperties defaults;
            static std::stack<ObjectProperties> defaultStack;
    };

    //Objetos Dinamicos.
    template<class G>
    class Dynamic : public Object{
        public:
            Dynamic(G *geometry, Body b);
            virtual ~Dynamic() { dBodyDestroy(body); delete geometry; }
            G &GetGeometry() { return *geometry; }
            const G &GetGeometry() const { return *geometry; }
            bool IsDynamic() const { return true; }
            Body GetBody() { return body; }
            void SetMass(dReal density) { geometry->SetMass(body, density); }
        protected:
            G *geometry;
            Body body;
    };

    //Objetos estaticos. Los que estan anclados al sistema.
    template<class G>
    class Static : public Object{
        public:
            Static(G *geometry) : geometry(geometry) {}
            virtual ~Static() { delete geometry; }
            G &GetGeometry() { return *geometry; }
            const G &GetGeometry() const { return *geometry; }
            bool IsDynamic() const { return false; }
            Body GetBody() { return 0; }
            void SetMass(dReal) {}
        protected:
            G *geometry;
    };

    // Un array entre 2 figuras geometricas en contacto.
    class ContactList{
        public:
            ContactList();
            void Reset(Object *o1, Object *o2);
            Object *Self() { return o1; }
            Object *Other() { return o2; }
            void ToggleNormalInversion() { invertNormals = !invertNormals; }
            void AddContact(const vec2 &position, const vec2 &normal, dReal depth);
            void Finalize();
            void CreateJoints(dWorldID world, dJointGroupID contactGroup) const;
            int Count() const { return count; }
            static const int Max = 64;
        private:
            Object *o1;
            Object *o2;
            dContact contacts[Max];
            int count;
            bool invertNormals;
    };


    class World{
        public:
            World();
            ~World();
            void QuickStep(dReal);
            Body BodyCreate();
            template<class S> void GenerateContacts(const S &space);
            template<class S> bool IsCorrupt(const S &space) const;
            int ContactCount() const { return contactCount; }
            void SetCFM(dReal);
            void SetAutoDisableFlag(bool);
            void SetERP(dReal);
            void SetContactMaxCorrectingVel(dReal);
            void SetContactSurfaceLayer(dReal);
            void SetAutoDisableLinearThreshold(dReal);
            void SetAutoDisableAngularThreshold(dReal);
            void SetGravity(const vec2&);
            dJointID AddMotor(Object &object);
            dJointID Glue(Object &object1, Object &object2);
            dJointID AnchorAxis(Object &object, const vec2 &axis);
            dJointID Anchor(Object &o1, Object &o2, const vec2 &point, dReal mu, dReal erp);
            dJointID Anchor(Object &o1, const vec2 &point, dReal mu, dReal erp);
            void DeleteJoint(dJointID joint);
            static void SetMotorVelocity(dJointID joint, dReal velocity);
            static dReal GetMotorVelocity(dJointID joint);
        private:
            int contactCount;
            dWorldID world;
            dJointGroupID contactGroup;
            ContactList contactList;
    };

    // Generador de contactos 2.
    template<class Container>
    void World::GenerateContacts(const Container &space){
        dJointGroupEmpty(contactGroup);
        contactCount = 0;
        typename Container::const_iterator o1;
        for (o1 = space.begin(); o1 != space.end(); ++o1){
            Object *object1 = (*o1)->GetFlatlandObject();
            if (!object1)
                continue;
            typename Container::const_iterator o2 = o1;
            for (++o2; o2 != space.end(); ++o2){
                Object *object2 = (*o2)->GetFlatlandObject();
                if (!object2)
                    continue;
                if (!object1->IsDynamic() && !object2->IsDynamic())
                    continue;
                if (!(object1->Property().collisionMask & object2->Property().collisionMask))
                    if (!object1->Property().callback && !object2->Property().callback)
                        continue;

                Geometry &g1 = object1->GetGeometry();
                Geometry &g2 = object2->GetGeometry();

                if (HenocUniverse::Intersection::Test(g1, g2)){
                    contactList.Reset(object1, object2);
                    HenocUniverse::Intersection::Find(g1, g2, contactList);
                    contactList.Finalize();
                    if ((object1->Property().collisionMask & object2->Property().collisionMask)){
                        contactList.CreateJoints(world, contactGroup);
                        contactCount += contactList.Count();
                    }
                }
            }
        }
    }

    // Deteccion de errores.
    template<class Container>
    bool World::IsCorrupt(const Container &space) const{
        typename Container::const_iterator o;
        for (o = space.begin(); o != space.end(); ++o){
            Object *object = (*o)->GetFlatlandObject();
            if (!object) continue;
            Body body = object->GetBody();
            if (!body) continue;

            const dReal *lvel = dBodyGetLinearVel(body);
            const dReal *avel = dBodyGetAngularVel(body);

            if (isnan(lvel[0])) return true;
            if (isnan(lvel[1])) return true;
            if (isnan(avel[0])) return true;
            if (isnan(avel[1])) return true;
        }
        return false;
    }

    template<class G>
    Dynamic<G>::Dynamic(G *g, Body b) : geometry(g), body(b){
        // Some geometries may not back with an ODE geom; guard ODE calls
        if (geometry->GetGeomID()){
            dGeomSetData(geometry->GetGeomID(), this);
            dGeomSetBody(geometry->GetGeomID(), b);
        }
        dMatrix3 R;
        dReal theta = atan2(geometry->Axis(0).y, geometry->Axis(0).x);
        dRFromAxisAndAngle(R, 0, 0, 1, theta);
        dBodySetPosition(body, geometry->Center().x, geometry->Center().y, 0);
        dBodySetRotation(body, R);
        SetMass(Object::Default().density);
    }
}

#endif
