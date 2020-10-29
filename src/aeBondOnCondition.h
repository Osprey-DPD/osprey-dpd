// aeBondOnCondition.h: interface for the aeBondOnCondition class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AEBONDONCONDITION_H__8902B3AF_2EA0_476C_8B1E_81F5856FAF54__INCLUDED_)
#define AFX_AEBONDONCONDITION_H__8902B3AF_2EA0_476C_8B1E_81F5856FAF54__INCLUDED_


// Forward declarations

class aeActiveBond;


class aeBondOnCondition  
{
	// ****************************************
	// Construction/Destruction - protected constructor declared below
public:

	virtual ~aeBondOnCondition();


	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes

	// The first checks if a free active bond can bind to a polymerised bond
	// by attaching to its head; the second checks for addition to the polymerised
	// bond's tail. Note that activation refers to the polymerised bond not the
	// free bond being added.

	virtual bool ActivateHead(const aeActiveBond* const pEnd, const aeActiveBond* const pFree) const = 0;
	virtual bool ActivateTail(const aeActiveBond* const pEnd, const aeActiveBond* const pFree) const = 0;
	
	// ****************************************
	// Public access functions
public:

    inline bool IsPhantom() const {return m_bPhantom;}
	void SetPhantom(bool bPhantom);

	// ****************************************
	// Protected local functions
protected:

	aeBondOnCondition();

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members

private:

    bool m_bPhantom;    // Flag showing if a bond instance is a phantom

};

#endif // !defined(AFX_AEBONDONCONDITION_H__8902B3AF_2EA0_476C_8B1E_81F5856FAF54__INCLUDED_)
