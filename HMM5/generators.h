#pragma once

#include <iostream>
#include <memory>
#include "units.h"
#include "defines.h"

using namespace std;

namespace generators {
    class RandomGen {
            static shared_ptr<RandomGen> _self;
        protected:
            RandomGen ();
        public:
            static shared_ptr<RandomGen> Instance ();
            int get_random (int low, int high);
    };
    class UnitIdGen {
            static shared_ptr<UnitIdGen> _self;
            static unsigned cnt;
        protected:
            UnitIdGen ();
        public:
            static shared_ptr<UnitIdGen> Instance ();
            shared_ptr<units::Identificator> next_uid (string department, unsigned arm_idx);
    };
};
