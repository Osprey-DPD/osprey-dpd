// IEventVesicleBindsWall.h: interface for the IEventVesicleBindsWall class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IEVENTVESICLEBINDSWALL_H__BCE21AAD_8775_42B6_B800_978D791AAA7F__INCLUDED_)
#define AFX_IEVENTVESICLEBINDSWALL_H__BCE21AAD_8775_42B6_B800_978D791AAA7F__INCLUDED_


class IEventVesicleBindsWall  
{
public:
	// Member function indicating that a derived class supports this event

	virtual bool VesicleBindsWallSupported() const;

protected:

    virtual ~IEventVesicleBindsWall();


public:
	// Access functions that must be overridden by derived classes that support this event

	virtual double GetCMXPos() const;
	virtual double GetCMYPos() const;
	virtual double GetCMZPos() const;
	virtual double GetRadius() const;

};

#endif // !defined(AFX_IEVENTVESICLEBINDSWALL_H__BCE21AAD_8775_42B6_B800_978D791AAA7F__INCLUDED_)
