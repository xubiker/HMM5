#pragma once

#include <list>
#include <vector>
#include <memory>
#include "units.h"
#include "defines.h"

namespace towns {
    class Town;
}

namespace armies {
    class Army {
        // Abstract army class
            int def_life_time;                                  // default life time of every dublicated warrior
            list<shared_ptr<units::ProxyUnit>> battle_rang;     // battle rang performed as a list
            virtual void printlog (const string&) = 0;          // log printing
            virtual void printlog (int, shared_ptr<units::ProxyUnit>) = 0;
        protected:
            towns::Town* town;                                  // pointer to army's parent town
            vector<shared_ptr<units::ProxyUnit>> warriors;      // all warriors in the army
            void delete_unit_pos (unsigned);                    // delete unit by its position in vector
            void delete_unit_id (int uid);                      // delete unit by its id number in army
        public:
            Army ();
#ifdef ejudge_compile
            Army (const Army&) = delete;
            Army& operator= (const Army&) = delete;
#endif
            virtual string get_name (void) = 0;                          // returns the town's army name
            void tic_time (void);                                        // make erthng needed when on a timer's tic
            void clone_unit (int);                                       // make a unit's clone by it's id
            shared_ptr<units::ProxyUnit> doublicate_unit (shared_ptr<units::ProxyUnit>);
            shared_ptr<units::ProxyUnit> get_unit (int);                 // returns a unit's pointer by it's id
            vector<shared_ptr<units::ProxyUnit>> get_warriors (void);
            virtual void add_unit (shared_ptr<units::ProxyUnit>) = 0;    // adding a new unit to army
            void init_battle_rang (void);                                // initialize the battle rang
            void recalc_battle_rang (void);                              // recalculatees the battle rang
            void remove_from_battle_rang (shared_ptr<units::ProxyUnit>); // remove unit from rang
            void remove_all_dublicates (void);                           // remove all dublicates from army
            int get_life_time (void);                                    // get default life time for all doublicates
            shared_ptr<units::ProxyUnit> get_attacker (void);            // returns unit which is able to attack
            shared_ptr<units::ProxyUnit> get_random_unit (void);         // returns random unit
            bool evrbody_dead (void);                                    // check if all warriors are dead
            void set_town (towns::Town*);                                // sets a pointer to army's parent town
            bool compare_units_f (shared_ptr<units::ProxyUnit>, shared_ptr<units::ProxyUnit>);
            virtual ~Army();
    };

    class Army_Tower : public Army {
        // Army's class specification for Tower
        protected:
            void add_unit (shared_ptr<units::ProxyUnit>);
            virtual void printlog (int, shared_ptr<units::ProxyUnit>);
            virtual void printlog (const string&);
        public:
            virtual string get_name (void);
    };
    class Army_Castle : public Army {
        // Army's class specification for Castle
        protected:
            void add_unit (shared_ptr<units::ProxyUnit>);
            virtual void printlog (int, shared_ptr<units::ProxyUnit>);
            virtual void printlog (const string&);
        public:
            virtual string get_name (void);
    };
    class Army_Dungeon : public Army {
        // Army's class specification for Dungeon
        protected:
            void add_unit (shared_ptr<units::ProxyUnit>);
            virtual void printlog (int, shared_ptr<units::ProxyUnit>);
            virtual void printlog (const string&);
        public:
            virtual string get_name (void);
    };

};
