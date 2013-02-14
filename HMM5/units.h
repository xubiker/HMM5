#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include "coords.h"
#include "weapon.h"
#include "defines.h"

using namespace std;

namespace visitors {
    class CommandVisitor;
};
namespace units {
    class Identificator {
        // class that can be used for identification purposes
            int _uid;
            string _department;
            unsigned arm_idx;
        public:
            Identificator (int, string, unsigned);
            int get_id (void);
            string get_department (void);
            unsigned get_arm_idx (void);
            void set_arm_idx (unsigned);
    };
    class Unit {
        // absract class of warrior
        private:
            int skill;
            int damage;
            int health;
            int freeze;
            int speed;
        protected:
            string name;
            vector<shared_ptr<weapon::Weapon>> weapon;
            void SetHealth (int);
            void SetSpeed (int);
        public:
            Unit() : skill(0), damage(0), health(0), freeze(0), speed(0), name(""), weapon() {};
#ifdef ejudge_compile
            Unit (const Unit&) = delete;
            Unit& operator= (const Unit&) = delete;
#endif
            int GetDamage (void);
            int GetSkill (void);
            int GetSpeed (void);
            int GetHealth (void);
            int GetFreezeTime (void);
            void DecFreezeTime (void);
            bool IsFrozen (void);
            void SetFreeze (int);
            void DecHealth (int);
            int WeaponGetDamage (int);
            int WeaponGetResource (int);
            int WeaponDecResource (int, int c = 1);
            std::string GetName (void);
            void IncSkill (int x = 1);
            void SetDamage (int);
            void fill_with (shared_ptr<Unit>);
            virtual void accept(visitors::CommandVisitor&) = 0;
            virtual ~Unit() {};
    };
    class Location {
        private:
            coords::coord crd;
        public:
            Location (unsigned r = 0, unsigned c = 0) : crd(r,c) {};
            void set_location (coords::coord);
            coords::coord get_location (void);
    };
    class ProxyUnit {
        // abstract proxy class of warrior
        private:
            shared_ptr<Unit> base_unit;
            shared_ptr<Identificator> id;
            shared_ptr<Location> location;
        public:
            ProxyUnit(shared_ptr<Unit>, string, unsigned);
            bool IsDead (void);
            int GetId (void);
            coords::coord GetLocation (void);
            int GetDamage (void);
            int GetSkill (void);
            int GetSpeed (void);
            int GetHealth (void);
            unsigned GetArmyIdx (void);
            bool IsFrozen (void);
            void DecFreezeTime (void);
            void SetFreeze (int);
            void SetLocation (coords::coord);
            void SetArmyIdx (unsigned);
            void DecHealth (int);
            string GetDepartment (void);
            std::string GetName (void);
            void IncSkill (int x = 1);
            int WeaponGetDamage (int);
            int WeaponGetResource (int);
            int WeaponDecResource (int, int c = 1);
            void SetDamage (int);
            void fill_with (shared_ptr<ProxyUnit>);
            void accept(visitors::CommandVisitor&);
    };

    class _Shooter : public Unit {
        // Unit's class specification for shooters
        public:
            virtual void accept(visitors::CommandVisitor&) = 0;
            virtual ~_Shooter();
    };
    class _Rider : public Unit {
        // Unit's class specification for riders
        public:
            virtual void accept(visitors::CommandVisitor&) = 0;
            virtual ~_Rider();
    };
    class _Wizard : public Unit {
        // Unit's class specification for wizards
        public:
            virtual void accept(visitors::CommandVisitor&) = 0;
            virtual ~_Wizard();
    };
    class _Dragon : public Unit {
        // Unit's class specification for dragons
        public:
            virtual void accept(visitors::CommandVisitor&) = 0;
            virtual ~_Dragon();
    };

    class Comparator {
        public:
            bool operator () (shared_ptr<ProxyUnit> u1, shared_ptr<ProxyUnit> u2);
    };

    class Archery : public _Shooter {
        public:
            Archery ();
            void accept (visitors::CommandVisitor&);
    };
    class MasterGremlin : public _Shooter {
        public:
            MasterGremlin ();
            void accept (visitors::CommandVisitor&);
    };
    class Knight : public _Rider {
        public:
            Knight ();
            void accept (visitors::CommandVisitor&);
    };
    class Golem : public _Rider {
        public:
            Golem ();
            void accept (visitors::CommandVisitor&);
    };
    class Monk : public _Wizard {
        public:
            Monk ();
            void accept (visitors::CommandVisitor&);
    };
    class Wizard : public _Wizard {
        public:
            Wizard ();
            void accept (visitors::CommandVisitor&);
    };
    class Beholder : public _Shooter {
        public:
            Beholder ();
            void accept (visitors::CommandVisitor&);
    };
    class Minotavr : public _Rider {
        public:
            Minotavr ();
            void accept (visitors::CommandVisitor&);
    };
    class Medusa : public _Wizard {
        public:
            Medusa ();
            void accept (visitors::CommandVisitor&);
    };
    class Titan  : public _Dragon {
        public:
            Titan ();
            void accept (visitors::CommandVisitor&);
    };
    class Angel : public _Dragon {
        public:
            Angel ();
            void accept (visitors::CommandVisitor&);
    };
    class BlackDragon : public _Dragon {
        public:
            BlackDragon ();
            void accept (visitors::CommandVisitor&);
    };
};
namespace visitors {
    class CommandVisitor {
        // abstract CommandVisitor class
    public:
        virtual void visit(units::Archery*) = 0;
        virtual void visit(units::MasterGremlin*) = 0;
        virtual void visit(units::Knight*) = 0;
        virtual void visit(units::Golem*) = 0;
        virtual void visit(units::Monk*) = 0;
        virtual void visit(units::Wizard*) = 0;
        virtual void visit(units::Beholder*) = 0;
        virtual void visit(units::Minotavr*) = 0;
        virtual void visit(units::Medusa*) = 0;
        virtual void visit(units::Titan*) = 0;
        virtual void visit(units::Angel*) = 0;
        virtual void visit(units::BlackDragon*) = 0;
        virtual ~CommandVisitor() {};
    };
    class AttackVisitor : public CommandVisitor {
        void visit(units::Archery*);
        void visit(units::MasterGremlin*);
        void visit(units::Knight*);
        void visit(units::Golem*);
        void visit(units::Monk*);
        void visit(units::Wizard*);
        void visit(units::Beholder*);
        void visit(units::Minotavr*);
        void visit(units::Medusa*);
        void visit(units::Titan*);
        void visit(units::Angel*);
        void visit(units::BlackDragon*);
    };
};
