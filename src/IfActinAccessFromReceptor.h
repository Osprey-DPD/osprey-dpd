// IfActinAccessFromReceptor.h: interface for the IfActinAccessFromReceptor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IFACTINACCESSFROMRECEPTOR_H__8d6bcaf5_99a7_4def_8210_aada582b1f30__INCLUDED_)
#define AFX_IFACTINACCESSFROMRECEPTOR_H__8d6bcaf5_99a7_4def_8210_aada582b1f30__INCLUDED_


// Forward declarations

class aeActiveBond;
class aeActivePolymer;


class IfActinAccessFromReceptor
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
		// Functions used by the Receptor network to interact with the fActin network

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

        virtual ~IfActinAccessFromReceptor();

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:

	// ****************************************
	// Data members:
private:


};

#endif // !defined(AFX_IFACTINACCESSFROMRECEPTOR_H__8d6bcaf5_99a7_4def_8210_aada582b1f30__INCLUDED_)
