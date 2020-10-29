// LogToggleDensityFieldOutput.h: interface for the CLogToggleDensityFieldOutput class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGTOGGLEDENSITYFIELDOUTPUT_H__EB99F316_8633_4213_A90A_A64C58F5593C__INCLUDED_)
#define AFX_LOGTOGGLEDENSITYFIELDOUTPUT_H__EB99F316_8633_4213_A90A_A64C58F5593C__INCLUDED_


#include "LogInfoMessage.h"

class CLogToggleDensityFieldOutput : public CLogInfoMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogToggleDensityFieldOutput(long time, bool bSave);

	virtual ~CLogToggleDensityFieldOutput();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogToggleDensityFieldOutput& rMsg);

	// ****************************************
	// Public access functions
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual	void Serialize(zOutStream& os) const;

	// ****************************************
	// Protected local functions
protected:

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:
	
	// Explicitly disallow the copy constructor and assignment operators
	// by declaring them private and providing NO definitions.

	CLogToggleDensityFieldOutput(const CLogToggleDensityFieldOutput& oldMessage);
	CLogToggleDensityFieldOutput& operator=(const CLogToggleDensityFieldOutput& rhs);


	// ****************************************
	// Data members
private:

	const bool m_bSave;	// Flag showing whether density fields are saved or not

};

#endif // !defined(AFX_LOGTOGGLEDENSITYFIELDOUTPUT_H__EB99F316_8633_4213_A90A_A64C58F5593C__INCLUDED_)
