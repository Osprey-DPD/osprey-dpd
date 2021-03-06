// acSetPolymerDissolvesEvent.h: interface for the acSetPolymerDissolvesEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACSETPOLYMERDISSOLVESEVENT_H__27106ADC_A90E_4A01_8B62_6922532A71D3__INCLUDED_)
#define AFX_ACSETPOLYMERDISSOLVESEVENT_H__27106ADC_A90E_4A01_8B62_6922532A71D3__INCLUDED_


#include "xxCommand.h" 

class acSetPolymerDissolvesEvent : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	acSetPolymerDissolvesEvent(long executionTime);
	acSetPolymerDissolvesEvent(const acSetPolymerDissolvesEvent& oldCommand);

	virtual ~acSetPolymerDissolvesEvent();
	
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

	inline const zString GetACNType() const {return m_ACNType;}

	inline long   GetDuration()       const {return m_Duration;}
	inline double GetSpringConstant() const {return m_SpringConstant;}
	inline double GetLength()         const {return m_Length;}


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

	zString m_ACNType;		// Name of ACN to modify

	long    m_Duration;			// No of steps needed for complete dissolution
	double  m_SpringConstant;	// Final value of the transient bond spring constant
	double  m_Length;			// Final value of bond's unstretched length

};

#endif // !defined(AFX_ACSETPOLYMERDISSOLVESEVENT_H__27106ADC_A90E_4A01_8B62_6922532A71D3__INCLUDED_)
