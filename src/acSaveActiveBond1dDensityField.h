// acSaveActiveBond1dDensityField.h: interface for the acSaveActiveBond1dDensityField class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACSAVEACTIVEBOND1DDENSITYFIELD_H__7B4B270A_C64F_4A30_B5E0_B768E0B995CC__INCLUDED_)
#define AFX_ACSAVEACTIVEBOND1DDENSITYFIELD_H__7B4B270A_C64F_4A30_B5E0_B768E0B995CC__INCLUDED_


#include "xxCommand.h" 

class acSaveActiveBond1dDensityField : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	acSaveActiveBond1dDensityField(long executionTime);
	acSaveActiveBond1dDensityField(const acSaveActiveBond1dDensityField& oldCommand);

	virtual ~acSaveActiveBond1dDensityField();
	
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

	inline long		GetStartTime()		const {return m_Start;}
	inline long		GetEndTime()		const {return m_End;}
	inline long		GetSamplePeriod()   const {return m_SamplePeriod;}
	inline long		GetAnalysisPeriod() const {return m_AnalysisPeriod;}
	inline long		GetXNormal()		const {return m_NX;}
	inline long		GetYNormal()		const {return m_NY;}
	inline long		GetZNormal()		const {return m_NZ;}
	inline long		GetSliceTotal()		const {return m_SliceTotal;}
	inline double	GetLowerFraction()  const {return m_LowerFraction;}
	inline double	GetUpperFraction()  const {return m_UpperFraction;}

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

	zString m_ACNType;			// Name of ACN to modify

	long	m_Start;			// Time at which density field analysis starts
	long	m_End;				// Time at which density field analysis ends
	long	m_SamplePeriod;		// Number of time steps between data samples
	long	m_AnalysisPeriod;	// Number of time steps between averaging the data

	long	m_NX;
	long	m_NY;				// Direction of density profile
	long	m_NZ;
	long	m_SliceTotal;		// Number of slices in the density profile
	double	m_LowerFraction;	// Minimum fractional value of coordinates in profile direction
	double  m_UpperFraction;	// Maximum    "

};

#endif // !defined(AFX_ACSAVEACTIVEBOND1DDENSITYFIELD_H__7B4B270A_C64F_4A30_B5E0_B768E0B995CC__INCLUDED_)
