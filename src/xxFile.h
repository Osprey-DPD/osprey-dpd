// xxFile.h: interface for the xxFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXFILE_H__B5F919C0_5A24_11D3_820E_0060088AD300__INCLUDED_)
#define AFX_XXFILE_H__B5F919C0_5A24_11D3_820E_0060088AD300__INCLUDED_


#include "xxParallelBase.h"

class xxFile : public xxParallelBase  
{
	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	virtual ~xxFile();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static bool DoesFileExist(zString iFileName);
	static bool DoesFileExistForReadOnly(zString iFileName);
	static bool DoesFileExistForWriteOnly(zString iFileName);

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual bool Serialize() = 0;

protected:
	
	virtual bool IOError(zString errStr);

	// ****************************************
	// Public access functions
public:

	const zString GetFileName() const;
	bool IsFileStateOk() const;
	bool IsFileWritable() const;

    zFileStream::pos_type GetCurrentReadPos()  const {return m_inPos;}
    zFileStream::pos_type GetCurrentWritePos() const {return m_outPos;}


	// ****************************************
	// Protected local functions
protected:

	// Protected constructors ensure that only subclasses can be instantiated

	xxFile();   // Default constructor to prevent PN from any file IO

	xxFile(zString FileName, bool IOFlag);

	zString AddTerminalSpaces(zString oldString) const;

	zString RemoveTerminalSpaces(zString oldString) const;

	bool Close();

	// ****************************************
	// Implementation
	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
protected:

	zString m_FileName;     // Name of file attached to the stream

	bool m_IOFlag;			// true = write to file, false = read from file
	bool m_IOSuccess;		// true = no error, false = error
	bool m_bOpenFlag;		// Flag showing whether the file is open or not

    zFileStream::pos_type m_inPos;     // Stream position to hold current read position
    zFileStream::pos_type m_outPos;    // Stream position to hold current write position

	zInFileStream  m_inStream;
	zOutFileStream m_outStream;
};

#endif // !defined(AFX_XXFILE_H__B5F919C0_5A24_11D3_820E_0060088AD300__INCLUDED_)


