// IEventLamellaMonolayerDomain.h: interface for the IEventLamellaMonolayerDomain class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IEVENTLAMELLAMONOLAYERDOMAIN_H__3B2E6636_BF58_4D83_AAC0_01D4E23A2577__INCLUDED_)
#define AFX_IEVENTLAMELLAMONOLAYERDOMAIN_H__3B2E6636_BF58_4D83_AAC0_01D4E23A2577__INCLUDED_


class IEventLamellaMonolayerDomain  
{
public:
	// Member function indicating that a derived class supports this event

	virtual bool LamellaMonolayerDomainSupported() const;

protected:

    virtual ~IEventLamellaMonolayerDomain();


public:
	// Access functions that must be overridden by derived classes that support this event

	virtual long   GetRaftTotal()    const;
	virtual double GetMeanRaftSize() const;
	virtual double GetMeanRaftPerimeter() const;

};

#endif // !defined(AFX_IEVENTLAMELLAMONOLAYERDOMAIN_H__3B2E6636_BF58_4D83_AAC0_01D4E23A2577__INCLUDED_)
