// RandomNumberSequence.h: interface for the CRandomNumberSequence class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RANDOMNUMBERSEQUENCE_H__465C4E22_B80E_4773_9431_419DF9EDB8ED__INCLUDED_)
#define AFX_RANDOMNUMBERSEQUENCE_H__465C4E22_B80E_4773_9431_419DF9EDB8ED__INCLUDED_


#include "FileReader.h"

class CRandomNumberSequence : public CFileReader  
{
	// ****************************************
	// Construction/Destruction
public:

    CRandomNumberSequence(const zString fileName, long sequenceSize);

	virtual ~CRandomNumberSequence();

	// ****************************************
	// Global functions, static member functions and variables
public:

    static bool m_bReadOnceOnly; // Flag showing if the file should be read only once

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

    bool Serialize();
  
	void ResetPointer();
	double GetFirstNumber();
	double GetNextNumber();

    inline long GetExpectedSequenceSize() const {return m_SequenceSize;}
    inline long GetActualSequenceSize()   const {return m_Numbers.size();}

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
private:

    bool          m_bReadFileAgain; // Local flag showing if the file can be read again
    const zString m_FileName;       // Full name of file containing numbers
    const long    m_SequenceSize;   // Number of random numbers to read from file

    unsigned long m_Pointer;        // Element containing next number
    zDoubleVector m_Numbers;        // Random numbers read in

};

#endif // !defined(AFX_RANDOMNUMBERSEQUENCE_H__465C4E22_B80E_4773_9431_419DF9EDB8ED__INCLUDED_)
