// ccSelectBeadTypeInSlice.h: interface for the ccSelectBeadTypeInSlice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSELECTBEADTYPEINSLICE_H__D6B55EEC_AD6C_46DF_8651_ECDDC8C1000A__INCLUDED_)
#define AFX_CCSELECTBEADTYPEINSLICE_H__D6B55EEC_AD6C_46DF_8651_ECDDC8C1000A__INCLUDED_


#include "ccSelectBeadTypeInRegion.h"

class ccSelectBeadTypeInSlice : public ccSelectBeadTypeInRegion  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ccSelectBeadTypeInSlice(long executionTime);
	ccSelectBeadTypeInSlice(const ccSelectBeadTypeInSlice& oldCommand);

	ccSelectBeadTypeInSlice(long executionTime, const zString target, const zString bead,
							   long xn, long yn, long zn, 
							   double xc, double yc, double zc, 
							   double xw, double yw, double zw);

	virtual ~ccSelectBeadTypeInSlice();
	
	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	 // Return the type of command
    static long  GetArgumentTotal(); // Return the number of arguments 

private:

	static const zString m_Type;	// Identifier used in control data file for command
    static long  m_ArgumentTotal;   // Number of arguments required not including the execution time


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an xxCommand pointer,
	// respond to the SimBox's request to execute and return the name of the command.

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const;


	virtual const xxCommand* GetCommand() const;

	virtual bool IsDataValid(const CInputData& riData) const;

    virtual long GetCommandArgumentTotal() const {return m_ArgumentTotal;}

	virtual bool Pack(const tguArgumentSequence& vArguments);

	// ****************************************
	// Public access functions
public:

	inline long		GetXNormal()		const {return m_X;}
	inline long		GetYNormal()		const {return m_Y;}
	inline long		GetZNormal()		const {return m_Z;}
	inline double	GetXCentre()		const {return m_XC;}
	inline double	GetYCentre()		const {return m_YC;}
	inline double	GetZCentre()		const {return m_ZC;}
	inline double	GetXHalfWidth()		const {return m_XHalfWidth;}
	inline double	GetYHalfWidth()		const {return m_YHalfWidth;}
	inline double	GetZHalfWidth()		const {return m_ZHalfWidth;}

	// ****************************************
	// Protected local functions
protected:

	virtual const zString GetCommandType() const;

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

	long		m_X;
	long		m_Y; 			// Slice normal must be (1,0,0), (0,1,0) or (0,0,1)
	long		m_Z; 
	double		m_XC;
	double		m_YC;		    // Slice centre as fractions of SimBox side lengths
	double		m_ZC;
	double		m_XHalfWidth;
	double		m_YHalfWidth;	// Slice half widths as fractions of SimBox side lengths
	double		m_ZHalfWidth;


};

#endif // !defined(AFX_CCSELECTBEADTYPEINSLICE_H__D6B55EEC_AD6C_46DF_8651_ECDDC8C1000A__INCLUDED_)
