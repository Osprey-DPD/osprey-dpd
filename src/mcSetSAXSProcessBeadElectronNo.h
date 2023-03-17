// mcSetSAXSProcessBeadElectronNo.h: interface for the mcSetSAXSProcessBeadElectronNo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSETSAXSPROCESSBEADELECTRONNO_H__17639006_c54c_48f9_9c87_2eca1e515f7a__INCLUDED_)
#define AFX_MCSETSAXSPROCESSBEADELECTRONNO_H__17639006_c54c_48f9_9c87_2eca1e515f7a__INCLUDED_


// Forward declarations

class ISimCmd;


#include "xxCommand.h"

class mcSetSAXSProcessBeadElectronNo : public xxCommand
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	mcSetSAXSProcessBeadElectronNo(long executionTime);
	mcSetSAXSProcessBeadElectronNo(const mcSetSAXSProcessBeadElectronNo& oldCommand);

	mcSetSAXSProcessBeadElectronNo(long executionTime, long pid, const zString name, double eno);

	virtual ~mcSetSAXSProcessBeadElectronNo();
	
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

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const;

	virtual const xxCommand* GetCommand() const;

	virtual bool IsDataValid(const CInputData& riData) const;


	// ****************************************
	// Public access functions
public:

    long    GetProcessId() const {return m_Pid;}
	const   zString GetBeadName()  const {return m_Name;}
    double  GetEno() const {return m_Eno;}

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

    long      m_Pid;        // Unique if of the process in the order of creation of all processes
    zString   m_Name;       // Name of bead whose electon number is set
    double    m_Eno;        // Number of electrons in the bead (we allow fractional values)

};

#endif // !defined(AFX_MCSETSAXSPROCESSBEADELECTRONNO_H__17639006_c54c_48f9_9c87_2eca1e515f7a__INCLUDED_)
