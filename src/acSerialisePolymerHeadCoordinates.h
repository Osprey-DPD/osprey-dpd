// acSerialisePolymerHeadCoordinates.h: interface for the acSerialisePolymerHeadCoordinates class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACSERIALISEPOLYMERHEADCOORDINATES_H__79D82C22_BFEE_4A17_9F9D_518A4DD0FD0E__INCLUDED_)
#define AFX_ACSERIALISEPOLYMERHEADCOORDINATES_H__79D82C22_BFEE_4A17_9F9D_518A4DD0FD0E__INCLUDED_


#include "acACNCreateEventAnalysis.h" 

class acSerialisePolymerHeadCoordinates : public acACNCreateEventAnalysis  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	acSerialisePolymerHeadCoordinates(long executionTime);
	acSerialisePolymerHeadCoordinates(const acSerialisePolymerHeadCoordinates& oldCommand);

	virtual ~acSerialisePolymerHeadCoordinates();
	
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

    inline long   GetPeriod()    const {return m_Period;}

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

    long m_Period;   // Number of time steps between data points


};

#endif // !defined(AFX_ACSERIALISEPOLYMERHEADCOORDINATES_H__79D82C22_BFEE_4A17_9F9D_518A4DD0FD0E__INCLUDED_)
