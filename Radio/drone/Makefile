
# The name of your project (used to name the compiled .hex file)
TARGET = drone

# configurable options
OPTIONS = -DF_CPU=96000000 -DUSB_SERIAL -DLAYOUT_US_ENGLISH

# options needed by many Arduino libraries to configure for Teensy 3.0
OPTIONS += -D__MK20DX256__ -DARDUINO=105 -DTEENSYDUINO=118


#************************************************************************
# Location of Teensyduino utilities, Toolchain, and Arduino Libraries.
# To use this makefile without Arduino, copy the resources from these
# locations and edit the pathnames.  The rest of Arduino is not needed.
#************************************************************************

ARDPATH = /c/Users/waldj/OneDrive/Programming/arduino-1.0.5-r2

# path location for Teensy Loader, teensy_post_compile and teensy_reboot
TOOLSPATH = $(ARDPATH)/hardware/tools

ARDCOREDIR = $(ARDPATH)/hardware/teensy/cores/teensy3

# path location for the arm-none-eabi compiler
COMPILERPATH = $(TOOLSPATH)/arm-none-eabi/bin

#************************************************************************
# Settings below this point usually do not need to be edited
#************************************************************************

INCDIR = -I. -ILogger -IPID -IRadio -IIMU -ISPI -IWire -ISoftwareSerial -ISensorInterface -IEEPROM -I$(ARDCOREDIR)

# CPPFLAGS = compiler options for C and C++
CPPFLAGS = -Wall -Wno-strict-aliasing -g -Os -mcpu=cortex-m4 -mthumb -nostdlib -MMD $(OPTIONS) $(INCDIR)

# compiler options for C++ only
CXXFLAGS = -std=gnu++0x -felide-constructors -fno-exceptions -fno-rtti

# compiler options for C only
CFLAGS =

# linker options
LDFLAGS = -Os -Wl,--gc-sections -mcpu=cortex-m4 -mthumb -Tmk20dx256.ld

# additional libraries to link
LIBS = -lm

# names for the compiler programs
CC = $(abspath $(COMPILERPATH))/arm-none-eabi-gcc
CXX = $(abspath $(COMPILERPATH))/arm-none-eabi-g++
OBJCOPY = $(abspath $(COMPILERPATH))/arm-none-eabi-objcopy
SIZE = $(abspath $(COMPILERPATH))/arm-none-eabi-size

# automatically create lists of the sources and objects
# TODO: this does not handle Arduino libraries yet...
C_FILES := Wire/utility/twi.c \
	$(wildcard $(ARDCOREDIR)/*.c) \

CPP_FILES := $(TARGET).cpp \
	BattMon/BattMon.cpp \
	EEPROM/EEPROM.cpp \
	HighLayerPIDLayer.cpp \
	IMU/Adafruit_10DOF/Adafruit_10DOF.cpp \
	IMU/Adafruit_BMP085_Unified/Adafruit_BMP085_U.cpp \
	IMU/Adafruit_L3GD20_U/Adafruit_L3GD20_U.cpp \
	IMU/Adafruit_LSM303DLHC/Adafruit_LSM303_U.cpp \
	IMU/Adafruit_Sensor/Adafruit_Sensor.cpp \
	IMU/IMU.cpp \
	Logger/Logger.cpp \
	LowLayerPIDLayer.cpp \
	main.cpp \
	MessageQueue.cpp \
	MidLayerPIDLayer.cpp \
	MotorController.cpp \
	PID/PID.cpp \
	Radio/DroneRadio.cpp \
	Radio/HardwareSPI.cpp \
	Radio/nrf24l01p.cpp \
	Radio/Packet.cpp \
	Radio/Radio.cpp \
	Records.cpp \
	Route.cpp \
	Sensor.cpp \
	SensorInterface/SensorInterface.cpp \
	Server.cpp \
	ServerAction.cpp \
	SoftwareSerial/SoftwareSerial.cpp \
	SPI/SPI.cpp \
	State.cpp \
	Storage.cpp \
	TinyGPSPlus/TinyGPS++.cpp \
	Wire/Wire.cpp \


CPP_FILES += $(filter-out $(ARDCOREDIR)/main.cpp, $(wildcard $(ARDCOREDIR)/*.cpp))

OBJS := $(C_FILES:.c=.o) $(CPP_FILES:.cpp=.o)


# the actual makefile rules (all .o files built by GNU make's default implicit rules)

all: only upload

only: $(TARGET).hex

$(TARGET).elf: $(OBJS) mk20dx256.ld
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

%.hex: %.elf
	$(SIZE) $<
	$(OBJCOPY) -O ihex -R .eeprom $< $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

upload:
	$(abspath $(TOOLSPATH))/teensy_post_compile -file=$(TARGET) -path=$(shell pwd) -tools=$(abspath $(TOOLSPATH))
	-$(abspath $(TOOLSPATH))/teensy_reboot

# compiler generated dependency info
-include $(OBJS:.o=.d)

clean:
	rm -f *.o *.d $(TARGET).elf $(TARGET).hex
	rm -f BattMon/*.o BattMon/*.d
	rm -f EEPROM/*.o EEPROM/*.d
	rm -f IMU/*.o IMU/*.d IMU/*/*.o IMU/*/*.d
	rm -f Logger/*.o Logger/*.d
	rm -f PID/*.o PID/*.d
	rm -f Radio/*.o Radio/*.d
	rm -f SoftwareSerial/*.o SoftwareSerial/*.d
	rm -f SPI/*.o SPI/*.d
	rm -f TinyGPSPlus/*.o TinyGPSPlus/*.d
	rm -f Wire/*.o Wire/*.d
	rm -f $(ARDCOREDIR)/*.o $(ARDCOREDIR)/*.d
