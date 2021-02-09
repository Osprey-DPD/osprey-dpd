// mcSavePolymerBeadRDF.h: interface for the mcSavePolymerBeadRDF class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSAVEPOLYMERBEADRDF_H__c169068d_357e_4549_a21d_0bd834fc64e3__INCLUDED_)
#define AFX_MCSAVEPOLYMERBEADRDF_H__c169068d_357e_4549_a21d_0bd834fc64e3__INCLUDED_


#include "xxCommand.h" 

class mcSavePolymerBeadRDF : public xxCommand
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	mcSavePolymerBeadRDF(long executionTime);
	mcSavePolymerBeadRDF(const mcSavePolymerBeadRDF& oldCommand);

	virtual ~mcSavePolymerBeadRDF();
	
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
    inline const  zString GetPolymerName()   const {return m_PolymerName;}
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
	
	zString m_PolymerName;          // String identifier of selected polymer
	zString m_BeadName;             // String identifier of bead in selected polymer whose RDF is required


};

#endif // !defined(AFX_MCSAVEPOLYMERBEADRDF_H__c169068d_357e_4549_a21d_0bd834fc64e3__INCLUDED_)
