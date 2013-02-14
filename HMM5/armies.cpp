#include <list>
#include <vector>
#include <string>
#include "armies.h"
#include "generators.h"
#include "towns.h"

namespace armies {
    //---------------------------------------------------------------- Abstract army functions
    Army::Army () : def_life_time(10), battle_rang(0), town(0), warriors() {};
    int Army::get_life_time (void) { return def_life_time; }
    void Army::set_town (towns::Town* t) { town = t; }
    Army::~Army () { cout << "army deleted" << endl; }
    string Army_Tower::get_name   (void) { return "Tower's Army";   }
    string Army_Castle::get_name  (void) { return "Castle's Army";  }
    string Army_Dungeon::get_name (void) { return "Dungeon's Army"; }
    bool Army::evrbody_dead (void) {
        for (unsigned i = 0; i < warriors.size(); i++) {
            if (warriors[i]->GetHealth() > 0)
                return false;
        }
        return true;
    }
    shared_ptr<units::ProxyUnit> Army::get_attacker (void) {
        shared_ptr<units::ProxyUnit> u;
        if (!battle_rang.empty()) {
            u = battle_rang.front();
            battle_rang.pop_front();
            return u;
        }
        return 0;
    }
    shared_ptr<units::ProxyUnit> Army::get_random_unit (void) {
        shared_ptr<units::ProxyUnit> u = 0;
        vector<shared_ptr<units::ProxyUnit>> vctr;
        for (unsigned i = 0; i < warriors.size(); i++) {
            if (!warriors[i]->IsDead()) {
                vctr.push_back(warriors[i]);
            }
        }
        shared_ptr<generators::RandomGen> rgen = generators::RandomGen::Instance();
        u = vctr[rgen->get_random(0, vctr.size())];
        return u;
    }
    void Army::clone_unit (int uid) {
        shared_ptr<units::ProxyUnit> q = get_unit(uid);
        if (!q) {
            printlog("Invalid id for making a new clone!");
            return;
        }
        town->new_unit(q->GetDepartment(), q->GetArmyIdx());
        town->timer_inc();
    };
    shared_ptr<units::ProxyUnit> Army::doublicate_unit (shared_ptr<units::ProxyUnit> q) {
        shared_ptr<units::ProxyUnit> p = town->create_unit(q->GetDepartment());
        p->fill_with(q);
        p->SetArmyIdx(q->GetArmyIdx());
        town->add_to_army(p);
        return p;
    };
    void Army::tic_time (void) {
        for (unsigned i = 0; i < warriors.size(); i++) {
            warriors[i]->DecFreezeTime();
        }
    };
    void Army::delete_unit_id (int uid) {
        for (unsigned i = 0; i < warriors.size(); i++)
            if (warriors[i]->GetId() == uid) {
                delete_unit_pos(i);
                return;
            }
        printlog("invalid id");
    };
    void Army::delete_unit_pos (unsigned num) {
        int uid = warriors[num]->GetId();
        warriors[num].reset();
        warriors.erase(warriors.begin() + num);
        cout << "warrior id " << uid << " deleted" << endl;
    }
    shared_ptr<units::ProxyUnit> Army::get_unit (int uid) {
        for (unsigned i = 0; i < warriors.size(); i++)
            if (warriors[i]->GetId() == uid)
                return warriors[i];
        return 0;
    }
    vector<shared_ptr<units::ProxyUnit>> Army::get_warriors (void) {
        vector<shared_ptr<units::ProxyUnit>> vctr;
        for (unsigned i = 0; i < warriors.size(); i++) {
            if (!warriors[i]->IsDead()) {
                vctr.push_back(warriors[i]);
            }
        }
        return vctr;
    }
    void Army::init_battle_rang (void) {
        battle_rang.clear();
        for (unsigned i = 0; i < warriors.size(); i++) {
            if (!warriors[i]->IsDead()) {
                battle_rang.push_back(warriors[i]);
            }
        }
        units::Comparator compare_units;
        battle_rang.sort(compare_units);
        list<shared_ptr<units::ProxyUnit>>::const_iterator li = battle_rang.begin();
        for (unsigned i = 0; i < battle_rang.size(); i++) {
            cout << (*li)->GetId() << " ";
            cout << (*li)->GetLocation();
            cout << "\t spd: " << (*li)->GetSpeed() << endl;
            ++li;
        }
        cout << endl;
    }
    void Army::recalc_battle_rang (void) {
        units::Comparator compare_units;
        battle_rang.sort(compare_units);
    }
    void Army::remove_from_battle_rang (shared_ptr<units::ProxyUnit> u) {
        // check if unit belongs to this army
        shared_ptr<units::ProxyUnit> p = get_unit(u->GetId());
        if (p) {
            battle_rang.remove(u);
        }
    }
    //------------------------------------------------------------ Specificated army functions
    void Army_Tower::add_unit (shared_ptr<units::ProxyUnit> q) {
        warriors.push_back(q);
        printlog(0, q);
    }
    void Army_Castle::add_unit (shared_ptr<units::ProxyUnit> q) {
        warriors.push_back(q);
        printlog(0, q);
    }
    void Army_Dungeon::add_unit (shared_ptr<units::ProxyUnit> q) {
        warriors.push_back(q);
        printlog(0, q);
    }
    void Army_Tower::printlog (int param, shared_ptr<units::ProxyUnit> q) {
        if (param == 0) {
            cout << "\t " <<  q->GetName() << " id " << q->GetId();
            cout << " created. placed in army " << q->GetArmyIdx() << endl;
        }
    };
    void Army_Castle::printlog (int param, shared_ptr<units::ProxyUnit> q) {
        if (param == 0) {
            cout << "\t " <<  q->GetName() << " id " << q->GetId();
            cout << " created. placed in army " << q->GetArmyIdx() << endl;
        }
    };
    void Army_Dungeon::printlog (int param, shared_ptr<units::ProxyUnit> q) {
        if (param == 0) {
            cout << "\t " <<  q->GetName() << " id " << q->GetId();
            cout << " created. placed in army " << q->GetArmyIdx() << endl;
        }
    };
    void Army_Tower::printlog (const string &s) {
        cout << "Tower's army report :" << endl;
        cout << "\t " << s << endl;
    };
    void Army_Castle::printlog (const string &s) {
        cout << "Tower's army report :" << endl;
        cout << "\t " << s << endl;
    };
    void Army_Dungeon::printlog (const string &s) {
        cout << "Tower's army report :" << endl;
        cout << "\t " << s << endl;
    };
};

