#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include "fields.h"
#include "units.h"
#include "defines.h"
#include "coords.h"

namespace parsers {
    class Parser {
            static shared_ptr<Parser> _self;
            static bool good_input;
        protected:
            Parser () {};
        public:
            static shared_ptr<Parser> Instance ();
            string get_command (void);
            string get_unit_type (void);
            string get_hero_method (void);
            string get_battle_mode (void);
            string get_town_type (void);
            unsigned get_index (void);
            coords::coord get_coords (void);
            bool check_if_good (void);
    };
};