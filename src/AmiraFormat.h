// AmiraFormat.h: interface for the CAmiraFormat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AMIRAFORMAT_H__93A788DC_A2F0_4086_92E8_80FD1306C097__INCLUDED_)
#define AFX_AMIRAFORMAT_H__93A788DC_A2F0_4086_92E8_80FD1306C097__INCLUDED_


#include "CurrentStateFormat.h"

class CAmiraFormat : public CCurrentStateFormat  
{
	// ****************************************
	// Construction/Destruction
public:

	CAmiraFormat(double lx, double ly, double lz, bool bDisplayBox,
				 const long beadTypeTotal);

	virtual ~CAmiraFormat();

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

	const long   m_AVar1;		// Unknown parameter for Amira header
	const long   m_AVar2;		//    "        "
	const double m_AVar3;	// Unknown parameter
	const double m_AVar4;	//    "
	const double m_AVar5;	//    "



};

#endif // !defined(AFX_AMIRAFORMAT_H__93A788DC_A2F0_4086_92E8_80FD1306C097__INCLUDED_)
