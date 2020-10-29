/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// xxFile.cpp: implementation of the xxFile class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "xxFile.h"


//////////////////////////////////////////////////////////////////////
// Static member functions to allow any routine to check the state of files.
//////////////////////////////////////////////////////////////////////

// Boolean function to show if a file exists for both reading or writing. 
// We use a temporary filestream object here to avoid using a member variable to
// hold temporary objects.

bool xxFile::DoesFileExist(zString iFileName)
{
	zInFileStream istr(iFileName.c_str(), zIos::in|zIos::out);

	if(istr.fail())
	{
		istr.close();
		return false;
	}
	else
	{
		istr.close();
		return true;
	}
}

// Boolean function to show if a file exists and can be opened for reading only.

bool xxFile::DoesFileExistForReadOnly(zString iFileName)
{
	zInFileStream istr(iFileName.c_str(), zIos::in);

	if(istr.fail())
	{
		istr.close();
		return false;
	}
	else
	{
		istr.close();
		return true;
	}
}

// Boolean function to show if a file exists and can be opened for writing only.

bool xxFile::DoesFileExistForWriteOnly(zString iFileName)
{
	zInFileStream istr(iFileName.c_str(), zIos::out);

	if(istr.fail())
	{
		istr.close();
		return false;
	}
	else
	{
		istr.close();
		return true;
	}
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// Default constructor that prevents PN from doing any file IO.
// We don't open/close any file streams here.

xxFile::xxFile() : m_FileName(""), 
				   m_IOFlag(false),
                   m_IOSuccess(false),
                   m_bOpenFlag(false),
                   m_inPos(0),
                   m_outPos(0)
{

}

// Constructor used in the serial code or by P0 to perform file IO.

xxFile::xxFile(zString FileName, bool IOFlag) : m_FileName(FileName), 
												m_IOFlag(IOFlag),
												m_IOSuccess(false),
												m_bOpenFlag(false),
                                                m_inPos(0),
                                                m_outPos(0)
{
    if(!IsParallel() || IsProcessZero())
    {
	    if(m_IOFlag)	// output: we overwrite files that already exist without a prompt
	    {
		    m_outStream.open(m_FileName.c_str());

		    // If the file could not be opened flag an error

		    if(m_outStream.fail())
		    {
			    IOError("Unable to open file for output: " + m_FileName);
			    m_IOSuccess = false;
			    m_bOpenFlag = false;
		    }
		    else
		    {
			    m_IOSuccess = true;
			    m_bOpenFlag = true;
                m_outPos = m_outStream.tellp();
		    }

	    }
	    else			// input
	    {
		    m_inStream.open(m_FileName.c_str());

		    // If the file could not be opened flag an error

		    if(m_inStream.fail())
		    {
			    IOError("Unable to open file for input: " + m_FileName);
			    m_IOSuccess = false;
			    m_bOpenFlag = false;
		    }
		    else
		    {
			    m_IOSuccess = true;
			    m_bOpenFlag = true;
                m_inPos = m_inStream.tellg();
		    }
	    }
    }
}

// We don't bother to reset the file open flag as the file is being destroyed.
// Only the serial code or P0 can close a file.

xxFile::~xxFile()
{
#if SimMPS == SimMPSDisabled
	if(m_IOFlag)
		m_outStream.close();
	else
		m_inStream.close();
#else

    if(IsProcessZero())
    {
	    if(m_IOFlag)
		    m_outStream.close();
	    else
		    m_inStream.close();
    }
#endif
}

// Virtual function to provide error message output when a read/write operation
// fails in any object that inherits from xxFile. It automatically closes the
// stream that had the error.

bool xxFile::IOError(zString errStr)
{
	Trace(errStr);

	if(m_IOFlag)				// write to file
	{
		m_outStream.clear();	// reset stream flag
		m_outStream.close();	// close the stream before passing a fail flag back
	}
	else						// read from file
	{
		m_inStream.clear();
		m_inStream.close();
	}

	m_IOSuccess = false;
	m_bOpenFlag = false;

	return m_IOSuccess;
}

// Function to add spaces to the front and back of a zString.
// This is useful for classes that need to read polymer shape strings.

zString xxFile::AddTerminalSpaces(zString oldString) const
{
	zStringIterator iterFor		= oldString.begin();
	zStringIterator iterBack	= oldString.end();
	iterBack--;

	if(*iterFor != ' ')
	{
		oldString.insert(iterFor, ' ');
	}

	if(*iterBack != ' ')
	{
		oldString += ' ';
	}

	return oldString;
}

// Function to remove spaces from the front and back of a zString. An empty
// string is just returned unchanged. Note that we don't want to remove spaces 
// within the string only at the front and back.

zString xxFile::RemoveTerminalSpaces(zString oldString) const
{
	if(!oldString.empty())
	{
		zStringIterator iterFor		= oldString.begin();
		zStringIterator iterBack	= oldString.end();
		iterBack--;

		for(unsigned long j=0; j<oldString.size(); j++)
		{
			if(*iterFor == ' ')
				oldString.erase(iterFor);	// erase automatically increments iterator
			if(*iterBack == ' ')
			{
				oldString.erase(iterBack);	
				iterBack--;					// move back from end()
			}
		}
	}

	return oldString;
}


// Function to show whether the state of a stream attached to a file is ok.

bool xxFile::IsFileStateOk() const
{
	return m_IOSuccess;
}

// Function showing if a derived class can write to its attached stream.

bool xxFile::IsFileWritable() const
{
    return m_IOFlag;
}

const zString xxFile::GetFileName() const
{
	return m_FileName;
}

// Protected function to allow derived classes to close their attached file
// and thus force data to be written before the simulation ends. We set a
// flag indicating that the file is closed so that further attempts to write
// to it can be trapped as errors.
//
// The function returns an error flag showing whether the file was closed 
// successfully.

bool xxFile::Close()
{
	if(m_IOFlag)
	{
		m_outStream.close();
		if(m_outStream.fail())
		{
			IOError("Error closing output file: " + m_FileName);
			m_IOSuccess = false;
		}
		else
		{
			m_IOSuccess = true;
			m_bOpenFlag = false;
		}
	}
	else
	{
		m_inStream.close();
		if(m_inStream.fail())
		{
			IOError("Error closing input file: " + m_FileName);
			m_IOSuccess = false;
		}
		else
		{
			m_IOSuccess = true;
			m_bOpenFlag = false;
		}
	}

	return m_IOSuccess;
}

