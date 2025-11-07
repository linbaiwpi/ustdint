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

## Optional string visualization

The helper methods are controlled by a CMake option:

- `std::string to_hex_string()` — 128-bit hex (zero-padded high 64 + low 64).
- `std::string to_dec_string()` — signed decimal string.

Enable at configure time:
```
cmake -S . -B build -DVISUALIZE_VALUE=ON
cmake --build build
```

Disable (default is OFF for lighter code):
```
cmake -S . -B build -DVISUALIZE_VALUE=OFF
```

### Interactive selection of VISUALIZE_VALUE
You can toggle the option interactively in either of the following ways:

- ccmake (curses UI):
  ```
  mkdir -p build && cd build
  ccmake ..
  # press 'c' to configure, toggle VISUALIZE_VALUE, then 'g' to generate
  cmake --build .
  ```

- cmake-gui:
  ```
  cmake-gui .
  # search VISUALIZE_VALUE, toggle ON/OFF, Configure -> Generate -> build
  ```

- Provided script:
  ```
  chmod +x tools/choose_visualize.sh
  ./tools/choose_visualize.sh            # prompts ON/OFF and builds optionally
  ./tools/choose_visualize.sh build-rel  # custom build dir
  ```

You can still set it non-interactively:
```
cmake -S . -B build -DVISUALIZE_VALUE=ON
cmake --build build
```

### Configure-time hints
CMake prints helper hints after configuration:
- When in Debug: suggests `-DCMAKE_BUILD_TYPE=Release`
- Shows current VISUALIZE_VALUE state and how to toggle:
  - If OFF: reconfigure with `-DVISUALIZE_VALUE=ON`
  - If ON: reconfigure with `-DVISUALIZE_VALUE=OFF`

Example:
```
-- ====================================== HINT ======================================
-- configure with -DCMAKE_BUILD_TYPE=Release for optimized build
-- configure with -DVISUALIZE_VALUE=ON for visuliazing values in string
-- ==================================================================================
```

### Helper script
An interactive helper is provided at `tools/choose_visualize.sh`:
```
chmod +x tools/choose_visualize.sh
./tools/choose_visualize.sh          # choose ON/OFF, optional build
./tools/choose_visualize.sh buildRel # custom build directory
```

Example (when enabled):
```cpp
#include "ustdint.h"

int main() {
    int128_t a("123456789012345678901234567890");
    std::cout << a.to_hex_string() << "\n";
    std::cout << a.to_dec_string() << "\n";
}
```

Without `VISUALIZE_VALUE`, these methods are not compiled and calls will fail to compile.

## String construction

Supports:
```cpp
int128_t a = "123456789012345678901234567890";
int128_t b("0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");
```

Hex parses up to 128 bits; excess high bits are discarded.

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
