#include "PipeClient.h"
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

PipeWriter::PipeWriter()
: m_filePipe(NULL)
{
		
}

PipeWriter::~PipeWriter()
{
	close();
}

int PipeWriter::open(std::string sName, int iMode)
{
	if(sName.empty())
		return -1;
	const char *cName = sName.c_str();
	unlink(cName);
	if((mkfifo(cName, 0777) < 0) && errno != EEXIST)
	{
		std::cout<<"PipeWriter::open mkfifo error"<<std::endl;
		return -1;
	}
	m_filePipe = fopen(cName, "a+");
	if(!m_filePipe)
	{
		std::cout<<"PipeWriter::open fopen error"<<std::endl;
		return -2;
	}
	return 0;
}

void PipeWriter::close()
{
	if(!m_filePipe)
	{
		fclose(m_filePipe);
		m_filePipe = NULL;
	}
}

int PipeWriter::write(PipeData *ppData, int iCount)
{
	if(!ppData)
		return -1;
	int writeNum = 0;

	while(1)
	{
		writeNum = fwrite(ppData, sizeof(PipeData), iCount, m_filePipe);
		if(writeNum != iCount)
		{
			std::cout<<"PipeWriter::write fwrite error"<<std::endl;
		}
		break;
	}

	return writeNum;
}

int PipeWriter::getFd()
{
	return fileno(m_filePipe);
}




PipeReader::PipeReader()
: m_filePipe(NULL)
{
	
}

PipeReader::~PipeReader()
{
	close();
}

int PipeReader::open(std::string sName, int iMode)
{
	if(sName.empty())
		return -1;
	const char *cName = sName.c_str();
	unlink(cName);
	if((mkfifo(cName, 0777) < 0) && errno != EEXIST)
	{
		std::cout<<"PipeReader::open mkfifo error"<<std::endl;
		return -1;
	}
	m_filePipe = fopen(cName, "a+");
	if(!m_filePipe)
	{
		std::cout<<"PipeReader::open fopen error"<<std::endl;
		return -2;
	}

}

PipeData PipeReader::read()
{
	PipeData ppData;
	int iRead = fread(&ppData, sizeof(PipeData), 1, m_filePipe);
	if(iRead != 1)
	{
		std::cout<<"PipeWriter::read fread error"<<std::endl;
	}
	return ppData;
}

void PipeReader::close()
{
	if(!m_filePipe)
	{
		fclose(m_filePipe);
		m_filePipe = NULL;
	}
}

int PipeReader::getFd()
{
	return fileno(m_filePipe);
}
