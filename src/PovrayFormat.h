// PovrayFormat.h: interface for the CPovrayFormat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POVRAYFORMAT_H__6F3CE7F1_0496_4F06_B901_F538D979EDC5__INCLUDED_)
#define AFX_POVRAYFORMAT_H__6F3CE7F1_0496_4F06_B901_F538D979EDC5__INCLUDED_


#include "CurrentStateFormat.h"

class CPovrayFormat : public CCurrentStateFormat  
{
	// ****************************************
	// Construction/Destruction
public:

	CPovrayFormat(double lx, double ly, double lz, bool bDisplayBox,
				  const long beadTypeTotal,
				  const double camera[3], const double target[3],
				 zDoubleVector vLightX, 
				 zDoubleVector vLightY, 
				 zDoubleVector vLightZ,
				 bool showBonds = false);

	virtual ~CPovrayFormat();

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

	virtual bool UsesBonds() const
	{ return m_ShowBonds; }

	// Function to ensure that derived classes can write their data to file

	virtual void SerializeHeader(zOutStream& os, const long beadTotal);
	virtual void SerializeBead(zOutStream& os, const zString name, const long type, const double radius,
								const double x, const double y, const double z);
	virtual void SerializeBond(zOutStream& os, const CPolymer &polymer, const CBond &bond);


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

	double m_Camera[3];				// Coordinates of CCurrentState camera
	double m_Target[3];

	zDoubleVector m_vLightX;		// Coordinates of lights
	zDoubleVector m_vLightY;
	zDoubleVector m_vLightZ;

	bool m_ShowBonds;  // Whether to show bonds as rods
};

#endif // !defined(AFX_POVRAYFORMAT_H__6F3CE7F1_0496_4F06_B901_F538D979EDC5__INCLUDED_)
