// pmConstantSpecificAreaGhost.h: interface for the pmConstantSpecificAreaGhost class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMCONSTANTSPECIFICAREAGHOST_H__f6eeade3_568e_4e0f_b151_1767afc73b30__INCLUDED_)
#define AFX_PMCONSTANTSPECIFICAREAGHOST_H__f6eeade3_568e_4e0f_b151_1767afc73b30__INCLUDED_


// Forward declarations

class ctConstantSpecificAreaGhost;


#include "mpmCommandWrapper.h"

class pmConstantSpecificAreaGhost : public mpmCommandWrapper  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmConstantSpecificAreaGhost();
	
	pmConstantSpecificAreaGhost(const pmConstantSpecificAreaGhost& oldMessage);

	virtual ~pmConstantSpecificAreaGhost();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmConstantSpecificAreaGhost& message);
	friend zInStream&  operator>>(zInStream& is,        pmConstantSpecificAreaGhost& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmConstantSpecificAreaGhost pointer,

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

    // Function to copy the data from the xxCommand-derived object to the message

	void SetMessageData(const xxCommand* const pCommand);

    // Accessor functions to the message data



    inline const zString GetTargetName()                    const  {return m_TargetName;}
    inline const zString GetGhostLipidName()             const  {return m_GhostLipidName;}
    inline const zString GetRealLipidName()              const  {return m_RealLipidName;}
    inline const zString GetDecLabel()                   const  {return m_DecLabel;}
    inline const zString GetBeadName()                   const  {return m_BeadName;}
    inline long  GetSampleRate()	                     const  {return m_SampleRate;}
    inline double GetTargetAN()	                         const  {return m_TargetAN;}
    inline double GetAreaMethodConst()	                 const  {return m_AreaMethodConst;}

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



    zString  m_TargetName;   // Membrane amphiphile target label
    zString  m_GhostLipidName;       // Water amphiphile target label
    zString  m_RealLipidName;  // Bulk ghost inside the membrane target label
    zString  m_DecLabel;             // Decorator label for the constant membrane area algorithm
    zString  m_BeadName;             // Bead type to use for triangulating the membrane surface area


    long       m_SampleRate;           // Number of time steps between invocations of the decorator
    double     m_TargetAN;             // Area per molecule to maintain for the membrane
    double     m_AreaMethodConst;  // a bool variable to say Triangulated Snapshot be made or not

};

#endif // !defined(AFX_PMCONSTANTSPECIFICAREAGHOST_H__f6eeade3_568e_4e0f_b151_1767afc73b30__INCLUDED_)
