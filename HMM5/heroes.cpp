#include <iostream>
#include <memory>
#include "units.h"
#include "heroes.h"

namespace heroes {
    int Hero::GetMana (void) { return mana; }
    void Hero::DecMana (int x) { mana-=x; }
    void Hero::ResetMana (void) { mana = DEF_MANA; }
    void Hero::set_army (shared_ptr<armies::Army> a) { army = a; }
    void Hero::SetMana (int m) { mana = m; }
    void Hero::SetUsed (void) { used = true; }
    void Hero::ResetUsed (void) { used = false; }
    bool Hero:: GetUsedStatus (void) { return used; }
    int Hero::DublicateUnit (shared_ptr<units::ProxyUnit>& u) {
        if (mana > 100) {
            u = army->doublicate_unit(u);
            DecMana(100);
            return 0;
        }
        cout << "\t not enough mana to perform this action" << endl;
        return -1;
    }
    int Hero::FreezeUnit (shared_ptr<units::ProxyUnit> u) {
        if (mana > 100) {
            u->SetFreeze(3);
            cout << "\t id" << u->GetId() << endl;
            DecMana(100);
            return 0;
        }
        cout << "\t not enough mana to perform this action" << endl;
        return -1;
    }
    int Hero::Fireball (vector<shared_ptr<units::ProxyUnit>> vctr) {
        int charge = 20;
        if (mana > charge * vctr.size()) {
            for (unsigned i = 0; i < vctr.size(); i++) {
                cout << "\t id" << vctr[i]->GetId() << " ";
                cout << "hp: " << vctr[i]->GetHealth();
                vctr[i]->DecHealth(charge);
                cout << " - [" << charge << "] -> " << vctr[i]->GetHealth() << endl;
            }
            DecMana(vctr.size() * charge);
            return 0;
        }
        cout << "\t not enough mana to perform this action" << endl;
        return -1;
    }
    Hero::~Hero () { cout << "hero deleted" << endl; }
};
