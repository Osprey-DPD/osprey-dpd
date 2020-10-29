// cgcSetArgumentsTo3dRectangularLatticeVector.h: interface for the cgcSetArgumentsTo3dRectangularLatticeVector class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGCSETARGUMENTSTO3DRECTANGULARLATTICEVECTOR_H__BE15B33C_60CF_4ACB_B034_1DD7167DEF63__INCLUDED_)
#define AFX_CGCSETARGUMENTSTO3DRECTANGULARLATTICEVECTOR_H__BE15B33C_60CF_4ACB_B034_1DD7167DEF63__INCLUDED_



#include "cgcSetArgumentsTo3dRealLatticeVector.h"

class cgcSetArgumentsTo3dRectangularLatticeVector : public cgcSetArgumentsTo3dRealLatticeVector  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	cgcSetArgumentsTo3dRectangularLatticeVector(long executionTime);
	cgcSetArgumentsTo3dRectangularLatticeVector(const cgcSetArgumentsTo3dRectangularLatticeVector& oldCommand);

	cgcSetArgumentsTo3dRectangularLatticeVector(long executionTime, const zString groupName, 
                         long cmdIndexX, long cmdIndexY, long cmdIndexZ,
                         const zString argNameX, const zString argNameY, const zString argNameZ,
                         long xmax, long ymax, long zmax, 
                         double xorigin, double yorigin, double zorigin, 
                         double lx, double ly, double lz);

	virtual ~cgcSetArgumentsTo3dRectangularLatticeVector();
	
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

#endif // !defined(AFX_CGCSETARGUMENTSTO3DRECTANGULARLATTICEVECTOR_H__BE15B33C_60CF_4ACB_B034_1DD7167DEF63__INCLUDED_)
