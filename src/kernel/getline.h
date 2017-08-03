// Thu Aug  3 18:43:18 UTC 2017
// 4735-b0e-05-

#include <Arduino.h>
#include "../../yaffa.h"

extern char getKey(void);
// dl_interpreter.h:extern uint8_t noInterpreter; // = FALSE ;

extern uint8_t noInterpreter; // = FALSE ;

extern char getDLKey(void);

extern uint8_t getLine(char* ptr, uint8_t buffSize);

extern uint8_t flags;                 // Internal Flags

extern uint8_t spiFlashReading; // = -1; // TRUE  // Adafruit SPI flash: reading
extern uint8_t spiFlashWaiting; // visit ainsuforthsketch.cpp or similar-named file
extern uint8_t fileClosed; // visit ainsuforthsketch.cpp or similar-named file

extern void setup_spi_flash(void);


// Wed Aug  2 01:22:51 UTC 2017
// 4735-b0c-03-

// #include <Arduino.h>
// #include "../../yaffa.h"

extern void ascii_xfer_setup_spi_flash(void);

extern void create_test_directory(void);

extern void write_a_test_file(void);

#ifndef HAS_STANDARD_BUILD_HERE
extern void read_a_test_file(void);
#endif

#ifndef HAS_STANDARD_BUILD_HERE
extern void read_from_code_py_file(void);
#endif

extern void tail_code_bb(void);

extern void ascii_xfer_spi_flash_main(void);
// extern void loop_flash_main(void);
extern char* cpSource;                 // Pointer to the string location that we will

