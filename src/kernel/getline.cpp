// Thu Aug  3 19:07:55 UTC 2017
// 4735-b0f-00-

#include <Arduino.h>
#include "../../yaffa.h"
// #include "../../Error_Codes.h"

#ifdef EXT_KERN_GETLINE
#include "getline.h"

#include <SPI.h>
#include <Adafruit_SPIFlash.h>
#include <Adafruit_SPIFlash_FatFs.h>

#define FLASH_TYPE     SPIFLASHTYPE_W25Q16BV  // Flash chip type.
#define FLASH_SS       SS                    // Flash chip SS pin.
#define FLASH_SPI_PORT SPI                   // What SPI port is Flash on?

Adafruit_SPIFlash flash(FLASH_SS, &FLASH_SPI_PORT);     // Use hardware SPI 

Adafruit_W25Q16BV_FatFs fatfs(flash);

/******************************************************************************/
/** getDLKey                                                                 **/
/**   waits for the next valid key to be entered in download mode (DL)       **/
/**   and return its value                                                   **/
/**   Watches for code-escape key '\' (backslash) to detect the '\end.' word **/
/**                                                                          **/
/**   Valid characters are:  Backspace, Carriage Return (0x0d), Escape,      **/
/**   Tab, Newline (0x0a) and standard (printable) characters                **/
/******************************************************************************/

void setup_spi_flash(void) {
  // Serial.println("SPI Flash - reading");
  if (!flash.begin(FLASH_TYPE)) {
    Serial.println("E: flash chip init fault.");
    while(1);
  }
  // Serial.print("Flash chip JEDEC ID: 0x"); Serial.println(flash.GetJEDECID(), HEX);

  if (!fatfs.begin()) {
    Serial.println("E: fatfs.begin() fault.");
    // Serial.println("Was the flash chip formatted with the fatfs_format example?");
    while(1);
  }
  // Serial.println("fatfs is mounted.");
}


/******************************************************************************/
/** getLine                                                                  **/
/**   read in a line of text ended by a Carriage Return (ASCII 13)           **/
/**   Valid characters are:  Backspace, Carriage Return, Escape, Tab, and    **/
/**   standard printable characters. Passed the address to store the string, **/
/**   and Returns the length of the string stored                            **/
/******************************************************************************/

File thisFile;

uint8_t getLine(char* ptr, uint8_t buffSize) {
  char inChar;
  uint8_t count = 0;

  // if (fileClosed) { Serial.println("Indeed, fileClosed is TRUE"); }

  if (spiFlashReading) {
      // cheap_test: if (fatfs.exists("/forth/job.fs")) {
      if (fatfs.exists("/forth/ascii_xfer_test.txt")) {
          if (fileClosed) {
            // Serial.println("(re)opening fatfs .. verify it is okay to do so.");

            // cheap_test: File forthSrcFile = fatfs.open("/forth/job.fs",             FILE_READ);
            File forthSrcFile = fatfs.open("/forth/ascii_xfer_test.txt", FILE_READ);
            thisFile = (File) forthSrcFile;
            fileClosed = FALSE ; // it is open, now.
          }
      }
  }


// another getLine() stanza:
  do {
    if (spiFlashReading) {
        if (thisFile) {
            if (thisFile.available()) {
             // this is where every character of job.fs is read from SPI flash:
                inChar = thisFile.read();

                spiFlashWaiting = FALSE;
                if (thisFile.available()) {
                    spiFlashWaiting = TRUE;
                } else {
                    fileClosed = TRUE; // want to close the file - set a flag
                    spiFlashReading = FALSE; // shutdown the reading mechanism
                }
            }
        }

    } else {

        if (noInterpreter) {
            inChar = getKey();
            if (inChar == '\\') {
                // Serial.print("ESC \\ SEEN in getLine().\r\n");
            }
        } else {
            inChar = getKey(); 
        }
    }

    // inChar is now populated; either by keypress or by byte stored in SPI flash.

    if (inChar == ASCII_BS || inChar == ASCII_DEL) {  // new: was only ASCII_BS
       if (count) {
         *--ptr = 0;
         count--; // ainsuForth improvement -- backspace behavior


if (silentReading) { } else {
    if (flags & ECHO_ON) Serial.print("\b \b");
}



      }
    }
    else if (inChar == ASCII_TAB || inChar == ASCII_ESC) {


if (silentReading) { } else {
      if (flags & ECHO_ON) Serial.print("\a");
}




    }
    else if (inChar == ASCII_CR || inChar == ASCII_NL) { // ainsuForth improvement

                          // iirc, this was for use during colon definition upload,
                          // using copy and paste into the serial terminal.

                          // Basis: lack of newline sensitivity; added some.

                          // Test: revert, attempt a multi-line code paste.  If
                          // it fails or behaves poorly, reenable this code.
                          // Compare the two behaviors.

                          // iirc, interactive typing to the interpreter masks
                          // this behavior, whereas a paste-in unmasks it.

// -----------------------------------------------------
// -----------------------------------------------------
// -----------------------------------------------------
// -----------------------------------------------------

      // if (silentReading && spiFlashReading) { // the 'load' word
      if (silentReading) { // the 'load' word
      } else {
              if (flags & ECHO_ON) Serial.print(" "); // seems to want a space after 'dot' for example.
      }
// -----------------------------------------------------
// -----------------------------------------------------
// TRY TO REMEMBER:
// 
// The mechanism to end a file upload to the SPI Flash
// is a parsed word '\end.'

// Whereas .. the mechanism to end a 'load' (compile
// into VM code, stored in RAM -- from a disk file forth
// source program that's stored on SPI FlashROM)
// 
// That mechanism is simply to detect if flash is waiting
// (if we have reached EOF or not while reading a file
// from the SPI FlashROM chip).
// -----------------------------------------------------

      // debug // Serial.println("EVERYONE");
      // Serial.println("debug: is flash waiting or not?");
      if (spiFlashWaiting) {
          // Serial.println("a good spot to turn off silentReading perhaps.");
          // Serial.println("Flash is WAITING .. more to read.");
      } else { 
          silentReading = FALSE; // we are interactive once more, when spiFlashWaiting changes state
      }

      break;

    } else {



// always suppress echo when the load word is executed.
      if (silentReading && spiFlashReading) { // the 'load' word
          int fakeSPxT = 0;
      } else {
          if (flags & ECHO_ON) {
// -----------------------------------------------------
// -----------------------------------------------------
// -----------------------------------------------------
if (silentReading) {
} else {
              // main forth interpreter typing echo is right here:
              Serial.print(inChar); // do NOT suppress this ordinarily, if ever.
}
// -----------------------------------------------------
// -----------------------------------------------------
// -----------------------------------------------------
// -----------------------------------------------------
          }
      }

      *ptr++ = inChar;
      *ptr = 0;
      count++;
    }
  } while (count < buffSize);
  return (count);
}

#endif


#ifdef EXT_KERN_GETKEY

/******************************************************************************/
/** getKey                                                                   **/
/**   waits for the next valid key to be entered and return its value        **/
/**   Valid characters are:  Backspace, Carriage Return (0x0d), Escape,      **/
/**   Tab, Newline (0x0a) and standard (printable) characters                **/
/******************************************************************************/
char getKey(void) {
    char inChar;
    // the load word provides this boolean:
    if (spiFlashReading) {
        if (inChar == ASCII_BS  ||
            inChar == ASCII_TAB ||
            inChar == ASCII_CR  ||  
            inChar == ASCII_NL  ||   // new
            inChar == ASCII_DEL ||   // new
            inChar == ASCII_ESC ||
            isprint(inChar)) {
            return inChar;
        }
    } else {
        while (1) {
            if (Serial.available()) {
                inChar = Serial.read();
                if (inChar == ASCII_BS  ||
                    inChar == ASCII_TAB ||
                    inChar == ASCII_CR  ||  
                    inChar == ASCII_NL  ||   // new
                    inChar == ASCII_DEL ||   // new
                    inChar == ASCII_ESC ||
                    isprint(inChar)) {
                    return inChar;
                }
            }
        }
    }
}

#endif







//  ---------------------------    ascii  transfer   --------------------------


//                      03 August 2017  15:49z begin.


// Wed Aug  2 01:22:51 UTC 2017
// 4735-b0c-03-

// #include <Arduino.h>
// #include "../../yaffa.h"
// #include "fatfs_writefile.h"

// Adafruit SPI Flash FatFs Full Usage Example
// Author: Tony DiCola

// locally modified July 2017 - wa1tnr

// modified to look at python in the fatfs - code.py and the /lib.

// This is an example of all the functions in the SPI Flash
// FatFs library.  Note that you MUST have a flash chip that's
// formatted with a flash filesystem before running.  See the
// fatfs_format example to perform this formatting.
//
// In general the API for this library closely follows the API
// for the Arduino SD card library.  However instead of interacting
// with a global SD object you create an instance of a fatfs class
// and use its open, exists, etc. functions.  See the SD library
// reference for more inspiration and examples to adapt:
//   https://www.arduino.cc/en/reference/SD
//
// Usage:
// - Modify the pins and type of fatfs object in the config
//   section below if necessary (usually not necessary).
// - Upload this sketch to your M0 express board.
// - Open the serial monitor at 115200 baud.  You should see the
//   example start to run and messages printed to the monitor.
//   If you don't see anything close the serial monitor, press
//   the board reset buttton, wait a few seconds, then open the
//   serial monitor again.
// #include <SPI.h>
// #include <Adafruit_SPIFlash.h>
// #include <Adafruit_SPIFlash_FatFs.h>


// Configuration of the flash chip pins and flash fatfs object.
// You don't normally need to change these if using a Feather/Metro
// M0 express board.
// #define FLASH_TYPE     SPIFLASHTYPE_W25Q16BV  // Flash chip type.
                                              // If you change this be
                                              // sure to change the fatfs
                                              // object type below to match.
// #define FLASH_SS       SS                    // Flash chip SS pin.
// #define FLASH_SPI_PORT SPI                   // What SPI port is Flash on?

// SHOULD NOT BE NEEDED 06 Aug:  Adafruit_SPIFlash ascii_xfer_flash(FLASH_SS, &FLASH_SPI_PORT);     // Use hardware SPI 

// Alternatively you can define and use non-SPI pins!
// Adafruit_SPIFlash flash(FLASH_SCK, FLASH_MISO, FLASH_MOSI, FLASH_SS);

// Adafruit_W25Q16BV_FatFs ascii_xfer_fatfs(ascii_xfer_flash);


void create_test_directory(void) {
  // Check if a directory called 'test' exists and create it if not there.
  // Note you should _not_ add a trailing slash (like '/test/') to directory names!
  // You can use the same exists function to check for the existance of a file too.

#ifdef HAS_STANDARD_BUILD_HERE

  if (!fatfs.exists("/test")) {
    Serial.println("Test directory not found, creating...");
    // Use mkdir to create directory (note you should _not_ have a trailing slash).
    if (!fatfs.mkdir("/test")) {
      Serial.println("Error, failed to create test directory!");
      while(1);
    }
    Serial.println("Created test directory!");
  }
#endif

#ifndef HAS_STANDARD_BUILD_HERE

  if (!fatfs.exists("/test")) { Serial.println("BAD ROBOT - fatfs.exists fails on line 97.");
  } else {
    Serial.println("local: assuming test directory already exists.");
  }

#endif
}






void write_a_capture_file(void) {
  // Create a file in the test directory and write data to it.
  // Note the FILE_WRITE parameter which tells the library you intend to
  // write to the file.  This will create the file if it doesn't exist,
  // otherwise it will open the file and start appending new data to the
  // end of it.  More advanced users can specify different file modes by
  // using the FatFs f_open modes directly (which can be specified instead
  // of FILE_WRITE), see documentation at:
  //   http://elm-chan.org/fsw/ff/en/open.html

  // 03 Aug 15:52z -- try to use existing fatfs 'handle' / whatever it is
  // result: does compile just fine.

  // File writeFile = ascii_xfer_fatfs.open("/test/ascii_xfer_test.txt", FILE_WRITE);
  File writeFile =               fatfs.open("/forth/ascii_xfer_test.txt", FILE_WRITE);
  if (!writeFile) {
    Serial.println("Error, failed to open ascii_xfer_test.txt for writing!");
    while(1);
  }
  // debug: // Serial.println("Opened file /forth/ascii_xfer_test.txt for writing/appending...");

  // Once open for writing you can print to the file as if you're printing
  // to the serial terminal, the same functions are available.



// payload -- download mode.

// model: Serial.print(cpSource);
  writeFile.println(cpSource);

  // Close the file when finished writing.
  writeFile.close();
  // debug: // Serial.println("Wrote -- appended -- to file /forth/ascii_xfer_test.txt!");
}









#ifndef HAS_STANDARD_BUILD_HERE
void read_a_test_file(void) {
  // Now open the same file but for reading.
  // cheap_test: File readFile = fatfs.open("/forth/job.fs",             FILE_READ);
  File readFile = fatfs.open("/forth/ascii_xfer_test.txt", FILE_READ);
  if (!readFile) {
    // cheap_test: Serial.println("Error, failed to open job.fs for reading!");
    Serial.println("Error, failed to open /forth/ascii_xfer_test.txt for reading!");
    while(1);
  }

  // Read data using the same read, find, readString, etc. functions as when using
  // the serial class.  See SD library File class for more documentation:
  //   https://www.arduino.cc/en/reference/SD
  // Read a line of data:
  String line = readFile.readStringUntil('\n');
  // cheap_test: Serial.print("First line of job.fs: "); Serial.println(line);
  Serial.print("First line of /forth/ascii_xfer_test.txt: ");


  Serial.println(line);

  // You can get the current position, remaining data, and total size of the file:
  Serial.println("Ignore job.fs and say /forth/ascii_xfer_test.txt here - several lines.");

  Serial.print("Total size of job.fs (bytes): "); Serial.println(readFile.size(), DEC);
  Serial.print("Current position in job.fs: "); Serial.println(readFile.position(), DEC);
  Serial.print("Available data to read in job.fs: "); Serial.println(readFile.available(), DEC);

  // And a few other interesting attributes of a file:
  Serial.print("File name: "); Serial.println(readFile.name());
  Serial.print("Is file a directory? "); Serial.println(readFile.isDirectory() ? "Yes" : "No");

  // You can seek around inside the file relative to the start of the file.
  // For example to skip back to the start (position 0):
  if (!readFile.seek(0)) {
    Serial.println("Error, failed to seek back to start of file!");
    while(1);
  }

  // And finally to read all the data and print it out a character at a time
  // (stopping when end of file is reached):
  Serial.println("Entire contents of job.fs:");
  while (readFile.available()) {
    char c = readFile.read();
    Serial.print(c);
  }

  // Close the file when finished reading.
  readFile.close();
}
#endif



#ifndef HAS_STANDARD_BUILD_HERE
void read_from_code_py_file(void) {
  File readCodeFile = fatfs.open("/main.py", FILE_READ);
  if (!readCodeFile) {
    Serial.println("Error, failed to open code.py for reading!");
    while(1);
  }

  // Read a line of data:
  String line = readCodeFile.readStringUntil('\n');
  Serial.print("First line of code.py: "); Serial.println(line);

  // You can get the current position, remaining data, and total size of the file:
  Serial.print("Total size of code.py (bytes): "); Serial.println(readCodeFile.size(), DEC);
  Serial.print("Current position in code.py: "); Serial.println(readCodeFile.position(), DEC);
  Serial.print("Available data to read in code.py: "); Serial.println(readCodeFile.available(), DEC);

  // And a few other interesting attributes of a file:
  Serial.print("File name: "); Serial.println(readCodeFile.name());
  Serial.print("Is file a directory? "); Serial.println(readCodeFile.isDirectory() ? "Yes" : "No");

  // You can seek around inside the file relative to the start of the file.
  // For example to skip back to the start (position 0):
  if (!readCodeFile.seek(0)) {
    Serial.println("Error, failed to seek back to start of file!");
    while(1);
  }

  // And finally to read all the data and print it out a character at a time
  // (stopping when end of file is reached):
  Serial.println("Entire contents of code.py:");
  while (readCodeFile.available()) {
    char c = readCodeFile.read();
    Serial.print(c);
  }

  // Close the file when finished reading.
  readCodeFile.close();
}
#endif




void tail_code_bb(void) {
  // You can open a directory to list all the children (files and directories).
  // Just like the SD library the File type represents either a file or directory.


#ifndef HAS_STANDARD_BUILD_HERE
  File testDirRoot = fatfs.open("/");
  if (!testDirRoot) {
    Serial.println("Error, failed to open root directory!");
    while(1);
  } else {
    Serial.println("Made it past the opening of the root directory.");
  }
#endif


#ifdef HAS_STANDARD_BUILD_HERE
  File testDir = fatfs.open("/lib");
  if (!testDir) {
    Serial.println("Error, failed to open test directory!");
    while(1);
  } else {
    Serial.println("Made it past the opening of the /lib directory.");
  }
#endif



#ifndef HAS_STANDARD_BUILD_HERE
  if (!testDirRoot.isDirectory()) {
    Serial.println("Error, expected root to be a directory!");
    while(1);
  } else {
    Serial.println("Good - root is a directory - not a file.  Continue.");
  }
#endif


#ifdef HAS_STANDARD_BUILD_HERE
  if (!testDir.isDirectory()) {
    Serial.println("Error, expected /lib to be a directory!");
    while(1);
  } else {
    Serial.println("Good - /lib is a directory - not a file.  Continue.");
  }
#endif



#ifndef HAS_STANDARD_BUILD_HERE
  Serial.println("Listing children of root directory:");
  File child = testDirRoot.openNextFile();
  while (child) {
    // Print the file name and mention if it's a directory.
    Serial.print("- "); Serial.print(child.name());
    if (child.isDirectory()) {
      Serial.print(" (directory)");
    }
    Serial.println();
    // Keep calling openNextFile to get a new file.
    // When you're done enumerating files an unopened one will
    // be returned (i.e. testing it for true/false like at the
    // top of this while loop will fail).
    child = testDirRoot.openNextFile();
  }

  // If you want to list the files in the directory again call
  // rewindDirectory().  Then openNextFile will start from the
  // top again.
  testDirRoot.rewindDirectory();
#endif


// hold: while(1); Serial.println("WILL ROBINSON.");
         while(1); Serial.println("WILL ROBINSON.");



#ifdef HAS_STANDARD_BUILD_HERE
  Serial.println("Listing children of directory /lib:");
  File child = testDir.openNextFile();
  while (child) {
    // Print the file name and mention if it's a directory.
    Serial.print("- "); Serial.print(child.name());
    if (child.isDirectory()) {
      Serial.print(" (directory)");
    }
    Serial.println();
    // Keep calling openNextFile to get a new file.
    // When you're done enumerating files an unopened one will
    // be returned (i.e. testing it for true/false like at the
    // top of this while loop will fail).
    child = testDir.openNextFile();
  }


  // If you want to list the files in the directory again call
  // rewindDirectory().  Then openNextFile will start from the
  // top again.
  testDir.rewindDirectory();
#endif



#ifdef HAS_EXTRA_STANDARD_BUILD_HERE

  // Delete a file with the remove command.  For example create a test2.txt file
  // inside /test/foo and then delete it.
  File test2File = fatfs.open("/forth/foo/test2.txt", FILE_WRITE);
  test2File.close();
  Serial.println("Deleting /forth/foo/test2.txt...");
  if (!fatfs.remove("/forth/foo/test2.txt")) {
    Serial.println("Error, couldn't delete test.txt file!");
    while(1);
  }
  Serial.println("Deleted file!");

  // Delete a directory with the rmdir command.  Be careful as
  // this will delete EVERYTHING in the directory at all levels!
  // I.e. this is like running a recursive delete, rm -rf, in
  // unix filesystems!
  Serial.println("Deleting /test directory and everything inside it...");
  if (!fatfs.rmdir("/test")) {
    Serial.println("Error, couldn't delete test directory!");
    while(1);
  }
  // Check that test is really deleted.
  if (fatfs.exists("/test")) {
    Serial.println("Error, test directory was not deleted!");
    while(1);
  }
  Serial.println("Test directory was deleted!");
  
  Serial.println("Finished!");
#endif

#ifndef HAS_STANDARD_BUILD_HERE
#endif

}

void ascii_xfer_spi_flash_main(void) {
  // SHOULD NOT BE NEEDED 06 AUG: ascii_xfer_setup_spi_flash();
  create_test_directory();
  // write_a_test_file();
  // read_a_test_file();
  read_from_code_py_file(); 
  tail_code_bb();

}

// void loop_flash_main(void) {
  // Nothing to be done in the main loop.
  // delay(100);
// }

// END.
