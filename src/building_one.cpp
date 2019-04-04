//
// Created by Zeph on 4/2/2019.
//

#include "building_one.h"
#include <iomanip>

BuildingOne::BuildingOne():
    rng_(SEED), vault_(), withdrawn_ids_(){

}

void BuildingOne::bull_market(double value) {
    for(auto &i : vault_){
        i.second.increase_value(value);
    }
}

void BuildingOne::bear_market(double value) {
    for(auto &i : vault_){
        i.second.decrease_value(value);
    }
}

void BuildingOne::deposit(DestlerDoubloon &&dd) {
    vault_.emplace(std::make_pair(dd.id(), std::move(dd)));
    withdrawn_ids_.erase(dd.id());
}

void BuildingOne::destroy(unsigned long long id) {
    auto i = vault_.find(id);
    if(i != vault_.end()){
        vault_.erase(i);
    }else{
        std::stringstream ss;
        ss << std::hex << std::setw(16) << std::uppercase << std::setfill('0') << id << std::setfill(' ');
        throw DDException("DD: 0x" + ss.str() + " not in vault!");
    }
}

const DestlerDoubloon& BuildingOne::doubloon(unsigned long long id) const {
    try{
        return vault_.at(id);
    }catch(std::out_of_range e){
        std::stringstream ss;
        ss << std::hex << std::setw(16) << std::uppercase << std::setfill('0') << id << std::setfill(' ');
        throw DDException("DD: 0x" + ss.str() + " not in vault!");
    }
}

void BuildingOne::mint(unsigned int num_doubloons) {
    for(unsigned int i = 0; i < num_doubloons; i++){
        unsigned long long id = rng_.rand();
        vault_.emplace(id, DestlerDoubloon(id,1));
    }
}

size_t BuildingOne::num_doubloons() const {
    return vault_.size();
}

double BuildingOne::total_worth() const {
    double val = 0;
    for(auto &i : vault_){
        val += i.second.value();
    }
    return val;
}

DestlerDoubloon BuildingOne::withdraw(unsigned long long id) {
    if(vault_.size() == 0){
        throw DDException("the vault is empty!");
    }
    withdrawn_ids_.emplace(id);
    try{
        DestlerDoubloon dd = std::move(vault_.at(id));
        destroy(id);
        return dd;
    }catch(std::out_of_range e){
        std::stringstream ss;
        ss << std::hex << std::setw(16) << std::uppercase << std::setfill('0') << id << std::setfill(' ');
        throw DDException("DD: 0x" + ss.str() + " not in vault!");
    }
}

std::ostream& operator<<(std::ostream &os, const BuildingOne &b1) {
    for(auto &i : b1.vault_){
        os << i.second << "\n";
    }
    return os;
}