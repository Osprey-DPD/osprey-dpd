// IEventLamellaPore.h: interface for the IEventLamellaPore class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IEVENTLAMELLAPORE_H__CDBF5A45_FB84_49C5_8681_13254DA8C8BB__INCLUDED_)
#define AFX_IEVENTLAMELLAPORE_H__CDBF5A45_FB84_49C5_8681_13254DA8C8BB__INCLUDED_


class IEventLamellaPore  
{
public:
	// Member function indicating that a derived class supports this event

	virtual bool LamellaPoreSupported() const;

protected:

    virtual ~IEventLamellaPore();


public:
	// Access functions that must be overridden by derived classes that support this event

	virtual bool IsPoreOpen() const;
	virtual long GetPoreSize() const;

};

#endif // !defined(AFX_IEVENTLAMELLAPORE_H__CDBF5A45_FB84_49C5_8681_13254DA8C8BB__INCLUDED_)
