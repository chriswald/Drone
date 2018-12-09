/*
TinyGPS++ - a small GPS library for Arduino providing universal NMEA parsing
Based on work by and "distanceBetween" and "courseTo" courtesy of Maarten Lamers.
Suggestion to add satellites, courseTo(), and cardinal() by Matt Monson.
Location precision improvements suggested by Wayne Holder.
Copyright (C) 2008-2013 Mikal Hart
All rights reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef __TinyGPSPlus_h
#define __TinyGPSPlus_h

#include <Arduino.h>
#include <limits.h>

#define _GPS_VERSION "0.92" // software version of this library
#define _GPS_MPH_PER_KNOT 1.15077945
#define _GPS_MPS_PER_KNOT 0.51444444
#define _GPS_KMPH_PER_KNOT 1.852
#define _GPS_MILES_PER_METER 0.00062137112
#define _GPS_KM_PER_METER 0.001
#define _GPS_FEET_PER_METER 3.2808399
#define _GPS_MAX_FIELD_SIZE 15

////////////////////////////////////////////////
// DEFINES FOR ADAFRUIT ULTIMATE GPS BREAKOUT //
////////////////////////////////////////////////

// different commands to set the update rate from once a second (1 Hz) to 10 times a second (10Hz)
#define PMTK_SET_NMEA_UPDATE_1HZ  "$PMTK220,1000*1F"
#define PMTK_SET_NMEA_UPDATE_5HZ  "$PMTK220,200*2C"
#define PMTK_SET_NMEA_UPDATE_10HZ "$PMTK220,100*2F"


#define PMTK_SET_BAUD_57600 "$PMTK251,57600*2C"
#define PMTK_SET_BAUD_9600 "$PMTK251,9600*17"

// turn on only the second sentence (GPRMC)
#define PMTK_SET_NMEA_OUTPUT_RMCONLY "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29"
// turn on GPRMC and GGA
#define PMTK_SET_NMEA_OUTPUT_RMCGGA "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"
// turn on ALL THE DATA
#define PMTK_SET_NMEA_OUTPUT_ALLDATA "$PMTK314,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0*28"
// turn off output
#define PMTK_SET_NMEA_OUTPUT_OFF "$PMTK314,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"

#define PMTK_LOCUS_STARTLOG  "$PMTK185,0*22"
#define PMTK_LOCUS_LOGSTARTED "$PMTK001,185,3*3C"
#define PMTK_LOCUS_QUERY_STATUS "$PMTK183*38"
#define PMTK_LOCUS_ERASE_FLASH "$PMTK184,1*22"
#define LOCUS_OVERLAP 0
#define LOCUS_FULLSTOP 1

// standby command & boot successful message
#define PMTK_STANDBY "$PMTK161,0*28"
#define PMTK_STANDBY_SUCCESS "$PMTK001,161,3*36"  // Not needed currently
#define PMTK_AWAKE "$PMTK010,002*2D"

// ask for the release and version
#define PMTK_Q_RELEASE "$PMTK605*31"

// request for updates on antenna status 
#define PGCMD_ANTENNA "$PGCMD,33,1*6C" 
#define PGCMD_NOANTENNA "$PGCMD,33,0*6C"

//////////////////////////////////////////////
// END OF DEFINES FOR ULTIMATE GPS BREAKOUT //
//////////////////////////////////////////////



struct RawDegrees
{
   uint16_t deg;
   uint32_t billionths;
   bool negative;
public:
   RawDegrees() : deg(0), billionths(0), negative(false)
   {}
};

struct TinyGPSLocation
{
   friend class TinyGPSPlus;
public:
   bool isValid() const    { return valid; }
   bool isUpdated() const  { return updated; }
   uint32_t age() const    { return valid ? millis() - lastCommitTime : (uint32_t)ULONG_MAX; }
   const RawDegrees &rawLat()     { updated = false; return rawLatData; }
   const RawDegrees &rawLng()     { updated = false; return rawLngData; }
   double lat();
   double lng();

   TinyGPSLocation() : valid(false), updated(false)
   {}

private:
   bool valid, updated;
   RawDegrees rawLatData, rawLngData, rawNewLatData, rawNewLngData;
   uint32_t lastCommitTime;
   void commit();
   void setLatitude(const char *term);
   void setLongitude(const char *term);
};

struct TinyGPSDate
{
   friend class TinyGPSPlus;
public:
   bool isValid() const       { return valid; }
   bool isUpdated() const     { return updated; }
   uint32_t age() const       { return valid ? millis() - lastCommitTime : (uint32_t)ULONG_MAX; }

   uint32_t value()           { updated = false; return date; }
   uint16_t year();
   uint8_t month();
   uint8_t day();

   TinyGPSDate() : valid(false), updated(false), date(0)
   {}

private:
   bool valid, updated;
   uint32_t date, newDate;
   uint32_t lastCommitTime;
   void commit();
   void setDate(const char *term);
};

struct TinyGPSTime
{
   friend class TinyGPSPlus;
public:
   bool isValid() const       { return valid; }
   bool isUpdated() const     { return updated; }
   uint32_t age() const       { return valid ? millis() - lastCommitTime : (uint32_t)ULONG_MAX; }

   uint32_t value()           { updated = false; return time; }
   uint8_t hour();
   uint8_t minute();
   uint8_t second();
   uint8_t centisecond();

   TinyGPSTime() : valid(false), updated(false), time(0)
   {}

private:
   bool valid, updated;
   uint32_t time, newTime;
   uint32_t lastCommitTime;
   void commit();
   void setTime(const char *term);
};

struct TinyGPSDecimal
{
   friend class TinyGPSPlus;
public:
   bool isValid() const    { return valid; }
   bool isUpdated() const  { return updated; }
   uint32_t age() const    { return valid ? millis() - lastCommitTime : (uint32_t)ULONG_MAX; }
   int32_t value()         { updated = false; return val; }

   TinyGPSDecimal() : valid(false), updated(false), val(0)
   {}

private:
   bool valid, updated;
   uint32_t lastCommitTime;
   int32_t val, newval;
   void commit();
   void set(const char *term);
};

struct TinyGPSInteger
{
   friend class TinyGPSPlus;
public:
   bool isValid() const    { return valid; }
   bool isUpdated() const  { return updated; }
   uint32_t age() const    { return valid ? millis() - lastCommitTime : (uint32_t)ULONG_MAX; }
   uint32_t value()        { updated = false; return val; }

   TinyGPSInteger() : valid(false), updated(false), val(0)
   {}

private:
   bool valid, updated;
   uint32_t lastCommitTime;
   uint32_t val, newval;
   void commit();
   void set(const char *term);
};

struct TinyGPSSpeed : TinyGPSDecimal
{
   double knots()    { return value() / 100.0; }
   double mph()      { return _GPS_MPH_PER_KNOT * value() / 100.0; }
   double mps()      { return _GPS_MPS_PER_KNOT * value() / 100.0; }
   double kmph()     { return _GPS_KMPH_PER_KNOT * value() / 100.0; }
};

struct TinyGPSCourse : public TinyGPSDecimal
{
   double deg()      { return value() / 100.0; }
};

struct TinyGPSAltitude : TinyGPSDecimal
{
   double meters()       { return value() / 100.0; }
   double miles()        { return _GPS_MILES_PER_METER * value() / 100.0; }
   double kilometers()   { return _GPS_KM_PER_METER * value() / 100.0; }
   double feet()         { return _GPS_FEET_PER_METER * value() / 100.0; }
};

class TinyGPSPlus;
class TinyGPSCustom
{
public:
   TinyGPSCustom() {};
   TinyGPSCustom(TinyGPSPlus &gps, const char *sentenceName, int termNumber);
   void begin(TinyGPSPlus &gps, const char *_sentenceName, int _termNumber);

   bool isUpdated() const  { return updated; }
   bool isValid() const    { return valid; }
   uint32_t age() const    { return valid ? millis() - lastCommitTime : (uint32_t)ULONG_MAX; }
   const char *value()     { updated = false; return buffer; }

private:
   void commit();
   void set(const char *term);

   char stagingBuffer[_GPS_MAX_FIELD_SIZE + 1];
   char buffer[_GPS_MAX_FIELD_SIZE + 1];
   unsigned long lastCommitTime;
   bool valid, updated;
   const char *sentenceName;
   int termNumber;
   friend class TinyGPSPlus;
   TinyGPSCustom *next;
};

class TinyGPSPlus
{
public:
  TinyGPSPlus();
  bool encode(char c); // process one character received from GPS
  TinyGPSPlus &operator << (char c) {encode(c); return *this;}

  TinyGPSLocation location;
  TinyGPSDate date;
  TinyGPSTime time;
  TinyGPSSpeed speed;
  TinyGPSCourse course;
  TinyGPSAltitude altitude;
  TinyGPSInteger satellites;
  TinyGPSDecimal hdop;

  static const char *libraryVersion() { return _GPS_VERSION; }

  static double distanceBetween(double lat1, double long1, double lat2, double long2);
  static double courseTo(double lat1, double long1, double lat2, double long2);
  static const char *cardinal(double course);

  static int32_t parseDecimal(const char *term);
  static void parseDegrees(const char *term, RawDegrees &deg);

  uint32_t charsProcessed()   const { return encodedCharCount; }
  uint32_t sentencesWithFix() const { return sentencesWithFixCount; }
  uint32_t failedChecksum()   const { return failedChecksumCount; }
  uint32_t passedChecksum()   const { return passedChecksumCount; }
  bool     fix()              const { return gpsHasFix; }

private:
  enum {GPS_SENTENCE_GPGGA, GPS_SENTENCE_GPRMC, GPS_SENTENCE_OTHER};

  // parsing state variables
  uint8_t parity;
  bool isChecksumTerm;
  char term[_GPS_MAX_FIELD_SIZE];
  uint8_t curSentenceType;
  uint8_t curTermNumber;
  uint8_t curTermOffset;
  bool sentenceHasFix;
  bool gpsHasFix;

  // custom element support
  friend class TinyGPSCustom;
  TinyGPSCustom *customElts;
  TinyGPSCustom *customCandidates;
  void insertCustom(TinyGPSCustom *pElt, const char *sentenceName, int index);

  // statistics
  uint32_t encodedCharCount;
  uint32_t sentencesWithFixCount;
  uint32_t failedChecksumCount;
  uint32_t passedChecksumCount;

  // internal utilities
  int fromHex(char a);
  bool endOfTermHandler();
};

#endif // def(__TinyGPSPlus_h)
