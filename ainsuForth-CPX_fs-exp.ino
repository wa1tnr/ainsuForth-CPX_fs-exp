// Fri Nov 24 05:23:48 UTC 2017
// 4735-b0c-09a-   the -09x- is new Nov 24, 2017.

// Use cpp macro to name the file read or written to SPI flashROM.


// MAKRIS   KETFILET   TAKKEV

// previous timestamp:

// Fri Nov 24 04:49:08 UTC 2017
// 4735-b0c-07z-   the -07x- is new Nov 19, 2017.

// Fri Nov 24 02:42:54 UTC 2017
// Thu Aug  3 19:07:55 UTC 2017
// 4735-b0f-00-

// MULAMOTTE   HELEGRIDAK   03 Aug 2017  19:07z  -  the echo! word controls some silences

// KELDRIN  NOKURVID

// next: suppress echo during a file download.  Hmm.  It's suppressed when 'load' is run.

// NOTE: it is a 'download' because one is logged into the SAMD21 board,
// via USB and a terminal, and issues the command to 'download' from
// the SPI flashROM (a peripheral device) to the internal RAM of the SAMD21 chip
// (and stored there in RAM, compiled such that the virtual machine can
// execute this newly-stored program).
// 
// In that sense, 'compile' could also be a reasonable name for this same word.


// GNAGA


// --------------------------------------------------
// --------------------------------------------------
// ---                                  -------------
// ---   Thu Aug  3 21:42:35 UTC 2017   -------------
// ---                                  -------------
// --------------------------------------------------
// --------------------------------------------------
// SUCCESS.  Wrote a download file, then read it in, using the 'download' and then the 'load' words.  w00t.
// --------------------------------------------------
// --------------------------------------------------
// --------------------------------------------------

// BUG:  captures the '\end.' word to SPI flash file.  Omit in future revision.



// MIPIXIC  CESSOME  RALDOXI  15:43z Thu 03 Aug 2017

// parser looks (provisionally) functional for whole-line echo to capture file.




// KELKANNE  GRIAMO  FISPARD 

// file echo complete.  Needs: parsing (or 'de-parsing' -- for whole-line repeat).

// partial: input line is echoed, and only once, in preparation for file capture to SPI flash as a file.fs or file.txt &c.


// the 'see' word was reinstated.  Appears to function well.  2 July 22:44z

// several words are now untested. 2 July 22:45z

// ++store_fetch.cpp
// ++dict_entries.cpp
// major housekeeping: dict_entries.h





// quite a bit of juggling.  Mistakes made?

// ------------------------------------------------------------------------
//   reserved.  messages go here.
// ------------------------------------------------------------------------

// TODO: test c! and 2drop as well as allot and variable

// Meanwhile:

// +max_min.cpp   max  min
// +char.cpp  char  [char]
// +do_loop.cpp
// +src/periph/neo_pixel.cpp
// +cblink.cpp

// , 2drop c! - three words added to dictionary.

// PRIMITIVE FORTH - working 18 June  -  HABILLEAUX   CHOGUFE   STELDONE   PARKEFF

/******************************************************************************/
/**  YAFFA - Yet Another Forth for Arduino                                   **/
/**  Version 0.7.0                                                           **/
/**                                                                          **/
/**  File: YAFFA.ino                                                         **/
/**  Copyright (C) 2012 Stuart Wood (swood@rochester.rr.com)                 **/
/**                                                                          **/
/**  This file is part of YAFFA.                                             **/
/**                                                                          **/
/**  YAFFA is free software: you can redistribute it and/or modify           **/
/**  it under the terms of the GNU General Public License as published by    **/
/**  the Free Software Foundation, either version 2 of the License, or       **/
/**  (at your option) any later version.                                     **/
/**                                                                          **/
/**  YAFFA is distributed in the hope that it will be useful,                **/
/**  but WITHOUT ANY WARRANTY; without even the implied warranty of          **/
/**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           **/
/**  GNU General Public License for more details.                            **/
/**                                                                          **/
/**  You should have received a copy of the GNU General Public License       **/
/**  along with YAFFA.  If not, see <http://www.gnu.org/licenses/>.          **/
/**                                                                          **/
/******************************************************************************/
/**                                                                          **/
/**  DESCRIPTION:                                                            **/
/**                                                                          **/
/**  YAFFA is an attempt to make a Forth environment for the Arduino that    **/
/**  is as close as possible to the ANSI Forth draft specification DPANS94.  **/
/**                                                                          **/
/**  The goal is to support at a minimum the ANS Forth C core word set and   **/
/**  to implement wrappers for the basic I/O functions found in the Arduino  **/
/**  library.                                                                **/
/**  YAFFA uses two dictionaries, one for built in words and is stored in    **/
/**  flash memory, and the other for user defined words, that is found in    **/
/**  RAM.                                                                    **/
/**                                                                          **/
/******************************************************************************/
/**                                                                          **/
/**  REVISION HISTORY:                                                       **/
/**                                                                          **/
/**    0.7.0                                                                 **/
/**    - Fixed the how LEAVE is handled in LOOP and +LOOP.                   **/
/**    0.6.2                                                                 **/
/**    - Added words ">NUMBER", "KEY?", ".(", "0<>", "0>", "2>R", "2R>",     **/
/**      "2R@".                                                              **/
/**    - Removed static from the function headers to avoid compilation       **/
/**      errors with the new 1.6.6 Arduino IDE.                              **/
/**                                                                          **/
/**                                                                          **/
/** 2017: changed to yaffa.h -- reverting possibly what is mentioned         **/
/**       on the lines, below this one.                                      **/
/**                                                                          **/
/**                                                                          **/
/**    - changed file names from yaffa.h to YAFFA.h and Yaffa.ino to         **/
/**      YAFFA.ino and the #includes to reflect the capatilized name. This   **/
/**      helps with cheking out the project from github without renaming     **/
/**      files.                                                              **/
/**                                                                          **/
/**                                                                          **/
/**    - Fixed comments for pinWrite and pinMode.                            **/
/**    - yaffa.h reorganized for different architectures                     **/
/**    - Replaced Serial.print(PSTR()) with Serial.print(F())                **/
/**    0.6.1                                                                 **/
/**    - Documentation cleanup. thanks to Dr. Hugh Sasse, BSc(Hons), PhD     **/
/**    0.6                                                                   **/
/**    - Fixed PROGMEM compilation errors do to new compiler in Arduino 1.6  **/
/**    - Embedded the revision in to the compiled code.                      **/
/**    - Revision is now displayed in greeting at start up.                  **/
/**    - the interpreter not clears the word flags before it starts.         **/
/**    - Updated TICK, WORD, and FIND to make use of primitive calls for to  **/
/**      reduce code size.                                                   **/
/**    - Added word flag checks in dot_quote() and _s_quote().               **/
/**                                                                          **/
/**  NOTES:                                                                  **/
/**                                                                          **/
/**    - Compiler now gives "Low memory available, stability problems may    **/
/**      occur." warning. This is expected since most memory is reserved for **/
/**      the FORTH environment. Excessive recursive calls may overrun the C  **/
/**      stack.                                                              **/
/**                                                                          **/
/**  THINGS TO DO:                                                           **/
/**                                                                          **/
/**  CORE WORDS TO ADD:                                                      **/
/**      >NUMBER                                                             **/
/**                                                                          **/
/**  THINGS TO FIX:                                                          **/
/**                                                                          **/
/**    Fix the outer interpreter to use FIND instead of isWord               **/
/**    Fix Serial.Print(w, HEX) from displaying negative numbers as 32 bits  **/
/**    Fix ENVIRONMENT? Query to take a string reference from the stack.     **/
/**                                                                          **/
/******************************************************************************/


