// GravityData.h: interface for the CGravityData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAVITYDATA_H__9E9808D3_9B3A_11D3_BF15_004095086186__INCLUDED_)
#define AFX_GRAVITYDATA_H__9E9808D3_9B3A_11D3_BF15_004095086186__INCLUDED_


#include "xxConstraint.h"

class CGravityData : public xxConstraint
{
public:
	CGravityData();
	~CGravityData();

	CGravityData(const CGravityData& oldGD);				// copy constructor
	CGravityData& operator=(const CGravityData& rGD);		// assignment operator

	friend zOutStream& operator<<(zOutStream& os, const CGravityData& gravity);
	friend zInStream&  operator>>(zInStream& is,        CGravityData& gravity);

	StringSequence GetBeads() const;
	double GetXForce() const;
	double GetYForce() const;
	double GetZForce() const;

private:
	StringSequence m_vBeads;			// Names of beads affected by the external force
	double m_XForce;					// Strength of force
	double m_YForce;
	double m_ZForce;
};

#endif // !defined(AFX_GRAVITYDATA_H__9E9808D3_9B3A_11D3_BF15_004095086186__INCLUDED_)
