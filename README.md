# M0110 Arduino Library for SAMD processors

Library implementing the Apple M0110(A) keyboard protocol to send keystrokes to a Macintosh 128K, Macintosh 512K, or Macintosh Plus.

## Usage

The library needs to be initialized with a data pin and clock pin.

```
Mac 128k/512k/Plus Keyboard Pinout (Socket on Computer)
.---------.
| 1 2 3 4 | 1 GND
|         | 2 Clock
|         | 3 Data
'---___---' 4 5V
```

Example:

```cpp
#include <M0110.h>

#define DAT_PIN 1
#define CLK_PIN 0

M0110 m0110(DAT_PIN, CLK_PIN);
```

## Functions

- `begin()`: allocates memory, configures pins, and enables interrupts
- `end()`: disables interrupts, releases pins, and frees memory
- `press(key)`: presses and holds a single key
- `print(characters)`: sends a string of keystrokes
- `println(characters)`: sends a string of keystrokes followed by a return key
- `release(key)` releases a single key
- `releaseAll()` releases all pressed keys
- `write(key)` presses and releases a single key
