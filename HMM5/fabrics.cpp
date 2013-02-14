#include <memory>
#include "units.h"
#include "fabrics.h"

namespace fabrics {
    shared_ptr<units::ProxyUnit> fabrics::Fabric_Tower::create_shooter (void) {
        shared_ptr<units::_Shooter> q (new units::MasterGremlin);
        return shared_ptr<units::ProxyUnit> (new units::ProxyUnit(q,"shooter", -1));
    }
    shared_ptr<units::ProxyUnit> fabrics::Fabric_Castle::create_shooter (void) {
        shared_ptr<units::_Shooter> q (new units::Archery);
        return shared_ptr<units::ProxyUnit> (new units::ProxyUnit(q,"shooter", -1));
    }
    shared_ptr<units::ProxyUnit> fabrics::Fabric_Dungeon::create_shooter (void) {
        shared_ptr<units::_Shooter> q (new units::Beholder);
        return shared_ptr<units::ProxyUnit> (new units::ProxyUnit(q,"shooter", -1));
    }
    shared_ptr<units::ProxyUnit> fabrics::Fabric_Tower::create_rider (void) {
        shared_ptr<units::_Rider> q (new units::Golem);
        return shared_ptr<units::ProxyUnit> (new units::ProxyUnit(q,"rider", -1));
    }
    shared_ptr<units::ProxyUnit> fabrics::Fabric_Castle::create_rider (void) {
        shared_ptr<units::_Rider> q (new units::Knight);
        return shared_ptr<units::ProxyUnit> (new units::ProxyUnit(q,"rider", -1));
    }
    shared_ptr<units::ProxyUnit> fabrics::Fabric_Dungeon::create_rider (void) {
        shared_ptr<units::_Rider> q (new units::Minotavr);
        return shared_ptr<units::ProxyUnit> (new units::ProxyUnit(q,"rider", -1));
    }
    shared_ptr<units::ProxyUnit> fabrics::Fabric_Tower::create_wizard (void) {
        shared_ptr<units::_Wizard> q (new units::Wizard);
        return shared_ptr<units::ProxyUnit> (new units::ProxyUnit(q,"wizard", -1));
    }
    shared_ptr<units::ProxyUnit> fabrics::Fabric_Castle::create_wizard (void) {
        shared_ptr<units::_Wizard> q (new units::Monk);
        return shared_ptr<units::ProxyUnit> (new units::ProxyUnit(q,"wizard", -1));
    }
    shared_ptr<units::ProxyUnit> fabrics::Fabric_Dungeon::create_wizard (void) {
        shared_ptr<units::_Wizard> q (new units::Medusa);
        return shared_ptr<units::ProxyUnit> (new units::ProxyUnit(q,"wizard", -1));
    }
    shared_ptr<units::ProxyUnit> fabrics::Fabric_Tower::create_dragon (void) {
        shared_ptr<units::_Dragon> q (new units::Angel);
        return shared_ptr<units::ProxyUnit> (new units::ProxyUnit(q,"dragon", -1));
    }
    shared_ptr<units::ProxyUnit> fabrics::Fabric_Castle::create_dragon (void) {
        shared_ptr<units::_Dragon> q (new units::Titan);
        return shared_ptr<units::ProxyUnit> (new units::ProxyUnit(q,"dragon", -1));
    }
    shared_ptr<units::ProxyUnit> fabrics::Fabric_Dungeon::create_dragon (void) {
        shared_ptr<units::_Dragon> q (new units::BlackDragon);
        return shared_ptr<units::ProxyUnit> (new units::ProxyUnit(q,"dragon", -1));
    }
};
