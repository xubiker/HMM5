#pragma once

#include <iostream>
#include <memory>
#include "defines.h"

using namespace std;

namespace weapon {    
    class Weapon {
        // abstarct class of weapon
        protected:
            string name;
            int damage;             // if (-1)  damaged should be recounted for unit itself
            int resource;           // if (-1) the resource is unlimited
            int range;              // 0 - close, 1 - at a distance
        public:
            Weapon();
            int dec_resource (int x = 1);
            int get_resource (void);
            int get_damage (void);
            int get_range (void);
            string get_name (void);
            void fill_with (shared_ptr<Weapon>);
            virtual ~Weapon();
    };
    class WBow : public Weapon {
        public:
            WBow();
    };
    class WLance : public Weapon {
        public:
            WLance();
    };
    class WFaith : public Weapon {
        public:
            WFaith();
    };
    class WSling : public Weapon {
        public:
            WSling();
    };
    class WArms : public Weapon {
        public:
            WArms();
    };
    class WBrain : public Weapon {
        public:
            WBrain();
    };
    class WFlame : public Weapon {
        public:
            WFlame();
    };
};