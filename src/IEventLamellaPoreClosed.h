// IEventLamellaPoreClosed.h: interface for the IEventLamellaPoreClosed class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IEVENTLAMELLAPORECLOSED_H__E9A37CF8_7065_4ABB_9B8B_430C84CA5666__INCLUDED_)
#define AFX_IEVENTLAMELLAPORECLOSED_H__E9A37CF8_7065_4ABB_9B8B_430C84CA5666__INCLUDED_


class IEventLamellaPoreClosed  
{
public:
	// Member function indicating that a derived class supports this event

	virtual bool LamellaPoreClosedSupported() const;

protected:

    virtual ~IEventLamellaPoreClosed();


public:
	// Access functions that must be overridden by derived classes that support this event

	virtual long GetPoreSize() const;

};

#endif // !defined(AFX_IEVENTLAMELLAPORECLOSED_H__E9A37CF8_7065_4ABB_9B8B_430C84CA5666__INCLUDED_)
