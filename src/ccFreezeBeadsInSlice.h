// ccFreezeBeadsInSlice.h: interface for the ccFreezeBeadsInSlice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCFREEZEBEADSINSLICE_H__418DBB7F_287E_40DC_908D_D46E701A0F4E__INCLUDED_)
#define AFX_CCFREEZEBEADSINSLICE_H__418DBB7F_287E_40DC_908D_D46E701A0F4E__INCLUDED_


#include "xxCommand.h"

class ccFreezeBeadsInSlice : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	ccFreezeBeadsInSlice(long executionTime);
	ccFreezeBeadsInSlice(const ccFreezeBeadsInSlice& oldCommand);

	virtual ~ccFreezeBeadsInSlice();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the type of command

private:

	static const zString m_Type;	// Identifier used in control data file for command

	// ****************************************
	// Public access functions
public:

	inline long   GetXNormal()	   const {return m_XN;}
	inline long   GetYNormal()	   const {return m_YN;}
	inline long   GetZNormal()	   const {return m_ZN;}
	inline double GetLowerBound()  const {return m_LowerBound;}
	inline double GetThickness()   const {return m_Thickness;}

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an xxCommand pointer,
	// respond to the SimBox's request to execute and return the name of the command,
	// and validate their data given the CInputData object to get access to the
	// SimBox definitions.

	virtual zOutStream& put(zOutStream& os) const;
	virtual zInStream&  get(zInStream& is);

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const;

	virtual const zString GetCommandType() const;

	virtual const xxCommand* GetCommand()  const;

	virtual bool IsDataValid(const CInputData& riData) const;

	// ****************************************
	// Protected local functions
protected:

	
	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

	long	m_XN;
	long	m_YN;				// Normal to slice
	long	m_ZN;
	double	m_LowerBound;		// in units of bead diameter
	double	m_Thickness;		// in units of bead diameter

};

#endif // !defined(AFX_CCFREEZEBEADSINSLICE_H__418DBB7F_287E_40DC_908D_D46E701A0F4E__INCLUDED_)
