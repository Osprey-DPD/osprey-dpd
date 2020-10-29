// IEventVesicleUnbindsWall.h: interface for the IEventVesicleUnbindsWall class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IEVENTVESICLEUNBINDSWALL_H__CCC2C1EC_B496_4190_A454_21199AA3FBE0__INCLUDED_)
#define AFX_IEVENTVESICLEUNBINDSWALL_H__CCC2C1EC_B496_4190_A454_21199AA3FBE0__INCLUDED_


class IEventVesicleUnbindsWall  
{
public:
	// Member function indicating that a derived class supports this event

	virtual bool VesicleUnbindsWallSupported() const;

protected:

    virtual ~IEventVesicleUnbindsWall();


public:
	// Access functions that must be overridden by derived classes that support this event

	virtual double GetCMXPos() const;
	virtual double GetCMYPos() const;
	virtual double GetCMZPos() const;
	virtual double GetRadius() const;

};

#endif // !defined(AFX_IEVENTVESICLEUNBINDSWALL_H__CCC2C1EC_B496_4190_A454_21199AA3FBE0__INCLUDED_)
