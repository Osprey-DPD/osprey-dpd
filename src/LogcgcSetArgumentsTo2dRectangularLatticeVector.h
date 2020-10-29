// LogcgcSetArgumentsTo2dRectangularLatticeVector.h: interface for the CLogcgcSetArgumentsTo2dRectangularLatticeVector class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCGCSETARGUMENTSTO2DRECTANGULARLATTICEVECTOR_H__9E785221_6667_47A8_8BDE_47ECBDBE9C42__INCLUDED_)
#define AFX_LOGCGCSETARGUMENTSTO2DRECTANGULARLATTICEVECTOR_H__9E785221_6667_47A8_8BDE_47ECBDBE9C42__INCLUDED_


#include "LogInfoMessage.h"

class CLogcgcSetArgumentsTo2dRectangularLatticeVector : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogcgcSetArgumentsTo2dRectangularLatticeVector(long time, const zString groupName, 
                                     const zString xCmdName, const zString yCmdName, 
                                     const zString xArgName, const zString yArgName, 
                                     long xCmdNo, long yCmdNo, long xmax, long ymax,
                                     double xorigin, double yorigin, double lx, double ly);

	virtual ~CLogcgcSetArgumentsTo2dRectangularLatticeVector();		// Public so the CLogState can delete messages

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogcgcSetArgumentsTo2dRectangularLatticeVector& rMsg);

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

	CLogcgcSetArgumentsTo2dRectangularLatticeVector(const CLogcgcSetArgumentsTo2dRectangularLatticeVector& oldMessage);
	CLogcgcSetArgumentsTo2dRectangularLatticeVector& operator=(const CLogcgcSetArgumentsTo2dRectangularLatticeVector& rhs);


	// ****************************************
	// Data members
private:

    const zString  m_GroupName;     // Name of command group
    const zString  m_XCommandName;  // Name of X command 
    const zString  m_YCommandName;  // Name of Y command 
    const zString  m_XArgName;      // Placeholder name of argument being set from X component
    const zString  m_YArgName;      // Placeholder name of argument being set from Y component
    const long     m_XCommandNo;    // Index to X command in the group's command container
    const long     m_YCommandNo;    // Index to Y command in the group's command container
    const long     m_XMax;          // Size of lattive in X dimension
    const long     m_YMax;          // Size of lattive in Y dimension
    const double   m_XOrigin;       // X origin of lattice
    const double   m_YOrigin;       // Y origin of lattice
    const double   m_LX;            // Length of X component of lattice vector
    const double   m_LY;            // Length of Y component of lattice vector


};

#endif // !defined(AFX_LOGCGCSETARGUMENTSTO2DRECTANGULARLATTICEVECTOR_H__9E785221_6667_47A8_8BDE_47ECBDBE9C42__INCLUDED_)
