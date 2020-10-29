// ccAddProfilinBond.h: interface for the ccAddProfilinBond class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCADDPROFILINBOND_H__1BEC4147_1392_4F32_8B64_6BCED281E081__INCLUDED_)
#define AFX_CCADDPROFILINBOND_H__1BEC4147_1392_4F32_8B64_6BCED281E081__INCLUDED_


#include "ccAddActiveCellNetwork.h"

class ccAddProfilinBond : public ccAddActiveCellNetwork  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ccAddProfilinBond(long executionTime);
	ccAddProfilinBond(const ccAddProfilinBond& oldCommand);

	virtual ~ccAddProfilinBond();
	
	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the type of command

	
private:

	static const zString m_Type;	// Identifier used in control data file for command


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

	// ****************************************
	// Public access functions
public:

	inline const zString GetHeadMonomerName()	const {return m_HeadMonomerName;}
	inline const zString GetTailMonomerName()	const {return m_TailMonomerName;}
	inline  double		  GetSpringConstant()	const {return m_SpringConstant;}
	inline  double	   GetUnstretchedLength()	const {return m_UnstretchedLength;}
	inline  double		  GetBendingModulus()	const {return m_BendingModulus;}
	inline  double					GetPhi0()	const {return m_Phi0;}

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

	zString m_HeadMonomerName;	// Name of polymer type bound to this bond type
	zString m_TailMonomerName;	// Name of polymer type wrapped by this bond type

	double m_SpringConstant;	// Parameters defining the bond's force law
	double m_UnstretchedLength;
	double m_BendingModulus;
	double m_Phi0;

};

#endif // !defined(AFX_CCADDPROFILINBOND_H__1BEC4147_1392_4F32_8B64_6BCED281E081__INCLUDED_)
