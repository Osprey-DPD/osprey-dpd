// LogctRadialDistribution.h: interface for the CLogctRadialDistribution class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTRADIALDISTRIBUTION_H__7c0d007b_2844_4084_a500_ac8d98f59e59__INCLUDED_)
#define AFX_LOGCTRADIALDISTRIBUTION_H__7c0d007b_2844_4084_a500_ac8d98f59e59__INCLUDED_


#include "LogConstraintMessage.h"

class CLogctRadialDistribution : public CLogConstraintMessage
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctRadialDistribution(long time, const zString targetlabel, const zString decLabel,
                              const zString symmetry, long start, long end, long samples,
                              long xn, long yn, long zn, double xc, double yc, double zc,
                              double shellWidth, double outerRadius);

	virtual ~CLogctRadialDistribution();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctRadialDistribution& rMsg);

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

	CLogctRadialDistribution(const CLogctRadialDistribution& oldMessage);
	CLogctRadialDistribution& operator=(const CLogctRadialDistribution& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_TargetLabel;	// Command target label
    const zString   m_DecLabel;     // Command wrapper label
    const zString   m_Symmetry;     // Symmetry of the distribution: linear, cylindrical, spherical
    const long      m_Start;        // Start time for analysis
    const long      m_End;          // End time for analysis
    const long      m_Samples;      // Multiple of "SamplePeriod" to get data
	const long	    m_X;
	const long	    m_Y;			// Normal vector along cylinder axis
	const long	    m_Z;
	const double	m_XC;
	const double	m_YC;			// Single point on the axis
    const double    m_ZC;
    const double    m_ShellWidth;   // Width of histogram shells
    const double    m_OuterRadius;  // Outer radius of analysis
};

#endif // !defined(AFX_LOGCTRADIALDISTRIBUTION_H__7c0d007b_2844_4084_a500_ac8d98f59e59__INCLUDED_)
