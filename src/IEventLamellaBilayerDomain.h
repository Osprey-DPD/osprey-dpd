// IEventLamellaBilayerDomain.h: interface for the IEventLamellaBilayerDomain class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IEVENTLAMELLABILAYERDOMAIN_H__4AAB6749_CDF1_433A_AA9F_05B13691085E__INCLUDED_)
#define AFX_IEVENTLAMELLABILAYERDOMAIN_H__4AAB6749_CDF1_433A_AA9F_05B13691085E__INCLUDED_


class IEventLamellaBilayerDomain  
{
public:
	// Member function indicating that a derived class supports this event

	virtual bool LamellaBilayerDomainSupported() const;

protected:

    virtual ~IEventLamellaBilayerDomain();


public:
	// Access functions that must be overridden by derived classes that support this event

};

#endif // !defined(AFX_IEVENTLAMELLABILAYERDOMAIN_H__4AAB6749_CDF1_433A_AA9F_05B13691085E__INCLUDED_)
