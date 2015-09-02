#ifndef _PIPE_CLIENT_H_
#define _PIPE_CLIENT_H_

#include <string>

struct PipeData
{
	int iTag;
};

class PipeWriter
{
public:
	PipeWriter();
	~PipeWriter();
	int open(std::string sFifoName, std::string sFifoMode = "a+");
	void close();
	int write(PipeData *ppData, int iCount);
	int getFd();
private:
	int m_fdPipe;
	FILE *m_filePipe;
	
};

class PipeReader
{
public:
	PipeReader();
	~PipeReader();
	int open(std::string sFifoName, std::string sFifoMode = "a+");
	void close();
	PipeData read();
	int getFd();
private:
	int m_fdPipe;
	FILE *m_filePipe;

	
};


#endif
