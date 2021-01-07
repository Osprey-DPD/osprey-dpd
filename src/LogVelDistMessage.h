// LogVelDistMessage.h: interface for the CLogVelDistMessage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGVELDISTMESSAGE_H__7BE15BC2_B4DB_43AD_B88B_680D4A3E2286__INCLUDED_)
#define AFX_LOGVELDISTMESSAGE_H__7BE15BC2_B4DB_43AD_B88B_680D4A3E2286__INCLUDED_


#include "LogInfoMessage.h"

class CLogVelDistMessage : public CLogInfoMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogVelDistMessage(long time, double temp, double vmean[3], double v2mean[3], double var[3]);

	virtual ~CLogVelDistMessage();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogVelDistMessage& rMsg);

	// ****************************************
	// Public access functions
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual	void Serialize(zOutStream& os) const;

	// ****************************************
	// Protected local functions
protected:

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:
	
	// Explicitly disallow the copy constructor and assignment operators
	// by declaring them private and providing NO definitions.

	CLogVelDistMessage(const CLogVelDistMessage& oldMessage);
	CLogVelDistMessage& operator=(const CLogVelDistMessage& rhs);

	// ****************************************
	// Data members
private:

	double m_Temperature;		// Temperature used to normalize the distribution
	double m_MeanVel[3];		// Components of mean velocity
	double m_Mean2Vel[3];		// Components of mean-square velocity
	double m_VarVel[3];			// Variance of velocity

};

#endif // !defined(AFX_LOGVELDISTMESSAGE_H__7BE15BC2_B4DB_43AD_B88B_680D4A3E2286__INCLUDED_)
