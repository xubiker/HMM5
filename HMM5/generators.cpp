#include <iostream>
#include <memory>
#include <string>
#include "units.h"
#include "generators.h"

using namespace std;

namespace generators {
    RandomGen::RandomGen() {};
    shared_ptr<RandomGen> RandomGen::Instance () {
        if (!_self) {
            _self = shared_ptr<RandomGen> (new RandomGen());
        }
        return _self;
    }
    int RandomGen::get_random (int low, int high) {
        return int(rand() / (RAND_MAX + 1.0) * (high - low) + low);
    }
    shared_ptr<RandomGen> RandomGen::_self = 0;
    
    UnitIdGen::UnitIdGen () {};
    shared_ptr<UnitIdGen> UnitIdGen::Instance () {
        if (!_self) {
            _self = shared_ptr<UnitIdGen> (new UnitIdGen());
        }
        return _self;
    }
    shared_ptr<units::Identificator> UnitIdGen::next_uid (string department, unsigned arm_idx) {
        return shared_ptr<units::Identificator> (new units::Identificator(cnt++, department, arm_idx));
    }
    shared_ptr<UnitIdGen> UnitIdGen::_self = 0;
    unsigned UnitIdGen::cnt = 1;
};
