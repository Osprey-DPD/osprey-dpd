// LogcgcSetArgumentsTo2dRealRandomVector.h: interface for the CLogcgcSetArgumentsTo2dRealRandomVector class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCGCSETARGUMENTSTO2DREALRANDOMVECTOR_H__9A36D52F_03BD_427B_A04A_F5BF962C1FC4__INCLUDED_)
#define AFX_LOGCGCSETARGUMENTSTO2DREALRANDOMVECTOR_H__9A36D52F_03BD_427B_A04A_F5BF962C1FC4__INCLUDED_


#include "LogInfoMessage.h"

class CLogcgcSetArgumentsTo2dRealRandomVector : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogcgcSetArgumentsTo2dRealRandomVector(long time, const zString groupName, const zString cmdName, 
                                     long cmdNo, const zString argName, double value);

	virtual ~CLogcgcSetArgumentsTo2dRealRandomVector();		// Public so the CLogState can delete messages

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogcgcSetArgumentsTo2dRealRandomVector& rMsg);

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

	CLogcgcSetArgumentsTo2dRealRandomVector(const CLogcgcSetArgumentsTo2dRealRandomVector& oldMessage);
	CLogcgcSetArgumentsTo2dRealRandomVector& operator=(const CLogcgcSetArgumentsTo2dRealRandomVector& rhs);


	// ****************************************
	// Data members
private:

    const zString  m_GroupName;     // Name of command group
    const zString  m_CommandName;   // Name of command 
    const zString  m_ArgName;       // Placeholder name of argument being set
    const long     m_CommandNo;     // Index to command in the group's command container
    const double   m_Value;         // Real value of the argument

};

#endif // !defined(AFX_LOGCGCSETARGUMENTSTO2DREALRANDOMVECTOR_H__9A36D52F_03BD_427B_A04A_F5BF962C1FC4__INCLUDED_)
