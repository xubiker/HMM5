#include "weapon.h"

using namespace std;

namespace weapon {
    Weapon::Weapon () : name(""), damage(-1), resource(-1), range(0) {};
    int Weapon::get_resource () { return resource; }
    int Weapon::get_damage () { return damage; }
    int Weapon::get_range () { return range; }
    string Weapon::get_name () { return name; }
    void Weapon::fill_with (shared_ptr<Weapon> q) {
        this->name = q->get_name();
        this->damage = q->get_damage();
        this->resource = q->get_resource();
        this->range = q->get_range();
    }
    int Weapon::dec_resource (int x) {
        if (resource == -1) {
            return -1;
        }
        if (resource - x < 0) {
            resource = 0;
            return -1;
        }
        resource -= x;
        return 0;
    }
    Weapon::~Weapon() {};
    //------------------------------------------------------ Weapon Customization
    WBow::WBow () {
        name = "Bow wiht arrows";
        damage = 10;
        resource = 15;
        range = 1;
    }
    WLance::WLance () {
        name = "Lans";
        damage = 20;
    }
    WFaith::WFaith () {
        name = "The Power of Faith";
        resource = 30;
        range = 1;
    }
    WSling::WSling () {
        name = "Sling";
        resource = 10;
        range = 1;
    }
    WArms::WArms() {
        name = "Arms";
        damage = 30;
    }
    WBrain::WBrain () {
        name = "The power of Brain";
        resource = 50;
        range = 1;
    }
    WFlame::WFlame () {
        name = "Flame";
        damage = 100;
        range = 1;
    }
};
