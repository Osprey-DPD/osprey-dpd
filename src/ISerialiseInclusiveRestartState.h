// ISerialiseInclusiveRestartState.h: interface for the ISerialiseInclusiveRestartState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISERIALISEINCLUSIVERESTARTSTATE_H__8A769478_6F9A_44B4_8EB5_1994745C5109__INCLUDED_)
#define AFX_ISERIALISEINCLUSIVERESTARTSTATE_H__8A769478_6F9A_44B4_8EB5_1994745C5109__INCLUDED_


// Include files

#include "xxBase.h"


class ISerialiseInclusiveRestartState  
{
	// ****************************************
	// Construction/Destruction
public:

	ISerialiseInclusiveRestartState();
	virtual ~ISerialiseInclusiveRestartState();

	// ****************************************
	// Global functions, static member functions and variables
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

    virtual zInStream&  Read(zInStream& is)         = 0;
    virtual zOutStream& Write(zOutStream& is) const = 0;

	// ****************************************
	// Public access functions
public:

    inline bool IsRestartStateWarningClear()  const {return m_bNoRestartWarnings;}
    inline bool IsRestartStateValid()         const {return m_bValidRestartData;}

	// ****************************************
	// Protected local functions
protected:

	void IssueRestartStateWarningMessage(const zString msg);
	void          SetRestartStateInvalid(const zString msg);

    inline void   ClearRestartStateWarnings() {m_bNoRestartWarnings = true;}
    inline void        SetRestartStateValid() {m_bValidRestartData  = true;}

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

    bool  m_bNoRestartWarnings; // Flag showing if any warning messages have been issued
    bool  m_bValidRestartData;  // Flag showing if restart state file is valid

};

#endif // !defined(AFX_ISERIALISEINCLUSIVERESTARTSTATE_H__8A769478_6F9A_44B4_8EB5_1994745C5109__INCLUDED_)
