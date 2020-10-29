// ctTranslateTargetToZPlane.h: interface for the ctTranslateTargetToZPlane class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTTRANSLATETARGETTOZPLANE_H__d847975e_9989_4fed_8b98_58d7c3ab509d__INCLUDED_)
#define AFX_CTTRANSLATETARGETTOZPLANE_H__d847975e_9989_4fed_8b98_58d7c3ab509d__INCLUDED_


#include "ctApplyCommand.h"

class ctTranslateTargetToZPlane : public ctApplyCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ctTranslateTargetToZPlane(long executionTime);
	ctTranslateTargetToZPlane(const ctTranslateTargetToZPlane& oldCommand);

	ctTranslateTargetToZPlane(long executionTime, const zString targetLabel, double zc);

	virtual ~ctTranslateTargetToZPlane();
	
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

	double	m_ZC;       // Fractional distance of plane along SimBox in Z direction (0 to 1)

};

#endif // !defined(AFX_CTTRANSLATETARGETTOZPLANE_H__d847975e_9989_4fed_8b98_58d7c3ab509d__INCLUDED_)
