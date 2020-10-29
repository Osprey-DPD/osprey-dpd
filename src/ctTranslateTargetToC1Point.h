// ctTranslateTargetToC1Point.h: interface for the ctTranslateTargetToC1Point class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTTRANSLATETARGETTOC1POINT_H__746e33b5_c5a5_4e40_8918_3d77f3fcc274__INCLUDED_)
#define AFX_CTTRANSLATETARGETTOC1POINT_H__746e33b5_c5a5_4e40_8918_3d77f3fcc274__INCLUDED_


#include "ctApplyCommand.h"

class ctTranslateTargetToC1Point : public ctApplyCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ctTranslateTargetToC1Point(long executionTime);
	ctTranslateTargetToC1Point(const ctTranslateTargetToC1Point& oldCommand);

	ctTranslateTargetToC1Point(long executionTime, const zString targetLabel, double xc, double yc, double zc);

	virtual ~ctTranslateTargetToC1Point();
	
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

	inline double GetXCentre() const {return m_XC;}
	inline double GetYCentre() const {return m_YC;}
	inline double GetZCentre() const {return m_ZC;}


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

	double	m_XC;       // Fractional distance of point along SimBox in X direction (0 to 1)
	double	m_YC;       // Fractional distance of point along SimBox in Y direction (0 to 1)
	double	m_ZC;       // Fractional distance of point along SimBox in Z direction (0 to 1)

};

#endif // !defined(AFX_CTTRANSLATETARGETTOC1POINT_H__746e33b5_c5a5_4e40_8918_3d77f3fcc274__INCLUDED_)
