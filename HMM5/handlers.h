#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include "towns.h"
#include "defines.h"
#include "fields.h"

using namespace std;

namespace handlers {
    class BattleHandler;
    class _Timer {
        // class controlling a timer and uid generator
        protected:
            int tic;
            vector<towns::Town*> towns;
            BattleHandler* bh;
        public:
#ifdef ejudge_compile
            _Timer (const _Timer&) = delete;
            _Timer& operator = (const _Timer&) = delete;
#endif
            virtual void tic_reset (void) = 0;
            virtual void add_town (towns::Town*) = 0;
            virtual void add_battle_handler (BattleHandler* b) = 0;
            virtual void tic_inc (int x = 1) = 0;
            virtual int get_tic (void) = 0;
            _Timer() : tic(0), towns(), bh(0) {};
            virtual ~_Timer() {};
    };
    class Timer : public _Timer {
        // Handler's specification to control the gaming process
        public:
            void tic_reset (void);
            void add_town (towns::Town* t);
            void add_battle_handler (BattleHandler* b);
            void tic_inc (int x = 1);
            int get_tic (void);
            ~Timer ();
    };
    class BattleHandler {
            shared_ptr<fields::Field> field;
            shared_ptr<towns::Town> town1_init;
            shared_ptr<towns::Town> town2_init;
            shared_ptr<towns::Town> town1;
            shared_ptr<towns::Town> town2;
            // all units that will be deleted in the nearest time
            vector< pair<shared_ptr<units::ProxyUnit>, int> > to_del;
            _Timer* _timer;
            bool mode; // 0 - auto, 1 - manual;
        public:
#ifdef ejudge_compile
            BattleHandler (const BattleHandler&) = delete;
            BattleHandler& operator= (const BattleHandler&) = delete;
#endif
            void tic_time (void);
            shared_ptr<units::ProxyUnit> choose_attacker (void);
            shared_ptr<units::ProxyUnit> get_attacked (void);
            void move_unit (coords::coord, coords::coord);
            void move_unit (vector<coords::coord>);
            shared_ptr<units::ProxyUnit> get_nearest_enemy (shared_ptr<units::ProxyUnit>);
            string get_action (shared_ptr<units::ProxyUnit>, coords::coord);
            coords::coord choose_aim (shared_ptr<units::ProxyUnit>);
            void attack (shared_ptr<units::ProxyUnit>, shared_ptr<units::ProxyUnit>);
            shared_ptr<units::ProxyUnit> get_dubl (void);
            void battle_results (void);
            int UnitStrike (void);
            int HeroStrike (void);
            int PerformRound (void);
            void PerformBattle (void);
            void remove_all_dublicates (void);
            BattleHandler(shared_ptr<towns::Town> t1,
                          shared_ptr<towns::Town> t2, _Timer* tmr, bool m)
                : field(0), town1_init(t1), town2_init(t2), town1(0), town2(0), to_del(0), _timer(tmr), mode(m) {
                field = shared_ptr<fields::Field> (new fields::Field(DEF_FIELD_ROWS, DEF_FIELD_COLS));
            };
            ~BattleHandler ();

    };
    class Game {
        // used to create, process and end the game
        private:
            vector<shared_ptr<towns::Town>> all_towns;
            shared_ptr<handlers::_Timer> timer;
            shared_ptr<handlers::BattleHandler> b_handler;
            bool battle_status;
        public:
            Game () : all_towns(0), timer(0), b_handler(0), battle_status(0) {};
#ifdef ejudge_compile
            Game (const Game&) = delete;
            Game& operator= (const Game&) = delete;
#endif
            void create ();
            void add_town (string);
            void add_unit ();
            void battle ();
            void end_battle ();
            void attack (int, int);
            void end_game ();
    };
};
namespace commands {
    class Command {     // abstract command class
        public:
            virtual ~Command() {}
            virtual void execute() = 0;
        protected:
            Command (shared_ptr<handlers::Game> p): pgame(p) {}
    #ifdef ejudge_compile
            Command (const Command&) = delete;
            Command& operator= (const Command&) = delete;
    #endif
            shared_ptr<handlers::Game> pgame;
    };
    class CreateGameCommand: public Command {
        public:
            CreateGameCommand (shared_ptr<handlers::Game> p) : Command(p) {}
            void execute();
    };
    class AddTownCommand: public Command {
        public:
            AddTownCommand (shared_ptr<handlers::Game> p) : Command( p) {}
            void execute();
    };
    class AddUnitCommand: public Command {
        public:
            AddUnitCommand (shared_ptr<handlers::Game> p) : Command( p) {}
            void execute();
    };
    class BattleStartCommand: public Command {
        public:
            BattleStartCommand (shared_ptr<handlers::Game> p) : Command( p) {}
            void execute();
    };
    class BattleEndCommand: public Command {
        public:
            BattleEndCommand (shared_ptr<handlers::Game> p) : Command( p) {}
            void execute();
    };
    class EndGameCommand: public Command {
        public:
            EndGameCommand (shared_ptr<handlers::Game> p) : Command( p) {}
            void execute();
    };
};
