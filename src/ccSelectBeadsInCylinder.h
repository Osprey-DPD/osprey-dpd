// ccSelectBeadsInCylinder.h: interface for the ccSelectBeadsInCylinder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSELECTBEADSINCYLINDER_H__15476498_1D73_4EE7_A555_13F86E1F0E53__INCLUDED_)
#define AFX_CCSELECTBEADSINCYLINDER_H__15476498_1D73_4EE7_A555_13F86E1F0E53__INCLUDED_


#include "ccSelection.h"

class ccSelectBeadsInCylinder : public ccSelection  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ccSelectBeadsInCylinder(long executionTime);
	ccSelectBeadsInCylinder(const ccSelectBeadsInCylinder& oldCommand);

	virtual ~ccSelectBeadsInCylinder();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the type of command

	friend bool operator ==(const ccSelectBeadsInCylinder &a, const ccSelectBeadsInCylinder &b);

private:

	static const zString m_Type;	// Identifier used in control data file for command

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const;

	virtual const xxCommand* GetCommand() const;

	virtual bool IsDataValid(const CInputData& riData) const;

	// ****************************************
	// Public access functions
public:

	inline double	GetCentre1()	 const {return m_Centre1;}
	inline double	GetCentre2()	 const {return m_Centre2;}
	inline long		GetLowerEdge()	 const {return m_LowerEdge;}
	inline long		GetUpperEdge()	 const {return m_UpperEdge;}
	inline double	GetInnerRadius() const {return m_InnerRadius;}
	inline double	GetOuterRadius() const {return m_OuterRadius;}

	// ****************************************
	// Protected local functions
protected:

	virtual const zString GetCommandType() const;

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:

	// ****************************************
	// Data members
private:

	long   m_LowerEdge;				// Lower edge as a CNTCell slice index (>= 0)
	long   m_UpperEdge;				// Upper edge       "   (< SimBox length in CNT cells)
	double m_Centre1;				// Central axis of cylinder as fractions of SimBox size
	double m_Centre2;
	double m_InnerRadius;			// Inner radius of cylinder in units of bead diameter
	double m_OuterRadius;			// Outer   "          "
};

#endif // !defined(AFX_CCSELECTBEADSINCYLINDER_H__15476498_1D73_4EE7_A555_13F86E1F0E53__INCLUDED_)
