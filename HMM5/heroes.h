#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include "armies.h"
#include "units.h"
#include "defines.h"

using namespace std;

namespace heroes {
    class Hero {
            unsigned mana;
            shared_ptr<armies::Army> army;
            bool used;
        public:
            Hero (int m) : mana(m), army(0), used(false) {};
#ifdef ejudge_compile
            Hero (const Hero&) = delete;
            Hero& operator= (const Hero&) = delete;
#endif
            int GetMana (void);
            void DecMana (int);
            void ResetMana (void);
            void set_army (shared_ptr<armies::Army>);
            void SetMana (int);
            void SetUsed (void);
            void ResetUsed (void);
            bool GetUsedStatus (void);
            int DublicateUnit (shared_ptr<units::ProxyUnit>&);
            int FreezeUnit (shared_ptr<units::ProxyUnit>);
            int Fireball (vector<shared_ptr<units::ProxyUnit>>);
            ~Hero ();
    };
};