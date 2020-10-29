// IEAD.h: interface for the IEAD class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IEAD_H__79CB1FDB_D837_410E_A09F_D3B077967EDB__INCLUDED_)
#define AFX_IEAD_H__79CB1FDB_D837_410E_A09F_D3B077967EDB__INCLUDED_


// Forward declarations

class taEventSourceDecorator;


class IEAD
{
	// ****************************************
	// Construction/Destruction - protected constructor declared below
public:

	virtual ~IEAD();


	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be implemented by all instantiated derived classes 
public:


	// ****************************************
	// Public access functions
public:


	// ****************************************
	// Protected local functions
protected:

	// Protected constructor allows only derived classes to be instantiated

	IEAD(taEventSourceDecorator* const pESD);



	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members

protected:

    // Pointer to the ESD that routes event data to EADs: this must be 
    // accessible to derived classes.

    taEventSourceDecorator* const m_pESD; 

private:

};

#endif // !defined(AFX_IEAD_H__79CB1FDB_D837_410E_A09F_D3B077967EDB__INCLUDED_)
