# ustdint

Small repository that provides a minimal compatibility header offering a simple 128-bit signed integer wrapper and a tiny test program.

Contents
- `ustdint.h` — minimal `int128_t` class (high/low representation). Implements constructors, addition, subtraction, equality/inequality, assignment, and a simple `print()` method.
- `test_int128.cpp` — example/test program (build and run to see simple usage).
- `CMakeLists.txt` — build configuration.

Quick start (Linux)
1. Create a build directory and configure:
   ```
   mkdir -p build
   cd build
   cmake ..
   ```
2. Build:
   ```
   cmake --build .
   ```
3. Run the test binary (created in `build`):
   ```
   ./test_int128
   ```

How to use the header
- Add the header to your project:
  ```cpp
  #include "ustdint.h"
  ```
- Basic usage example:
  ```cpp
  int128_t a(1);            // represents 1
  int128_t b(-2);           // represents -2
  int128_t c = a + b;       // addition
  if (c != a) { c.print(); } // prints high and low parts
  ```
- `print()` outputs the internal `high` (signed) and `low` (unsigned) 64-bit parts, not a decimal 128-bit scalar. Use this for debugging internal state.

Limitations / notes
- This header is intentionally minimal and not a full replacement for a proper 128-bit integer library:
  - Only addition, subtraction, equality/inequality, and basic constructors are implemented.
  - No multiplication, division, bit shifts, or full ordering comparisons.
  - `print()` shows the two 64-bit words; it does not emit a single decimal 128-bit value.
- For production use requiring full 128-bit arithmetic consider:
  - Native compiler support (GCC/Clang `__int128`)
  - `boost::multiprecision::int128_t` or arbitrary-precision types in Boost.Multiprecision
- Header is self-contained and safe to include in C++ projects; adjust or extend it as needed.

Development
- Edit `ustdint.h` to add operations or conversion helpers.
- Use the provided `test_int128.cpp` as a starting point for unit tests or examples.
