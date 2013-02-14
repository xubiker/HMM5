#include <string>
#include <memory>
#include "handlers.h"
#include "parsers.h"
#include "defines.h"
//#include "vld.h"

int h_m_m () {
    shared_ptr<handlers::Game> game (new handlers::Game);
    shared_ptr<commands::Command> cmd;
    cmd = shared_ptr<commands::Command> (new commands::CreateGameCommand(game));
    cmd->execute();

    shared_ptr<parsers::Parser> parser = parsers::Parser::Instance();
    string com = "init";
    while (com != "") {
        com = parser->get_command();
        if (!parser->check_if_good()) { break; }
        if (com == "town") {
            cmd = shared_ptr<commands::Command> (new commands::AddTownCommand(game));
            cmd->execute();
        } else if (com == "unit") {
            cmd = shared_ptr<commands::Command> (new commands::AddUnitCommand(game));
            cmd->execute();
        } else if (com == "battle") {
            cmd = shared_ptr<commands::Command> (new commands::BattleStartCommand(game));
            cmd->execute();
        } else if (com == "end") {
            break;
        }
    }
    cmd = shared_ptr<commands::Command> (new commands::EndGameCommand(game));
    cmd->execute();
    return 0;
}
int main() {
    h_m_m();
#ifndef ejudge_compile
    system("pause");
#endif
    return 0;
}