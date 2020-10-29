// mcSaveBead1dDensityProfile.h: interface for the mcSaveBead1dDensityProfile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSAVEBEAD1DDENSITYPROFILE_H__4E32617D_1B09_46F7_B0E2_15069082A1E6__INCLUDED_)
#define AFX_MCSAVEBEAD1DDENSITYPROFILE_H__4E32617D_1B09_46F7_B0E2_15069082A1E6__INCLUDED_


#include "xxCommand.h" 

class mcSaveBead1dDensityProfile : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	mcSaveBead1dDensityProfile(long executionTime);
	mcSaveBead1dDensityProfile(const mcSaveBead1dDensityProfile& oldCommand);

	virtual ~mcSaveBead1dDensityProfile();
	
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

	inline long	    GetBeadType()		const {return m_BeadType;}
	inline long		GetStartTime()		const {return m_Start;}
	inline long		GetEndTime()		const {return m_End;}
	inline long		GetSamplePeriod()   const {return m_SamplePeriod;}
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

	long	m_BeadType;			// Bead type to modify
	long	m_Start;			// Time at which density field analysis starts
	long	m_End;				// Time at which density field analysis ends
	long	m_SamplePeriod;		// Number of time steps between data samples

	long	m_NX;
	long	m_NY;				// Direction of density profile
	long	m_NZ;
	long	m_SliceTotal;		// Number of slices in the density profile

	// Following input disabled to avoid ambiguity in the total number beads counted in profile
	const double  m_LowerFraction;	// Minimum fractional value of coordinates in profile direction
	const double  m_UpperFraction;	// Maximum    "

};

#endif // !defined(AFX_MCSAVEBEAD1DDENSITYPROFILE_H__4E32617D_1B09_46F7_B0E2_15069082A1E6__INCLUDED_)
