#include "the_loop.h"

#include <iomanip>

using std::cout;
using std::endl;
using std::flush;
using std::function;
using std::hex;
using std::ios;
using std::istream;
using std::move;
using std::streampos;
using std::string;
using std::ws;

TheLoop::TheLoop(istream& is, bool std_in) :
        in_{is},
        stdin_{std_in},
        b1_{},
        wallet_{},
        commands_{} {

    commands_["help"] = []() {
        cout <<
             "debug: toggle debugging of the doubloons\n" <<
             "deposit: deposit doubloon into building 1 vault (from front of wallet)\n" <<
             "doubloon id: list a doubloon in the building 1 vault by id\n" <<
             "fedora #: fedoras song: decrease value of all coins in vault by #\n" <<
             "gun id: prelude to gunbrella: destroy a coin in vault by id\n" <<
             "help: this help message\n" <<
             "quit: end program\n" <<
             "ritchie #: ritchie's lullaby: increase value of all coins in vault by #\n" <<
             "sos #: song of storms: mint # coins\n" <<
             "vault: display all doubloons in building 1 vault\n" <<
             "wallet: display all doubloons in wallet\n" <<
             "withdraw id: withdraw a doubloon from building 1 vault (into back of wallet)\n";
    };

    commands_["debug"] = []() {
        DestlerDoubloon::DDEBUG = !DestlerDoubloon::DDEBUG;
    };

    commands_["deposit"] = [this](){
        if(wallet_.size() != 0){
            b1_.deposit(std::move(wallet_.front()));
            wallet_.pop_front();
        }else{
            cout << "Wallet is empty!\n";
        }
    };

    commands_["doubloon"] = [this](){
        unsigned long long id;
        in_ >> std::hex >> id;
        try{
            cout << b1_.doubloon(id) << "\n";
        }catch(DDException e){
            cout << e.what() << "\n";
        }
    };

    commands_["fedora"] = [this](){
        string num;
        in_ >> num;
        b1_.bear_market(std::stod(num));
    };

    commands_["gun"] = [this](){
        unsigned long long id;
        in_ >> std::hex >> id;
        try{
            b1_.destroy(id);
        }catch(DDException e){
            cout << e.what() << "\n";
        }

    };

    commands_["ritchie"] = [this](){
        string num;
        in_ >> num;
        b1_.bull_market(std::stod(num));
    };

    commands_["sos"] = [this](){
        string num;
        in_ >> num;
        b1_.mint(std::stoul(num));
    };

    commands_["vault"] = [this](){
        cout << b1_;
    };

    commands_["wallet"] = [this](){
        for(auto i = wallet_.begin(); i != wallet_.end(); i++){
            cout << *i << "\n";
        }
    };

    commands_["withdraw"] = [this](){
        unsigned long long id;
        in_ >> std::hex >> id;
        try{
            wallet_.push_back(b1_.withdraw(id));
        }catch(DDException e){
            cout << e.what() << "\n";
        }

    };

}

void TheLoop::main_loop() {
    string cmd;
    while (true) {
        if (stdin_) {
            cout << "$ " << flush;
        } else {
            // do this stream manipulation so we can print the entire line
            // if the command is coming from a file, but we want to reset
            // it before the command is read in later
            in_ >> ws;
            streampos pos{in_.tellg()};
            string line{};
            getline(in_, line);
            cout << "$ " << line << endl;
            in_.seekg(pos);
        }

        in_ >> cmd;
        if (cmd == "quit") {
            break;
        } else if (commands_.count(cmd) == 1) {
            commands_[cmd]();
        } else {
            cout << "unrecognized command!" << endl;
            // consume rest of line
            string tmp;
            getline(in_, tmp);
        }
    }


    // move any Doubloons in wallet back to the vault
    cout << "returning doubloons in wallet to bank!\n";
    while(wallet_.size() != 0){
        b1_.deposit(std::move(wallet_.front()));
        wallet_.pop_front();
    }
    //display the vault statistics
    cout << "doubloons in vault: " << b1_.num_doubloons() << "\n";
    cout << "total worth: " << b1_.total_worth() << endl;
}
