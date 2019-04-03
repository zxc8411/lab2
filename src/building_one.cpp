//
// Created by Zeph on 4/2/2019.
//

#include "building_one.h"

BuildingOne::BuildingOne():
    rng_(), vault_(), withdrawn_ids_(){

}

void BuildingOne::bull_market(double value) {
    for(auto i : vault_){
        i.second.decrease_value(value);
    }
}

void BuildingOne::bear_market(double value) {
    for(auto i : vault_){
        i.second.increase_value(value);
    }
}

void BuildingOne::deposit(DestlerDoubloon &&dd) {
    vault_.emplace(make_pair(dd.id(), dd));
    //adjust wallet
    withdrawn_ids_.erase(dd.id());
}

void BuildingOne::destroy(unsigned long long id) {
    auto i = vault_.find(id);
    if(i != vault_.end()){
        vault_.erase(i);
    }else{
        throw DDException;
    }
}

const DestlerDoubloon& BuildingOne::doubloon(unsigned long long id) const {
    try{
        return vault_.at(id);
    }catch(std::out_of_range e){
        throw DDException;
    }
}

void BuildingOne::mint(unsigned int num_doubloons) {
    for(int i = 0; i < num_doubloons; i++){
        DestlerDoubloon dd(rng_.rand(),1);
        vault_.emplace(make_pair(dd.id(), dd));
    }
}

size_t BuildingOne::num_doubloons() const {
    return vault_.size();
}

double BuildingOne::total_worth() const {
    double val = 0;
    for(auto i : vault_){
        val += i.second.value();
    }
    return val;
}

DestlerDoubloon BuildingOne::withdraw(unsigned long long id) {
    //adjust wallet
    withdrawn_ids_.emplace(id);
    destroy(id);
}

std::ostream& operator<<(std::ostream &os, const BuildingOne &b1) {
    for(auto i : vault_){
        os << i.second << endl;
    }
    return os;
}