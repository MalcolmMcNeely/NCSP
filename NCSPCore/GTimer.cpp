//
// Implementation of GTimer.h
//
#include "GTimer.h"
#include <sstream>

using namespace std;

GTimer::GTimer()
{
}

GTimer::~GTimer()
{
}

void GTimer::Start()
{
   start_ = Clock::now();
}

void GTimer::Stop()
{
   end_ = Clock::now();
}

long long GTimer::GetTime()
{
   return GetTimeDiff();
}

string GTimer::GetTimeString(long long timestamp)
{
   stringstream ss;

   long milliseconds = (long)timestamp % 1000;
   long seconds = (long)(timestamp / 1000) % 60;
   long minutes = (long)((timestamp / (1000 * 60)) % 60);
   long hours = (long)((timestamp / (1000 * 60 * 60)) % 24);

   if (hours > 0)
   {
      ss << hours << ":";
   }
   if (minutes > 0)
   {
      ss << minutes << ":";
   }
   if (seconds > 0)
   {
      ss << seconds << ".";
   }
   else
   {
      ss << "0.";
   }

   ss << milliseconds << "ms";
   return ss.str();
}

string GTimer::GetTimeStringSeconds(long long timestamp,
                                    bool bNotation)
{
   stringstream ss;

   long milliseconds = (long)timestamp % 1000;
   long seconds = (long)(timestamp / 1000);

   if (seconds > 0)
   {
      ss << seconds;
   }
   else
   {
      ss << "0";
   }
   
   ss << "." << milliseconds;
   
   if (bNotation)
   {
      ss << "ms";
   }

   return ss.str();
}

long long GTimer::GetTimeDiff()
{
   return std::chrono::duration_cast<std::chrono::milliseconds>(end_ - start_).count();
}
