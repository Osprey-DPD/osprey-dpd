// ccSetBondStiffness.h: interface for the ccSetBondStiffness class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSETBONDSTIFFNESS_H__7F288134_9B81_11D4_BF4C_004095086186__INCLUDED_)
#define AFX_CCSETBONDSTIFFNESS_H__7F288134_9B81_11D4_BF4C_004095086186__INCLUDED_


#include "xxCommand.h"

class ccSetBondStiffness : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ccSetBondStiffness(long executionTime);
	ccSetBondStiffness(const ccSetBondStiffness& oldCommand);

	ccSetBondStiffness(long executionTime, const zString name, double modulus, double phi0);

	virtual ~ccSetBondStiffness();
	
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
	
	inline const zString GetName()			 const {return m_Name;};
	inline		 double	 GetBendingModulus() const {return m_BendingModulus;}
	inline		 double  GetPhi0()			 const {return m_Phi0;}

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

	zString m_Name;					// Name of corresponding CBondPair object
	double m_BendingModulus;		// New value of bending modulus
	double m_Phi0;					// New preferred angle for bond pair
};

#endif // !defined(AFX_CCSETBONDSTIFFNESS_H__7F288134_9B81_11D4_BF4C_004095086186__INCLUDED_)
