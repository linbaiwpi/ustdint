#pragma once

#ifndef __USTDINT_H__

#include <cstdint>
#include <string>
#include <iostream>

class int128_t {
public:
    // constructors
    int128_t() : high_(0), low_(0) {}
    int128_t(int64_t high, int64_t low) : high_(high), low_(static_cast<uint64_t>(low)) {}
    int128_t(int64_t val) : high_(val < 0 ? -1 : 0), low_(static_cast<uint64_t>(val)) {}
    
    // Copy constructor
    int128_t(const int128_t& other) = default;
    
    // Move constructor (optional, for completeness)
    int128_t(int128_t&& other) noexcept = default;

    // operators
    int128_t operator+(int128_t other) const {
        uint64_t new_low = low_ + other.low_;
        int64_t new_high = high_ + other.high_;
        
        // Check for carry: if sum is less than either operand, there was a carry
        if (new_low < low_) {
            new_high++;
        }
        
        return int128_t(new_high, static_cast<int64_t>(new_low));
    }

    int128_t operator-(int128_t other) const {
        uint64_t new_low = low_ - other.low_;
        int64_t new_high = high_ - other.high_;
        
        // Check for borrow from high to low
        if (low_ < other.low_) {
            new_high--;
        }
        
        return int128_t(new_high, static_cast<int64_t>(new_low));
    }

    int128_t& operator=(const int128_t& other) = default;
    
    // Move assignment (optional, for completeness)
    int128_t& operator=(int128_t&& other) noexcept = default;

    bool operator==(const int128_t& other) const {
        return high_ == other.high_ && low_ == other.low_;
    }

    bool operator!=(const int128_t& other) const {
        return !(*this == other);
    }

    void print() const {        
        std::cout << high_ << " " << low_ << std::endl;
    }


private:
    int64_t high_;
    uint64_t low_;
};

#endif // __USTDINT_H__