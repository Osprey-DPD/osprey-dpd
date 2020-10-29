// IEventLamellaFlipflop.h: interface for the IEventLamellaFlipflop class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IEVENTLAMELLAFLIPFLOP_H__49E170D5_D0DE_42CF_8AEB_3BBAF49FCBD1__INCLUDED_)
#define AFX_IEVENTLAMELLAFLIPFLOP_H__49E170D5_D0DE_42CF_8AEB_3BBAF49FCBD1__INCLUDED_


class IEventLamellaFlipflop  
{
public:
	// Member function indicating that a derived class supports this event

	virtual bool LamellaFlipflopSupported() const;

protected:

    virtual ~IEventLamellaFlipflop();


public:
	// Access functions that must be overridden by derived classes that support this event

};

#endif // !defined(AFX_IEVENTLAMELLAFLIPFLOP_H__49E170D5_D0DE_42CF_8AEB_3BBAF49FCBD1__INCLUDED_)
