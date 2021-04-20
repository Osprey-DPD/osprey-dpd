// ctCylindricalDistributionOfPolymerTarget.h: interface for the ctCylindricalDistributionOfPolymerTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTCYLINDRICALDISTRIBUTIONOFPOLYMERTARGET_H__f42ca914_6617_493f_8a3c_68eee25dfb28__INCLUDED_)
#define AFX_CTCYLINDRICALDISTRIBUTIONOFPOLYMERTARGET_H__f42ca914_6617_493f_8a3c_68eee25dfb28__INCLUDED_


#include "ctApplyCommand.h"

class ctCylindricalDistributionOfPolymerTarget : public ctApplyCommand
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ctCylindricalDistributionOfPolymerTarget(long executionTime);
	ctCylindricalDistributionOfPolymerTarget(const ctCylindricalDistributionOfPolymerTarget& oldCommand);

	ctCylindricalDistributionOfPolymerTarget(long executionTime, const zString targetLabel,
						  const zString decLabel, long start, long end, long samples,
						  long xn, long yn, long zn, double xc, double yc, double zc, double shellWidth, double outerRadius);

	virtual ~ctCylindricalDistributionOfPolymerTarget();
	
	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	 // Return the type of command
    static long  GetArgumentTotal(); // Return the number of arguments 

private:

	static const zString m_Type;	// Identifier used in control data file for command
    static long  m_ArgumentTotal;   // Number of arguments required not including the execution time

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an xxCommand pointer,
	// respond to the SimBox's request to execute and return the name of the command.

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const;

	virtual const xxCommand* GetCommand() const;

	virtual bool IsDataValid(const CInputData& riData) const;

    virtual long GetCommandArgumentTotal() const {return m_ArgumentTotal;}


	// ****************************************
	// Public access functions
public:

	inline const zString GetDecLabel()	  const {return m_DecLabel;}
    inline long          GetStartTime()   const {return m_Start;}
    inline long          GetEndTime()     const {return m_End;}
    inline long          GetSamples()     const {return m_Samples;}
    inline long          GetXNormal()     const {return m_XN;}
    inline long          GetYNormal()     const {return m_YN;}
	inline long		     GetZNormal()	  const {return m_ZN;}
	inline double		 GetXCentre()	  const {return m_XC;}
	inline double		 GetYCentre()	  const {return m_YC;}
	inline double		 GetZCentre()	  const {return m_ZC;}
    inline double        GetShellWidth()  const {return m_ShellWidth;}
    inline double        GetOuterRadius() const {return m_OuterRadius;}


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

	zString	m_DecLabel;	// Label for the decorator object
    long    m_Start;        // Start time of analysis
    long    m_End;          // End time of analysis
    long    m_Samples;      // Multiple of SamplePeriod at which to sample data
	long	m_XN;
	long	m_YN;			// Unit vector along cylinder axis - currentlyhardwired to 0,0,1
	long	m_ZN;
	double	m_XC;
	double	m_YC;			// Coordinates of axis "origin" as fractions of SimBox sides
	double	m_ZC;
	double	m_ShellWidth;   // Width of the histogram bins
    double  m_OuterRadius;  // Outer radius to which to analyse

};

#endif // !defined(AFX_CTCYLINDRICALDISTRIBUTIONOFPOLYMERTARGET_H__f42ca914_6617_493f_8a3c_68eee25dfb28__INCLUDED_)
