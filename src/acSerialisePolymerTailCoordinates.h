// acSerialisePolymerTailCoordinates.h: interface for the acSerialisePolymerTailCoordinates class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACSERIALISEPOLYMERTAILCOORDINATES_H__8BBAEEF5_8D23_4C14_8B41_A45BE45A9D68__INCLUDED_)
#define AFX_ACSERIALISEPOLYMERTAILCOORDINATES_H__8BBAEEF5_8D23_4C14_8B41_A45BE45A9D68__INCLUDED_


#include "acACNCreateEventAnalysis.h" 

class acSerialisePolymerTailCoordinates : public acACNCreateEventAnalysis  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	acSerialisePolymerTailCoordinates(long executionTime);
	acSerialisePolymerTailCoordinates(const acSerialisePolymerTailCoordinates& oldCommand);

	virtual ~acSerialisePolymerTailCoordinates();
	
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

#endif // !defined(AFX_ACSERIALISEPOLYMERTAILCOORDINATES_H__8BBAEEF5_8D23_4C14_8B41_A45BE45A9D68__INCLUDED_)
