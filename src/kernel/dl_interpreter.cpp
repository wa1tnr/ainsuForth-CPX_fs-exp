// Thu Aug  3 01:46:56 UTC 2017
// 4735-b0d-00-

// download interpreter

#include <Arduino.h>
#include "../../yaffa.h"
// #include "../../Error_Codes.h"

#include "dl_interpreter.h"
/******************************************************************************/
/** Interpeter - Interprets a new string                                     **/
/**                                                                          **/
/** Parse the new line. For each parsed subString, try to execute it.  If it **/
/** can't be executed, try to interpret it as a number.  If that fails,      **/
/** signal an error.                                                         **/
/******************************************************************************/
void dl_interpreter(void) {
    func function;
    if (noInterpreter) {
        Serial.println("\r\nnoInterpreter == TRUE\r\n");
    }
    while (getToken()) {


#ifdef NEVER_DEFINED_
        if (state) {
        /*************************/
        /** Compile Mode        **/
        /*************************/
            if (isDLWord(cTokenBuffer)) {
                if (wordFlags & IMMEDIATE) {
                    if (w > 255) {
                        rStack_push(0);        // Push 0 as our return address
                        ip = (cell_t *)w;    // set the ip to the XT (memory location)
                        // executeWord();
                        Serial.println("\r\ndebug: Error line 38 dl_interpreter.cpp.\r\n");
                    } else {
                        function = DLflashDict[w - 1].function;
                        function();
                        if (errorCode) return;
                    }
                    // executeWord();  // Why is this here?
                    Serial.println("\r\ndebug: Error line 45 dl_interpreter.cpp.\r\n");
                } else {
                    *pHere++ = w;
                } // ends stanza that began 'if (wordFlags & IMMEDIATE)'
            } // ends stanza that began 'if (isDLWord(cTokenBuffer))'
            else if (isDLNumber(cTokenBuffer)) {
                _literal();
            }
            else {
                dStack_push(-13);
                _throw();
            }
        } // ends stanza that began 'if (state)'


        else { // 'if (state) { foo } else ...
#endif


            /************************/
            /* Interpret Mode       */
            /************************/
            if (isDLWord(cTokenBuffer)) {
                if (wordFlags & COMP_ONLY) {
                    dStack_push(-14);
                    _throw();
                    return;
                }
                if (w > 255) {
                    rStack_push(0);           // Push 0 as our return address
                    ip = (cell_t *)w;         // set the ip to the XT (memory location)
                    // executeWord();
                    Serial.println("\r\ndebug: Error line 77 dl_interpreter.cpp.\r\n");
                    if (errorCode) return;
                }
                else {
                    function = DLflashDict[w - 1].function;
                    function();
                    if (errorCode) return;
                }
            } // ends stanza that began 'if (isDLWord(cTokenBuffer))'
            else if (isDLNumber(cTokenBuffer)) {
                int fake_Int_xx = 0 ; // noop // Is something supposed to be here?
            }
            else {
                dStack_push(-13);
                _throw();
                return;
            }

#ifdef NEVER_DEFINED_
        } // ends stanza that began 'if (state) .. else ..'
#endif



    } // ends stanza that began 'while (getToken())'
    cpToIn = cpSource;
  }

