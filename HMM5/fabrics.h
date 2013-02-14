#pragma once

#include <memory>
#include "units.h"
#include "defines.h"

namespace fabrics {
    class Fabric {
        // Abstract fabric class
        public:
            virtual shared_ptr<units::ProxyUnit> create_shooter (void) = 0;         // create one new shooter
            virtual shared_ptr<units::ProxyUnit> create_rider   (void) = 0;         // create one new rider
            virtual shared_ptr<units::ProxyUnit> create_wizard  (void) = 0;         // create one new wizard
            virtual shared_ptr<units::ProxyUnit> create_dragon  (void) = 0;         // create one new dragon
            virtual ~Fabric() {};
    };
    class Fabric_Tower : public Fabric {
        protected:
            shared_ptr<units::ProxyUnit> create_shooter (void);
            shared_ptr<units::ProxyUnit> create_rider   (void);
            shared_ptr<units::ProxyUnit> create_wizard  (void);
            shared_ptr<units::ProxyUnit> create_dragon  (void);
    };
    class Fabric_Castle : public Fabric {
        protected:
            shared_ptr<units::ProxyUnit> create_shooter (void);
            shared_ptr<units::ProxyUnit> create_rider   (void);
            shared_ptr<units::ProxyUnit> create_wizard  (void);
            shared_ptr<units::ProxyUnit> create_dragon  (void);
    };
    class Fabric_Dungeon : public Fabric {
        protected:
            shared_ptr<units::ProxyUnit> create_shooter (void);
            shared_ptr<units::ProxyUnit> create_rider   (void);
            shared_ptr<units::ProxyUnit> create_wizard  (void);
            shared_ptr<units::ProxyUnit> create_dragon  (void);
    };
};
