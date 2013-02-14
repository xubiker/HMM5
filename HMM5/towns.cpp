#include <string>
#include "towns.h"
#include "handlers.h"

namespace towns {
    void Town::add_to_army (shared_ptr<units::ProxyUnit> punit) {army->add_unit(punit); }
    int Town::GetTownId (void) { return TownId; }
    void Town::SetTownId (int tid) { TownId = tid; }
    void Town::handler_set (handlers::_Timer* q) { timer = q; }        // set a game handler
    void Town::timer_inc (int x) { timer->tic_inc(x); }                // increment the game timer
    void Town::tic_time (void) { army->tic_time(); }                   // make erthng needed on timer's tic
    Town::~Town() { cout << "town deleted" << endl; }
    // army's interface
    shared_ptr<units::ProxyUnit> Town::get_attacker (void) { return army->get_attacker(); }
    shared_ptr<units::ProxyUnit> Town::get_attacked (void) { return army->get_random_unit(); }   
    shared_ptr<units::ProxyUnit> Town::get_unit (int uid) { return army->get_unit(uid); }
    bool Town::evrbody_dead (void) { return army->evrbody_dead(); }
    void Town::remove_from_battle_rang (shared_ptr<units::ProxyUnit> u) { army->remove_from_battle_rang(u); }
    vector<shared_ptr<units::ProxyUnit>> Town::get_warriors (void) {
        return army->get_warriors();
    }
    void Town::init_battle_rang (void) { army->init_battle_rang(); }
    // hero's interface
    void Town::HeroResetMana (void) { hero->ResetMana(); }
    int Town::HeroFreezeUnit (shared_ptr<units::ProxyUnit> u) { return hero->FreezeUnit(u); }
    int Town::HeroFireball (vector<shared_ptr<units::ProxyUnit>> vctr) {return hero->Fireball(vctr); }
    int Town::HeroDublicateUnit (shared_ptr<units::ProxyUnit>& u) { return hero->DublicateUnit(u); }
    void Town::HeroSetUsed (void) { hero->SetUsed(); }
    void Town::HeroResetUsed (void) { hero->ResetUsed(); }
    bool Town::HeroGetUsedStatus (void) { return hero->GetUsedStatus(); }

    Castle::Castle() {
        hero = shared_ptr<heroes::Hero> (new heroes::Hero(DEF_MANA));
        army = shared_ptr<armies::Army> (new armies::Army_Castle);
        fabric = shared_ptr<fabrics::Fabric_Castle> (new fabrics::Fabric_Castle);
        army->set_town(this);
        hero->set_army(army);
    }
    Tower::Tower() {
        hero = shared_ptr<heroes::Hero> (new heroes::Hero(DEF_MANA));
        army = shared_ptr<armies::Army>(new armies::Army_Tower);
        fabric = shared_ptr<fabrics::Fabric_Tower> (new fabrics::Fabric_Tower);
        army->set_town(this);
        hero->set_army(army);
    }
    Dungeon::Dungeon() {
        hero = shared_ptr<heroes::Hero> (new heroes::Hero(DEF_MANA));
        army = shared_ptr<armies::Army>(new armies::Army_Dungeon);
        fabric = shared_ptr<fabrics::Fabric_Dungeon> (new fabrics::Fabric_Dungeon);
        army->set_town(this);
        hero->set_army(army);
    }
    shared_ptr<units::ProxyUnit> Town::create_unit (string type) {
        if (type == "shooter") { return fabric->create_shooter(); }
        if (type == "rider")   { return fabric->create_rider();   }
        if (type == "wizard")  { return fabric->create_wizard();  }
        if (type == "dragon")  { return fabric->create_dragon();  }
        return 0;
    }
    void Town::new_unit (string type, unsigned arm_idx) {
        shared_ptr<units::ProxyUnit> punit = Town::create_unit(type);
        punit->SetArmyIdx(arm_idx);
        army->add_unit(punit);
    }
};
