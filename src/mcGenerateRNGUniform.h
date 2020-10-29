// mcGenerateRNGUniform.h: interface for the mcGenerateRNGUniform class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCGENERATERNGUNIFORM_H__181920D0_790C_4580_8ABB_ED88CD827E05__INCLUDED_)
#define AFX_MCGENERATERNGUNIFORM_H__181920D0_790C_4580_8ABB_ED88CD827E05__INCLUDED_


// Forward declarations

class ISimCmd;


#include "xxCommand.h"

class mcGenerateRNGUniform : public xxCommand 
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	mcGenerateRNGUniform(long executionTime);

	mcGenerateRNGUniform(long executionTime, long size);

	mcGenerateRNGUniform(const mcGenerateRNGUniform& oldCommand);

	virtual ~mcGenerateRNGUniform();
	
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

	inline long GetSequenceSize()	const {return m_SequenceSize;}

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

    long m_SequenceSize;    // Number of random numbers to generate
};

#endif // !defined(AFX_MCGENERATERNGUNIFORM_H__181920D0_790C_4580_8ABB_ED88CD827E05__INCLUDED_)
