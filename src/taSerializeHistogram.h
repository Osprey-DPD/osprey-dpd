// taSerializeHistogram.h: interface for the taSerializeHistogram class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASERIALIZEHISTOGRAM_H__C98CF226_94E1_4607_9670_655AF9E0EBDF__INCLUDED_)
#define AFX_TASERIALIZEHISTOGRAM_H__C98CF226_94E1_4607_9670_655AF9E0EBDF__INCLUDED_


// Forward declarations

class taHistogramDecorator;


#include "taFileLabel.h"

class taSerializeHistogram : public taFileLabel
{
public:
	// ****************************************
	// Construction/Destruction
public:

	taSerializeHistogram(const zString fileName, const zString label, 
                CCommandTargetNode* const pDec);

	virtual ~taSerializeHistogram();


    // ****************************************
	// Global functions, static member functions and variables
public:

    static const zString GetType();		// return the target's type

private:

	static const zString m_Type;

	// ****************************************
	// PVFs that must be implemented by all instantiated derived classes 
public:

    const zString GetTargetType() const;    // return the target's type

    // Implementation of the ISerialiseInclusiveRestartState interface
    // to allow this class to read/write data that can be modified
    // for restarts.

    virtual zInStream& Read(zInStream& is);
    virtual zOutStream& Write(zOutStream& is) const;


    bool Serialize();

	// ****************************************
	// Public access functions
public:

    void SetHistogram(taHistogramDecorator* const pHistogram);


	// ****************************************
	// Protected local functions
protected:



	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members

protected:


private:

    taHistogramDecorator* m_pHistogram;   // Pointer to the histogram whose data is to be serialized

};

#endif // !defined(AFX_TASERIALIZEHISTOGRAM_H__C98CF226_94E1_4607_9670_655AF9E0EBDF__INCLUDED_)
