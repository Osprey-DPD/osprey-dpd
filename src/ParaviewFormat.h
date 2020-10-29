// ParaviewFormat.h: interface for the CParaviewFormat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARAVIEWFORMAT_H__6DCC8E32_FBBA_46E8_B1D8_6FAF212DBDDB__INCLUDED_)
#define AFX_PARAVIEWFORMAT_H__6DCC8E32_FBBA_46E8_B1D8_6FAF212DBDDB__INCLUDED_


#include "CurrentStateFormat.h"

class CParaviewFormat : public CCurrentStateFormat  
{
	// ****************************************
	// Construction/Destruction
public:

	CParaviewFormat(double lx, double ly, double lz, bool bDisplayBox,
				 const long beadTypeTotal);

	virtual ~CParaviewFormat();

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


	virtual void SerializeHeader(zOutStream& os, const long beadTotal);
	virtual void SerializeFooter(zOutStream& os, const long beadTotal);
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

    zLongVector  m_vBeadTypes;  // Local vector of displayIds

};

#endif // !defined(AFX_PARAVIEWFORMAT_H__6DCC8E32_FBBA_46E8_B1D8_6FAF212DBDDB__INCLUDED_)
