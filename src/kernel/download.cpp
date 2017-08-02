// Wed Aug  2 18:04:12 UTC 2017
// 4735-b0c-05-

#include <Arduino.h>
#include "../../yaffa.h"
// #include "Error_Codes.h"

#include "download.h"
const char download_str[] = "download";
void _download(void) {
    noInterpreter = TRUE; // download mode -- no standard interpreter -- use dl_interpreter
    Serial.println("\r\n Nothing.  Placeholder for the download word.\r\n");
}

// end.

