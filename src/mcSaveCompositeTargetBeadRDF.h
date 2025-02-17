// mcSaveCompositeTargetBeadRDF.h: interface for the mcSaveCompositeTargetBeadRDF class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSaveCompositeTargetBeadRDF_H__b5c56872_479d_403b_b4b5_5d78dbe8843c__INCLUDED_)
#define AFX_MCSaveCompositeTargetBeadRDF_H__b5c56872_479d_403b_b4b5_5d78dbe8843c__INCLUDED_


#include "xxCommand.h" 

class mcSaveCompositeTargetBeadRDF : public xxCommand
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	mcSaveCompositeTargetBeadRDF(long executionTime);
	mcSaveCompositeTargetBeadRDF(const mcSaveCompositeTargetBeadRDF& oldCommand);

	virtual ~mcSaveCompositeTargetBeadRDF();
	
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

	inline long	  GetAnalysisPeriods()       const {return m_TotalAnalysisPeriods;}
	inline long   GetTotalDataPoints()       const {return m_TotalDataPoints;}
	inline double GetRMax()                  const {return m_RMax;}
    inline const  zString GetTargetName()    const {return m_TargetName;}
    inline const  zString GetBeadName()      const {return m_BeadName;}

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

	long    m_TotalAnalysisPeriods;	 // Number of analysis periods to sample over
	long    m_TotalDataPoints;	     // Number of bins in histogram
	double  m_RMax;                  // Maximum separation to calculate histogram
	
	zString m_TargetName;           // String identifier of selected composite target
	zString m_BeadName;             // String identifier of bead in target whose RDF is required


};

#endif // !defined(AFX_MCSaveCompositeTargetBeadRDF_H__b5c56872_479d_403b_b4b5_5d78dbe8843c__INCLUDED_)
