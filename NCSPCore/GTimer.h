//
// Basic chrono-based timer
//
#pragma once

#include <chrono>
#include <string>

typedef std::chrono::high_resolution_clock Clock;
typedef Clock::time_point Time;

class GTimer
{
public:
   GTimer();
   ~GTimer();

   void Start();
   void Stop();
   long long GetTime();
   static std::string GetTimeString(long long timestamp);
   static std::string GetTimeStringSeconds(long long timestamp,
                                           bool bNotation = false);

private:

   long long GetTimeDiff();

   Time start_;
   Time end_;
};

