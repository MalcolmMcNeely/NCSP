//
// Implementation of Logger.h
//
#include "Logger.h"
#include "common.h"

using namespace std;

Logger::Logger(const char* filename, int mode)
{
   fout.open(filename);
	mode_ = mode;
}

Logger::~Logger()
{
   fout.flush();		
   fout.close();
}

bool Logger::InitialisePipe()
{
	LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\NCSPPipe1");
	hPipe_ = CreateFile(lpszPipename, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);

	// If pipe is valid, return true
	if (hPipe_ != INVALID_HANDLE_VALUE)
	{
		return true;
	}

	// Exit if an error other than ERROR_PIPE_BUSY occurs
	if (GetLastError() != ERROR_PIPE_BUSY)
	{
		cout << "Could not open pipe" << endl;
		return false;
	}

	// If the pipe is busy, wait to see if it becomes available
	if (!WaitNamedPipe(lpszPipename, PIPETIMEOUTTIME))
	{
		cout << "Could not open pipe" << endl;
		return false;
	}	

	return true;
}

void Logger::Dispose()
{
	CloseHandle(hPipe_);
}

void Logger::WriteToPipe(string& message)
{
	char buf[1024];
	strncpy(buf, message.c_str(), sizeof(buf));
	buf[sizeof(buf)-1] = 0;
	DWORD dwBytesToWrite = (DWORD)strlen(buf);
	WriteFile(hPipe_, buf, dwBytesToWrite, NULL, NULL);
}