//
// Basic logger
//
#pragma once
#include <fstream>
#include <iostream>
#include <iomanip>
#include <Windows.h>

class Logger
{
public:
   Logger(const char* filename, int mode = 0);
   ~Logger();
      
	bool InitialisePipe();
	void Dispose();	
	void WriteToPipe(std::string& message);
	int GetMode() { return mode_; }

   template<class T>   
   friend Logger& operator<<(Logger& log, const T& output)
   {
      std::cout << output;
      log.fout << output << std::flush;
      return log;
   }

	template<std::string*>
	friend Logger& operator<<(Logger& log, const std::string* output)
	{
		if (mode_ == 1)
		{
			WriteToPipe(ptr(output));
		}
		else
		{
			std::cout << ptr(output);
			log.fout << ptr(output) << std::flush;
		}
		return log;
	}

   // This function accepts std::endl and std::flush
   friend Logger& operator<< (Logger& log, std::ostream& (*manip)(std::ostream &))
   {
      manip(std::cout);
      manip(log.fout);
      return log;
   }

   // This function accepts modifiers: std::hex, std::fixed
   friend Logger& operator<<(Logger& log, std::ios_base& (*manip)(std::ios_base&))
   {
      manip(std::cout);
      manip(log.fout);
      return log;
   }

   // This function accepts parameterized modifiers: std::setw(5)
   friend Logger& operator<<(Logger& log, std::_Smanip<std::streamsize> MySmanip)
   {
      return log;
   }

	template<typename T>
	T * ptr(T & obj) { return &obj; } // turn reference into pointer

	template<typename T>
	T * ptr(T * obj) { return obj; } // obj is already pointer, return it

private:	

	int mode_; // 0 write to cout, 1 write to pipe
   std::ofstream fout;
	HANDLE hPipe_;
};
