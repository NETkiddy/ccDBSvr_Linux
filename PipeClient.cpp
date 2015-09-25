#include "PipeClient.h"
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

PipeWriter::PipeWriter()
: m_filePipe(NULL)
, m_fdPipe(-1)
{
		
}

PipeWriter::~PipeWriter()
{
	close();
}

int PipeWriter::open(std::string sName, std::string sMode)
{
	if(sName.empty() || sMode.empty())
		return -1;
	const char *cName = sName.c_str();
	unlink(cName);
	if((mkfifo(cName, 0777) < 0) && errno != EEXIST)
	{
		std::cout<<"PipeWriter::open mkfifo error"<<std::endl;
		return -1;
	}

	//m_filePipe = fopen(cName, sMode.c_str());
	//if(!m_filePipe)
	////cann't open the pipe with _O_WRONLY, cause of the open principle of fifo
	m_fdPipe = ::open(cName, O_RDWR|O_NONBLOCK, 0);
	if(-1 == m_fdPipe)
	{
		std::cout<<"PipeWriter::open fopen error"<<std::endl;
		return -2;
	}

	std::cout<<"PipeWriter Open Success"<<std::endl;
	return 0;
}

void PipeWriter::close()
{
	/*
	if(!m_filePipe)
	{
		fclose(m_filePipe);
		m_filePipe = NULL;
	}
	*/
	if(-1 != m_fdPipe)
	{
		if(0 == ::close(m_fdPipe))
			std::cout<<"close pipeWriter success"<<std::endl;
	}
}

int PipeWriter::write(char *iTag, int iCount)
{
	int writeNum = 0;

	while(1)
	{
		//writeNum = fwrite(&iTag, sizeof(int), iCount, m_filePipe);
		//if(writeNum != iCount)
		writeNum = ::write(m_fdPipe, iTag, sizeof(char));
		if(-1 == writeNum)
		{
			std::cout<<"PipeWriter::write fwrite error"<<std::endl;
		}
		break;
	}
	
	std::cout<<"PipeWriter success:"<<*iTag<<std::endl;
	return writeNum;
}

int PipeWriter::getCount()
{
	if(-1 == m_fdPipe)
		return -1;
	
	struct stat fileStat;  
	if( -1 == fstat(m_fdPipe, &fileStat))  
	{  
		return -1;  
	}  

	// deal returns.  
	return fileStat.st_size;
}


int PipeWriter::getFd()
{
	return m_fdPipe;
	//return fileno(m_filePipe);
}




PipeReader::PipeReader()
: m_filePipe(NULL)
{
	
}

PipeReader::~PipeReader()
{
	close();
}

int PipeReader::open(std::string sName, std::string sMode)
{
	if(sName.empty() || sMode.empty())
		return -1;
	const char *cName = sName.c_str();
	
	/*unlink(cName);
	if((mkfifo(cName, 0777) < 0) && errno != EEXIST)
	{
		std::cout<<"PipeReader::open mkfifo error"<<std::endl;
		return -1;
	}
	*/

	//m_filePipe = fopen(cName, sMode.c_str());
	//if(!m_filePipe)
	m_fdPipe = ::open(cName, O_RDONLY|O_NONBLOCK, 0);
	if(-1 == m_fdPipe)
	{
		std::cout<<"PipeReader::open fopen error, "<<strerror(errno)<<std::endl;
		return -2;
	}

	std::cout<<"PipeReader Open Success"<<std::endl;
	return 0;
}

int PipeReader::read()
{
	char temp[1] = {};
	//int iRead = fread(&temp, sizeof(int), 1, m_filePipe);
	//if(iRead != 1)
	::lseek(m_fdPipe, 0, SEEK_SET);
	int iRead = ::read(m_fdPipe, temp, sizeof(char));
	if(-1 == iRead)
	{
		std::cout<<"PipeReader::read fread error, "<<strerror(errno)<<std::endl;
		return iRead;
	}

	std::cout<<"PipeReader success: "<<*temp<<std::endl;
	return iRead;//temp;
}

void PipeReader::close()
{
	/*
	if(!m_filePipe)
	{
		fclose(m_filePipe);
		m_filePipe = NULL;
	}
	*/
	if(-1 != m_fdPipe)
	{
		if(0 == ::close(m_fdPipe))
			std::cout<<"close pipeReader success"<<std::endl;
	}
}

int PipeReader::getFd()
{
	return m_fdPipe;
	//return fileno(m_filePipe);
}
