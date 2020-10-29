// pmInputData.h: interface for the pmInputData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMINPUTDATA_H__E1449E6A_8CAA_4F7C_A1AB_1FC1EEF79F89__INCLUDED_)
#define AFX_PMINPUTDATA_H__E1449E6A_8CAA_4F7C_A1AB_1FC1EEF79F89__INCLUDED_


// Forward declarations

class CInputData;


#include "mpmMessage.h"

class pmInputData : public mpmMessage  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmInputData();
	
	pmInputData(const pmInputData& oldMessage);

	virtual ~pmInputData();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmInputData& message);
	friend zInStream&  operator>>(zInStream& is,        pmInputData& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmInputData pointer,

	virtual zOutStream& put(zOutStream& os) const;
	virtual zInStream&  get(zInStream& is);

	virtual const mpmMessage* GetMessage()  const;

    // ********************
    // Messaging interface that defines PVFs that must be overridden by derived classes,
    // and VFs that are overridden  when the derived class wants to call the 
    // corresponding MPI function.

	virtual void BuildDerivedType(MsgDataTypePtr pMsgDataType);

    virtual bool Validate();

    virtual void SendAllP();
    virtual void Receive();
    virtual void Receive(long procId);

	// ****************************************
	// Public access functions
public:

    // Function to copy the data from the input data object to the message instance

	void SetMessageData(const CInputData* const pData);

    // Accessor functions to the message data for extraction by the input data object

    inline  long GetBeadTypeTotal()        const {return m_BeadTypes;}
    inline  long GetBondTypeTotal()        const {return m_BondTypes;}
    inline  long GetBondPairTypeTotal()    const {return m_BondPairTypes;}
    inline  long GetPolymerTypeTotal()     const {return m_PolymerTypes;}
    inline  long GetRNGSeed()              const {return m_RNGSeed;}
    inline  long GetTotalMCTime()          const {return m_TotalMCTime;}  // MD only
    inline  long GetTotalTime()            const {return m_TotalTime;}
    inline  long GetSamplePeriod()         const {return m_SamplePeriod;}
    inline  long GetAnalysisPeriod()       const {return m_AnalysisPeriod;}
    inline  long GetDensityPeriod()        const {return m_DensityPeriod;}
    inline  long GetDisplayPeriod()        const {return m_DisplayPeriod;}
    inline  long GetRestartPeriod()        const {return m_RestartPeriod;}
    inline  long GetProcessorsXNo()        const {return m_ProcessorsXNo;}
    inline  long GetProcessorsYNo()        const {return m_ProcessorsYNo;}
    inline  long GetProcessorsZNo()        const {return m_ProcessorsZNo;}
    inline  long GetCNTXCellNo()           const {return m_CNTXCellNo;}
    inline  long GetCNTYCellNo()           const {return m_CNTYCellNo;}
    inline  long GetCNTZCellNo()           const {return m_CNTZCellNo;}
    inline  long GetGridXCellNo()          const {return m_GridXCellNo;}
    inline  long GetGridYCellNo()          const {return m_GridYCellNo;}
    inline  long GetGridZCellNo()          const {return m_GridZCellNo;}

    inline  double GetBeadDensity()        const {return m_BeadDensity;}
    inline  double GetTemperature()        const {return m_Temperature;}
    inline  double GetLambda()             const {return m_Lambda;}  // BD, DPD only
    inline  double GetRCutOff()            const {return m_RCutOff;}  // MD only
    inline  double GetMCStepSize()         const {return m_MCStepSize;}  // MD only
    inline  double GetStepSize()           const {return m_StepSize;}
    inline  double GetCNTXCellWidth()      const {return m_CNTXCellWidth;}
    inline  double GetCNTYCellWidth()      const {return m_CNTYCellWidth;}
    inline  double GetCNTZCellWidth()      const {return m_CNTZCellWidth;}

	// ****************************************
	// Protected local functions
protected:

	virtual const zString GetMessageType() const;

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

    long    m_BeadTypes;
    long    m_BondTypes;
    long    m_BondPairTypes;
    long    m_PolymerTypes;
    long    m_RNGSeed;
    long    m_TotalTime;
    long    m_SamplePeriod;
    long    m_AnalysisPeriod;
    long    m_DensityPeriod;
    long    m_DisplayPeriod;
    long    m_RestartPeriod;

    long    m_ProcessorsXNo;
    long    m_ProcessorsYNo;
    long    m_ProcessorsZNo;
    long    m_CNTXCellNo;
    long    m_CNTYCellNo;
    long    m_CNTZCellNo;
    long    m_GridXCellNo;
    long    m_GridYCellNo;
    long    m_GridZCellNo;

    double  m_BeadDensity;
    double  m_Temperature;
    double  m_StepSize;
    double  m_CNTXCellWidth;
    double  m_CNTYCellWidth;
    double  m_CNTZCellWidth;

    // BD and DPD-only data
    double  m_Lambda;

    // MD-only data
    long    m_TotalMCTime; 
    double  m_RCutOff;     
    double  m_MCStepSize; 
};

#endif // !defined(AFX_PMINPUTDATA_H__E1449E6A_8CAA_4F7C_A1AB_1FC1EEF79F89__INCLUDED_)
