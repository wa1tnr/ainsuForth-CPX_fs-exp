// Thu Aug  3 01:46:56 UTC 2017
// 4735-b0d-00-

#include <Arduino.h>
#include "../../yaffa.h"
// #include "Error_Codes.h"

#include "download.h"
const char download_str[] = "download"; // part of the main forth vocabulary, only
void _download(void) {
    noInterpreter = TRUE; // download mode -- no standard interpreter -- use dl_interpreter
    Serial.println("\r\n debug: download.cpp - the download word.\r\n");
}

// 53   { dl_ends_str,        _dl_ends,         NORMAL },  // IMMEDIATE

const char dl_ends_str[] = "endeth";
void _dl_ends(void) {
    noInterpreter = FALSE; 
    Serial.println("\r\n DOWNLOAD ENDS.\r\n");
}


// end.

