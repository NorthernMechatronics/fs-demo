#******************************************************************************
#
# Step 1
# Define the locations of the various SDKs and libraries.
#
#******************************************************************************
NM_SDK    ?= $(shell pwd)/../nmsdk
AMBIQ_SDK ?= $(shell pwd)/../AmbiqSuite-R2.5.1
FREERTOS  ?= $(shell pwd)/../FreeRTOS-Kernel
CORDIO    ?= $(shell pwd)/../AmbiqSuite-R2.5.1/third_party/cordio
UECC      ?= $(shell pwd)/../AmbiqSuite-R2.5.1/third_party/uecc
LORAMAC   ?= $(shell pwd)/../LoRaMac-node

#******************************************************************************
#
# Step 2
# Specify the location of the board support package to be used.
#
#******************************************************************************
BSP_DIR := $(NM_SDK)/bsp/nm180100evb

#******************************************************************************
#
# Step 3
# Specify output target version and name
#
#******************************************************************************
TARGET_VERSION := 0x00

ifdef DEBUG
    TARGET      := fs-demo-dev
    TARGET_OTA  := fs-demo_ota-dev
    TARGET_WIRE := fs-demo_wire-dev
else
    TARGET      := fs-demo
    TARGET_OTA  := fs-demo_ota
    TARGET_WIRE := fs-demo_wire
endif

#******************************************************************************
#
# Step 4
# Include additional source, header, libraries or paths below.
#
# Examples:
#   INCLUDES += -Iadditional_include_path
#   VPATH    += additional_source_path
#   LIBS     += -ladditional_library
#******************************************************************************
INCLUDES += -I./littlefs
VPATH += ./littlefs
SRC += lfs.c
SRC += lfs_util.c
SRC += lfs_ambiq.c

INCLUDES += -I$(NM_SDK)/platform/console

VPATH += $(NM_SDK)/platform/console

SRC += console_task.c
SRC += gpio_service.c
SRC += iom_service.c

SRC += application.c
