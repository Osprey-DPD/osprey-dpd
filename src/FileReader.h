// FileReader.h: interface for the CFileReader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEREADER_H__336DFCBE_B778_479C_9DEF_A9D52135C827__INCLUDED_)
#define AFX_FILEREADER_H__336DFCBE_B778_479C_9DEF_A9D52135C827__INCLUDED_


#include "xxFile.h"

class CFileReader : public xxFile
{
	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	virtual ~CFileReader();

	// ****************************************
	// Global functions, static member functions and variables
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual bool Serialize() = 0;

	// ****************************************
	// Public access functions
public:

	// ****************************************
	// Protected local functions
protected:

	// Protected constructor ensures that only subclasses can be instantiated

	CFileReader(zString FileName);
	
	// ****************************************
	// Implementation

protected:


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

};

#endif // !defined(AFX_FILEREADER_H__336DFCBE_B778_479C_9DEF_A9D52135C827__INCLUDED_)
