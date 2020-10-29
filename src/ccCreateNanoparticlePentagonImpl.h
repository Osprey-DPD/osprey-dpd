// ccCreateNanoparticlePentagonImpl.h: interface for the ccCreateNanoparticlePentagonImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCCREATENANOPARTICLEPENTAGONIMPL_H__8d6c7552_4507_4914_9071_cd86c719929a__INCLUDED_)
#define AFX_CCCREATENANOPARTICLEPENTAGONIMPL_H__8d6c7552_4507_4914_9071_cd86c719929a__INCLUDED_


#include "ISimBoxCmd.h"

class ccCreateNanoparticlePentagonImpl : public virtual ISimBoxCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	ccCreateNanoparticlePentagonImpl();

	virtual ~ccCreateNanoparticlePentagonImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void CreateNanoparticlePentagon(const xxCommand* const pCommand);


	// ****************************************
	// Protected local functions
protected:

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:

    void CalculateRotationMatrix(double theta, double phi);
    
	// ****************************************
	// Data members
private:
    
    double m_RNormal[3][3];		// Rotation matrix for pentagon's normal 

};

#endif // !defined(AFX_CCCREATENANOPARTICLEPENTAGONIMPL_H__8d6c7552_4507_4914_9071_cd86c719929a__INCLUDED_)
