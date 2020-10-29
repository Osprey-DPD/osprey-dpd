// aeActiveSimBox.h: interface for the aeActiveSimBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AEACTIVESIMBOX_H__581F3756_5F08_4ACF_966A_40A7EB2B0655__INCLUDED_)
#define AFX_AEACTIVESIMBOX_H__581F3756_5F08_4ACF_966A_40A7EB2B0655__INCLUDED_


// Forward declarations

class aeActiveCellNetwork;
class ISimBox;
class aeArp23Network;
class aefActinNetwork;
class aeForminNetwork;
class aeReceptorNetwork;


#include "IActiveSimBox.h"

class aeActiveSimBox : public IActiveSimBox
{
	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	virtual ~aeActiveSimBox();

	// ****************************************
	// Global functions, static member functions and variables
public:

	// Public Instance() function and protected constructor ensure that
	// only a single aeActiveSimBox object can be created. We pass it the 
	// ISimBox object to connect it to the CSimBox.

	static aeActiveSimBox* Instance(const ISimBox* const pISimBox);

	// Function to provide active classes global access the IActiveSimBox interface.

	static const IActiveSimBox* GetIActiveSimBox();

private:

	static aeActiveSimBox* m_pInstance;

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual const zString GetRunId()  const;
	virtual long   GetCurrentTime()   const;
	virtual double GetSimBoxXLength() const;
	virtual double GetSimBoxYLength() const;
	virtual double GetSimBoxZLength() const;
	virtual double GetHalfSimBoxXLength() const;
	virtual double GetHalfSimBoxYLength() const;
	virtual double GetHalfSimBoxZLength() const;

    virtual bool IsACNPresent(const zString acnType) const;

	virtual aeActiveCellNetwork* GetACNFromType(const zString acnType) const;

	virtual IModifyActiveCellNetwork* GetIModifyActiveCellNetworkFromType(const zString type);

	virtual IACNAccessControl* GetIACNAccessControlFromType(const zString type);

	// ****************************************
	// Public access functions
public:

	// Main function that updates the state of the active networks

	void Evolve();

	// Deprecated functions to create, delete and access the active networks.
	// I have left them in to avoid having to modify the calling routines, but
	// they now just map to the new functions below.

	bool IsArp23NetworkPresent()		const;
	bool IsfActinNetworkPresent()		const;
	bool IsForminNetworkPresent()		const;
	bool IsReceptorNetworkPresent()		const;

	aeArp23Network*       GetArp23Network() const;
	aefActinNetwork*     GetfActinNetwork() const;
	aeForminNetwork*     GetForminNetwork() const;
	aeReceptorNetwork* GetReceptorNetwork() const;

	void CreateArp23Network(long xn, long yn, long zn);
	void CreatefActinNetwork(long xn, long yn, long zn);
	void CreateForminNetwork(long xn, long yn, long zn);
	void CreateReceptorNetwork(long xn, long yn, long zn);

	// New create/delete functions that use a map of aeActiveCellNetworks
	// accessed by their string identifiers

	void CreateACNFromType(const zString acnType, long xn, long yn, long zn);
	void DeleteACNFromType(const zString acnType);


	// Active Event Interfaces

	IActivePolymerForms* GetIActivePolymerForms();
	IActivePolymerFormsBondTriple* GetIActivePolymerFormsBondTriple();
	IActivePolymerDissolves* GetIActivePolymerDissolves();
	IActiveBondBindsToPolymer* GetIActiveBondBindsToPolymer();
	IActiveBondUnbindsFromPolymer* GetIActiveBondUnbindsFromPolymer();
	IActiveBondHydrolysesATP* GetIActiveBondHydrolysesATP();
	IActiveBondReleasesPi* GetIActiveBondReleasesPi();
	IActiveBondPhosphorylation* GetIActiveBondPhosphorylation();


	// ****************************************
	// Protected local functions
protected:

	// Protected constructor prevents this class from being instantiated

	aeActiveSimBox(const ISimBox* const pISimBox);

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

	const ISimBox* const m_pISimBox;	// Connection to the ISimBox and normal CSimBox

	// Map of ACN instances that track active bonds and polymers

	StringActiveACNMap     m_mACN;		// Map of (string, aeActiveCellNetwork*) pairs

	// Map of ACNs that allow acXXX commands to be routed to them

	StringModifiableACNMap	m_mModifiableACN;	// Map of (string, IModifyActiveCellNetwork*) pairs

	// Map of ACNs that provide access to other networks allowing them to interact
	
	StringAccessibleACNMap	m_mAccessACN;	// Map of (string, IAccessControlACN*) pairs

};

#endif // !defined(AFX_AEACTIVESIMBOX_H__581F3756_5F08_4ACF_966A_40A7EB2B0655__INCLUDED_)
