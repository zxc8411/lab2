//
// Created by Zeph on 4/1/2019.
//

#include "destler_doubloon.h"
#include <iomanip>

using std::cout;
using std::endl;

bool DestlerDoubloon::DDEBUG = false;

DestlerDoubloon::DestlerDoubloon(DestlerDoubloon &&other):
    id_(other.id()), value_(other.value()) {
    if(DDEBUG){
        cout << this << " moved from " << other << "!" << endl;
    }
}

DestlerDoubloon::DestlerDoubloon(unsigned long long id, double value):
    id_(id), value_(value) {
        if(DDEBUG){
            cout << this << " being created!" << endl;
        }
}

DestlerDoubloon::~DestlerDoubloon() {
    if(DDEBUG){
        cout << this << " being destroyed!" << endl;
    }
}

bool DestlerDoubloon::operator==(const DestlerDoubloon &other) const {
    return id_ == other.id_;
}

bool DestlerDoubloon::operator<(const DestlerDoubloon &other) const {
    return id_.to_ullong() < other.id();
}

unsigned long long DestlerDoubloon::id() const {
    return id_.to_ullong();
}

double DestlerDoubloon::value() const {
    return value_;
}

void DestlerDoubloon::increase_value(double value) {
    value_ += value;
}

void DestlerDoubloon::decrease_value(double value) {
    value_ -= value;
    if(value_ < 0){
        value_ = 0;
    }
}

std::ostream& operator<<(std::ostream &os, const DestlerDoubloon &dd) {
    os << "DD: 0x" << std::hex << std::setw(16) << std::uppercase << std::setfill('0') << dd.id_.to_ulong() << std::setfill(' ') << ", value: " << dd.value_;
    return os;
}