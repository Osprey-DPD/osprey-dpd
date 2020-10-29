// acSetPolymerFormsEvent.h: interface for the acSetPolymerFormsEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACSETPOLYMERFORMSEVENT_H__CA04E5F8_C67D_4F0A_8D3F_E30062B586CC__INCLUDED_)
#define AFX_ACSETPOLYMERFORMSEVENT_H__CA04E5F8_C67D_4F0A_8D3F_E30062B586CC__INCLUDED_


#include "xxCommand.h" 

class acSetPolymerFormsEvent : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	acSetPolymerFormsEvent(long executionTime);
	acSetPolymerFormsEvent(const acSetPolymerFormsEvent& oldCommand);

	virtual ~acSetPolymerFormsEvent();
	
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

	inline long   GetDuration()       const {return m_Duration;}
	inline double GetRange()          const {return m_Range;}
	inline double GetSpringConstant() const {return m_SpringConstant;}
	inline double GetLength()         const {return m_Length;}


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

	long    m_Duration;			// No of steps needed for complete binding
	double  m_Range;			// Distance out to which second bond will be captured
	double  m_SpringConstant;	// Final value of the transient bond spring constant
	double  m_Length;			// Final value of bond's unstretched length

};

#endif // !defined(AFX_ACSETPOLYMERFORMSEVENT_H__CA04E5F8_C67D_4F0A_8D3F_E30062B586CC__INCLUDED_)
