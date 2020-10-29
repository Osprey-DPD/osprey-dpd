// ccAddActiveCellNetwork.h: interface for the ccAddActiveCellNetwork class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCADDACTIVECELLNETWORK_H__288B5FE3_BA07_42A9_BE07_196BC82ADB4D__INCLUDED_)
#define AFX_CCADDACTIVECELLNETWORK_H__288B5FE3_BA07_42A9_BE07_196BC82ADB4D__INCLUDED_


#include "xxCommand.h"

class ccAddActiveCellNetwork : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ccAddActiveCellNetwork(long executionTime);
	ccAddActiveCellNetwork(const ccAddActiveCellNetwork& oldCommand);

	virtual ~ccAddActiveCellNetwork();
	
	// ****************************************
	// Global functions, static member functions and variables
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an xxCommand pointer,
	// respond to the SimBox's request to execute and return the name of the command.

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const = 0;

	virtual const xxCommand* GetCommand() const = 0;

	virtual bool IsDataValid(const CInputData& riData) const;

	// ****************************************
	// Public access functions
public:

	inline long GetXCellNo() const {return m_XCellNo;}
	inline long GetYCellNo() const {return m_YCellNo;}
	inline long GetZCellNo() const {return m_ZCellNo;}

	// ****************************************
	// Protected local functions
protected:

	virtual const zString GetCommandType() const = 0;

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

	long m_XCellNo;		// No of CNT cells in each active cell per dimension
	long m_YCellNo;
	long m_ZCellNo;

};

#endif // !defined(AFX_CCADDACTIVECELLNETWORK_H__288B5FE3_BA07_42A9_BE07_196BC82ADB4D__INCLUDED_)
