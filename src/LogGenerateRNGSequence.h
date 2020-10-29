// LogGenerateRNGSequence.h: interface for the CLogGenerateRNGSequence class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGGENERATERNGSEQUENCE_H__ADCF1532_6AE8_48DB_939F_BC29FDA8D831__INCLUDED_)
#define AFX_LOGGENERATERNGSEQUENCE_H__ADCF1532_6AE8_48DB_939F_BC29FDA8D831__INCLUDED_


#include "LogInfoMessage.h"

class CLogGenerateRNGSequence : public CLogInfoMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogGenerateRNGSequence(long time, const zString type, zDoubleVector sequence);

	virtual ~CLogGenerateRNGSequence();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogGenerateRNGSequence& rMsg);

	// ****************************************
	// Public access functions
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual	void Serialize(zOutStream& os) const;

	// ****************************************
	// Protected local functions
protected:

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:
	
	// Explicitly disallow the copy constructor and assignment operators
	// by declaring them private and providing NO definitions.

	CLogGenerateRNGSequence(const CLogGenerateRNGSequence& oldMessage);
	CLogGenerateRNGSequence& operator=(const CLogGenerateRNGSequence& rhs);

	// ****************************************
	// Data members
private:

    const zString  m_Type;        // Distribution type: Uniform or Gaussian
    zDoubleVector  m_Sequence;    // Sequence of random numbers to write out	
};

#endif // !defined(AFX_LOGGENERATERNGSEQUENCE_H__ADCF1532_6AE8_48DB_939F_BC29FDA8D831__INCLUDED_)
