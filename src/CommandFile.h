// CommandFile.h: interface for the CCommandFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMANDFILE_H__869320B2_7CFD_4F92_8735_58A75826982A__INCLUDED_)
#define AFX_COMMANDFILE_H__869320B2_7CFD_4F92_8735_58A75826982A__INCLUDED_


// Forward declarations

class ISimBox;

// Include files


#include "xxFile.h"

class CCommandFile : public xxFile
{
	// ****************************************
	// Construction/Destruction
public:

	CCommandFile(const ISimBox* const pISimBox,
		         const zString protocol, const zString runId, const zString extension,
				 CommandSequence commands);

	virtual ~CCommandFile();

	// ****************************************
	// Global functions, static member functions and variables
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual bool Serialize();

	// ****************************************
	// Public access functions
public:

	// ****************************************
	// Protected local functions
protected:


	// ****************************************
	// Implementation

protected:

	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members

private:

	const ISimBox* const m_pISimBox;	// ISimBox pointer for access to polymer data

	// Simulation Data
	// ***************

	const zString m_Protocol;		// Protocol identifier
	const zString m_RunId;			// Control data file suffix
	const zString m_Extension;		// Command file suffix (".xml" or ".dat" Note the "."!)

	CommandSequence		m_Commands; // Container of commands to be serialised

};

#endif // !defined(AFX_COMMANDFILE_H__869320B2_7CFD_4F92_8735_58A75826982A__INCLUDED_)
