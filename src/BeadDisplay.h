// BeadDisplay.h: interface for the CBeadDisplay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BEADDISPLAY_H__8EF620D7_9101_44B8_BF57_381713F21CD9__INCLUDED_)
#define AFX_BEADDISPLAY_H__8EF620D7_9101_44B8_BF57_381713F21CD9__INCLUDED_


// Forward declarations

class CBead;


class CBeadDisplay  
{
	// ****************************************
	// Construction/Destruction
public:

	CBeadDisplay(long id, long type, double x, double y, double z);

    // Constructor to retrieve display data from a bead instance

	CBeadDisplay(const CBead* const pBead);

	CBeadDisplay(const CBeadDisplay& oldBead);

	virtual ~CBeadDisplay();

	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs
public:


	// ****************************************
	// Public access functions
public:


	// ****************************************
	// Protected local functions
protected:						



	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:						


	// ****************************************
	// Data members - public so that we can access them directly.
public:

    const long  m_id;       // Bead id (unique among ALL processors
    const long  m_Type;     // Bead numeric type

    const double m_X;
    const double m_Y;       // Bead's position coordinates (modulo SimBox size)
    const double m_Z;



};

#endif // !defined(AFX_BEADDISPLAY_H__8EF620D7_9101_44B8_BF57_381713F21CD9__INCLUDED_)
