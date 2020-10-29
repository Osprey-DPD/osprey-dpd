// IEventMotorATPase.h: interface for the IEventMotorATPase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IEVENTMOTORATPASE_H__202ADE3D_D916_462D_AF61_2424CD52E72B__INCLUDED_)
#define AFX_IEVENTMOTORATPASE_H__202ADE3D_D916_462D_AF61_2424CD52E72B__INCLUDED_


class IEventMotorATPase  
{
public:
	// Member function indicating that a derived class supports this event

	virtual bool MotorATPaseSupported() const;

protected:

    virtual ~IEventMotorATPase();


public:
	// Access functions that must be overridden by derived classes that support this event

};

#endif // !defined(AFX_IEVENTMOTORATPASE_H__202ADE3D_D916_462D_AF61_2424CD52E72B__INCLUDED_)
