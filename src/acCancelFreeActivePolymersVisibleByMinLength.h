// acCancelFreeActivePolymersVisibleByMinLength.h: interface for the acCancelFreeActivePolymersVisibleByMinLength class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACCANCELFREEACTIVEPOLYMERSVISIBLEBYMINLENGTH_H__D47BE9CC_3D5F_4138_AA8F_E5F6907A21FF__INCLUDED_)
#define AFX_ACCANCELFREEACTIVEPOLYMERSVISIBLEBYMINLENGTH_H__D47BE9CC_3D5F_4138_AA8F_E5F6907A21FF__INCLUDED_


#include "xxCommand.h" 

class acCancelFreeActivePolymersVisibleByMinLength : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	acCancelFreeActivePolymersVisibleByMinLength(long executionTime);
	acCancelFreeActivePolymersVisibleByMinLength(const acCancelFreeActivePolymersVisibleByMinLength& oldCommand);

	virtual ~acCancelFreeActivePolymersVisibleByMinLength();
	
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
	inline long          GetMinSize() const {return m_MinSize;}


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

	zString m_ACNType;		 // Name of ACN to modify
	const long    m_MinSize; // Fixed at 0 to indicate restriction is turned off

};

#endif // !defined(AFX_ACCANCELFREEACTIVEPOLYMERSVISIBLEBYMINLENGTH_H__D47BE9CC_3D5F_4138_AA8F_E5F6907A21FF__INCLUDED_)
