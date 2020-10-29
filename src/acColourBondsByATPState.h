// acColourBondsByATPState.h: interface for the acColourBondsByATPState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACCOLOURBONDSBYATPSTATE_H__E639CF46_050E_4923_BBEC_48A7623C2F09__INCLUDED_)
#define AFX_ACCOLOURBONDSBYATPSTATE_H__E639CF46_050E_4923_BBEC_48A7623C2F09__INCLUDED_


#include "xxCommand.h" 

class acColourBondsByATPState : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	acColourBondsByATPState(long executionTime);
	acColourBondsByATPState(const acColourBondsByATPState& oldCommand);

	virtual ~acColourBondsByATPState();
	
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

	inline long GetATPDisplayId()   const {return m_ATPDisplayId;}
	inline long GetADPPiDisplayId() const {return m_ADPPiDisplayId;}
	inline long GetADPDisplayId()   const {return m_ADPDisplayId;}


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

	long  m_ATPDisplayId;    // Display Id for ATP state
	long  m_ADPPiDisplayId;  // Display Id for ADP-Pi state
	long  m_ADPDisplayId;    // Display Id for ADP state

};

#endif // !defined(AFX_ACCOLOURBONDSBYATPSTATE_H__E639CF46_050E_4923_BBEC_48A7623C2F09__INCLUDED_)
