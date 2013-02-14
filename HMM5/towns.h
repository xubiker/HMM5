#pragma once

#include <memory>
#include "generators.h"
#include "fabrics.h"
#include "armies.h"
#include "heroes.h"
#include "defines.h"


namespace handlers {
    class _Timer;
};

namespace towns {
    class Town {
        // Abstract town class
            handlers::_Timer *timer;                                 // a pointer to game handler
            int TownId;
        protected:
            shared_ptr<fabrics::Fabric> fabric;
            shared_ptr<armies::Army> army;                           // a pointer to town's army
            shared_ptr<heroes::Hero> hero;
        public:
            Town() : timer(0), TownId(0), fabric(0), army(0), hero(0) {}
#ifdef ejudge_compile
            Town(const Town&) = delete;
            Town& operator= (const Town&) = delete;
#endif
            void new_unit (string, unsigned);                        // creates a new unit and puts it to the army
            shared_ptr<units::ProxyUnit> create_unit (string);       // only creates a new unit
            void add_to_army (shared_ptr<units::ProxyUnit>);
            int GetTownId (void);
            void SetTownId (int);
            void handler_set (handlers::_Timer*);                    // set a game handler
            void timer_inc (int x = 1);                              // increment the game timer
            void tic_time (void);                                    // make erthng needed on timer's tic
            virtual ~Town();
            // army's interface
            shared_ptr<units::ProxyUnit> get_attacker (void);
            shared_ptr<units::ProxyUnit> get_attacked (void);
            shared_ptr<units::ProxyUnit> get_unit (int uid);
            bool evrbody_dead (void);
            void remove_from_battle_rang (shared_ptr<units::ProxyUnit>);
            vector<shared_ptr<units::ProxyUnit>> get_warriors (void);
            void init_battle_rang (void);
            // hero's interface
            void HeroResetMana (void);
            int HeroFreezeUnit (shared_ptr<units::ProxyUnit>);
            int HeroFireball (vector<shared_ptr<units::ProxyUnit>>);
            int HeroDublicateUnit (shared_ptr<units::ProxyUnit>&);
            void HeroSetUsed (void);
            void HeroResetUsed (void);
            bool HeroGetUsedStatus (void);
    };
    class Castle : public Town {
        public:
            Castle();
    };
    class Tower : public Town {
        public:
            Tower();
    };
    class Dungeon : public Town {
        public:
            Dungeon();
    };
};
