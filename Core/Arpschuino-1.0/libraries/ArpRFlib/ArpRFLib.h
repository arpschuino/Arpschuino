/// @file
/// This is a Fork from the wonderfull jeelib adapted for all the arpschuino Board.
/// Trivial wrapper around the Ports.h and RF12.h header files.

#ifndef ArpRFLib_h
#define ArpRFLib_h

// Include RF69 in compatibility mode if so requested, i.e. by inserting the
// line "#define RF69_COMPAT 1" before including this <JeeLib.h> header file.
// Define it as 0 otherwise, to support "if (RF69_COMPAT) ..." in app code.

#if RF69_COMPAT
#include <RF69_compat.h>
#elif !defined(RF69_COMPAT)
#define RF69_COMPAT 0
#endif

#include "ArpPorts.h"
#include "ArpRF12.h"
#include "ArpRF69.h"
#endif

/// @dir examples
/// Contains all example sketches, this is automatically scanned by the IDE.

/// @dir examples/Ports
/// All example sketches related to the Port class and plugs using it.

/// @dir examples/RF12
/// All example sketches related to the RF12.h wireless driver.

/// @dir examples/RF69
/// All example sketches related to the RF69.h wireless driver.
