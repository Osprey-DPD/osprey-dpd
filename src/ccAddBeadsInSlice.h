// ccAddBeadsInSlice.h: interface for the ccAddBeadsInSlice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCADDBEADSINSLICE_H__3FE65EB4_0EB5_11D4_BF32_004095086186__INCLUDED_)
#define AFX_CCADDBEADSINSLICE_H__3FE65EB4_0EB5_11D4_BF32_004095086186__INCLUDED_


#include "ccSelection.h"

class ccAddBeadsInSlice : public ccSelection
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ccAddBeadsInSlice(long executionTime);
	ccAddBeadsInSlice(const ccAddBeadsInSlice& oldCommand);

	virtual ~ccAddBeadsInSlice();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the type of command

	friend bool operator ==(const ccAddBeadsInSlice &a, const ccAddBeadsInSlice &b);

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

#endif // !defined(AFX_CCADDBEADSINSLICE_H__3FE65EB4_0EB5_11D4_BF32_004095086186__INCLUDED_)
