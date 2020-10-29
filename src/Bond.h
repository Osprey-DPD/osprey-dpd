// Bond.h: interface for the CBond class.
//
// This class is a friend of CBead as it needs to add the bond force to the
// current force on the beads at its ends.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOND_H__47E30900_37A1_11D3_820E_0060088AD300__INCLUDED_)
#define AFX_BOND_H__47E30900_37A1_11D3_820E_0060088AD300__INCLUDED_


// Forward declarations

class CAbstractBead;

class CBond  
{
	// Friend classes need access to bond data in order to
	// calculate forces between bonded beads and observables

	friend class CBondPair;
	friend class CMonitor;

	// ****************************************
	// Construction/Destruction
public:

	CBond();
	CBond(long type, double spring, double length0);
	CBond(const CBond& oldBond);
	~CBond();

	// ****************************************
	// Global functions, static member functions and variables
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	inline long   GetId()			const {return m_id;} 
	inline long   GetType()			const {return m_Type;} 
	inline double GetSprConst()		const {return m_SprConst;} 
	inline double GetUnStrLength()	const {return m_UnStrLen;}
	inline long   GetHeadIndex()	const {return m_HeadIndex;} 
	inline long   GetTailIndex()	const {return m_TailIndex;} 
	inline CAbstractBead* GetHead()	const {return m_pHead;} 
	inline CAbstractBead* GetTail()	const {return m_pTail;} 

	// Access functions to the bond's length, force and potential energy

	inline double GetLength()		const {return m_Length;}
	inline double GetXLength()		const {return m_dx;}
	inline double GetYLength()		const {return m_dy;}
	inline double GetZLength()		const {return m_dz;}
	inline double GetXForce()		const {return m_fx;}
	inline double GetYForce()		const {return m_fy;}
	inline double GetZForce()		const {return m_fz;}

	double CalculateLength() const;
	double CalculatePBCLength() const;
	double CalculateunPBCLength() const;

	inline void SetId(long id)							{m_id = id;}
	inline void SetType(long type)						{m_Type = type;}
	inline void SetSpringConstant(double springConst)	{m_SprConst = springConst;}
	inline void SetUnStretchedLength(double length)		{m_UnStrLen = length;}

	void SetBeadIndex(long lHead, long lTail);
	void SetBeads(CAbstractBead* pHead, CAbstractBead* pTail);
	void SetHeadBead(CAbstractBead* pHead);
	void SetTailBead(CAbstractBead* pTail);

	void	AddForce();
	void    AddPBCForce();
	double  AddPotentialEnergy();

	// ****************************************
	// Protected local functions
protected:

	// ****************************************
	// Implementation

	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

	long   m_id;
	long   m_Type;
	double m_SprConst;
	double m_UnStrLen;
	long   m_HeadIndex;
	long   m_TailIndex;
	CAbstractBead* m_pHead;
	CAbstractBead* m_pTail;

	double m_dx;				// Bond vector components
	double m_dy;				
	double m_dz;				
	double m_Length;			// Current bond length used in CBondPair
	double m_fx;				// Bond force components
	double m_fy;				
	double m_fz;				

};

#endif // !defined(AFX_BOND_H__47E30900_37A1_11D3_820E_0060088AD300__INCLUDED_)
