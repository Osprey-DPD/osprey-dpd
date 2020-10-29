// cgcSetArgumentsTo2dRectangularLatticeVector.h: interface for the cgcSetArgumentsTo2dRectangularLatticeVector class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGCSETARGUMENTSTO2DRECTANGULARLATTICEVECTOR_H__FD8B44BF_E2F9_4A21_96EE_59748A68266D__INCLUDED_)
#define AFX_CGCSETARGUMENTSTO2DRECTANGULARLATTICEVECTOR_H__FD8B44BF_E2F9_4A21_96EE_59748A68266D__INCLUDED_



#include "cgcSetArgumentsTo2dRealLatticeVector.h"

class cgcSetArgumentsTo2dRectangularLatticeVector : public cgcSetArgumentsTo2dRealLatticeVector  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	cgcSetArgumentsTo2dRectangularLatticeVector(long executionTime);
	cgcSetArgumentsTo2dRectangularLatticeVector(const cgcSetArgumentsTo2dRectangularLatticeVector& oldCommand);

	cgcSetArgumentsTo2dRectangularLatticeVector(long executionTime, const zString groupName, 
                         long cmdIndexX, long cmdIndexY, const zString argNameX ,const zString argNameY,
                         long xmax, long ymax, double xorigin, double yorigin, double lx, double ly);

	virtual ~cgcSetArgumentsTo2dRectangularLatticeVector();
	
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

#endif // !defined(AFX_CGCSETARGUMENTSTO2DRECTANGULARLATTICEVECTOR_H__FD8B44BF_E2F9_4A21_96EE_59748A68266D__INCLUDED_)
