// IModifySimStateBeadTypes.h: interface for the IModifySimStateBeadTypes class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMODIFYSIMSTATEBEADTYPES_H__DB8232BB_FF8C_4921_B951_1D8E932CDE2C__INCLUDED_)
#define AFX_IMODIFYSIMSTATEBEADTYPES_H__DB8232BB_FF8C_4921_B951_1D8E932CDE2C__INCLUDED_


#include "IModifySimState.h"

class IModifySimStateBeadTypes : public IModifySimState  
{
	// ****************************************
	// Construction/Destruction: 
public:

	IModifySimStateBeadTypes(CSimState& simState);

	virtual ~IModifySimStateBeadTypes();


	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes 
public:


	// ****************************************
	// Public access functions
public:

	inline bool IsBeadNameInMap(const zString name) const {return m_rSimState.IsBeadNameInMap(name);}
	inline bool IsBeadTypeInMap(long type)          const {return m_rSimState.IsBeadTypeInMap(type);}

	void AddNewType(long oldType);
	bool AddNewNamedType(long oldType, long newType, const zString name);

	void CalculateTypeTotals();

	void SetDPDConsInt(long firstType, long secondType, double newValue);
	void SetDPDDissInt(long firstType, long secondType, double newValue);

	// ****************************************
	// Protected local functions
protected:

	

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:

	// Explicitly disallow the copy constructor and assignment operators
	// by declaring them privately but providing NO definitions.

	IModifySimStateBeadTypes(const IModifySimStateBeadTypes& oldIModify);
	IModifySimStateBeadTypes& operator=(const IModifySimStateBeadTypes& rhs);

	// ****************************************
	// Data members
private:

};

#endif // !defined(AFX_IMODIFYSIMSTATEBEADTYPES_H__DB8232BB_FF8C_4921_B951_1D8E932CDE2C__INCLUDED_)
