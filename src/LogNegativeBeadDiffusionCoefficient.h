// LogNegativeBeadDiffusionCoefficient.h: interface for the CLogNegativeBeadDiffusionCoefficient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGNEGATIVEBEADDIFFUSIONCOEFFICIENT_H__A2B49F28_F2B9_466B_B3F6_53AFE896DCF4__INCLUDED_)
#define AFX_LOGNEGATIVEBEADDIFFUSIONCOEFFICIENT_H__A2B49F28_F2B9_466B_B3F6_53AFE896DCF4__INCLUDED_


#include "LogErrorMessage.h" 

class CLogNegativeBeadDiffusionCoefficient : public CLogErrorMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogNegativeBeadDiffusionCoefficient(long time, const zString name, long type);

	virtual ~CLogNegativeBeadDiffusionCoefficient();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogNegativeBeadDiffusionCoefficient& rMsg);

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

	CLogNegativeBeadDiffusionCoefficient(const CLogNegativeBeadDiffusionCoefficient& oldMessage);
	CLogNegativeBeadDiffusionCoefficient& operator=(const CLogNegativeBeadDiffusionCoefficient& rhs);

	// ****************************************
	// Data members
private:

	const zString m_BeadName;	// Name of bead type to hide/display
	const long    m_BeadType;	// Bead type to hide/display
};

#endif // !defined(AFX_LOGNEGATIVEBEADDIFFUSIONCOEFFICIENT_H__A2B49F28_F2B9_466B_B3F6_53AFE896DCF4__INCLUDED_)
