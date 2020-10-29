// SolventFreeFormat.h: interface for the CSolventFreeFormat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOLVENTFREEFORMAT_H__d1c5472b_1dbd_496e_b992_5fdeba3bad5c__INCLUDED_)
#define AFX_SOLVENTFREEFORMAT_H__d1c5472b_1dbd_496e_b992_5fdeba3bad5c__INCLUDED_

// Forward declarations

class ISimBox;


#include "CurrentStateFormat.h"

class CSolventFreeFormat : public CCurrentStateFormat  
{
	// ****************************************
	// Construction/Destruction
public:

    CSolventFreeFormat(double lx, double ly, double lz, bool bDisplayBox,
                       const long beadTypeTotal, const ISimBox* pISimBox);

	virtual ~CSolventFreeFormat();

	// ****************************************
	// Global functions, static member functions and variables
private:

	static const zString m_FileExtension;

	// ****************************************
	// Public access functions
public:

	inline const zString GetFileExtension() const {return m_FileExtension;}

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// Function to ensure that derived classes can write their data to file

	virtual void SerializeHeader(zOutStream& os, const long beadTotal);
	virtual void SerializeBead(zOutStream& os, const zString name, const long type, const double radius,
								const double x, const double y, const double z);

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

    const ISimBox* m_pISimBox;
    
};

#endif // !defined(AFX_SOLVENTFREEFORMAT_H__d1c5472b_1dbd_496e_b992_5fdeba3bad5c__INCLUDED_)
