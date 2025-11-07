#pragma once

#ifndef __USTDINT_H__

// #define VISUALIZE_VALUE  // removed: now controlled by CMake option

#include <cstdint>
#include <string>
#ifdef VISUALIZE_VALUE
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <sstream>
#endif // VISUALIZE_VALUE

class int128_t {
public:
    // constructors
    int128_t() : high_(0), low_(0) {}
    int128_t(int64_t high, int64_t low) : high_(high), low_(static_cast<uint64_t>(low)) {}
    int128_t(int64_t val) : high_(val < 0 ? -1 : 0), low_(static_cast<uint64_t>(val)) {}
    int128_t(int val) : int128_t(int64_t(val)) {}

    // Constructor from string (supports decimal and hex with 0x prefix)
    // Removed explicit to allow int128_t a = "123";
    int128_t(const std::string& str) : high_(0), low_(0) {
        if (str.empty()) return;
        
        size_t start = 0;
        bool is_negative = false;
        
        // Check for negative sign
        if (str[0] == '-') {
            is_negative = true;
            start = 1;
        }
        
        // Check for hex prefix
        if (start + 1 < str.size() && str[start] == '0' && (str[start + 1] == 'x' || str[start + 1] == 'X')) {
            // Hexadecimal string
            parse_hex_string(str.substr(start + 2));
        } else {
            // Decimal string
            parse_dec_string(str.substr(start));
        }
        
        if (is_negative) {
            *this = int128_t(~high_, ~low_) + int128_t(0, 1);
        }
    }

    int128_t(std::nullptr_t) = delete;
    
    
    // Constructor from const char* for string literals
    int128_t(const char* str) : int128_t(str ? std::string(str) : std::string()) {}

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
    
    int128_t& operator=(const std::string& str) {
        *this = int128_t(str);
        return *this;
    }
    
    // Non-const char* overload to handle string literals
    int128_t& operator=(const char* str) {
        if (str) {
            *this = int128_t(std::string(str));
        }
        return *this;
    }

    bool operator==(const int128_t& other) const {
        return high_ == other.high_ && low_ == other.low_;
    }

    bool operator!=(const int128_t& other) const {
        return !(*this == other);
    }

    void print() const {        
        std::cout << high_ << " " << low_ << std::endl;
    }

#ifdef VISUALIZE_VALUE
    std::string to_hex_string() const {
        return to_hex64(static_cast<uint64_t>(high_)) + to_hex64(low_);
    }

    std::string to_dec_string() const {
        return hex_string_to_dec_string();
    }
#endif // VISUALIZE_VALUE

private:
    int64_t high_;
    uint64_t low_;

    // Multiply (hi:lo) by 10 and add a decimal digit [0..9], all with 64-bit ops
    static inline void mul10_add(uint64_t& hi, uint64_t& lo, uint32_t digit) {
        // lo * 10 = (lo<<3) + (lo<<1), track carries
        uint64_t lo2 = lo << 1;
        uint64_t lo8 = lo << 3;
        uint64_t c2  = lo >> 63;
        uint64_t c8  = lo >> 61;
        uint64_t lo10 = lo8 + lo2;
        uint64_t csum_lo = (lo10 < lo8) ? 1u : 0u;
        uint64_t carry_from_lo = c2 + c8 + csum_lo;

        // hi * 10 = (hi<<3) + (hi<<1), overflow beyond 128 bits is discarded
        uint64_t hi2 = hi << 1;
        uint64_t hi8 = hi << 3;
        uint64_t hi10 = hi8 + hi2;
        // add carry from low mult
        uint64_t hi_tmp = hi10 + carry_from_lo;

        // add digit to low
        uint64_t lo_new = lo10 + static_cast<uint64_t>(digit);
        uint64_t carry_d = (lo_new < lo10) ? 1u : 0u;

        hi = hi_tmp + carry_d;
        lo = lo_new;
    }

    void parse_dec_string(const std::string& str) {
        high_ = 0;
        low_ = 0;

        uint64_t hi = 0, lo = 0;
        for (char c : str) {
            if (c < '0' || c > '9') continue;
            uint32_t d = static_cast<uint32_t>(c - '0');
            mul10_add(hi, lo, d);
        }
        high_ = static_cast<int64_t>(hi);
        low_ = lo;
    }
    
    void parse_hex_string(const std::string& str) {
        high_ = 0;
        low_ = 0;
        
        for (char c : str) {
            int digit;
            if (c >= '0' && c <= '9') {
                digit = c - '0';
            } else if (c >= 'a' && c <= 'f') {
                digit = c - 'a' + 10;
            } else if (c >= 'A' && c <= 'F') {
                digit = c - 'A' + 10;
            } else {
                continue;
            }
            
            // Shift left by 4 bits
            uint64_t old_high = static_cast<uint64_t>(high_);
            high_ = static_cast<int64_t>((old_high << 4) | (low_ >> 60));
            low_ = (low_ << 4) | digit;
        }
    }

#ifdef VISUALIZE_VALUE

    std::string to_hex64(uint64_t value) const {
        std::stringstream ss;
        ss << std::hex << std::setfill('0') << std::setw(64) << value;
        return ss.str();
    }

    std::string hex_string_to_dec_string() const {
        // Handle zero
        if (high_ == 0 && low_ == 0) {
            return "0";
        }
        
        // Handle negative numbers
        bool is_negative = (high_ < 0);
        int128_t abs_val = *this;
        if (is_negative) {
            // Two's complement negation
            abs_val = int128_t(~high_, ~low_) + int128_t(0, 1);
        }
        
        // Convert to decimal using division by 10
        std::string result;
        int128_t temp = abs_val;
        
        while (!(temp.high_ == 0 && temp.low_ == 0)) {
            // Divide by 10 and get remainder
            uint64_t remainder = 0;
            
            // Divide high part
            uint64_t high_unsigned = static_cast<uint64_t>(temp.high_);
            uint64_t high_quotient = high_unsigned / 10;
            remainder = high_unsigned % 10;
            
            // Divide low part (with remainder from high)
            unsigned __int128 low_with_remainder = (static_cast<unsigned __int128>(remainder) << 64) | temp.low_;
            uint64_t low_quotient = static_cast<uint64_t>(low_with_remainder / 10);
            remainder = static_cast<uint64_t>(low_with_remainder % 10);
            
            result = char('0' + remainder) + result;
            temp = int128_t(static_cast<int64_t>(high_quotient), static_cast<int64_t>(low_quotient));
        }
        
        if (is_negative) {
            result = "-" + result;
        }
        
        return result;
    }
#endif // VISUALIZE_VALUE
};

#endif // __USTDINT_H__