// ccApplyForce.h: interface for the ccApplyForce class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCAPPLYFORCE_H__488D9845_0C5C_11D4_BF30_004095086186__INCLUDED_)
#define AFX_CCAPPLYFORCE_H__488D9845_0C5C_11D4_BF30_004095086186__INCLUDED_


#include "xxCommand.h"

class ccApplyForce : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	virtual ~ccApplyForce();

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

	virtual const zString GetCommandType() const = 0;

	virtual const xxCommand* GetCommand()  const = 0;

	// Not a PVF because it needs to check data common to all force commands

	virtual bool IsDataValid(const CInputData& riData) const;

	// ****************************************
	// Public access functions
public:

	inline const zString GetLabel() const {return m_TargetLabel;}
	inline long GetDuration() const {return m_Duration;}
	inline long GetX() const {return m_X;}
	inline long GetY() const {return m_Y;}
	inline long GetZ() const {return m_Z;}

	// ****************************************
	// Protected local functions
protected:

	// Protected constructor ensures that only subclasses can be instantiated

	ccApplyForce(long executionTime);
	ccApplyForce(const ccApplyForce& oldCommand);

	// Protected local data

protected:

	zString m_TargetLabel;		// Identifier of ccSelection to receive force command
	long m_Duration;			// SimTime over which to apply force
	long m_X;
	long m_Y;					// Direction in which force acts
	long m_Z;

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:

	// ****************************************
	// Data members
private:


};

#endif // !defined(AFX_CCAPPLYFORCE_H__488D9845_0C5C_11D4_BF30_004095086186__INCLUDED_)
