// acACNCreateEventAnalysis.h: interface for the acACNCreateEventAnalysis class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACACNCREATEEVENTANALYSIS_H__3CD2864A_318A_4499_823D_0D3841D14688__INCLUDED_)
#define AFX_ACACNCREATEEVENTANALYSIS_H__3CD2864A_318A_4499_823D_0D3841D14688__INCLUDED_


#include "xxCommand.h" 

class acACNCreateEventAnalysis : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: protected constructor defined below
public:


	virtual ~acACNCreateEventAnalysis();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an xxCommand pointer,
	// respond to the SimBox's request to execute and return the name of the command.

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const = 0;

	virtual const zString GetCommandType() const = 0;

	virtual const xxCommand* GetCommand()  const = 0;

	// The following function is not a PVF in this class as it is used by 
	// derived classes to check their common data stored here

	virtual bool IsDataValid(const CInputData& riData) const;

    
	// ****************************************
	// Public access functions
public:

	inline const zString GetEventAnalysisId() const {return m_AnalysisId;}
	inline const zString GetEventSourceId()   const {return m_SourceId;}
	inline const zString GetKey()             const {return m_Key;}


	// ****************************************
	// Protected local functions
protected:

 	// Protected constructor ensures that only subclasses can be instantiated

    acACNCreateEventAnalysis(long executionTime);
	acACNCreateEventAnalysis(const acACNCreateEventAnalysis& oldCommand);


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

	zString m_AnalysisId;  // Name of the newly-created event analysis instance that wraps the ESD
	zString m_SourceId;    // Name of the ESD instance that wraps the ACNtarget
	zString m_Key;         // Unique key to select which EAD interface to call


};

#endif // !defined(AFX_ACACNCREATEEVENTANALYSIS_H__3CD2864A_318A_4499_823D_0D3841D14688__INCLUDED_)
