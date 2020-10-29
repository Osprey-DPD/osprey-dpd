// mcComment.h: interface for the mcComment class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCCOMMENT_H__C740B7FB_008E_4879_AA9B_3187FCC9BF46__INCLUDED_)
#define AFX_MCCOMMENT_H__C740B7FB_008E_4879_AA9B_3187FCC9BF46__INCLUDED_


// Forward declarations

class ISimCmd;


#include "xxCommand.h"

class mcComment : public xxCommand 
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	mcComment(long executionTime);

	mcComment(long executionTime, const zString start, const zString end);

	mcComment(long executionTime, const zString message);

	mcComment(const mcComment& oldCommand);

	virtual ~mcComment();
	
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

	inline const zString GetMessage()	const {return m_Message;}

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

    const zString m_StartToken; // Token indicating start of comment
    const zString m_EndToken;   // Token indicating end of comment

	zString	m_Message;	// Text message to be written to the log file
};

#endif // !defined(AFX_MCCOMMENT_H__C740B7FB_008E_4879_AA9B_3187FCC9BF46__INCLUDED_)
