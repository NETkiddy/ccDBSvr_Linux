#ifndf _PIPE_CLIENT_H_
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
	int open(std::string sFifoName, int iFifoMode = 0);
	void close()
	void write(std::string sData);
	int getFd();
private:
	int m_fdPipe;
	FILE *filePipe;
	
};

class PipeReader
{
public:
	PipeReader();
	~PipeReader();
	int open(std::string sFifoName, int iFifoMode = 0);
	void close()
	std::string read();
	int getFd();
private:
	int m_fdPipe;
	FILE *m_filePipe;

	
};


#endif
