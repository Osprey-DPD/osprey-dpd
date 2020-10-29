// IModifyProcess.h: interface for the IModifyProcess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMODIFYPROCESS_H__99E9C609_27F9_466B_86F1_4E4DE30BCB51__INCLUDED_)
#define AFX_IMODIFYPROCESS_H__99E9C609_27F9_466B_86F1_4E4DE30BCB51__INCLUDED_


#include "xxProcess.h" 

class IModifyProcess : public xxProcess
{
	// ****************************************
	// Construction/Destruction: ctor below is needed to pass 
	// data to the base class.
public:

	virtual ~IModifyProcess();

	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The IsModifiable() function allows derived classes to explicitly state 
	// whether they allow modification or not; the remaining functions allow
	// the modification state of the process to be changed by command.
	// Although no arguments are necessary, we include the command object so
	// that the execution time is available to the target process.

	virtual bool IsModifiable() const = 0;

	virtual void SetModifiable(const xxCommand* const pCommand) = 0;
	virtual void SetUnmodifiable(const xxCommand* const pCommand) = 0;
	virtual void SetIrreversibleUnmodifiable(const xxCommand* const pCommand) = 0;

	// ****************************************
	// Public access functions
public:

	// ****************************************
	// Protected local functions
protected:

	IModifyProcess();

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members: No member variables as this is an interface class
private:


};

#endif // !defined(AFX_IMODIFYPROCESS_H__99E9C609_27F9_466B_86F1_4E4DE30BCB51__INCLUDED_)

