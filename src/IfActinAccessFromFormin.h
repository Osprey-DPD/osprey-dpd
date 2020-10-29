// IfActinAccessFromFormin.h: interface for the IfActinAccessFromFormin class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IFACTINACCESSFROMFORMIN_H__281882E8_7D31_470F_BA11_5899DAB338C7__INCLUDED_)
#define AFX_IFACTINACCESSFROMFORMIN_H__281882E8_7D31_470F_BA11_5899DAB338C7__INCLUDED_


// Forward declarations

class aeActiveBond;
class aeActivePolymer;


class IfActinAccessFromFormin
{
	// ****************************************
	// Construction/Destruction: no ctor or dtor as this is an interface class
public:

	// ****************************************
	// Global functions, static member functions and variables
public:

	// ****************************************
	// PVFs
public:
		// Functions used by the Formin network to interact with the fActin network

		virtual void		  AddFreeBond(aeActiveBond* const pBond)			= 0;
		virtual void		  AddFreePolymer(aeActivePolymer* const pPolymer)	= 0;
		virtual aeActiveBond* GetNearbyMonomer(double x, double y, double z)	= 0;
		virtual void		  RemoveNearbyMonomer(aeActiveBond* const pBond)	= 0;
		virtual void          SetBondExternalTriggerOn(aeActiveBond* pBond)		= 0;



	// ****************************************
	// Public access functions
public:



	// ****************************************
	// Protected local functions
protected:

        virtual ~IfActinAccessFromFormin();

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:

	// ****************************************
	// Data members:
private:


};

#endif // !defined(AFX_IFACTINACCESSFROMFORMIN_H__281882E8_7D31_470F_BA11_5899DAB338C7__INCLUDED_)
