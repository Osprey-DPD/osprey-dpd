// acSetBondBindsForwardConeToPolymerTailEvent.h: interface for the acSetBondBindsForwardConeToPolymerTailEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACSETBONDBINDSFORWARDCONETOPOLYMERTAILEVENT_H__14EB2910_9FF3_48DE_AF8F_BD527F6C5C9B__INCLUDED_)
#define AFX_ACSETBONDBINDSFORWARDCONETOPOLYMERTAILEVENT_H__14EB2910_9FF3_48DE_AF8F_BD527F6C5C9B__INCLUDED_


#include "xxCommand.h" 

class acSetBondBindsForwardConeToPolymerTailEvent : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	acSetBondBindsForwardConeToPolymerTailEvent(long executionTime);
	acSetBondBindsForwardConeToPolymerTailEvent(const acSetBondBindsForwardConeToPolymerTailEvent& oldCommand);

	virtual ~acSetBondBindsForwardConeToPolymerTailEvent();
	
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
	inline double GetConeAngle()      const {return m_ConeAngle;}
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
	double  m_Range;			// Distance out to which a bond will looked for
    double  m_ConeAngle;		// Half-angle made by cone with filament axis (radians <= pi/2)
	double  m_SpringConstant;	// Final value of the transient bond spring constant
	double  m_Length;			// Final value of bond's unstretched length

};

#endif // !defined(AFX_ACSETBONDBINDSFORWARDCONETOPOLYMERTAILEVENT_H__14EB2910_9FF3_48DE_AF8F_BD527F6C5C9B__INCLUDED_)
