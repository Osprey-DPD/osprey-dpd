// mcGenerateRNGGaussian.h: interface for the mcGenerateRNGGaussian class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCGENERATERNGGAUSSIAN_H__7224A841_C437_4EF6_B326_D394F1A73CCB__INCLUDED_)
#define AFX_MCGENERATERNGGAUSSIAN_H__7224A841_C437_4EF6_B326_D394F1A73CCB__INCLUDED_


// Forward declarations

class ISimCmd;


#include "xxCommand.h"

class mcGenerateRNGGaussian : public xxCommand 
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	mcGenerateRNGGaussian(long executionTime);

	mcGenerateRNGGaussian(long executionTime, long size);

	mcGenerateRNGGaussian(const mcGenerateRNGGaussian& oldCommand);

	virtual ~mcGenerateRNGGaussian();
	
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

#endif // !defined(AFX_MCGENERATERNGGAUSSIAN_H__7224A841_C437_4EF6_B326_D394F1A73CCB__INCLUDED_)
