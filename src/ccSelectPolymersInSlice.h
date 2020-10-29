// ccSelectPolymersInSlice.h: interface for the ccSelectPolymersInSlice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSELECTPOLYMERSINSLICE_H__0FF39E49_0723_11D4_BF30_004095086186__INCLUDED_)
#define AFX_CCSELECTPOLYMERSINSLICE_H__0FF39E49_0723_11D4_BF30_004095086186__INCLUDED_


#include "ccSelection.h"

class ccSelectPolymersInSlice : public ccSelection  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ccSelectPolymersInSlice(long executionTime);
	ccSelectPolymersInSlice(const ccSelectPolymersInSlice& oldCommand);
	
	virtual ~ccSelectPolymersInSlice();

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

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const;

	virtual const xxCommand* GetCommand() const;

	virtual bool IsDataValid(const CInputData& riData) const;

	// ****************************************
	// Public access functions
public:

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


};

#endif // !defined(AFX_CCSELECTPOLYMERSINSLICE_H__0FF39E49_0723_11D4_BF30_004095086186__INCLUDED_)
