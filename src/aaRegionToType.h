// aaRegionToType.h: interface for the aaRegionToType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AAREGIONTOTYPE_H__7F4A21C1_B13C_11D3_BF19_004095086186__INCLUDED_)
#define AFX_AAREGIONTOTYPE_H__7F4A21C1_B13C_11D3_BF19_004095086186__INCLUDED_


// Include files

#include "SimAlgorithmFlags.h"
#include "SimACNFlags.h"
#include "xxBase.h"
#include "AbstractBead.h"
#include "Polymer.h"
#include "Slice.h"
#include "Row.h"
#include "Cell.h"
#include "CNTCell.h"
#include "aaScalar.h"
#include "aaVector.h"
#include "aaTensor.h"
#include "aaStressTensorPoint.h"
#include "Analysis.h"
#include "xxCommand.h"
#include "xxEvent.h"
#include "xxProcess.h"

#if EnableShadowSimBox == SimACNEnabled
#include "aeActiveBond.h"
#include "aeActivePolymer.h"
#endif

// STL using declarations

	using std::count_if;
	using std::unary_function;

// **********************************************************************
// Definitions of function objects used in analysing aggregates. 
//
// These objects are iterated over a profile to generate data
// relating to the aggregate, although some are used to feed into other 
// function objects.
//
//////////////////////////////////////////////////////////////////////
//
// aaDouble
// ********
//
// This function object simply converts a single double value to a newly-created 
// aaScalar object before returning a pointer to it. Note that the object must 
// be destroyed by the calling routine before using the function object again 
// or else a memory leak results.
//
//////////////////////////////////////////////////////////////////////

class aaDouble : public unary_function<double, aaScalar>  
{
public:
	aaDouble();
	~aaDouble();

	// operator() function that converts a double to an aaScalar

	aaScalar* operator() (double x) const
	{
		aaScalar* pData = new aaScalar(x);

		return pData;
	}
};

//////////////////////////////////////////////////////////////////////
//
// aaGetAggregateId
// ****************
// 
// This function object returns true if its argument is an instance of
// a class derived from CAnalysis and whose id matches that specified. 
// It is used by xxEvents and xxProcesses to check that the user-specified
// aggregates exist.
//
//////////////////////////////////////////////////////////////////////

class aaGetAggregateId  : public unary_function<CAnalysis, bool>  
{
public:
	aaGetAggregateId(long id);
	~aaGetAggregateId();

	result_type operator() (const argument_type* const pInst) const
	{
		return (pInst->GetId() == m_id);
	}

private:
	const long m_id;
};

//////////////////////////////////////////////////////////////////////
//
// aaGetCommandId
// **************
// 
// This function object returns true if its argument is an instance of
// a class derived from xxCommand and whose id matches that specified. 
//
//////////////////////////////////////////////////////////////////////

class aaGetCommandId  : public unary_function<xxCommand, bool>  
{
public:
	aaGetCommandId(long id);
	~aaGetCommandId();

	result_type operator() (const argument_type* const pInst) const
	{
		return (pInst->GetId() == m_id);
	}

private:
	const long m_id;
};



//////////////////////////////////////////////////////////////////////
//
// aaGetEventId
// ************
// 
// This function object returns true if its argument is an instance of
// a class derived from xxEvent and whose id matches that specified. 
// It is used by xxProcess to check that the user-specified
// events exist.
//
//////////////////////////////////////////////////////////////////////

class aaGetEventId  : public unary_function<xxEvent, bool>  
{
public:
	aaGetEventId(long id);
	~aaGetEventId();

	result_type operator() (const argument_type* const pInst) const
	{
		return (pInst->GetId() == m_id);
	}

private:
	const long m_id;
};

//////////////////////////////////////////////////////////////////////
//
// aaGetProcessId
// **************
// 
// This function object returns true if its argument is an instance of
// a class derived from xxProcess and whose id matches that specified. 
//
//////////////////////////////////////////////////////////////////////

class aaGetProcessId  : public unary_function<xxProcess, bool>  
{
public:
	aaGetProcessId(long id);
	~aaGetProcessId();

	result_type operator() (const argument_type* const pInst) const
	{
		return (pInst->GetId() == m_id);
	}

private:
	const long m_id;
};

//////////////////////////////////////////////////////////////////////
//
// aaGetAggregateName
// ******************
// 
// This function object returns true if its argument is an instance of
// a class derived from CAnalysis and whose name matches that specified. 
//
//////////////////////////////////////////////////////////////////////

class aaGetAggregateName  : public unary_function<CAnalysis, bool>  
{
public:
	aaGetAggregateName(const zString name);
	~aaGetAggregateName();

	result_type operator() (const argument_type* const pInst) const
	{
		return (pInst->GetType() == m_Name);
	}

private:
	const zString m_Name;
};

//////////////////////////////////////////////////////////////////////
//
// aaGetCommandName
// ****************
// 
// This function object returns true if its argument is an instance of
// a class derived from xxCommand and whose type matches that specified. 
//
//////////////////////////////////////////////////////////////////////

class aaGetCommandName  : public unary_function<xxCommand, bool>  
{
public:
	aaGetCommandName(const zString name);
	~aaGetCommandName();

	result_type operator() (const argument_type* const pInst) const
	{
		return (pInst->GetCommandType() == m_Name);
	}

private:
	const zString m_Name;
};

//////////////////////////////////////////////////////////////////////
//
// aaGetCommandExecutionTime
// *************************
// 
// This function object returns true if its argument is an instance of
// a class derived from xxCommand and whose execution time matches that specified. 
//
//////////////////////////////////////////////////////////////////////

class aaGetCommandExecutionTime  : public unary_function<xxCommand, bool>  
{
public:
	aaGetCommandExecutionTime(long time);
	~aaGetCommandExecutionTime();

	result_type operator() (const argument_type* const pInst) const
	{
		return (pInst->GetExecutionTime() == m_ExecutionTime);
	}

private:
	const long m_ExecutionTime;
};

//////////////////////////////////////////////////////////////////////
//
// aaCommandExecutionTimeLess
// **************************
// 
// This function object returns true if its argument is an instance of
// a class derived from xxCommand and whose execution time is later
// than that specified.
//
//////////////////////////////////////////////////////////////////////

class aaCommandExecutionTimeLess  : public unary_function<xxCommand, bool>  
{
public:
	aaCommandExecutionTimeLess(long time);
	~aaCommandExecutionTimeLess();

	result_type operator() (const argument_type* const pInst) const
	{
		return (pInst->GetExecutionTime() < m_ExecutionTime);
	}

private:
	const long m_ExecutionTime;
};

//////////////////////////////////////////////////////////////////////
//
// aaCommandExecutionTimeGreater
// *****************************
// 
// This function object returns true if its argument is an instance of
// a class derived from xxCommand and whose execution time is later
// than that specified.
//
//////////////////////////////////////////////////////////////////////

class aaCommandExecutionTimeGreater  : public unary_function<xxCommand, bool>  
{
public:
	aaCommandExecutionTimeGreater(long time);
	~aaCommandExecutionTimeGreater();

	result_type operator() (const argument_type* const pInst) const
	{
		return (pInst->GetExecutionTime() > m_ExecutionTime);
	}

private:
	const long m_ExecutionTime;
};

//////////////////////////////////////////////////////////////////////
//
// aaGetEventName
// **************
// 
// This function object returns true if its argument is an instance of
// a class derived from xxEvent and whose id matches that specified. 
//
//////////////////////////////////////////////////////////////////////

class aaGetEventName  : public unary_function<xxEvent, bool>  
{
public:
	aaGetEventName(const zString name);
	~aaGetEventName();

	result_type operator() (const argument_type* const pInst) const
	{
		return (pInst->GetEventType() == m_Name);
	}

private:
	const zString m_Name;
};

//////////////////////////////////////////////////////////////////////
//
// aaGetProcessName
// ****************
// 
// This function object returns true if its argument is an instance of
// a class derived from xxProcess and whose id matches that specified. 
//
//////////////////////////////////////////////////////////////////////

class aaGetProcessName  : public unary_function<xxProcess, bool>  
{
public:
	aaGetProcessName(const zString name);
	~aaGetProcessName();

	result_type operator() (const argument_type* const pInst) const
	{
		return (pInst->GetProcessType() == m_Name);
	}

private:
	const zString m_Name;
};

//////////////////////////////////////////////////////////////////////
//
// aaBeadId
// ********
// 
// This function object returns true if its argument is the bead 
// with the specified id, and false otherwise. 
//
//////////////////////////////////////////////////////////////////////

class aaBeadId  : public unary_function<CAbstractBead*, bool>  
{
public:
	aaBeadId(long beadId);
	aaBeadId();

	bool operator() (const CAbstractBead* const pBead) const
	{
		if(pBead->GetId() == m_Id)
			return true;
		else
			return false;
	}
private:
	const long m_Id;
};


//////////////////////////////////////////////////////////////////////
//
// aaBeadType
// **********
// 
// This function object returns true if its argument is a bead 
// of the specified type, and false otherwise. It is used by many of
// the following function objects to identify polymers and beads.
//
//////////////////////////////////////////////////////////////////////

class aaBeadType  : public unary_function<CAbstractBead*, bool>  
{
public:
	aaBeadType(long type);
	~aaBeadType();

	bool operator() (const CAbstractBead* const pBead) const
	{
		if(pBead->GetType() == m_Type)
			return true;
		else
			return false;
	}
private:
	const long m_Type;
};

//////////////////////////////////////////////////////////////////////
//
// aaBeadType2
// ***********
// 
// This function object returns true if its argument is a bead 
// of either of the specified types, and false otherwise. It is used 
// to identify the presence of a polymer from its head and tail beads
// when checking just one bead type is not enough.
//
//////////////////////////////////////////////////////////////////////

class aaBeadType2  : public unary_function<CAbstractBead*, bool>  
{
public:
	aaBeadType2(long type1, long type2);
	~aaBeadType2();

	bool operator() (const CAbstractBead* const pBead) const
	{
		if(pBead->GetType() == m_Type1 || pBead->GetType() == m_Type2)
			return true;
		else
			return false;
	}
private:
	const long m_Type1;
	const long m_Type2;
};

//////////////////////////////////////////////////////////////////////
//
// aaBeadTypeLess, aaBeadTypeGreater
// *********************************
// 
// These function objects return true if the bead's type satisfies their
// condition compared to the value specified in their constructors.
//////////////////////////////////////////////////////////////////////

class aaBeadTypeLess  : public unary_function<CAbstractBead*, bool>  
{
public:
	aaBeadTypeLess(long type);
	~aaBeadTypeLess();

	bool operator() (const CAbstractBead* const pBead) const
	{
		return (pBead->GetType() < m_Type);
	}
private:
	const long m_Type;
};

class aaBeadTypeGreater  : public unary_function<CAbstractBead*, bool>  
{
public:
	aaBeadTypeGreater(long type);
	~aaBeadTypeGreater();

	bool operator() (const CAbstractBead* const pBead) const
	{
		return (pBead->GetType() > m_Type);
	}
private:
	const long m_Type;
};

//////////////////////////////////////////////////////////////////////
//
// aaBeadTypeCompLess, aaBeadTypeCompGreater
// ******************************************************
// 
// These function objects return true if the types of the two beads satisfy
// the appropriate condition. Note that we cannot define these classe
// for less-than-or-equal, etc, because such conditions do not define
// at least a strict weak ordering (see STL Tutorial and Reference Guide, 
// pages 103ff.

//////////////////////////////////////////////////////////////////////

class aaBeadTypeCompLess  : public unary_function<CAbstractBead*, bool>  
{
public:
	bool operator() (const CAbstractBead* const pBead1,
					 const CAbstractBead* const pBead2) const
	{
		return (pBead1->GetType() < pBead2->GetType());
	}
};

class aaBeadTypeCompGreater  : public unary_function<CAbstractBead*, bool>  
{
public:
	bool operator() (const CAbstractBead* const pBead1,
					 const CAbstractBead* const pBead2) const
	{
		return (pBead1->GetType() > pBead2->GetType());
	}
};

//////////////////////////////////////////////////////////////////////
//
// aaPolymerType
// *************
// 
// This function object returns true if its argument is a polymer 
// of the specified type, and false otherwise. It is used to identify
// polymers that belong in a bilayer and other aggregates.
//
//////////////////////////////////////////////////////////////////////

class aaPolymerType  : public unary_function<CPolymer*, bool>  
{
public:
	aaPolymerType(long type);
	~aaPolymerType();

	bool operator() (const CPolymer* const pPolymer) const
	{
		if(pPolymer->GetType() == m_Type)
			return true;
		else
			return false;
	}
private:
	const long m_Type;
};

//////////////////////////////////////////////////////////////////////
//
// aaPolymerTypeLess, aaPolymerTypeGreater
// ***************************************
// 
// These function objects return true if their arguments satisfy the
// appropriate conditions.
//
//////////////////////////////////////////////////////////////////////

class aaPolymerTypeLess  : public unary_function<CPolymer*, bool>  
{
public:
	aaPolymerTypeLess(long type);
	~aaPolymerTypeLess();

	bool operator() (CPolymer* pPolymer) const
	{
		return (pPolymer->GetType() < m_Type);
	}
private:
	const long m_Type;
};

class aaPolymerTypeGreater  : public unary_function<CPolymer*, bool>  
{
public:
	aaPolymerTypeGreater(long type);
	~aaPolymerTypeGreater();

	bool operator() (const CPolymer* const pPolymer) const
	{
		return (pPolymer->GetType() > m_Type);
	}
private:
	const long m_Type;
};

//////////////////////////////////////////////////////////////////////
//
// aaPolymerTypeCompLess, aaPolymerTypeCompGreater
// ***********************************************
// 
// These function objects return true if the types of the two polymers satisfy
// the appropriate condition. Note that we cannot define these classe
// for less-than-or-equal, etc, because such conditions do not define
// at least a strict weak ordering (see STL Tutorial and Reference Guide, 
// pages 103ff.

//////////////////////////////////////////////////////////////////////

class aaPolymerTypeCompLess : public unary_function<CPolymer*, bool>  
{
public:
	bool operator() (const CPolymer* const pPolymer1,
					 const CPolymer* const pPolymer2) const
	{
		return (pPolymer1->GetType() < pPolymer2->GetType());
	}
};

class aaPolymerTypeCompGreater : public unary_function<CPolymer*, bool>  
{
public:
	bool operator() (const CPolymer* const pPolymer1,
					 const CPolymer* const pPolymer2) const
	{
		return (pPolymer1->GetType() > pPolymer2->GetType());
	}
};

//////////////////////////////////////////////////////////////////////
//
// aaCNTCellNumber
// *************
//
// This function object counts the number of beads of the specified type 
// in a CNT cell. It is a more specialised version of the aaCellNumber 
// function object below.
//
//////////////////////////////////////////////////////////////////////

class aaCNTCellNumber  : public unary_function<CCNTCell, long>
{
public:
	aaCNTCellNumber(long beadType);
	~aaCNTCellNumber();

	// operator() function that transforms a CCNTCell object 
	// into a number.
	//
	// Note that the const qualifier is not necessary for count_if() but might
	// be needed for more complicated functions

	long operator() (CCNTCell* pRegion) const
	{
		const BeadList lBeads = pRegion->GetBeads();

		long beadsOfType = count_if(lBeads.begin(), lBeads.end(), aaBeadType(m_BeadType));

		return beadsOfType;
	}

private:
	long m_BeadType;
};



//////////////////////////////////////////////////////////////////////
//
// aaRegionToType
// *************
// 
// Base class of all function objects that transform a CRegion to
// an aaType object, e.g., aaScalar, aaVector or aaTensor. Originally, 
// these were derived from a base class aaType, but in the interests of
// speed of creation/destruction I removed the base class. Some of the comments
// below still refer to aaType. It should be interpreted as a metaphorical
// base class to the type classes.
//
//////////////////////////////////////////////////////////////////////

template <class R, class T> class aaRegionToType  : public unary_function<R, T>  
{
public:
	virtual ~aaRegionToType() {};

	virtual T* operator() (R* pRegion) const = 0;

protected:
	aaRegionToType(long beadType) : m_BeadType(beadType) {}

	long m_BeadType;
};


//////////////////////////////////////////////////////////////////////
//
// aaCellNumber
// *************
//
// This function object reads bead data from a CCell profile and calculates 
// the number of beads of the specified type in each cell.
//
//////////////////////////////////////////////////////////////////////

class aaCellNumber  : public unary_function<CCell, double>
{
public:
	aaCellNumber(long beadType);
	~aaCellNumber();

	// operator() function that transforms a CRegion-derived object 
	// into a number. This version counts the number of head beads 
	// for the specified polymer in the region.
	//
	// Note that the const qualifier is not necessary for count_if() but might
	// be needed for more complicated functions

	double operator() (CCell* pRegion) const
	{
		BeadList lBeads = pRegion->GetBeads();

		long beadsOfType = count_if(lBeads.begin(), lBeads.end(), aaBeadType(m_BeadType));

		return static_cast<double>(beadsOfType);
	}

private:
	long m_BeadType;
};

//////////////////////////////////////////////////////////////////////
//
// aaCellXCM, aaCellYCM, aaCellZCM
// *******************************
//
// These function objects read a CCell profile and calculates the X, Y or Z
// coordinate of the centre of mass of beads of the specified type.
//
//////////////////////////////////////////////////////////////////////

class aaCellXCM  : public unary_function<CCell, double>
{
public:
	aaCellXCM(long beadType);
	~aaCellXCM();

	// operator() function that transforms a CRegion-derived object 
	// into a double.
	
	double operator() (CCell* pRegion) const
	{
		BeadList lBeads = pRegion->GetBeads();

		double beadTotal = 0.0;
		double CMPos	 = 0.0;

		for(cBeadListIterator iterBead=lBeads.begin(); iterBead!=lBeads.end(); iterBead++)
		{
			if((*iterBead)->GetType() == m_BeadType)
			{
				beadTotal += 1.0;
				CMPos += (*iterBead)->GetXPos();
			}
		}

		if(beadTotal > 0)
		{
			CMPos /= beadTotal;
		}

		return CMPos;
	}

private:
	long m_BeadType;
};

class aaCellYCM  : public unary_function<CCell, double>
{
public:
	aaCellYCM(long beadType);
	~aaCellYCM();

	// operator() function that transforms a CRegion-derived object 
	// into a double.
	
	double operator() (CCell* pRegion) const
	{
		BeadList lBeads = pRegion->GetBeads();

		double beadTotal = 0.0;
		double CMPos	 = 0.0;

		for(cBeadListIterator iterBead=lBeads.begin(); iterBead!=lBeads.end(); iterBead++)
		{
			if((*iterBead)->GetType() == m_BeadType)
			{
				beadTotal += 1.0;
				CMPos += (*iterBead)->GetYPos();
			}
		}

		if(beadTotal > 0)
		{
			CMPos /= beadTotal;
		}

		return CMPos;
	}

private:
	long m_BeadType;
};

class aaCellZCM  : public unary_function<CCell, double>
{
public:
	aaCellZCM(long beadType);
	~aaCellZCM();

	// operator() function that transforms a CRegion-derived object 
	// into a double.
	
	double operator() (CCell* pRegion) const
	{
		BeadList lBeads = pRegion->GetBeads();

		double beadTotal = 0.0;
		double CMPos	 = 0.0;

		for(cBeadListIterator iterBead=lBeads.begin(); iterBead!=lBeads.end(); iterBead++)
		{
			if((*iterBead)->GetType() == m_BeadType)
			{
				beadTotal += 1.0;
				CMPos += (*iterBead)->GetZPos();
			}
		}

		if(beadTotal > 0)
		{
			CMPos /= beadTotal;
		}

		return CMPos;
	}

private:
	long m_BeadType;
};

//////////////////////////////////////////////////////////////////////
//
// aaCellCM
// ********
//
// This function object reads a CCell profile and calculates the centre of mass 
// of beads of the specified type, storing the result in a newly-created aaVector 
// object before returning a pointer to it. It also calculates the total sum of 
// all the values from the cells and stores it in its member variables. It
// counts the number of times the operator() is applied to cells to normalize
// the cm sum;
// Note that the object must be destroyed by the calling routine before using 
// the function object again or else a memory leak results.
//
//////////////////////////////////////////////////////////////////////

class aaCellCM  : public aaRegionToType<CCell, aaVector>
{
public:
	aaCellCM(long beadType);
	~aaCellCM();

	// operator() function that transforms a CRegion-derived object 
	// into an type.
	
	aaVector* operator() (CCell* pRegion) const
	{
		BeadList lBeads = pRegion->GetBeads();

		double beadTotal = 0.0;
		double X		 = 0.0;
		double Y		 = 0.0;
		double Z		 = 0.0;

		for(cBeadListIterator iterBead=lBeads.begin(); iterBead!=lBeads.end(); iterBead++)
		{
			if((*iterBead)->GetType() == m_BeadType)
			{
				beadTotal += 1.0;
				X += (*iterBead)->GetXPos();
				Y += (*iterBead)->GetYPos();
				Z += (*iterBead)->GetZPos();
			}
		}

		if(beadTotal > 0)
		{
			X /= beadTotal;
			Y /= beadTotal;
			Z /= beadTotal;
		}

		aaVector* pData = new aaVector(X, Y, Z);

		return pData;
	}
};

//////////////////////////////////////////////////////////////////////
//
// aaDensity
// ***********
//
// This function object reads bead data from a CSlice profile and calculates 
// the density of beads of the specified type in each slice, storing the result in 
// a newly-created aaScalar object before returning a pointer to it. Note that 
// the object must be destroyed by the calling routine before using the function
// object again or else a memory leak results.
//
//////////////////////////////////////////////////////////////////////

class aaDensity  : public aaRegionToType<CSlice, aaScalar>
{
public:
	aaDensity(long beadType);
	~aaDensity();

	// operator() function that transforms a CRegion-derived object 
	// into an aaType-derived one. This version counts the number of head beads for the
	// specified polymer in the region and divides by the volume to give the density of
	// polymers of the specified type.
	//
	// Note that the const qualifier is not necessary for count_if() but might
	// be needed for more complicated functions

	aaScalar* operator() (CSlice* pRegion) const
	{
		BeadList lBeads = pRegion->GetBeads();

		long beadsOfType = count_if(lBeads.begin(), lBeads.end(), aaBeadType(m_BeadType));

		aaScalar* pData = new aaScalar(static_cast<double>(beadsOfType)/pRegion->GetVolume());

		return pData;
	}
};

//////////////////////////////////////////////////////////////////////
//
// aaMomentum
// ***********
//
// This function object takes bead data from a CSlice profile and calculates 
// the average momentum of the beads of the specified type in each slice. It 
// stores the result in an 
// aaVector object before returning a pointer to it. The resulting aaVector
// object must be destroyed by the calling routine before further calls to this
// function object are made or else a memory leak results.
//
//////////////////////////////////////////////////////////////////////

class aaMomentum  : public aaRegionToType<CSlice, aaVector>  
{
public:
	aaMomentum(long beadType);
	~aaMomentum();

	// operator() function that transforms a CRegion-derived object 
	// into an aaType-derived one. 

	aaVector* operator() (CSlice* pRegion) const
	{
		BeadList lBeads = pRegion->GetBeads();

		double beadTotal = 0.0;
		double beadXMom	 = 0.0;
		double beadYMom  = 0.0;
		double beadZMom  = 0.0;

		for(cBeadListIterator iterBead=lBeads.begin(); iterBead!=lBeads.end(); iterBead++)
		{
			if((*iterBead)->GetType() == m_BeadType)
			{
				beadTotal += 1.0;
				beadXMom+= (*iterBead)->GetXMom();
				beadYMom+= (*iterBead)->GetYMom();
				beadZMom+= (*iterBead)->GetZMom();
			}
		}

// We have to check that at least one bead of the type was in the slice

		if(beadTotal > 0)	
		{
			beadXMom /= beadTotal;
			beadYMom /= beadTotal;
			beadZMom /= beadTotal;
		}

// Use an aaVector object to hold the mean bead velocity

		aaVector* pData = new aaVector(beadXMom, beadYMom, beadZMom);

		return pData;
	}
};

//////////////////////////////////////////////////////////////////////
//
// aaBeadStressXProfile, aaBeadStressYProfile, aaBeadStressZProfile
// ****************************************************************
//
// These function objects take data from aaStressTensorPoint objects and
// returns it in newly-created aaScalar objects. The data are the contributions
// to the stress tensor from the various bead-bead interactions averaged over
// slices through the SimBox whose normal direction is indicated by the name.
// The bead-bead interactions are identified by the index parameter that maps
// a pair of bead types into a single index.
//
// Note that the aaScalar objects must be destroyed by the calling routine 
// before using the function object again or else a memory leak results.
//
//////////////////////////////////////////////////////////////////////

class aaBeadStressXProfile  : public unary_function<aaStressTensorPoint, aaScalar>
{
public:
	aaBeadStressXProfile(long index);
	~aaBeadStressXProfile();

	// operator() function that transforms data from an aaStressTensorPoint object 
	// into an aaScalar. 
	//
	// Note that the const qualifier is not necessary for count_if() but might
	// be needed for more complicated functions

	aaScalar* operator() (aaStressTensorPoint* pSP) const
	{
		aaScalar* pData = new aaScalar(pSP->GetBeadStressXProfile(m_Index));

		return pData;
	}

private:
	long m_Index;
};

class aaBeadStressYProfile  : public unary_function<aaStressTensorPoint, aaScalar>
{
public:
	aaBeadStressYProfile(long index);
	~aaBeadStressYProfile();

	// operator() function that transforms data from an aaStressTensorPoint object 
	// into an aaScalar. 
	//
	// Note that the const qualifier is not necessary for count_if() but might
	// be needed for more complicated functions

	aaScalar* operator() (aaStressTensorPoint* pSP) const
	{
		aaScalar* pData = new aaScalar(pSP->GetBeadStressYProfile(m_Index));

		return pData;
	}

private:
	long m_Index;
};

class aaBeadStressZProfile  : public unary_function<aaStressTensorPoint, aaScalar>
{
public:
	aaBeadStressZProfile(long index);
	~aaBeadStressZProfile();

	// operator() function that transforms data from an aaStressTensorPoint object 
	// into an aaScalar. 
	//
	// Note that the const qualifier is not necessary for count_if() but might
	// be needed for more complicated functions

	aaScalar* operator() (aaStressTensorPoint* pSP) const
	{
		aaScalar* pData = new aaScalar(pSP->GetBeadStressZProfile(m_Index));

		return pData;
	}

private:
	long m_Index;
};

//////////////////////////////////////////////////////////////////////
//
// aaBondStressXProfile, aaBondStressYProfile, aaBondStressZProfile
// ****************************************************************
//
// These function objects take data from aaStressTensorPoint objects and
// returns it in newly-created aaScalar objects. The data are the contributions
// to the stress tensor from the various (two-body) bond forces averaged over
// slices through the SimBox whose normal direction is indicated by the name.
// The bond interactions are identified by the index parameter that is copied
// from the bond type identifier.
//
// Note that the aaScalar objects must be destroyed by the calling routine 
// before using the function object again or else a memory leak results.
//
//////////////////////////////////////////////////////////////////////

class aaBondStressXProfile  : public unary_function<aaStressTensorPoint, aaScalar>
{
public:
	aaBondStressXProfile(long index);
	~aaBondStressXProfile();

	// operator() function that transforms data from an aaStressTensorPoint object 
	// into an aaScalar. 
	//
	// Note that the const qualifier is not necessary for count_if() but might
	// be needed for more complicated functions

	aaScalar* operator() (aaStressTensorPoint* pSP) const
	{
		aaScalar* pData = new aaScalar(pSP->GetBondStressXProfile(m_Index));

		return pData;
	}

private:
	long m_Index;
};

class aaBondStressYProfile  : public unary_function<aaStressTensorPoint, aaScalar>
{
public:
	aaBondStressYProfile(long index);
	~aaBondStressYProfile();

	// operator() function that transforms data from an aaStressTensorPoint object 
	// into an aaScalar. 
	//
	// Note that the const qualifier is not necessary for count_if() but might
	// be needed for more complicated functions

	aaScalar* operator() (aaStressTensorPoint* pSP) const
	{
		aaScalar* pData = new aaScalar(pSP->GetBondStressYProfile(m_Index));

		return pData;
	}

private:
	long m_Index;
};

class aaBondStressZProfile  : public unary_function<aaStressTensorPoint, aaScalar>
{
public:
	aaBondStressZProfile(long index);
	~aaBondStressZProfile();

	// operator() function that transforms data from an aaStressTensorPoint object 
	// into an aaScalar. 
	//
	// Note that the const qualifier is not necessary for count_if() but might
	// be needed for more complicated functions

	aaScalar* operator() (aaStressTensorPoint* pSP) const
	{
		aaScalar* pData = new aaScalar(pSP->GetBondStressZProfile(m_Index));

		return pData;
	}

private:
	long m_Index;
};

//////////////////////////////////////////////////////////////////////
//
// aaBondPairStressXProfile, aaBondPairStressYProfile, aaBondPairStressZProfile
// ****************************************************************************
//
// These function objects take data from aaStressTensorPoint objects and
// returns it in newly-created aaScalar objects. The data are the contributions
// to the stress tensor from the various (three-body) bondpair forces averaged over
// slices through the SimBox whose normal direction is indicated by the name.
// The bond interactions are identified by the index parameter that is copied
// from the bondpair type identifier.
//
// Note that the aaScalar objects must be destroyed by the calling routine 
// before using the function object again or else a memory leak results.
//
//////////////////////////////////////////////////////////////////////

class aaBondPairStressXProfile  : public unary_function<aaStressTensorPoint, aaScalar>
{
public:
	aaBondPairStressXProfile(long index);
	~aaBondPairStressXProfile();

	// operator() function that transforms data from an aaStressTensorPoint object 
	// into an aaScalar. 
	//
	// Note that the const qualifier is not necessary for count_if() but might
	// be needed for more complicated functions

	aaScalar* operator() (aaStressTensorPoint* pSP) const
	{
		aaScalar* pData = new aaScalar(pSP->GetBondPairStressXProfile(m_Index));

		return pData;
	}

private:
	long m_Index;
};

class aaBondPairStressYProfile  : public unary_function<aaStressTensorPoint, aaScalar>
{
public:
	aaBondPairStressYProfile(long index);
	~aaBondPairStressYProfile();

	// operator() function that transforms data from an aaStressTensorPoint object 
	// into an aaScalar. 
	//
	// Note that the const qualifier is not necessary for count_if() but might
	// be needed for more complicated functions

	aaScalar* operator() (aaStressTensorPoint* pSP) const
	{
		aaScalar* pData = new aaScalar(pSP->GetBondPairStressYProfile(m_Index));

		return pData;
	}

private:
	long m_Index;
};

class aaBondPairStressZProfile  : public unary_function<aaStressTensorPoint, aaScalar>
{
public:
	aaBondPairStressZProfile(long index);
	~aaBondPairStressZProfile();

	// operator() function that transforms data from an aaStressTensorPoint object 
	// into an aaScalar. 
	//
	// Note that the const qualifier is not necessary for count_if() but might
	// be needed for more complicated functions

	aaScalar* operator() (aaStressTensorPoint* pSP) const
	{
		aaScalar* pData = new aaScalar(pSP->GetBondPairStressZProfile(m_Index));

		return pData;
	}

private:
	long m_Index;
};

//////////////////////////////////////////////////////////////////////
//
// aaStressZProfile
// ****************
//
// This function object takes data from aaStressTensorPoint objects and
// returns it in newly-created aaScalar objects. It calculates the
// stress profile from the diagonal elements of the stress tensor averaged 
// over slices through the SimBox whose normal direction is indicated by the 
// name. It also performs a running sum of the stresses and stores the
// surface tension. It includes all bead-bead, bond and bondpair contributions
// to the stress profile.
//
// Note that the aaScalar objects must be destroyed by the calling routine 
// before using the function object again or else a memory leak results.
//
//////////////////////////////////////////////////////////////////////

class aaStressZProfile  : public unary_function<aaStressTensorPoint, aaScalar>
{
public:
	aaStressZProfile();
	~aaStressZProfile();

	// operator() function that transforms data from an aaStressTensorPoint object 
	// into an aaScalar. 
	
	aaScalar* operator() (const aaStressTensorPoint* const pSP) const
	{
		return new aaScalar(pSP->GetZProfile());
	}
};

//////////////////////////////////////////////////////////////////////
//
// aaStressComponentProfile
// ************************
//
// This function object takes data from aaStressTensorPoint objects and
// returns it in newly-created aaScalar objects. It calculates the
// component of the stress tensor profile identified by the index passed in 
// by the calling routine.
//
// Note that the aaScalar objects must be destroyed by the calling routine 
// before using the function object again or else a memory leak results.
//
//////////////////////////////////////////////////////////////////////

class aaStressComponentProfile  : public unary_function<aaStressTensorPoint, aaScalar>
{
public:
	aaStressComponentProfile(long index);
	~aaStressComponentProfile();

	// operator() function that transforms data from an aaStressTensorPoint object 
	// into an aaScalar. 
	
	aaScalar* operator() (const aaStressTensorPoint* const pSP) const
	{
		return new aaScalar(pSP->GetComponent(m_Index));
	}

private:
	long m_Index;
};


//////////////////////////////////////////////////////////////////////
//
// aaBeadXPos, aaBeadYPos, aaBeadZPos
// **********************************
// 
// These function objects return the X, Y or Z coordinates of their
// CAbstractBead* argument.
//
//////////////////////////////////////////////////////////////////////

class aaBeadXPos  : public unary_function<CAbstractBead*, double>  
{
public:
	aaBeadXPos();
	~aaBeadXPos();

	double operator() (const CAbstractBead* const pBead) const
	{
		return pBead->GetXPos();
	}
};

class aaBeadYPos  : public unary_function<CAbstractBead*, double>  
{
public:
	aaBeadYPos();
	~aaBeadYPos();

	double operator() (const CAbstractBead* const pBead) const
	{
		return pBead->GetYPos();
	}
};

class aaBeadZPos  : public unary_function<CAbstractBead*, double>  
{
public:
	aaBeadZPos();
	~aaBeadZPos();

	double operator() (const CAbstractBead* const pBead) const
	{
		return pBead->GetZPos();
	}
};

//////////////////////////////////////////////////////////////////////
//
// aaBeadXMom, aaBeadYMom, aaBeadZMom
// **********************************
// 
// These function objects return the X, Y or Z components of the
// momentum of their CAbstractBead* argument.
//
//////////////////////////////////////////////////////////////////////

class aaBeadXMom  : public unary_function<CAbstractBead*, double>  
{
public:
	aaBeadXMom();
	~aaBeadXMom();

	double operator() (const CAbstractBead* const pBead) const
	{
		return pBead->GetXMom();
	}
};

class aaBeadYMom  : public unary_function<CAbstractBead*, double>  
{
public:
	aaBeadYMom();
	~aaBeadYMom();

	double operator() (const CAbstractBead* const pBead) const
	{
		return pBead->GetYMom();
	}
};

class aaBeadZMom  : public unary_function<CAbstractBead*, double>  
{
public:
	aaBeadZMom();
	~aaBeadZMom();

	double operator() (const CAbstractBead* const pBead) const
	{
		return pBead->GetZMom();
	}
};


//////////////////////////////////////////////////////////////////////
//
// aaBeadXForce, aaBeadYForce, aaBeadZForce
// ****************************************
// 
// These function objects return the X, Y or Z components of the total
// force on their CAbstractBead* argument.
//
//////////////////////////////////////////////////////////////////////

class aaBeadXForce  : public unary_function<CAbstractBead*, double>  
{
public:
	aaBeadXForce();
	~aaBeadXForce();

	double operator() (const CAbstractBead* const pBead) const
	{
		return pBead->GetXForce();
	}
};

class aaBeadYForce  : public unary_function<CAbstractBead*, double>  
{
public:
	aaBeadYForce();
	~aaBeadYForce();

	double operator() (const CAbstractBead* const pBead) const
	{
		return pBead->GetYForce();
	}
};

class aaBeadZForce  : public unary_function<CAbstractBead*, double>  
{
public:
	aaBeadZForce();
	~aaBeadZForce();

	double operator() (const CAbstractBead* const pBead) const
	{
		return pBead->GetZForce();
	}
};

#if EnableShadowSimBox == SimACNEnabled
//////////////////////////////////////////////////////////////////////
//
// aaActiveBondXPos, aaActiveBondYPos, aaActiveBondZPos
// ****************************************************
// 
// These function objects return the X, Y or Z coordinates of their
// aeActiveBond* argument.
//
//////////////////////////////////////////////////////////////////////

class aaActiveBondXPos  : public unary_function<aeActiveBond*, double>  
{
public:
	aaActiveBondXPos();
	~aaActiveBondXPos();

	double operator() (const aeActiveBond* const pBond) const
	{
		return pBond->GetTailXPos();
	}
};

class aaActiveBondYPos  : public unary_function<aeActiveBond*, double>  
{
public:
	aaActiveBondYPos();
	~aaActiveBondYPos();

	double operator() (const aeActiveBond* const pBond) const
	{
		return pBond->GetTailYPos();
	}
};

class aaActiveBondZPos  : public unary_function<aeActiveBond*, double>  
{
public:
	aaActiveBondZPos();
	~aaActiveBondZPos();

	double operator() (const aeActiveBond* const pBond) const
	{
		return pBond->GetTailZPos();
	}
};
#endif


//////////////////////////////////////////////////////////////////////
//
// aaBeadIdEqual
// *************
// 
// This function object returns true if the first bead's id is equal
// to the second's. Because the class has no state, we do not need
// to define a constructor or destructor.
//
//////////////////////////////////////////////////////////////////////

class aaBeadIdEqual  : public unary_function<CAbstractBead, bool>  
{
public:

	result_type operator() (const argument_type* const pInst1, const argument_type* const pInst2) const
	{
		return (pInst1->GetId() == pInst2->GetId());
	}

};

//////////////////////////////////////////////////////////////////////
//
// aaBeadIdGreater
// ***************
// 
// This function object returns true if the first bead's id is greater
// than the second's. Because the class has no state, we do not need
// to define a constructor or destructor.
//
//////////////////////////////////////////////////////////////////////

class aaBeadIdGreater  : public unary_function<CAbstractBead, bool>  
{
public:

	result_type operator() (const argument_type* const pInst1, const argument_type* const pInst2) const
	{
		return (pInst1->GetId() > pInst2->GetId());
	}

};


//////////////////////////////////////////////////////////////////////
//
// aaBeadIdLess
// ************
// 
// This function object returns true if the first bead's id is less
// than the second's. Because the class has no state, we do not need
// to define a constructor or destructor.
//
//////////////////////////////////////////////////////////////////////

class aaBeadIdLess  : public unary_function<CAbstractBead, bool>  
{
public:

	result_type operator() (const argument_type* const pInst1, const argument_type* const pInst2) const
	{
		return (pInst1->GetId() < pInst2->GetId());
	}

};

//////////////////////////////////////////////////////////////////////
//
// aaBeadXPosLess, aaBeadYPosLess, aaBeadZPosLess
// **********************************************
// 
// These function objects return true if the X, Y or Z coordinates
// respectively of their CAbstractBead* arguments are less than the specified value.
//
//////////////////////////////////////////////////////////////////////

class aaBeadXPosLess  : public unary_function<CAbstractBead*, bool>  
{
public:
	aaBeadXPosLess(double limit);
	~aaBeadXPosLess();

	bool operator() (const CAbstractBead* const pBead) const
	{
		return (pBead->GetXPos() < m_Limit);
	}
private:
	const double m_Limit;
};

class aaBeadYPosLess  : public unary_function<CAbstractBead*, bool>  
{
public:
	aaBeadYPosLess(double limit);
	~aaBeadYPosLess();

	bool operator() (const CAbstractBead* const pBead) const
	{
		return (pBead->GetYPos() < m_Limit);
	}
private:
	const double m_Limit;
};


class aaBeadZPosLess  : public unary_function<CAbstractBead*, bool>  
{
public:
	aaBeadZPosLess(double limit);
	~aaBeadZPosLess();

	bool operator() (const CAbstractBead* const pBead) const
	{
		return (pBead->GetZPos() < m_Limit);
	}
private:
	const double m_Limit;
};

//////////////////////////////////////////////////////////////////////
//
// aaBeadXPosGreater, aaBeadYPosGreater, aaBeadZPosGreater
// *******************************************************
// 
// These function objects return true if the X, Y or Z coordinates
// respectively of their CAbstractBead* arguments are greater than 
// the specified value.
//
//////////////////////////////////////////////////////////////////////

class aaBeadXPosGreater  : public unary_function<CAbstractBead*, bool>  
{
public:
	aaBeadXPosGreater(double limit);
	~aaBeadXPosGreater();

	bool operator() (const CAbstractBead* const pBead) const
	{
		return (pBead->GetXPos() > m_Limit);
	}
private:
	const double m_Limit;
};

class aaBeadYPosGreater  : public unary_function<CAbstractBead*, bool>  
{
public:
	aaBeadYPosGreater(double limit);
	~aaBeadYPosGreater();

	bool operator() (const CAbstractBead* const pBead) const
	{
		return (pBead->GetYPos() > m_Limit);
	}
private:
	const double m_Limit;
};


class aaBeadZPosGreater  : public unary_function<CAbstractBead*, bool>  
{
public:
	aaBeadZPosGreater(double limit);
	~aaBeadZPosGreater();

	bool operator() (const CAbstractBead* const pBead) const
	{
		return (pBead->GetZPos() > m_Limit);
	}
private:
	const double m_Limit;
};

//////////////////////////////////////////////////////////////////////
//
// aaBeadXPosGreaterEqual, aaBeadYPosGreaterEqual, aaBeadZPosGreaterEqual
// **********************************************************************
// 
// These function objects return true if the X, Y or Z coordinates
// respectively of their CAbstractBead* arguments are greater or equal
// to the specified value. Although it is not always a good idea to
// use equality when comparing reals, we sometimes need to do this if
// a bead coordinate can take a value exactly equal to a slice coordinate.
//
//////////////////////////////////////////////////////////////////////

class aaBeadXPosGreaterEqual  : public unary_function<CAbstractBead*, bool>  
{
public:
	aaBeadXPosGreaterEqual(double limit);
	~aaBeadXPosGreaterEqual();

	bool operator() (const CAbstractBead* const pBead) const
	{
		return (pBead->GetXPos() >= m_Limit);
	}
private:
	const double m_Limit;
};

class aaBeadYPosGreaterEqual  : public unary_function<CAbstractBead*, bool>  
{
public:
	aaBeadYPosGreaterEqual(double limit);
	~aaBeadYPosGreaterEqual();

	bool operator() (const CAbstractBead* const pBead) const
	{
		return (pBead->GetYPos() >= m_Limit);
	}
private:
	const double m_Limit;
};


class aaBeadZPosGreaterEqual  : public unary_function<CAbstractBead*, bool>  
{
public:
	aaBeadZPosGreaterEqual(double limit);
	~aaBeadZPosGreaterEqual();

	bool operator() (const CAbstractBead* const pBead) const
	{
		return (pBead->GetZPos() >= m_Limit);
	}
private:
	const double m_Limit;
};

//////////////////////////////////////////////////////////////////////
//
// aaTranslateCoordinate
// ****************************************
//
// This function object translates its argument by a constant distance L.
//
//////////////////////////////////////////////////////////////////////

class aaTranslateCoordinate : public unary_function<double, double>
{
public:
	aaTranslateCoordinate(double l);
	~aaTranslateCoordinate();

	double operator() (double x) const
	{
		return (x - m_L);
	}
private:
    const double m_L;
};



// End of function object definitions
// **********************************************************************



#endif // !defined(AFX_AAREGIONTOTYPE_H__7F4A21C1_B13C_11D3_BF19_004095086186__INCLUDED_)
