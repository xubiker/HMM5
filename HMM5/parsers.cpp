#include "parsers.h"

namespace parsers {
    shared_ptr<Parser> Parser::Instance () {
        if (!_self) {
            _self = shared_ptr<Parser> (new Parser());
        }
        return _self;
    }
    string Parser::get_command (void) {
        good_input = false;
        string str;
        while (cin >> str) {
            if ( str == "town" || str == "unit" || str == "battle" || str == "end" ) {
                good_input = true;
                return str;
            } else {
                cout << "invalid comand. try once more" << endl;
            }
        }
        return "";
    }
    string Parser::get_unit_type (void) {
        good_input = false;
        string str;
        while (cin >> str) {
            if ( str == "shooter" || str == "rider" || str == "wizard" || str == "dragon" ) {
                good_input = true;
                return str;
            } else {
                cout << "invalid unit type. try once more" << endl;
            }
        }
        return "";
    }
    string Parser::get_hero_method (void) {
        good_input = false;
        string str;
        while (cin >> str) {
            if ( str == "freeze" || str == "fireball" || str == "dublicate" || str == "." ) {
                good_input = true;
                return str;
            } else {
                cout << "invalid hero's method. try once more" << endl;
            }
        }
        return "";
    }
    string Parser::get_battle_mode (void) {
        good_input = false;
        string str;
        while (cin >> str) {
            if ( str == "auto" || str == "manual" ) {
                good_input = true;
                return str;
            } else {
                cout << "invalid battle mode. try once more" << endl;
            }
        }
        return "";
    }
    string Parser::get_town_type (void) {
        good_input = false;
        string str;
        while (cin >> str) {
            if ( str == "castle" || str == "1" ) {
                good_input = true;
                return "castle";
            } else if ( str == "tower" || str == "2" ) {
                good_input = true;
                return "tower";
            } else if ( str == "dungeon" || str == "3" ) {
                good_input = true;
                return "dungeon";
            } else {
                cout << "invalid town type. try once more" << endl;
            }
        }
        return "";
    }
    unsigned Parser::get_index (void) {
        good_input = false;
        string s;
        while (cin >> s) {
            int x = atoi(s.c_str());
            if (x) {
                good_input = true;
                return x;
            } else {
                cout << "invalid index. try once more" << endl;
            }
        }
        return 0;
    }
    coords::coord Parser::get_coords (void) {
        good_input = false;
        string s;
        unsigned row = -1, col = -1;
        while (cin >> s) {
            bool flg = true;
            for (unsigned i = 0; i < s.size(); i++) {
                if (s[i] < '0' || s[i] > '9') {
                    cout << "invalid coordinate. try once more" << endl;
                    flg = false;
                    break;
                }
            }
            if (flg) {
                row = atoi(s.c_str());
                break;
            }
        }
        while (cin >> s) {
            bool flg = true;
            for (unsigned i = 0; i < s.size(); i++) {
                if (s[i] < '0' || s[i] > '9') {
                    cout << "invalid coordinate. try once more" << endl;
                    flg = false;
                    break;
                }
            }
            if (flg) {
                col = atoi(s.c_str());
                good_input = true;
                return coords::coord(row, col);
            }
        }
        return coords::coord(-1, -1);
    }
    bool Parser::check_if_good (void) { return good_input; }
    shared_ptr<Parser> Parser::_self = 0;
    bool Parser::good_input = false;
};
