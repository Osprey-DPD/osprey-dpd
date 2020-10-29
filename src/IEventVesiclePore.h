// IEventVesiclePore.h: interface for the IEventVesiclePore class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IEVENTVESICLEPORE_H__335374F4_F8FC_4124_8465_B1D51E93DD10__INCLUDED_)
#define AFX_IEVENTVESICLEPORE_H__335374F4_F8FC_4124_8465_B1D51E93DD10__INCLUDED_


class IEventVesiclePore  
{
public:
	// Member function indicating that a derived class supports this event

	virtual bool VesiclePoreSupported() const;

protected:

    virtual ~IEventVesiclePore();


public:
	// Access functions that must be overridden by derived classes that support this event

};

#endif // !defined(AFX_IEVENTVESICLEPORE_H__335374F4_F8FC_4124_8465_B1D51E93DD10__INCLUDED_)
