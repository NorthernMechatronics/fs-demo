# LittleFS Demo with Ambiq Flash APIs

This is a littleFS demo based on the nmapp template.  The flash routines
required for littleFS are implemented in lfs_ambiq.c

# Build Instructions

There are two build configurations: one for debug and another for release.  The
configuration to be build is defined by the variable DEBUG.  When DEBUG is defined,
the debug configuration is selected and if it is left undefined, then the release
configuration is selected.  The output target will be located in either the debug or
the release directory.

## Debug Configuration
* make DEBUG=1
* make DEBUG=1 clean

## Release Configuration
* make
* make clean
