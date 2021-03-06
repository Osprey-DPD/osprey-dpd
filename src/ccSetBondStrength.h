// ccSetBondStrength.h: interface for the ccSetBondStrength class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSETBONDSTRENGTH_H__FBB62754_F581_4F43_8011_D463C1313B18__INCLUDED_)
#define AFX_CCSETBONDSTRENGTH_H__FBB62754_F581_4F43_8011_D463C1313B18__INCLUDED_


#include "xxCommand.h"

class ccSetBondStrength : public xxCommand  
{
public:
	ccSetBondStrength(long executionTime);
	ccSetBondStrength(const ccSetBondStrength& oldCommand);

	virtual ~ccSetBondStrength();
	
	// xxCommand functions
	
	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an xxCommand pointer,
	// respond to the SimBox's request to execute and return the name of the command.

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const;

	static const zString GetType();	// Return the type of command

	virtual const xxCommand* GetCommand() const;

	virtual bool IsDataValid(const CInputData& riData) const;

	// Access functions to command's data

	// Local data access functions

	inline const zString GetName()				const {return m_Name;};
	inline		 double	 GetSpringConstant()	const {return m_SpringConstant;}
	inline		 double  GetUnstretchedLength()	const {return m_UnstretchedLength;}

protected:
	virtual const zString GetCommandType() const;

private:
	static const zString m_Type;	// Identifier used in control data file for command

	zString m_Name;					// Name of corresponding CBond object
	double  m_SpringConstant;		// New value of spring constant
	double  m_UnstretchedLength;			// New unstretched length
};
#endif // !defined(AFX_CCSETBONDSTRENGTH_H__FBB62754_F581_4F43_8011_D463C1313B18__INCLUDED_)



