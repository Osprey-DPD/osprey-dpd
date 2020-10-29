// IEventLamellaCusp.h: interface for the IEventLamellaCusp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IEVENTLAMELLACUSP_H__F9655C50_F302_4AD4_AD18_34E266E17111__INCLUDED_)
#define AFX_IEVENTLAMELLACUSP_H__F9655C50_F302_4AD4_AD18_34E266E17111__INCLUDED_


class IEventLamellaCusp  
{
public:
	// Member function indicating that a derived class supports this event

	virtual bool LamellaCuspSupported() const;

protected:

    virtual ~IEventLamellaCusp();


public:
	// Access functions that must be overridden by derived classes that support this event

	virtual bool   IsCuspPresent() const;
	virtual double GetCurvature()  const;

};

#endif // !defined(AFX_IEVENTLAMELLACUSP_H__F9655C50_F302_4AD4_AD18_34E266E17111__INCLUDED_)
