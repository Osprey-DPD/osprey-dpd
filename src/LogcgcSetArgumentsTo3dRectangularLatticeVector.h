// LogcgcSetArgumentsTo3dRectangularLatticeVector.h: interface for the CLogcgcSetArgumentsTo3dRectangularLatticeVector class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCGCSETARGUMENTSTO3DRECTANGULARLATTICEVECTOR_H__D1AD5B53_E9FA_4A21_B590_D9E11B2DA56A__INCLUDED_)
#define AFX_LOGCGCSETARGUMENTSTO3DRECTANGULARLATTICEVECTOR_H__D1AD5B53_E9FA_4A21_B590_D9E11B2DA56A__INCLUDED_


#include "LogInfoMessage.h"

class CLogcgcSetArgumentsTo3dRectangularLatticeVector : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogcgcSetArgumentsTo3dRectangularLatticeVector(long time, const zString groupName, 
                                     const zString xCmdName, const zString yCmdName, const zString zCmdName, 
                                     const zString xArgName, const zString yArgName, const zString zArgName, 
                                     long xCmdNo, long yCmdNo, long zCmdNo, long xmax, long ymax, long zmax,
                                     double xorigin, double yorigin, double zorigin, double lx, double ly, double lz);

	virtual ~CLogcgcSetArgumentsTo3dRectangularLatticeVector();		// Public so the CLogState can delete messages

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogcgcSetArgumentsTo3dRectangularLatticeVector& rMsg);

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

	CLogcgcSetArgumentsTo3dRectangularLatticeVector(const CLogcgcSetArgumentsTo3dRectangularLatticeVector& oldMessage);
	CLogcgcSetArgumentsTo3dRectangularLatticeVector& operator=(const CLogcgcSetArgumentsTo3dRectangularLatticeVector& rhs);


	// ****************************************
	// Data members
private:

    const zString  m_GroupName;     // Name of command group
    const zString  m_XCommandName;  // Name of X command 
    const zString  m_YCommandName;  // Name of Y command 
    const zString  m_ZCommandName;  // Name of Z command 
    const zString  m_XArgName;      // Placeholder name of argument being set from X component
    const zString  m_YArgName;      // Placeholder name of argument being set from Y component
    const zString  m_ZArgName;      // Placeholder name of argument being set from Z component
    const long     m_XCommandNo;    // Index to X command in the group's command container
    const long     m_YCommandNo;    // Index to Y command in the group's command container
    const long     m_ZCommandNo;    // Index to Z command in the group's command container
    const long     m_XMax;          // Size of lattive in X dimension
    const long     m_YMax;          // Size of lattive in Y dimension
    const long     m_ZMax;          // Size of lattive in Z dimension
    const double   m_XOrigin;       // X origin of lattice
    const double   m_YOrigin;       // Y origin of lattice
    const double   m_ZOrigin;       // Z origin of lattice
    const double   m_LX;            // Length of X component of lattice vector
    const double   m_LY;            // Length of Y component of lattice vector
    const double   m_LZ;            // Length of Z component of lattice vector


};

#endif // !defined(AFX_LOGCGCSETARGUMENTSTO3DRECTANGULARLATTICEVECTOR_H__D1AD5B53_E9FA_4A21_B590_D9E11B2DA56A__INCLUDED_)
