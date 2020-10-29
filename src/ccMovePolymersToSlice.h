// ccMovePolymersToSlice.h: interface for the ccMovePolymersToSlice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCMOVEPOLYMERSTOSLICE_H__EC6EAD53_3006_11D4_BF39_004095086186__INCLUDED_)
#define AFX_CCMOVEPOLYMERSTOSLICE_H__EC6EAD53_3006_11D4_BF39_004095086186__INCLUDED_


#include "ccSelection.h"

class ccMovePolymersToSlice : public ccSelection  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ccMovePolymersToSlice(long executionTime);
	ccMovePolymersToSlice(const ccMovePolymersToSlice& oldCommand);

	virtual ~ccMovePolymersToSlice();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the type of command

	friend bool operator ==(const ccMovePolymersToSlice &a, const ccMovePolymersToSlice &b);

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

	inline long GetSliceIndex() const {return m_SliceIndex;}

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

	long m_SliceIndex;

};

#endif // !defined(AFX_CCMOVEPOLYMERSTOSLICE_H__EC6EAD53_3006_11D4_BF39_004095086186__INCLUDED_)
