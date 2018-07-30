//
// Helper functions
//
#pragma once
#include <vector>

class Logger;

namespace Utility
{
	void OutputText(const char* c, Logger* log = nullptr);

   template<typename T>
   T Clamp(T value, T min, T max)
   {
      if (value < min)
      {
         return min;
      }

      if (value > max)
      {
         return max;
      }

      return value;
   }

   template<typename T>
   bool VectorContains(std::vector<T> container, T value)
   {
      if (container.size() == 0)
      {
         return false;
      }

      if (container.size() == 1)
      {
         return value == container[0];
      }

      for (auto& t : container)
      {
         if (t == value)
         {
            return true;
         }
      }

      return false;
   }

   template<typename T>
   bool AddValueIfUnique(std::vector<T>& container, T value)
   {
      if (!VectorContains(container, value))
      {
         container.push_back(value);
         return true;
      }

      return false;
   }

   // @params:
   // s = string 1
   // n = string 1 length
   // t = string 2
   // m = string 2 length
   // @example:
   // int dist = levenshtein_distance(str1, strlen(str1), str1, strlen(str2));
   size_t levenshtein(const char* s, size_t n, const char* t, size_t m);

   std::vector<int> ExtractUniqueValues(std::vector<std::vector<int>> data);
}