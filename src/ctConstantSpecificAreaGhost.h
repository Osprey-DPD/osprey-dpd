// ctConstantSpecificAreaGhost.h: interface for the ctConstantSpecificAreaGhost class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTCONSTANTSPECIFICAREAGHOST_H_bbfad9ec_84b3_4dbc_9bd5_458a48bb51bc_INCLUDED_)
#define AFX_CTCONSTANTSPECIFICAREAGHOST_H_bbfad9ec_84b3_4dbc_9bd5_458a48bb51bc_INCLUDED_

 
#include "xxCommand.h"

class ctConstantSpecificAreaGhost : public xxCommand
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ctConstantSpecificAreaGhost(long executionTime);
 
	ctConstantSpecificAreaGhost(long executionTime, const zString membraneName, const zString reservoirName, 
	                            const zString decLabel, const zString beadName, long sampleRate, double targetAN);
 
	ctConstantSpecificAreaGhost(const ctConstantSpecificAreaGhost& oldCommand);

	virtual ~ctConstantSpecificAreaGhost();
	
	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	 // Return the type of command
    static long  GetArgumentTotal(); // Return the number of arguments 

private:

	static const zString m_Type;	// Identifier used in control data file for command
    static long  m_ArgumentTotal;   // Number of arguments required not including the execution time


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

    virtual long GetCommandArgumentTotal() const {return m_ArgumentTotal;}

	virtual bool Pack(const tguArgumentSequence& vArguments);

	// ****************************************
	// Public access functions
public:

    inline const zString GetMembraneTargetName()  const  {return m_MembraneTargetName;}
    inline const zString GetReservoirTargetName() const  {return m_ReservoirTargetName;}
    inline const zString GetDecLabel()            const  {return m_DecLabel;}
    inline const zString GetBeadName()            const  {return m_BeadName;}
 
    inline long GetSampleRate()	                  const {return m_SampleRate;}
    inline double GetTargetAN()	                  const {return m_TargetAN;}


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

    zString  m_MembraneTargetName;   // Membrane amphiphile target label
    zString  m_ReservoirTargetName;  // Bulk ghost reservoir target label
    zString  m_DecLabel;             // Decorator label for the constant membrane area algorithm
    zString  m_BeadName;             // Bead type to use for triangulating the membrane surface area

    long     m_SampleRate;           // Number of time steps between invocations of the decorator
    double   m_TargetAN;             // Area per molecule to maintain for the membrane
	
};

#endif // !defined(AFX_CTCONSTANTSPECIFICAREAGHOST_H_bbfad9ec_84b3_4dbc_9bd5_458a48bb51bc_INCLUDED_)
