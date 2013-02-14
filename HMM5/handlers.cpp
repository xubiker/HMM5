#include <memory>
#include <ctime>
#include "defines.h"
#include "handlers.h"
#include "parsers.h"
#include "fields.h"

namespace handlers {
    void Timer::tic_reset (void) {tic = 0;}
    void Timer::add_town (towns::Town* t) {
        t->SetTownId(towns.size()+1);
        towns.push_back(t);
    }
    void Timer::add_battle_handler (BattleHandler* b) { bh = b; }
    void Timer::tic_inc (int x) {
        tic += x;
        for (unsigned i = 0; i < towns.size(); i++) {
            towns[i]->tic_time();
        }
        bh->tic_time();
    }
    int Timer::get_tic (void) { return tic; }
    Timer::~Timer () { cout << "Game timer deleted" << endl; }
    BattleHandler::~BattleHandler() { cout << "battle handler deleted" << endl; }
    // BattleHandler's class realization
    void BattleHandler::tic_time (void) {
        for (unsigned i = 0; i < to_del.size(); i++) {
            if (--to_del[i].second == 0) {
                shared_ptr<units::ProxyUnit> p = to_del[i].first;
                if (p) {
                    p->DecHealth(-1);
                    town1->remove_from_battle_rang(p);
                    town2->remove_from_battle_rang(p);
                    field->reset_cell(p->GetLocation());
                    cout << "------ dublicate id" << p->GetId() << " deleted" << endl;
                }
                to_del.erase(to_del.begin() + i--);
            }
        }
    }
    shared_ptr<units::ProxyUnit> BattleHandler::choose_attacker (void) {
        return town1->get_attacker();
    }
    shared_ptr<units::ProxyUnit> BattleHandler::get_attacked (void) {
        shared_ptr<units::ProxyUnit> _attacked = 0;
        if (mode == 0) {
            _attacked = town2->get_attacked();
        } else {
            cout << "enter the coords, please" << endl;
            while (true) {
                shared_ptr<parsers::Parser> parser = parsers::Parser::Instance();
                coords::coord crd = parser->get_coords();
                if (!parser->check_if_good()) return 0;
                _attacked = field->get_cell(crd);
                if (_attacked == 0) {
                    cout << "invalid coords, try once more" << endl;
                } else if (_attacked->GetArmyIdx() != unsigned(town2->GetTownId())) {
                    cout  << "not enermy unit, try once more" << endl;
                } else {
                    break;
                }
            }
        }
        return _attacked;
    }
    coords::coord BattleHandler::choose_aim (shared_ptr<units::ProxyUnit> attacker) {
        coords::coord crd;
        if (mode == 1) {
            cout << "enter the coords, please" << endl;
            while (true) {
                shared_ptr<parsers::Parser> parser = parsers::Parser::Instance();
                crd = parser->get_coords();
                if (!parser->check_if_good()) { break; }
                if (field->invalid_cell(crd)) {
                    cout << "invalid coords, try once more" << endl;
                } else { break; }
            }
        } else if (mode == 0) {
            shared_ptr<units::ProxyUnit> aim = get_nearest_enemy(attacker);
            if (aim == 0) { return coords::coord(); }
            crd = aim->GetLocation();
        }
        return crd;
    }
    void BattleHandler::attack (shared_ptr<units::ProxyUnit> p1, shared_ptr<units::ProxyUnit> p2) {
        visitors::AttackVisitor a_cmd;
        p1->accept(a_cmd);
        int damage = p1->GetDamage();
        cout << "\t attack id" << p2->GetId() << endl;
        cout << "\t hp: " << p2->GetHealth();
        p2->DecHealth(damage);
        cout << " - [" << damage;
        cout << "] -> " << p2->GetHealth() << endl;
        if (p2->IsDead()) {
            town2->remove_from_battle_rang(p2);
            field->reset_cell(p2->GetLocation());
        }
        p1->IncSkill();
    }
    void BattleHandler::move_unit (coords::coord crd1, coords::coord crd2) {
        shared_ptr<units::ProxyUnit> punit = field->get_cell(crd1);
        if (punit == 0) { cout << endl << "an error occured" << endl; return; }
        punit->SetLocation(crd2);
        field->set_cell(crd2, punit);
        field->reset_cell(crd1);
    }
    void BattleHandler::move_unit (vector<coords::coord> path) {
        cout << "\t move  " << path[0];
        for (unsigned i = 0; i < path.size()-1; i++) {
            move_unit(path[i], path[i+1]);
            cout << "-" << path[i+1];
        }
        cout << endl;
    }
    shared_ptr<units::ProxyUnit> BattleHandler::get_nearest_enemy (shared_ptr<units::ProxyUnit> attacker) {
        vector< shared_ptr<units::ProxyUnit> > warriors = town2->get_warriors();
        unsigned dist = -1;
        shared_ptr<units::ProxyUnit> attacked = 0;
        for (unsigned i = 0; i < warriors.size(); i++) {
            unsigned new_dist = field->get_distance(attacker->GetLocation(), warriors[i]->GetLocation());
            if (new_dist < dist) {
                dist = new_dist;
                attacked = warriors[i];
            }
        }
        return attacked;
    }
    string BattleHandler::get_action (shared_ptr<units::ProxyUnit> attacker, coords::coord aim) {
        if (field->is_empty(aim)) {
            // only can be used in a manual mode
            return "move";
        }
        if (  (field->get_cell(aim)->GetArmyIdx() != attacker->GetArmyIdx()) &&
              (attacker->GetDepartment() == "shooter" || attacker->GetDepartment() == "wizard") &&
              (attacker->WeaponGetResource(0) > 0)  )
        {
            // can be used in both modes
            return "attack";
        }
        if (  (field->get_cell(aim)->GetArmyIdx() != attacker->GetArmyIdx()) &&
              ((attacker->GetDepartment() == "rider") || (attacker->GetDepartment() == "dragon") ||
              ((attacker->GetDepartment() == "shooter" || attacker->GetDepartment() == "wizard") &&
              (attacker->WeaponGetResource(0) == 0)))  )
        {
            // can be used in both modes
            return "move&attack";
        }
        return "";
    }
    shared_ptr<units::ProxyUnit> BattleHandler::get_dubl (void) {
        shared_ptr<units::ProxyUnit> dubl = 0;
        if (mode == 0) {
            dubl = town1->get_attacked();
        } else {
            cout << "enter id" << endl;
            while (true) {
                shared_ptr<parsers::Parser> parser = parsers::Parser::Instance();
                int id = parser->get_index();
                if (!parser->check_if_good()) return 0;
                dubl = town1->get_unit(id);
                if (dubl == 0) {
                    cout << "invalid id, try once more" << endl;
                } else { break; }
            }
        }
        return dubl;
    }
    void BattleHandler::battle_results (void) {
        if (!town1_init->evrbody_dead()) {
            cout << "town " << town1_init->GetTownId() << " has won the battle :)" << endl;
            cout << "******************************" << endl;
            town1->HeroResetMana();
            return;
        }
        if (!town2_init->evrbody_dead()) {
            cout << "town " << town2_init->GetTownId() << " has won the battle :)" << endl;
            cout << "******************************" << endl;
            town2->HeroResetMana();
            return;
        }
        cout << "Nobody has won. It's rather sad :(" << endl;
        cout << "******************************" << endl;
    }
    int BattleHandler::UnitStrike (void) {
        // returns 1 on successful attack, else 0;
        // attacks arm2 with arm1
        // So, this is evthg that a unit performs on it's turn
        shared_ptr<units::ProxyUnit> attacker = choose_attacker();
        if (attacker == 0) { return 0; }
        cout << "unit id" << attacker->GetId() << " turn: " << endl;
        if (attacker->IsFrozen()) {
            cout << "\t oops. I'm frozen." << endl;
            return 1;
        }
        coords::coord aim = choose_aim(attacker);
        if (aim == coords::coord()) { return 0; }
        cout << "\t aim: "<< aim << " distance: " << field->get_distance(attacker->GetLocation(), aim) << endl;
        string action = get_action(attacker, aim);
        if (action == "move") {
            vector<coords::coord> way = field->best_path(attacker, aim, "direct");
            if (way.size() == 0) {
                cout << "\t sorry. can't move there" << endl;
            } else {
                move_unit(way);
            }
        } else if (action == "attack") {
            attack(attacker, field->get_cell(aim));
        } else if (action == "move&attack") {
            if (mode == 0) {
                vector<coords::coord> way = field->best_path(attacker, aim, "nearest");
                if (way.size() == 0) {
                    cout << "\t sorry. can't move" << endl;
                } else {
                    if (way.size() > 1) {
                        move_unit(way);
                    }
                    if (field->get_distance(attacker->GetLocation(), aim) == 1) {
                        attack(attacker, field->get_cell(aim));
                    }
                }
            } else if (mode == 1) {
                vector<coords::coord> way = field->best_path(attacker, aim, "adjacent");
                if (way.size() == 0) {
                    cout << "\tsorry. can't move" << endl;
                } else {
                    move_unit(way);
                    attack(attacker, field->get_cell(aim));
                }
            }
        } else {
            return 0;
        }
        return 1;
    }
    int BattleHandler::HeroStrike (void) {
        // evthg that a hero performs on it's turn
        cout << "Hero " << town1->GetTownId() << " turn: ";
        int cmd = -1;
        string s = "";
        if (mode == 0) {
            shared_ptr<generators::RandomGen> rgen = generators::RandomGen::Instance();
            cmd = rgen->get_random(0, 4);
        } else {
            cout << "enter hero's attack method: ";
            shared_ptr<parsers::Parser> parser = parsers::Parser::Instance();
            s = parser->get_hero_method();
            if (!parser->check_if_good()) { return 0; }
        }
        if ( (mode == 1 && s == "freeze") || (mode == 0 && cmd == 0) ) {
            cout << "freeze" << endl;
            shared_ptr<units::ProxyUnit> attacked = get_attacked();
            if (attacked == 0) { return 1; }
            town1->HeroFreezeUnit(attacked);
            return 0;
        }
        if ( (mode == 1 && s == "fireball") || (mode == 0 && cmd == 1) ) {
            cout << "Fireball" << endl;
            shared_ptr<units::ProxyUnit> attacked = get_attacked();
            if (attacked == 0) { return 1; }
            vector<coords::coord> crds = field->get_neighbor(attacked->GetLocation());
            crds.push_back(attacked->GetLocation());
            vector<shared_ptr<units::ProxyUnit>> vctr;
            for (unsigned i = 0; i < crds.size(); i++) {
                if (!(field->is_empty(crds[i]))) {
                    vctr.push_back(field->get_cell(crds[i]));
                }
            }
            town1->HeroFireball(vctr);
            for (unsigned i = 0; i < vctr.size(); i++) {
                if (vctr[i]->IsDead()) {
                    town2->remove_from_battle_rang(vctr[i]);
                    field->reset_cell(vctr[i]->GetLocation());
                }
            }
            return 0;
        }
        if ( (mode == 1 && s == "dublicate") || (mode == 0 && cmd == 2) ) {
            cout << "dublicate" << endl;
            coords::coord new_cell = field->get_new_cell();
            if (new_cell == coords::coord()) {
                cout << "oops. There's no space" << endl;
                return 0;
            }
            shared_ptr<units::ProxyUnit> dubl = get_dubl();
            if (dubl == 0) { return 1; }
            town1->HeroDublicateUnit(dubl);
            dubl->SetLocation(new_cell);
            field->set_cell(new_cell, dubl);
            cout << "\t location setted to " << new_cell << endl;
            pair<shared_ptr<units::ProxyUnit>, int> par(dubl, DEF_LIFE_TIME);
            to_del.push_back(par);
            return 0;
        }
        if ( (mode == 1 && s == ".") || (mode == 0 && cmd == 3) ) {
            cout << "skip move" << endl;
            return 0;
        }
        return 0;
    }
    int BattleHandler::PerformRound (void) {
        // this function performes one round from the whole battle
        town1 = town1_init;
        town2 = town2_init;
        cout << "attacking army:" << endl;
        town1->init_battle_rang();
        cout << "attacked army:" << endl;
        town2->init_battle_rang();
        town1->HeroResetUsed();
        town2->HeroResetUsed();
        int res = 2, prev_res;
        while (true) {
            if (town2->evrbody_dead())
                return 0;
            prev_res = res;
            res = UnitStrike();
            _timer->tic_inc();
            if (town2->evrbody_dead()) {
                return 0;
            } else if (res == 0) {
                if (town1->HeroGetUsedStatus() == false) {
                    HeroStrike();
                    _timer->tic_inc();
                    town1->HeroSetUsed();
                    if (town2->evrbody_dead()) {
                        return 0;
                    }
                }
                if (prev_res == 0) {
                    cout << "---------- Round finished ----------" << endl;
                    return 1;
                }
            }
            swap(town1, town2);
        }
    }
    void BattleHandler::PerformBattle (void) {
        vector<shared_ptr<units::ProxyUnit>> warriors;
        warriors = town1_init->get_warriors();
        unsigned row, col;
        for (unsigned i = 0; i < warriors.size(); i++) {
            row = i % DEF_FIELD_ROWS;
            col = i / DEF_FIELD_ROWS;
            coords::coord crd(row, col);
            warriors[i]->SetLocation(crd);
            field->set_cell(crd, warriors[i]);
        }
        warriors = town2_init->get_warriors();
        for (unsigned i = 0; i < warriors.size(); i++) {
            row = i % DEF_FIELD_ROWS;
            col = DEF_FIELD_COLS - 1 - i / DEF_FIELD_ROWS;
            coords::coord crd(row, col);
            warriors[i]->SetLocation(crd);
            field->set_cell(crd, warriors[i]);
        }
        while(PerformRound()) {}
    }
    void BattleHandler::remove_all_dublicates (void) {
//        for (unsigned i = 0; i < to_del.size(); i++) {
//            shared_ptr<units::ProxyUnit> p = to_del[i].first;
//            if (p) {
//                p->DecHealth(-1);
//            }
//        }
//        for (unsigned i = 0; i < to_del.size(); i++) {
//            to_del.erase(to_del.begin() + i);
//        }
    }
    // Game's class realization
    void Game::create () {
        srand(int(time(0)));
        timer = shared_ptr<handlers::Timer> (new handlers::Timer);
        timer->tic_reset();
        battle_status = false;
        cout << "Now you can play HMM4. Enjoy! =) " << endl;
    }
    void Game::add_town (string type) {
        shared_ptr<towns::Town> t;
        if (type == "castle") {
            t = shared_ptr<towns::Town> (new towns::Castle);
            cout << "\t OK, you've created a Castle.";
        } else if (type == "tower") {
            t = shared_ptr<towns::Town> (new towns::Tower);
            cout << "\t OK, you've created a Tower.";
        } else if (type == "dungeon") {
            t = shared_ptr<towns::Town> (new towns::Dungeon);
            cout << "\t OK, you've created a Dungeon.";
        }
        all_towns.push_back(t);
        timer->add_town(all_towns[all_towns.size()-1].get());
        all_towns[all_towns.size()-1]->handler_set(timer.get());
        cout << " TownID: " << t->GetTownId() << endl;
    }
    void Game::add_unit () {
        if (all_towns.size() == 0) {
            cout << "can't create a new unit till there isn't any town" << endl;
            return;
        }
        shared_ptr<parsers::Parser> parser = parsers::Parser::Instance();
        string type = parser->get_unit_type();
        if (!parser->check_if_good()) return;
        unsigned town_idx;
        while (true) {
            town_idx = parser->get_index();
            if (!parser->check_if_good()) return;
            if (!(town_idx > 0 && town_idx <= all_towns.size())) {
                cout << "incorrect town index. try once more" << endl;
            } else { break; }
        }
        all_towns[town_idx-1]->new_unit(type, town_idx);
    }
    void Game::battle () {
        shared_ptr<parsers::Parser> parser = parsers::Parser::Instance();
        unsigned town1, town2;
        while (true) {
            town1 = parser->get_index();
            if (!parser->check_if_good()) return;
            if (town1 > all_towns.size())
                cout << "invalid town index to perform battle. Try once more" << endl;
            else break;
        }
        while (true) {
            town2 = parser->get_index();
            if (!parser->check_if_good()) return;
            if (town2 > all_towns.size())
                cout << "invalid town index to perform battle. Try once more" << endl;
            else break;
        }
        string s = parser->get_battle_mode();
        if (!parser->check_if_good()) return;
        bool mode = 0; // auto mode by default
        if (s == "manual") { mode = 1; }
        if (town1 == town2) {
            cout << "sorry. you can't attack town with itselt." << endl;
            return;
        }
        if (all_towns[town1-1]->evrbody_dead() || all_towns[town2-1]->evrbody_dead()) {
            cout << "sorry. At least one army is empty" << endl;
            return;
        }
        b_handler = shared_ptr<handlers::BattleHandler>
            (new handlers::BattleHandler(all_towns[town1-1], all_towns[town2-1], timer.get(), mode));
        cout << "\n---------- Battle started ----------" << endl;
        battle_status = true;
        timer->add_battle_handler(b_handler.get());
        b_handler->PerformBattle();
        end_battle();
    }
    void Game::end_battle () {
        if (battle_status) {
            b_handler->remove_all_dublicates();
            battle_status = false;
            cout << "---------- The battle is finished ----------" << endl;
            b_handler->battle_results();
            cout << "Total tics: " << timer->get_tic() << endl;
            timer->tic_reset();
            b_handler.reset();
        } else {
            cout << "There isn't any battle to end it" << endl;
        }
    }
    void Game::end_game () {
        if (battle_status) {
            end_battle();
        }
        cout << "game finished" << endl;
        cout << "That's it!" << endl;
    }
};
namespace commands {
    void CreateGameCommand::execute () { pgame->create(); }
    void AddTownCommand::execute () {
        shared_ptr<parsers::Parser> parser = parsers::Parser::Instance();
        string type = parser->get_town_type();
        if (parser->check_if_good()) {
            pgame->add_town(type);
        }
    }
    void AddUnitCommand::execute () { pgame->add_unit(); }
    void BattleStartCommand::execute () { pgame->battle(); }
    void BattleEndCommand::execute () { pgame->end_battle(); }
    void EndGameCommand::execute () { pgame->end_game(); }
};
