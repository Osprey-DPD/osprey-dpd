// acSetConstantFreeActiveBondConcentration.h: interface for the acSetConstantFreeActiveBondConcentration class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACSETCONSTANTFREEACTIVEBONDCONCENTRATION_H__65876DFC_CCFB_415C_876F_BD54335600BB__INCLUDED_)
#define AFX_ACSETCONSTANTFREEACTIVEBONDCONCENTRATION_H__65876DFC_CCFB_415C_876F_BD54335600BB__INCLUDED_


#include "xxCommand.h" 

class acSetConstantFreeActiveBondConcentration : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	acSetConstantFreeActiveBondConcentration(long executionTime);
	acSetConstantFreeActiveBondConcentration(const acSetConstantFreeActiveBondConcentration& oldCommand);

	virtual ~acSetConstantFreeActiveBondConcentration();
	
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

	inline const zString GetACNType()   const {return m_ACNType;}
	inline double GetConstantFraction() const {return m_Fraction;}
	inline long GetPhantomsPerEvent()   const {return m_PhantomsPerEvent;}


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

    double  m_Fraction;         // Constant fraction of all active bonds
	long    m_PhantomsPerEvent;	// Number of phantom bonds created per event

};

#endif // !defined(AFX_ACSETCONSTANTFREEACTIVEBONDCONCENTRATION_H__65876DFC_CCFB_415C_876F_BD54335600BB__INCLUDED_)
