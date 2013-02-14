#include <algorithm>
#include "units.h"
#include "generators.h"

namespace units {
    Identificator::Identificator (int uid, string dep, unsigned arm)
        : _uid(uid), _department(dep), arm_idx(arm) {};
    int Identificator::get_id (void) { return _uid; }
    string Identificator::get_department (void) { return _department; }
    unsigned Identificator::get_arm_idx (void) { return arm_idx; }
    void Identificator::set_arm_idx (unsigned idx) { arm_idx = idx; }

    coords::coord Location::get_location (void) {
        return crd;
    }
    void Location::set_location (coords::coord new_crd) {
        crd = new_crd;
    }

    void Unit::SetHealth (int h) { health = h; }
    void Unit::SetSpeed (int s) { speed = s; }
    int Unit::GetDamage (void) { return damage; }
    int Unit::GetSkill (void) { return skill; }
    int Unit::GetSpeed (void) { return speed; }
    int Unit::GetHealth (void) { return health; }
    int Unit::GetFreezeTime (void) { return freeze; }
    void Unit::DecFreezeTime (void) { freeze = (freeze > 0) ? (freeze - 1) : 0 ; }
    bool Unit::IsFrozen (void) { return (freeze > 0); }
    void Unit::SetFreeze (int x) { freeze = x; }
    void Unit::DecHealth (int x) { health = (health > x) ? (health - x) : 0; if (x == -1) health = 0; }
    int Unit::WeaponGetDamage (int w) { return weapon[w]->get_damage(); }
    int Unit::WeaponGetResource (int w) { return weapon[w]->get_resource(); }
    int Unit::WeaponDecResource (int w, int c) { return weapon[w]->dec_resource(c); }
    string Unit::GetName (void) { return name; }
    void Unit::IncSkill (int x) { skill += x; }
    void Unit::SetDamage (int x) { damage = x; }
    void Unit::fill_with (shared_ptr<Unit> q) {
        this->health = q->GetHealth();
        this->name = q->GetName();
        this->damage = q->GetDamage();
        this->skill = q->GetSkill();
        this->freeze = q->GetFreezeTime();
        for (unsigned i = 0; i < this->weapon.size(); i++) {
            this->weapon[i]->fill_with(q->weapon[i]);
        }
    }

    ProxyUnit::ProxyUnit(shared_ptr<Unit> _unit, string depart, unsigned arm_idx)
        : base_unit(_unit), id(0), location(0) {
        shared_ptr<generators::UnitIdGen> id_gen = generators::UnitIdGen::Instance();
        id = id_gen->next_uid(depart, arm_idx);
        location = shared_ptr<Location> (new Location);
    }
    bool ProxyUnit::IsDead (void) { return base_unit->GetHealth() == 0; }
    int ProxyUnit::GetId (void) { return id->get_id(); }
    coords::coord ProxyUnit::GetLocation (void) { return location->get_location(); }
    int ProxyUnit::GetDamage (void) { return base_unit->GetDamage(); }
    int ProxyUnit::GetSkill (void) { return base_unit->GetSkill(); }
    int ProxyUnit::GetSpeed (void) { return base_unit->GetSpeed(); }
    int ProxyUnit::GetHealth (void) { return base_unit->GetHealth(); }
    unsigned ProxyUnit::GetArmyIdx (void) { return id->get_arm_idx(); }
    bool ProxyUnit::IsFrozen (void) { return base_unit->IsFrozen(); }
    void ProxyUnit::DecFreezeTime (void) { base_unit->DecFreezeTime(); }
    void ProxyUnit::SetFreeze (int x) { base_unit->SetFreeze(x); }
    void ProxyUnit::SetLocation (coords::coord crd) { location->set_location(crd); }
    void ProxyUnit::SetArmyIdx (unsigned idx) { id->set_arm_idx(idx); }
    void ProxyUnit::DecHealth (int x) { base_unit->DecHealth(x); }
    string ProxyUnit::GetDepartment (void) { return id->get_department(); }
    string ProxyUnit::GetName (void) { return base_unit->GetName(); }
    void ProxyUnit::IncSkill (int x) { base_unit->IncSkill(x); }
    int ProxyUnit::WeaponGetDamage (int w) { return base_unit->WeaponGetDamage(w); }
    int ProxyUnit::WeaponGetResource (int w) { return base_unit->WeaponGetResource(w); }
    int ProxyUnit::WeaponDecResource (int w, int c) { return base_unit->WeaponDecResource(w,c); }
    void ProxyUnit::SetDamage (int x) { base_unit->SetDamage(x); }
    void ProxyUnit::fill_with (shared_ptr<ProxyUnit> punit) { base_unit->fill_with(punit->base_unit); }
    void ProxyUnit::accept(visitors::CommandVisitor &v) { base_unit->accept(v); }

    _Shooter::~_Shooter () {};
    _Rider::  ~_Rider   () {};
    _Wizard:: ~_Wizard  () {};
    _Dragon:: ~_Dragon  () {};

    bool Comparator::operator () (shared_ptr<ProxyUnit> u1, shared_ptr<ProxyUnit> u2) {
        return (u1->GetSpeed() > u2->GetSpeed());
    }

    Archery::Archery() {
        name = "Archery";
        SetHealth(45);
        SetSpeed(2);
        weapon.push_back(shared_ptr<weapon::Weapon> (new weapon::WBow));
        weapon.push_back(shared_ptr<weapon::Weapon> (new weapon::WArms));
    }
    MasterGremlin::MasterGremlin() {
        name = "MasterGremlin";
        SetHealth(25);
        SetSpeed(3);
        weapon.push_back(shared_ptr<weapon::Weapon> (new weapon::WSling));
        weapon.push_back(shared_ptr<weapon::Weapon> (new weapon::WArms));
    }
    Knight::Knight() {
        name = "Knight";
        SetHealth(145);
        SetSpeed(5);
        weapon.push_back(shared_ptr<weapon::Weapon> (new weapon::WLance));
    }
    Golem::Golem() {
        name = "Golem";
        SetHealth(145);
        SetSpeed(3);
        weapon.push_back(shared_ptr<weapon::Weapon> (new weapon::WArms));
    }
    Monk::Monk() {
        name = "Monk";
        SetHealth(100);
        SetSpeed(4);
        weapon.push_back(shared_ptr<weapon::Weapon> (new weapon::WFaith));
        weapon.push_back(shared_ptr<weapon::Weapon> (new weapon::WArms));
    }
    Wizard::Wizard() {
        name = "Wizard";
        SetHealth(90);
        SetSpeed(4);
        weapon.push_back(shared_ptr<weapon::Weapon> (new weapon::WBrain));
        weapon.push_back(shared_ptr<weapon::Weapon> (new weapon::WArms));
    }
    Beholder::Beholder(){
        name = "Beholder";
        SetHealth(35);
        SetSpeed(3);
        weapon.push_back(shared_ptr<weapon::Weapon> (new weapon::WBow));
        weapon.push_back(shared_ptr<weapon::Weapon> (new weapon::WArms));
    }
    Minotavr::Minotavr() {
        name = "Minotavr";
        SetHealth(190);
        SetSpeed(4);
        weapon.push_back(shared_ptr<weapon::Weapon> (new weapon::WLance));
    }
    Medusa::Medusa() {
        name = "Medusa";
        SetHealth(80);
        SetSpeed(10);
        weapon.push_back(shared_ptr<weapon::Weapon> (new weapon::WFaith));
        weapon.push_back(shared_ptr<weapon::Weapon> (new weapon::WArms));
    }
    Titan::Titan() {
        name = "Titan";
        SetHealth(800);
        SetSpeed(10);
        weapon.push_back(shared_ptr<weapon::Weapon> (new weapon::WFlame));
    }
    Angel::Angel() {
        name = "Angel";
        SetHealth(800);
        SetSpeed(10);
        weapon.push_back(shared_ptr<weapon::Weapon> (new weapon::WFlame));
    }
    BlackDragon::BlackDragon() {
        name = "BlackDragon";
        SetHealth(900);
        SetSpeed(12);
        weapon.push_back(shared_ptr<weapon::Weapon> (new weapon::WFlame));
    }

    void Archery::      accept (visitors::CommandVisitor& v) { return v.visit(this); }
    void MasterGremlin::accept (visitors::CommandVisitor& v) { return v.visit(this); }
    void Knight::       accept (visitors::CommandVisitor& v) { return v.visit(this); }
    void Golem::        accept (visitors::CommandVisitor& v) { return v.visit(this); }
    void Monk::         accept (visitors::CommandVisitor& v) { return v.visit(this); }
    void Wizard::       accept (visitors::CommandVisitor& v) { return v.visit(this); }
    void Beholder::     accept (visitors::CommandVisitor& v) { return v.visit(this); }
    void Minotavr::     accept (visitors::CommandVisitor& v) { return v.visit(this); }
    void Medusa::       accept (visitors::CommandVisitor& v) { return v.visit(this); }
    void Titan::        accept (visitors::CommandVisitor& v) { return v.visit(this); }
    void Angel::        accept (visitors::CommandVisitor& v) { return v.visit(this); }
    void BlackDragon::  accept (visitors::CommandVisitor& v) { return v.visit(this); }
};
namespace visitors {
    void AttackVisitor::visit(units::Archery* q) {
        if (q->WeaponDecResource(0,2) == 0) {
            q->SetDamage(q->WeaponGetDamage(0));
        } else {
            q->SetDamage(q->WeaponGetDamage(1));
        }
    }
    void AttackVisitor::visit(units::MasterGremlin* q) {
        if (q->WeaponDecResource(0) == 0) {
            q->SetDamage(15 + q->WeaponGetResource(0) / 2);
        } else {
            q->SetDamage(q->WeaponGetDamage(1));
        }
    }
    void AttackVisitor::visit(units::Knight* q) {
        q->SetDamage(q->WeaponGetDamage(0));
    }
    void AttackVisitor::visit(units::Golem* q) {
        q->SetDamage(q->WeaponGetDamage(0));
    }
    void AttackVisitor::visit(units::Monk* q) {
        shared_ptr<generators::RandomGen> rgen = generators::RandomGen::Instance();
        int t = rgen->get_random(20, 31);
        if (q->WeaponDecResource(0, t / 10) == 0) {
            q->SetDamage(t);
        } else {
            q->SetDamage(q->WeaponGetDamage(1));
        }
    }        
    void AttackVisitor::visit(units::Wizard* q) {
        if (q->WeaponDecResource(0, 10) == 0) {
            shared_ptr<generators::RandomGen> rgen = generators::RandomGen::Instance();
            q->SetDamage(rgen->get_random(30, 51));
        } else {
            q->SetDamage(q->WeaponGetDamage(1));
        }
    }
    void AttackVisitor::visit(units::Beholder* q) {
        q->SetDamage(q->WeaponGetDamage(0));
    }
    void AttackVisitor::visit(units::Minotavr* q) {
        q->SetDamage(q->WeaponGetDamage(0));
    }
    void AttackVisitor::visit(units::Medusa* q) {
        shared_ptr<generators::RandomGen> rgen = generators::RandomGen::Instance();
        int t = rgen->get_random(20, 31);
        if (q->WeaponDecResource(0, t / 10) == 0) {
            q->SetDamage(t);
        } else {
            q->SetDamage(q->WeaponGetDamage(1));
        }
    }
    void AttackVisitor::visit(units::Titan* q) {
        q->SetDamage(q->WeaponGetDamage(0));
    }
    void AttackVisitor::visit(units::Angel* q) {
        q->SetDamage(q->WeaponGetDamage(0));
    }
    void AttackVisitor::visit(units::BlackDragon* q) {
        q->SetDamage(q->WeaponGetDamage(0));
    }
};