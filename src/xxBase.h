// xxBase.h: interface for the xxBase class.
//
// This class provides error message handling facilities to all derived classes.
//
// It also defines types that are used throughout the program to represent
// collections of beads, bonds, polymers, commands, etc. To enable this it 
// includes all the STL header files needed by its derived classes. Although this
// makes available container classes that are not needed by all the derived
// classes, it ensures that the typedefs are uniformly defined and used.
// 
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXBASE_H__83457702_3C59_11D3_820E_0060088AD300__INCLUDED_)
#define AFX_XXBASE_H__83457702_3C59_11D3_820E_0060088AD300__INCLUDED_


#include "SimACNFlags.h"

// Forward declarations

#if EnableShadowSimBox == SimACNEnabled
class aeActiveBond;
class aeActiveCellNetwork;
class aeActivePolymer;
class aevActiveEvent;
class IACNAccessControl;
class IModifyActiveCellNetwork;
#endif

class aaScalarProfile;
class aaStressTensorPoint;
class aeCNTCell;
class CAbstractBead;
class CAnalysis;
class CAnalysisData;
class CAnalysisObject;
class CAnalysisTool;
class CBead;
class CBeadType;
class CBeadChargeWrapper;
class CBond;
class CBondType;
class CBondPair;
class CBondPairType;
class CCell;
class CCellProfile;
class CCellProfileSet;
class CCNTCell;
class CCommandTargetNode;
class CCurrentState;
class CDensityFieldState;
class CDensityState;
class CForceTarget;
class CGridObservable;
class CInitialStateData;
class CNanoparticle;
class CObservable;
class CPolymer;
class CPolymerType;
class CPolymerCrossLink;
class CRaft;
class CRow;
class CRowProfile;
class CRowProfileSet;
class CSlice;
class CStressGridCell;
class CTimeSeriesData;
class IActiveSimBox;
class IModifyProcess;
class IRegionAnalysis;
class mpuExtendedBond;
class mpuExtendedBondPair;
class mpuExtendedNanoparticle;
class mpuExtendedNPBond;
class mpuExtendedPolymer;
class mpuGhostBead;
class mpmMessage;
class pmSendGhostBeadCoords;
class taEventSourceDecorator;
class taEventAnalysisDecorator;
class tguArgumentType;
class tguCommandGroup;
class xxCommand;
class xxCommandObject;
class xxEvent;
class xxEventObject;
class xxMessage;
class xxProcess;
class xxProcessObject;


// **********************************************************************
// The following statements are platform-dependent but to avoid including the
// SimDefs.h header file here (where it would be included in every header file)
// we manually change the platform name. To distinguish the platform from the
// one in SimDefs.h I prefix it with xxBase.
// **********************************************************************

#define xxBaseMFC			1
#define xxBaseCONSOLE		2
#define xxBaseDECALPHA		3
#define xxBaseSGICC			4
#define xxBaseCRAYJ90		5
#define xxBaseBORLAND6		6
#define xxBaseI7XEON		7
#define xxBaseI7ITANIUM		8
#define xxBaseGCC 			9
#define xxBaseCW55MAC		10
#define xxBaseXCMAC			11
#define xxBaseCRAYHERMIT    12
#define xxBaseNEWPLATFORM2 	13
#define xxBaseNEWPLATFORM3 	14
#define xxBaseNEWPLATFORM4 	15
#define xxBaseNEWPLATFORM5 	16
#define xxBaseNEWPLATFORM6 	17
#define xxBaseNEWPLATFORM7 	18
#define xxBaseNEWPLATFORM8 	19
#define xxBaseNEWPLATFORM9 	20
#define xxBaseNEWPLATFORM10 21

// Select appropriate definitions for the current platform.

#define xxBasePlatform xxBaseXCMAC


// ****************************************
#if xxBasePlatform == xxBaseCONSOLE	// PC console application

	#include <string>
	#include <list>
	#include <vector>
	#include <map>
	#include <iostream>
	#include <fstream>
	#include <sstream>
	#include <iomanip>
	#include <valarray>

	// **********************************************************************
	// Define typedefs to allow each platform to access the STL containers
	// and other standard libraries using std or not as required.
	// Typedefs beginning with z are used throughout the code. But macros
	// that begin with xxBase are only used in this header file to simplify
	// the replacement of the std name.

	typedef std::istream				zInStream;
	typedef std::ostream				zOutStream;
	typedef std::ifstream				zInFileStream;
	typedef std::ofstream				zOutFileStream;
	typedef std::fstream				zFileStream;
	typedef std::istringstream			zInStringStream;
	typedef std::ostringstream			zOutStringStream;
	typedef std::ios					zIos;

	// Define macros to wrap the std namespace name for different platforms

	#define zEndl						std::endl
	#define zFlush						std::flush
	#define zLeft						std::left
	#define xxBaselist					std::list
	#define xxBasemap					std::map
	#define xxBasemultimap				std::multimap
	#define xxBasestring				std::basic_string<char>

	// **********************************************************************
	// class xxBasevector
	// ******************
	//
	// Local vector class to provide the member at() functions for those platforms that
	// do not have it in their C++ language. Both non-const and const versions are
	// needed.
	// I have to provide a default constructor with no arguments and one which sets each
	// element of the vector to a specified initial value to accomodate the various
	// cases in the code. In particular, see Builder.cpp where the initial value form is
	// required. A copy constructor is also needed, but I hope that the assignment and 
	// comparison operators will default to the base class ones.

	template <class T>
	class xxBasevector : public std::vector<T>
	{
	public:
		xxBasevector() : std::vector<T>(){}
		xxBasevector(size_type num, const T& val=T()) : std::vector<T>(num,val){}
		xxBasevector(const xxBasevector& oldVec) : std::vector<T>(oldVec){}
		~xxBasevector(){}

//		inline T& at(size_type i)             {return (*(begin() + i)); }
//		inline const T& at(size_type i) const {return (*(begin() + i)); }
	};

	// The following pair functions have to take different forms on the Cray platform

	typedef std::pair<long, long>					zPairLL;
	typedef std::pair<long, double>					zPairLD;
	typedef std::pair<long, xxBasestring>			zPairLS;
	typedef std::pair<xxBasestring, long>			zPairSL;
	typedef std::pair<xxBasestring, double>			zPairSD;
	typedef std::pair<xxBasestring, xxBasestring>	zPairSS;
	typedef std::pair<long, CBeadType*>				zPairLongBeadType;
	typedef std::pair<long, CBondType*>				zPairLongBondType;
	typedef std::pair<long, CBondPairType*>			zPairLongBondPairType;
	typedef std::pair<long, CPolymerType*>			zPairLongPolymerType;
	typedef std::pair<long, CAbstractBead*>			zPairLongBead;
	typedef std::pair<long, CBond*>					zPairLongBond;
	typedef std::pair<long, CBondPair*>				zPairLongBondPair;
	typedef std::pair<long, CPolymer*>				zPairLongPolymer;
	typedef std::pair<long, mpuGhostBead*>	        zPairLongGhostBead;
	typedef std::pair<long, mpuExtendedBond*>	    zPairLongExtendedBond;
	typedef std::pair<long, mpuExtendedNPBond*>	    zPairLongExtendedNPBond;
	typedef std::pair<long, mpuExtendedPolymer*>	zPairLongExtendedPolymer;
	typedef std::pair<long, CCommandTargetNode*>	zPairLongTarget;
	typedef std::pair<long, CNanoparticle*>	        zPairLongNanoparticle;

	typedef std::pair<xxBasestring, tguArgumentType*>			    zPairSVAT;
	typedef std::pair<xxBasestring, CCommandTargetNode*>			zPairST;
	typedef std::pair<xxBasestring, IModifyProcess*>				zPairSMP;
	typedef std::pair<xxBasestring, taEventSourceDecorator*>		zPairSESD;
	typedef std::pair<xxBasestring, taEventAnalysisDecorator*>		zPairSEAD;

#if EnableShadowSimBox == SimACNEnabled
	typedef std::pair<xxBasestring, aeActiveCellNetwork*>		    zPairSACN;
	typedef std::pair<xxBasestring, IModifyActiveCellNetwork*>		zPairSMACN;
	typedef std::pair<xxBasestring, IACNAccessControl*>				zPairSAACN;

    typedef aeActiveCellNetwork* (*CreateACNCallback)(IActiveSimBox* const pShadow, long xn, long yn, long zn,
					                                  double xw, double yw, double zw);
	typedef aevActiveEvent*		(*CreateActiveEventCallback)();
#endif
	// Typedef for the factory classes that are used to create the commands,
	// events, processes, etc.
	// These are called: acfCommandFactory, etc.

	typedef CAnalysisData*		   (*CreateAnalysisCallback)();
	typedef CInitialStateData* (*CreateInitialStateCallback)();
	typedef mpmMessage*		(*CreateParallelMessageCallback)();
	typedef xxCommand*				(*CreateCommandCallback)(long executionTime);
	typedef xxEvent*				  (*CreateEventCallback)();
	typedef xxProcess*				(*CreateProcessCallback)();

	// The following typedef uses valarrays where available or vectors where not

	typedef std::valarray<size_t>				zLongValArray;
	typedef std::valarray<double>				zDoubleValArray;

// ****************************************
#elif xxBasePlatform == xxBaseXCMAC	// Mac OS X/XCode compiler console app

    #include <cstdint>     // Needed for uint32_t and uint64_t types
    #include <string.h>    // Needed for parallel compilation using mpicxx
    #include <string>
	#include <list>
	#include <vector>
	#include <map>
	#include <iostream>
	#include <fstream>
	#include <sstream>
	#include <iomanip>
	#include <valarray>

	// **********************************************************************
	// Define typedefs to allow each platform to access the STL containers
	// and other standard libraries using std or not as required.
	// Typedefs beginning with z are used throughout the code. But macros
	// that begin with xxBase are only used in this header file to simplify
	// the replacement of the std name.

	typedef std::istream				zInStream;
	typedef std::ostream				zOutStream;
	typedef std::ifstream				zInFileStream;
	typedef std::ofstream				zOutFileStream;
	typedef std::fstream				zFileStream;
	typedef std::istringstream			zInStringStream;
	typedef std::ostringstream			zOutStringStream;
	typedef std::ios					zIos;

	// Define macros to wrap the std namespace name for different platforms

	#define zEndl						std::endl
	#define zFlush						std::flush
	#define zLeft						std::left
	#define xxBaselist					std::list
	#define xxBasemap					std::map
	#define xxBasemultimap				std::multimap
	#define xxBasestring				std::basic_string<char>

	// Macros needed for STL types

	#define size_type						long

	// **********************************************************************
	// class xxBasevector
	// ******************
	//
	// Local vector class to provide the member at() functions for those platforms that
	// do not have it in their C++ language. Both non-const and const versions are
	// needed.
	// I have to provide a default constructor with no arguments and one which sets each
	// element of the vector to a specified initial value to accomodate the various
	// cases in the code. In particular, see Builder.cpp where the initial value form is
	// required. A copy constructor is also needed, but I hope that the assignment and 
	// comparison operators will default to the base class ones.

	template <class T>
	class xxBasevector : public std::vector<T>
	{
	public:
		xxBasevector() : std::vector<T>(){}
		xxBasevector(size_type num, const T& val=T()) : std::vector<T>(num,val){}
		xxBasevector(const xxBasevector& oldVec) : std::vector<T>(oldVec){}
		~xxBasevector(){}

//		inline T& at(size_type i)             {return (*(begin() + i)); }
//		inline const T& at(size_type i) const {return (*(begin() + i)); }
	};

	// The following pair functions have to take different forms on the Cray platform

	typedef std::pair<long, long>					zPairLL;
	typedef std::pair<long, double>					zPairLD;
	typedef std::pair<long, xxBasestring>			zPairLS;
	typedef std::pair<xxBasestring, long>			zPairSL;
	typedef std::pair<xxBasestring, double>			zPairSD;
	typedef std::pair<xxBasestring, xxBasestring>	zPairSS;
	typedef std::pair<long, CBeadType*>				zPairLongBeadType;
	typedef std::pair<long, CBondType*>				zPairLongBondType;
	typedef std::pair<long, CBondPairType*>			zPairLongBondPairType;
	typedef std::pair<long, CPolymerType*>			zPairLongPolymerType;
	typedef std::pair<long, CAbstractBead*>			zPairLongBead;
	typedef std::pair<long, CBond*>					zPairLongBond;
	typedef std::pair<long, CBondPair*>				zPairLongBondPair;
	typedef std::pair<long, CPolymer*>				zPairLongPolymer;
	typedef std::pair<long, mpuGhostBead*>	        zPairLongGhostBead;
	typedef std::pair<long, mpuExtendedBond*>	    zPairLongExtendedBond;
	typedef std::pair<long, mpuExtendedNPBond*>	    zPairLongExtendedNPBond;
	typedef std::pair<long, mpuExtendedPolymer*>	zPairLongExtendedPolymer;
	typedef std::pair<long, CCommandTargetNode*>	zPairLongTarget;
	typedef std::pair<long, CNanoparticle*>	        zPairLongNanoparticle;

	typedef std::pair<xxBasestring, tguArgumentType*>			    zPairSVAT;
	typedef std::pair<xxBasestring, CCommandTargetNode*>			zPairST;
	typedef std::pair<xxBasestring, IModifyProcess*>				zPairSMP;
	typedef std::pair<xxBasestring, taEventSourceDecorator*>		zPairSESD;
	typedef std::pair<xxBasestring, taEventAnalysisDecorator*>		zPairSEAD;

#if EnableShadowSimBox == SimACNEnabled
	typedef std::pair<xxBasestring, aeActiveCellNetwork*>		    zPairSACN;
	typedef std::pair<xxBasestring, IModifyActiveCellNetwork*>		zPairSMACN;
	typedef std::pair<xxBasestring, IACNAccessControl*>				zPairSAACN;
	typedef aeActiveCellNetwork* (*CreateACNCallback)(IActiveSimBox* const pShadow, long xn, long yn, long zn,
					                                  double xw, double yw, double zw);
	typedef aevActiveEvent*		(*CreateActiveEventCallback)();
#endif

	// Typedef for the factory classes that are used to create the commands,
	// events, processes, etc.
	// These are called: acfCommandFactory, etc.

	typedef CAnalysisData*		   (*CreateAnalysisCallback)();
	typedef CInitialStateData* (*CreateInitialStateCallback)();
	typedef mpmMessage*		(*CreateParallelMessageCallback)();
	typedef xxCommand*				(*CreateCommandCallback)(long executionTime);
	typedef xxEvent*				  (*CreateEventCallback)();
	typedef xxProcess*				(*CreateProcessCallback)();

	// The following typedef uses valarrays where available or vectors where not

	typedef std::valarray<size_t>				zLongValArray;
	typedef std::valarray<double>				zDoubleValArray;

// ****************************************
#elif xxBasePlatform == xxBaseCRAYHERMIT	// hermit1.hww.de Cray

	#include <string.h>
	#include <list>
	#include <vector>
	#include <map>
	#include <iostream>
	#include <fstream>
	#include <sstream>
	#include <iomanip>
	#include <valarray>

	// **********************************************************************
	// Define typedefs to allow each platform to access the STL containers
	// and other standard libraries using std or not as required.
	// Typedefs beginning with z are used throughout the code. But macros
	// that begin with xxBase are only used in this header file to simplify
	// the replacement of the std name.

	typedef std::istream				zInStream;
	typedef std::ostream				zOutStream;
	typedef std::ifstream				zInFileStream;
	typedef std::ofstream				zOutFileStream;
	typedef std::fstream				zFileStream;
	typedef std::istringstream			zInStringStream;
	typedef std::ostringstream			zOutStringStream;
	typedef std::ios					zIos;

	// Define macros to wrap the std namespace name for different platforms

	#define zEndl						std::endl
	#define zFlush						std::flush
	#define zLeft						std::left
	#define xxBaselist					std::list
	#define xxBasemap					std::map
	#define xxBasemultimap				std::multimap
	#define xxBasestring				std::basic_string<char>

	// Macros needed for STL types

	#define size_type						long

	// **********************************************************************
	// class xxBasevector
	// ******************
	//
	// Local vector class to provide the member at() functions for those platforms that
	// do not have it in their C++ language. Both non-const and const versions are
	// needed.
	// I have to provide a default constructor with no arguments and one which sets each
	// element of the vector to a specified initial value to accomodate the various
	// cases in the code. In particular, see Builder.cpp where the initial value form is
	// required. A copy constructor is also needed, but I hope that the assignment and 
	// comparison operators will default to the base class ones.

	template <class T>
	class xxBasevector : public std::vector<T>
	{
	public:
		xxBasevector() : std::vector<T>(){}
		xxBasevector(size_type num, const T& val=T()) : std::vector<T>(num,val){}
		xxBasevector(const xxBasevector& oldVec) : std::vector<T>(oldVec){}
		~xxBasevector(){}

//		inline T& at(size_type i)             {return (*(begin() + i)); }
//		inline const T& at(size_type i) const {return (*(begin() + i)); }
	};

	// The following pair functions have to take different forms on the Cray platform

	typedef std::pair<long, long>					zPairLL;
	typedef std::pair<long, double>					zPairLD;
	typedef std::pair<long, xxBasestring>			zPairLS;
	typedef std::pair<xxBasestring, long>			zPairSL;
	typedef std::pair<xxBasestring, double>			zPairSD;
	typedef std::pair<xxBasestring, xxBasestring>	zPairSS;
	typedef std::pair<long, CBeadType*>				zPairLongBeadType;
	typedef std::pair<long, CBondType*>				zPairLongBondType;
	typedef std::pair<long, CBondPairType*>			zPairLongBondPairType;
	typedef std::pair<long, CPolymerType*>			zPairLongPolymerType;
	typedef std::pair<long, CAbstractBead*>			zPairLongBead;
	typedef std::pair<long, CBond*>					zPairLongBond;
	typedef std::pair<long, CBondPair*>				zPairLongBondPair;
	typedef std::pair<long, CPolymer*>				zPairLongPolymer;
	typedef std::pair<long, mpuGhostBead*>	        zPairLongGhostBead;
	typedef std::pair<long, mpuExtendedBond*>	    zPairLongExtendedBond;
	typedef std::pair<long, mpuExtendedNPBond*>	    zPairLongExtendedNPBond;
	typedef std::pair<long, mpuExtendedPolymer*>	zPairLongExtendedPolymer;
	typedef std::pair<long, CCommandTargetNode*>	zPairLongTarget;
	typedef std::pair<long, CNanoparticle*>	        zPairLongNanoparticle;

	typedef std::pair<xxBasestring, tguArgumentType*>			    zPairSVAT;
	typedef std::pair<xxBasestring, CCommandTargetNode*>			zPairST;
	typedef std::pair<xxBasestring, IModifyProcess*>				zPairSMP;
	typedef std::pair<xxBasestring, taEventSourceDecorator*>		zPairSESD;
	typedef std::pair<xxBasestring, taEventAnalysisDecorator*>		zPairSEAD;

#if EnableShadowSimBox == SimACNEnabled
	typedef std::pair<xxBasestring, aeActiveCellNetwork*>		    zPairSACN;
	typedef std::pair<xxBasestring, IModifyActiveCellNetwork*>		zPairSMACN;
	typedef std::pair<xxBasestring, IACNAccessControl*>				zPairSAACN;
	typedef aeActiveCellNetwork* (*CreateACNCallback)(IActiveSimBox* const pShadow, long xn, long yn, long zn,
					                                  double xw, double yw, double zw);
	typedef aevActiveEvent*		(*CreateActiveEventCallback)();
#endif

	// Typedef for the factory classes that are used to create the commands,
	// events, processes, etc.
	// These are called: acfCommandFactory, etc.

	typedef CAnalysisData*		   (*CreateAnalysisCallback)();
	typedef CInitialStateData* (*CreateInitialStateCallback)();
	typedef mpmMessage*		(*CreateParallelMessageCallback)();
	typedef xxCommand*				(*CreateCommandCallback)(long executionTime);
	typedef xxEvent*				  (*CreateEventCallback)();
	typedef xxProcess*				(*CreateProcessCallback)();

	// The following typedef uses valarrays where available or vectors where not

	typedef std::valarray<size_t>				zLongValArray;
	typedef std::valarray<double>				zDoubleValArray;

// ****************************************
#elif xxBasePlatform == xxBaseCW55MAC	// Mac OS X/Code Warrior compiler

	#include <string>
	#include <list>
	#include <vector>
	#include <map>
	#include <iostream>
	#include <fstream>
	#include <sstream>
	#include <iomanip>
	#include <valarray>

	// **********************************************************************
	// Define typedefs to allow each platform to access the STL containers
	// and other standard libraries using std or not as required.
	// Typedefs beginning with z are used throughout the code. But macros
	// that begin with xxBase are only used in this header file to simplify
	// the replacement of the std name.

	typedef std::istream				zInStream;
	typedef std::ostream				zOutStream;
	typedef std::ifstream				zInFileStream;
	typedef std::ofstream				zOutFileStream;
	typedef std::fstream				zFileStream;
	typedef std::istringstream			zInStringStream;
	typedef std::ostringstream			zOutStringStream;
	typedef std::ios					zIos;

	// Define macros to wrap the std namespace name for different platforms

	#define zEndl						std::endl
	#define zFlush						std::flush
	#define zLeft						std::left
	#define xxBaselist					std::list
	#define xxBasemap					std::map
	#define xxBasemultimap				std::multimap
	#define xxBasestring				std::basic_string<char>

	// Macros needed for STL types

	#define size_type						long

	// **********************************************************************
	// class xxBasevector
	// ******************
	//
	// Local vector class to provide the member at() functions for those platforms that
	// do not have it in their C++ language. Both non-const and const versions are
	// needed.
	// I have to provide a default constructor with no arguments and one which sets each
	// element of the vector to a specified initial value to accomodate the various
	// cases in the code. In particular, see Builder.cpp where the initial value form is
	// required. A copy constructor is also needed, but I hope that the assignment and 
	// comparison operators will default to the base class ones.

	template <class T>
	class xxBasevector : public std::vector<T>
	{
	public:
		xxBasevector() : std::vector<T>(){}
		xxBasevector(size_type num, const T& val=T()) : std::vector<T>(num,val){}
		xxBasevector(const xxBasevector& oldVec) : std::vector<T>(oldVec){}
		~xxBasevector(){}

//		inline T& at(size_type i)             {return (*(begin() + i)); }
//		inline const T& at(size_type i) const {return (*(begin() + i)); }
	};

	// The following pair functions have to take different forms on the Cray platform

	typedef std::pair<long, long>					zPairLL;
	typedef std::pair<long, double>					zPairLD;
	typedef std::pair<long, xxBasestring>			zPairLS;
	typedef std::pair<xxBasestring, long>			zPairSL;
	typedef std::pair<xxBasestring, double>			zPairSD;
	typedef std::pair<xxBasestring, xxBasestring>	zPairSS;
	typedef std::pair<long, CBeadType*>				zPairLongBeadType;
	typedef std::pair<long, CBondType*>				zPairLongBondType;
	typedef std::pair<long, CBondPairType*>			zPairLongBondPairType;
	typedef std::pair<long, CPolymerType*>			zPairLongPolymerType;
	typedef std::pair<long, CAbstractBead*>			zPairLongBead;
	typedef std::pair<long, CBond*>					zPairLongBond;
	typedef std::pair<long, CBondPair*>				zPairLongBondPair;
	typedef std::pair<long, CPolymer*>				zPairLongPolymer;
	typedef std::pair<long, mpuGhostBead*>	        zPairLongGhostBead;
	typedef std::pair<long, mpuExtendedBond*>	    zPairLongExtendedBond;
	typedef std::pair<long, mpuExtendedNPBond*>	    zPairLongExtendedNPBond;
	typedef std::pair<long, mpuExtendedPolymer*>	zPairLongExtendedPolymer;
	typedef std::pair<long, CCommandTargetNode*>	zPairLongTarget;
	typedef std::pair<long, CNanoparticle*>	        zPairLongNanoparticle;

	typedef std::pair<xxBasestring, tguArgumentType*>			    zPairSVAT;
	typedef std::pair<xxBasestring, CCommandTargetNode*>			zPairST;
	typedef std::pair<xxBasestring, IModifyProcess*>				zPairSMP;
	typedef std::pair<xxBasestring, taEventSourceDecorator*>		zPairSESD;
	typedef std::pair<xxBasestring, taEventAnalysisDecorator*>		zPairSEAD;

#if EnableShadowSimBox == SimACNEnabled
	typedef std::pair<xxBasestring, aeActiveCellNetwork*>		    zPairSACN;
	typedef std::pair<xxBasestring, IModifyActiveCellNetwork*>		zPairSMACN;
	typedef std::pair<xxBasestring, IACNAccessControl*>				zPairSAACN;
	typedef aeActiveCellNetwork* (*CreateACNCallback)(IActiveSimBox* const pShadow, long xn, long yn, long zn,
					                                  double xw, double yw, double zw);
	typedef aevActiveEvent*		(*CreateActiveEventCallback)();
#endif

	// Typedef for the factory classes that are used to create the commands,
	// events, processes, etc.
	// These are called: acfCommandFactory, etc.

	typedef CAnalysisData*		   (*CreateAnalysisCallback)();
	typedef CInitialStateData* (*CreateInitialStateCallback)();
	typedef mpmMessage*		(*CreateParallelMessageCallback)();
	typedef xxCommand*				(*CreateCommandCallback)(long executionTime);
	typedef xxEvent*				  (*CreateEventCallback)();
	typedef xxProcess*				(*CreateProcessCallback)();

	// The following typedef uses valarrays where available or vectors where not

	typedef std::valarray<size_t>				zLongValArray;
	typedef std::valarray<double>				zDoubleValArray;

// ****************************************
#elif xxBasePlatform == xxBaseDECALPHA	// Dec alpha machines

	#include <string>
	#include <list>
	#include <vector>
	#include <map>
	#include <iostream>
	#include <fstream>
	#include <sstream>
	#include <iomanip>
	#include <valarray>

	typedef std::istream				zInStream;
	typedef std::ostream				zOutStream;
	typedef std::ifstream				zInFileStream;
	typedef std::ofstream				zOutFileStream;
	typedef std::fstream				zFileStream;
	typedef std::istringstream			zInStringStream;
	typedef std::ostringstream			zOutStringStream;
	typedef std::ios					zIos;

	#define zEndl						std::endl
	#define zFlush						std::flush
	#define zLeft						std::left
	#define xxBaselist					std::list
	#define xxBasemap					std::map
	#define xxBasemultimap				std::multimap

	template <class T>
	class xxBasevector : public std::vector<T>
	{
	public:
		xxBasevector() : std::vector<T>(){}
		xxBasevector(size_type num, const T& val=T()) : std::vector<T>(num,val){}
		xxBasevector(const xxBasevector& oldVec) : std::vector<T>(oldVec){}
		~xxBasevector(){}
	};

	class xxBasestring : public std::basic_string<char>
	{
	public:
		xxBasestring() : std::basic_string<char>(){}
		xxBasestring(std::string bStr) : std::basic_string<char>(bStr){}
		xxBasestring(const char* cStr) : std::basic_string<char>(cStr){}
		xxBasestring(const xxBasestring& oldStr) : std::basic_string<char>(oldStr){}
		~xxBasestring(){}
	};

	// The following pair functions have to take different forms on the Cray platform

	typedef pair<long, long>						zPairLL;
	typedef pair<long, double>						zPairLD;
	typedef pair<long, xxBasestring>				zPairLS;
	typedef pair<xxBasestring, long>				zPairSL;
	typedef pair<xxBasestring, double>			    zPairSD;
	typedef pair<xxBasestring, xxBasestring>		zPairSS;
	typedef pair<long, CBeadType*>					zPairLongBeadType;
	typedef pair<long, CBondType*>					zPairLongBondType;
	typedef pair<long, CBondPairType*>				zPairLongBondPairType;
	typedef pair<long, CPolymerType*>				zPairLongPolymerType;
	typedef pair<long, CAbstractBead*>				zPairLongBead;
	typedef pair<long, CBond*>						zPairLongBond;
	typedef pair<long, CBondPair*>					zPairLongBondPair;
	typedef pair<long, CPolymer*>					zPairLongPolymer;
	typedef pair<long, mpuGhostBead*>	            zPairLongGhostBead;
	typedef pair<long, mpuExtendedBond*>	        zPairLongExtendedBond;
	typedef pair<long, mpuExtendedNPBond*>          zPairLongExtendedNPBond;
	typedef pair<long, mpuExtendedPolymer*>	        zPairLongExtendedPolymer;
	typedef pair<long, CCommandTargetNode*>			zPairLongTarget;
	typedef pair<long, CNanoparticle*>	            zPairLongNanoparticle;

	typedef pair<xxBasestring, tguArgumentType*>			    zPairSVAT;
	typedef pair<xxBasestring, CCommandTargetNode*>				zPairST;
	typedef pair<xxBasestring, IModifyProcess*>					zPairSMP;
	typedef pair<xxBasestring, taEventSourceDecorator*>	        zPairSESD;
	typedef pair<xxBasestring, taEventAnalysisDecorator*>       zPairSEAD;

#if EnableShadowSimBox == SimACNEnabled
	typedef pair<xxBasestring, aeActiveCellNetwork*>		    zPairSACN;
	typedef pair<xxBasestring, IModifyActiveCellNetwork*>		zPairSMACN;
	typedef pair<xxBasestring, IACNAccessControl*>				zPairSAACN;
	typedef aeActiveCellNetwork* (*CreateACNCallback)(IActiveSimBox* const pShadow, long xn, long yn, long zn,
					                                  double xw, double yw, double zw);
	typedef aevActiveEvent*		(*CreateActiveEventCallback)();
#endif

	// Typedef for the factory classes that are used to create the commands,
	// events, processes, etc.
	// These are called: acfCommandFactory, acfAnalysisFactory, 
	// acfEventFactory, acfProcessFactory  

	typedef CAnalysisData*		   (*CreateAnalysisCallback)();
	typedef CInitialStateData* (*CreateInitialStateCallback)();
	typedef mpmMessage*		(*CreateParallelMessageCallback)();
	typedef xxCommand*				(*CreateCommandCallback)(long executionTime);
	typedef xxEvent*				  (*CreateEventCallback)();
	typedef xxProcess*				(*CreateProcessCallback)();


	// The following typedef uses valarrays where available or vectors where not

	typedef std::valarray<size_t>			zLongValArray;
	typedef std::valarray<double>			zDoubleValArray;


// ****************************************
#elif xxBasePlatform == xxBaseSGICC	// SGI platform with CC compiler

	#include <utility>
	#include <vector.h>
	#include <string>
	#include <list>
	#include <map>
	#include <iostream.h>
	#include <fstream.h>
	#include <strstream.h>
	#include <iomanip.h>

	typedef istream					zInStream;
	typedef ostream					zOutStream;
	typedef ifstream				zInFileStream;
	typedef ofstream				zOutFileStream;
	typedef fstream					zFileStream;
	typedef istrstream				zInStringStream;
	typedef ostrstream				zOutStringStream;
	typedef ios						zIos;

	#define zEndl						"\n"
	#define zFlush						" "
	#define zLeft						" "
	#define xxBaselist					std::list
	#define xxBasemap					std::map
	#define xxBasemultimap				std::multimap

	// The vector class is not in the std namespace, so don't prefix with std::.

	template <class T>
	class xxBasevector : public vector<T>
	{
	public:
		xxBasevector() : vector<T>(){}
		xxBasevector(size_type num, const T& val=T()) : vector<T>(num,val){}
		xxBasevector(const xxBasevector& oldVec) : vector<T>(oldVec){}
		~xxBasevector(){}

		inline T& at(size_type i)             {return (*(begin() + i)); }
		inline const T& at(size_type i) const {return (*(begin() + i)); }
	};

	// The basic_string<> class is in the std namespace, so we use the prefix std::

	class xxBasestring : public std::basic_string<char>
	{
	public:
		xxBasestring() : std::basic_string<char>(){}
		xxBasestring(std::basic_string<char> bStr) : std::basic_string<char>(bStr){}
		xxBasestring(const char* cStr) : std::basic_string<char>(cStr){}
		xxBasestring(const xxBasestring& oldStr) : std::basic_string<char>(oldStr){}
		~xxBasestring(){}

		friend inline bool operator!=(const xxBasestring& L, const xxBasestring& R) {return (!(L == R));}
		friend inline bool operator!=(const char *L, const xxBasestring& R)			{return (!(L == R));}
		friend inline bool operator!=(const xxBasestring& L, const char *R)			{return (!(L == R));}
	};

	// The following pair functions have to take different forms on the Cray platform

	typedef pair<long, long>						zPairLL;
	typedef pair<long, double>						zPairLD;
	typedef pair<long, xxBasestring>				zPairLS;
	typedef pair<xxBasestring, long>				zPairSL;
	typedef pair<xxBasestring, double>			    zPairSD;
	typedef pair<xxBasestring, xxBasestring>		zPairSS;
	typedef pair<long, CBeadType*>					zPairLongBeadType;
	typedef pair<long, CBondType*>					zPairLongBondType;
	typedef pair<long, CBondPairType*>				zPairLongBondPairType;
	typedef pair<long, CPolymerType*>				zPairLongPolymerType;
	typedef pair<long, CAbstractBead*>				zPairLongBead;
	typedef pair<long, CBond*>						zPairLongBond;
	typedef pair<long, CBondPair*>					zPairLongBondPair;
	typedef pair<long, CPolymer*>					zPairLongPolymer;
	typedef pair<long, mpuGhostBead*>	            zPairLongGhostBead;
	typedef pair<long, mpuExtendedBond*>	        zPairLongExtendedBond;
	typedef pair<long, mpuExtendedNPBond*>	        zPairLongExtendedNPBond;
	typedef pair<long, mpuExtendedPolymer*>	        zPairLongExtendedPolymer;
	typedef pair<long, CCommandTargetNode*>			zPairLongTarget;
	typedef pair<long, CNanoparticle*>	            zPairLongNanoparticle;

	typedef pair<xxBasestring, tguArgumentType*>			    zPairSVAT;
	typedef pair<xxBasestring, CCommandTargetNode*>				zPairST;
	typedef pair<xxBasestring, IModifyProcess*>					zPairSMP;
	typedef pair<xxBasestring, taEventSourceDecorator*>	        zPairSESD;
	typedef pair<xxBasestring, taEventAnalysisDecorator*>	    zPairSEAD;

#if EnableShadowSimBox == SimACNEnabled
	typedef pair<xxBasestring, aeActiveCellNetwork*>		    zPairSACN;
	typedef pair<xxBasestring, IModifyActiveCellNetwork*>		zPairSMACN;
	typedef pair<xxBasestring, IACNAccessControl*>				zPairSAACN;
	typedef aeActiveCellNetwork* (*CreateACNCallback)(IActiveSimBox* const pShadow, long xn, long yn, long zn,
					                                  double xw, double yw, double zw);
	typedef aevActiveEvent*		(*CreateActiveEventCallback)();
#endif

	// Typedef for the factory classes that are used to create the commands,
	// events, processes, etc.
	// These are called: acfCommandFactory, acfAnalysisFactory, 
	// acfEventFactory, acfProcessFactory  

	typedef CAnalysisData*		   (*CreateAnalysisCallback)();
	typedef CInitialStateData* (*CreateInitialStateCallback)();
	typedef mpmMessage*		(*CreateParallelMessageCallback)();
	typedef xxCommand*				(*CreateCommandCallback)(long executionTime);
	typedef xxEvent*				  (*CreateEventCallback)();
	typedef xxProcess*				(*CreateProcessCallback)();

	// The following typedef uses valarrays where available or vectors where not

	typedef std::vector<long>					zLongValArray;
	typedef std::vector<double>					zDoubleValArray;


// ****************************************
#elif xxBasePlatform == xxBaseCRAYJ90		// Cray platform with CC compiler

	#include <stl_config.h>
	#include <mstring.h>
	#include <list.h>
	#include <vector.h>
	#include <map.h>
	#include <pair.h>
	#include <iostream.h>
	#include <fstream.h>
	#include <strstream.h>
	#include <iomanip.h>

	typedef istream					zInStream;
	typedef ostream					zOutStream;
	typedef ifstream				zInFileStream;
	typedef ofstream				zOutFileStream;
	typedef fstream					zFileStream;
	typedef istrstream				zInStringStream;
	typedef ostrstream				zOutStringStream;
	typedef ios						zIos;


	#define zEndl					"\n"
	#define zFlush					" "
	#define zLeft					" "
	#define xxBaselist				list
	#define xxBasemap				map
	#define xxBasemultimap			multimap


	template <class T>
	class xxBasevector : public vector<T>
	{
	public:
		xxBasevector() : vector<T>(){}
		xxBasevector(size_type num, const T& val=T()) : vector<T>(num,val){}
		xxBasevector(const xxBasevector& oldVec) : vector<T>(oldVec){}
		~xxBasevector(){}

		inline T& at(size_type i)             {return (*(begin() + i)); }
		inline const T& at(size_type i) const {return (*(begin() + i)); }
	};


	class xxBasestring : public basic_string<char>
	{
	public:
		xxBasestring() : basic_string<char>(){}
		xxBasestring(basic_string<char> bStr) : basic_string<char>(bStr){}
		xxBasestring(const char* cStr) : basic_string<char>(cStr){}
		xxBasestring(const xxBasestring& oldStr) : basic_string<char>(oldStr){}
		~xxBasestring(){}

		inline const char& at(size_type i) const {return (*(begin()+i));}

//		friend inline bool operator!=(const xxBasestring& L, const xxBasestring& R) {return (!(L == R));}
//		friend inline bool operator!=(const char *L, const xxBasestring& R)		    {return (!(L == R));}
//		friend inline bool operator!=(const xxBasestring& L, const char *R)		    {return (!(L == R));}

	};


	// The following pair functions have to take different forms on the Cray platform

	typedef pair<const long, long>					zPairLL;
	typedef pair<long, double>						zPairLD;
	typedef pair<const long, xxBasestring>			zPairLS;
	typedef pair<const xxBasestring, long>			zPairSL;
	typedef pair<xxBasestring, double>			    zPairSD;
	typedef pair<xxBasestring, xxBasestring>		zPairSS;
	typedef pair<const long, CBeadType*>			zPairLongBeadType;
	typedef pair<const long, CBondType*>			zPairLongBondType;
	typedef pair<const long, CBondPairType*>		zPairLongBondPairType;
	typedef pair<const long, CPolymerType*>			zPairLongPolymerType;
	typedef pair<const long, CAbstractBead*>		zPairLongBead;
	typedef pair<const long, CBond*>				zPairLongBond;
	typedef pair<const long, CBondPair*>			zPairLongBondPair;
	typedef pair<const long, CPolymer*>				zPairLongPolymer;
	typedef pair<long, mpuGhostBead*>	            zPairLongGhostBead;
	typedef pair<long, mpuExtendedBond*>	        zPairLongExtendedBond;
	typedef pair<long, mpuExtendedNPBond*>	        zPairLongExtendedNPBond;
	typedef pair<long, mpuExtendedPolymer*>	        zPairLongExtendedPolymer;
	typedef pair<long, CCommandTargetNode*>			zPairLongTarget;
	typedef pair<long, CNanoparticle*>	            zPairLongNanoparticle;

	typedef pair<xxBasestring, tguArgumentType*>			    zPairSVAT;
	typedef pair<xxBasestring, CCommandTargetNode*>				zPairST;
	typedef pair<xxBasestring, IModifyProcess*>					zPairSMP;
	typedef pair<xxBasestring, taEventSourceDecorator*>	        zPairSESD;
	typedef pair<xxBasestring, taEventAnalysisDecorator*>	    zPairSEAD;

#if EnableShadowSimBox == SimACNEnabled
	typedef pair<xxBasestring, aeActiveCellNetwork*>		    zPairSACN;
	typedef pair<xxBasestring, IModifyActiveCellNetwork*>		zPairSMACN;
	typedef pair<xxBasestring, IACNAccessControl*>				zPairSAACN;
	typedef aeActiveCellNetwork* (*CreateACNCallback)(IActiveSimBox* const pShadow, long xn, long yn, long zn,
					                                  double xw, double yw, double zw);
	typedef aevActiveEvent*		(*CreateActiveEventCallback)();
#endif

	// Typedef for the factory classes that are used to create the commands,
	// events, processes, etc.
	// These are called: acfCommandFactory, acfAnalysisFactory, 
	// acfEventFactory, acfProcessFactory  

	typedef CAnalysisData*		   (*CreateAnalysisCallback)();
	typedef CInitialStateData* (*CreateInitialStateCallback)();
	typedef mpmMessage*		(*CreateParallelMessageCallback)();
	typedef xxCommand*				(*CreateCommandCallback)(long executionTime);
	typedef xxEvent*				  (*CreateEventCallback)();
	typedef xxProcess*				(*CreateProcessCallback)();


	// The following typedef uses valarrays where available or vectors where not

	typedef vector<long>						zLongValArray;
	typedef vector<double>						zDoubleValArray;

// ****************************************
#elif xxBasePlatform == xxBaseBORLAND6	// Borland 6 compiler in Windows application

	#include <string>
	#include <list>
	#include <vector>
	#include <map>
	#include <iostream>
	#include <fstream>
	#include <sstream>
	#include <iomanip>
	#include <valarray>

	// **********************************************************************
	// Define typedefs to allow each platform to access the STL containers
	// and other standard libraries using std or not as required.
	// Typedefs beginning with z are used throughout the code. But macros
	// that begin with xxBase are only used in this header file to simplify
	// the replacement of the std name.

	typedef std::istream				zInStream;
	typedef std::ostream				zOutStream;
	typedef std::ifstream				zInFileStream;
	typedef std::ofstream				zOutFileStream;
	typedef std::fstream				zFileStream;
	typedef std::istringstream			zInStringStream;
	typedef std::ostringstream			zOutStringStream;
	typedef std::ios					zIos;

	// The following typedef uses valarrays where available or vectors where not

	typedef std::valarray<size_t>				zLongValArray;
	typedef std::valarray<double>				zDoubleValArray;

	// Define macros to wrap the std namespace names for different platforms

	#define zEndl						std::endl
	#define zFlush						std::flush
	#define zLeft						std::left
	#define xxBaselist					std::list
	#define xxBasemap					std::map
	#define xxBasemultimap				std::multimap
	#define xxBasestring			    std::basic_string<char>
    #define xxBasevector                std::vector

	// The following pair functions have to take different forms on the Cray platform

	typedef std::pair<long, long>					zPairLL;
	typedef std::pair<long, double>					zPairLD;
	typedef std::pair<long, xxBasestring>		    zPairLS;
	typedef std::pair<xxBasestring, long>		    zPairSL;
	typedef std::pair<xxBasestring, double>			zPairSD;
	typedef std::pair<xxBasestring, xxBasestring>	zPairSS;
	typedef std::pair<long, CBeadType*>				zPairLongBeadType;
	typedef std::pair<long, CBondType*>				zPairLongBondType;
	typedef std::pair<long, CBondPairType*>		    zPairLongBondPairType;
	typedef std::pair<long, CPolymerType*>		    zPairLongPolymerType;
	typedef std::pair<long, CAbstractBead*>	        zPairLongBead;
	typedef std::pair<long, CBond*>					zPairLongBond;
	typedef std::pair<long, CBondPair*>				zPairLongBondPair;
	typedef std::pair<long, CPolymer*>				zPairLongPolymer;
	typedef std::pair<long, mpuGhostBead*>	        zPairLongGhostBead;
	typedef std::pair<long, mpuExtendedBond*>	    zPairLongExtendedBond;
	typedef std::pair<long, mpuExtendedNPBond*>	    zPairLongExtendedNPBond;
	typedef std::pair<long, mpuExtendedPolymer*>	zPairLongExtendedPolymer;
	typedef std::pair<long, CCommandTargetNode*>	zPairLongTarget;
	typedef pair<long, CNanoparticle*>	            zPairLongNanoparticle;

	typedef std::pair<xxBasestring, tguArgumentType*>			    zPairSVAT;
	typedef std::pair<xxBasestring, CCommandTargetNode*>		    zPairST;
	typedef std::pair<xxBasestring, IModifyProcess*>			    zPairSMP;
	typedef std::pair<xxBasestring, taEventSourceDecorator*>	    zPairSESD;
	typedef std::pair<xxBasestring, taEventAnalysisDecorator*>	    zPairSEAD;

#if EnableShadowSimBox == SimACNEnabled
	typedef std::pair<xxBasestring, aeActiveCellNetwork*>		    zPairSACN;
	typedef std::pair<xxBasestring, IModifyActiveCellNetwork*>		zPairSMACN;
	typedef std::pair<xxBasestring, IACNAccessControl*>				zPairSAACN;
	typedef aeActiveCellNetwork* (*CreateACNCallback)(IActiveSimBox* const pShadow, long xn, long yn, long zn,
					                                  double xw, double yw, double zw);
	typedef aevActiveEvent*		(*CreateActiveEventCallback)();
#endif

	// Typedef for the factory classes that are used to create the commands,
	// events, processes, etc.
	// These are called: acfCommandFactory, etc.

	typedef CAnalysisData*		   (*CreateAnalysisCallback)();
	typedef CInitialStateData* (*CreateInitialStateCallback)();
	typedef mpmMessage*		(*CreateParallelMessageCallback)();
	typedef xxCommand*				(*CreateCommandCallback)(long executionTime);
	typedef xxEvent*				  (*CreateEventCallback)();
	typedef xxProcess*				(*CreateProcessCallback)();

#endif


// **********************************************************************
// Typedefs to be propagated to the derived classes and others by including this 
// header file. Types whose names begin with z are used to name containers of
// the standard types (long, double, etc) while others have descriptive names.

// STL string containers. The Cray platform only has a const_iterator.

typedef xxBasestring									zString;
typedef xxBasestring::const_iterator					czStringIterator;

#if xxBasePlatform != xxBaseCRAYJ90
	typedef xxBasestring::iterator						zStringIterator;
#endif


// STL vector containers

typedef xxBasevector<bool>								zBoolVector;
typedef xxBasevector<bool>::iterator					zBoolVectorIterator;
typedef xxBasevector<bool>::const_iterator				czBoolVectorIterator;

typedef xxBasevector<long>								zLongVector;
typedef xxBasevector<long>::iterator					zLongVectorIterator;
typedef xxBasevector<long>::const_iterator				czLongVectorIterator;

typedef xxBasevector<double>							zDoubleVector;
typedef xxBasevector<double>::iterator					zDoubleVectorIterator;
typedef xxBasevector<double>::const_iterator			czDoubleVectorIterator;

typedef	xxBasevector<xxBasevector<long> >				zArray2dLong;
typedef	xxBasevector<xxBasevector<double> >				zArray2dDouble;
typedef	xxBasevector<xxBasevector<zString> >	        zArray2dString;


typedef xxBasevector<zString>							StringSequence;
typedef xxBasevector<zString>::iterator					StringSequenceIterator;
typedef xxBasevector<zString>::const_iterator			cStringSequenceIterator;

#if EnableShadowSimBox == SimACNEnabled
typedef xxBasevector<aeCNTCell*>						aeCNTCellVector;
typedef xxBasevector<aeCNTCell*>::iterator				aeCNTCellIterator;
typedef xxBasevector<aeCNTCell*>::const_iterator		caeCNTCellIterator;
#endif

typedef xxBasevector<CBeadType*>						BeadTypeVector;
typedef xxBasevector<CBeadType*>::iterator				BeadTypeVectorIterator;
typedef xxBasevector<CBeadType*>::const_iterator		cBeadTypeVectorIterator;

typedef xxBasevector<CAbstractBead*>					AbstractBeadVector;
typedef xxBasevector<CAbstractBead*>::iterator			AbstractBeadVectorIterator;
typedef xxBasevector<CAbstractBead*>::const_iterator	cAbstractBeadVectorIterator;

typedef xxBasevector<CBead*>							        BeadVector;
typedef xxBasevector<CBead*>::iterator					        BeadVectorIterator;
typedef xxBasevector<CBead*>::const_iterator			        cBeadVectorIterator;

typedef xxBasevector<CBeadChargeWrapper*>							ChargedBeadVector;
typedef xxBasevector<CBeadChargeWrapper*>::iterator					ChargedBeadVectorIterator;
typedef xxBasevector<CBeadChargeWrapper*>::const_iterator			cChargedBeadVectorIterator;
typedef xxBasevector<CBeadChargeWrapper*>::reverse_iterator			rChargedBeadVectorIterator;
typedef xxBasevector<CBeadChargeWrapper*>::const_reverse_iterator	crChargedBeadVectorIterator;

typedef xxBasevector<CBond*>							        BondVector;
typedef xxBasevector<CBond*>::iterator					        BondVectorIterator;
typedef xxBasevector<CBond*>::const_iterator			        cBondVectorIterator;

typedef xxBasevector<CBondPair*>						        BondPairVector;
typedef xxBasevector<CBondPair*>::iterator				        BondPairVectorIterator;
typedef xxBasevector<CBondPair*>::const_iterator		        cBondPairVectorIterator;

typedef xxBasevector<CPolymer*>							        PolymerVector;
typedef xxBasevector<CPolymer*>::iterator				        PolymerVectorIterator;
typedef xxBasevector<CPolymer*>::const_iterator		            cPolymerVectorIterator;

typedef xxBasevector<CPolymerCrossLink*>					    PolymerCrossLinkVector;
typedef xxBasevector<CPolymerCrossLink*>::iterator			    PolymerCrossLinkIterator;
typedef xxBasevector<CPolymerCrossLink*>::const_iterator        cPolymerCrossLinkIterator;

typedef xxBasevector<mpuGhostBead*>					            GhostBeadVector;
typedef xxBasevector<mpuGhostBead*>::iterator			        GhostBeadVectorIterator;
typedef xxBasevector<mpuGhostBead*>::const_iterator	            cGhostBeadVectorIterator;

typedef xxBasevector<mpuExtendedBond*>						    ExtendedBondVector;
typedef xxBasevector<mpuExtendedBond*>::iterator			    ExtendedBondVectorIterator;
typedef xxBasevector<mpuExtendedBond*>::const_iterator		    cExtendedBondVectorIterator;

typedef xxBasevector<mpuExtendedBondPair*>					    ExtendedBondPairVector;
typedef xxBasevector<mpuExtendedBondPair*>::iterator		    ExtendedBondPairVectorIterator;
typedef xxBasevector<mpuExtendedBondPair*>::const_iterator	    cExtendedBondPairVectorIterator;

typedef xxBasevector<mpuExtendedNPBond*>					    ExtendedNPBondVector;
typedef xxBasevector<mpuExtendedNPBond*>::iterator			    ExtendedNPBondVectorIterator;
typedef xxBasevector<mpuExtendedNPBond*>::const_iterator	    cExtendedNPBondVectorIterator;

typedef xxBasevector<mpuExtendedNanoparticle*>					ExtendedNanoparticleVector;
typedef xxBasevector<mpuExtendedNanoparticle*>::iterator        ExtendedNanoparticleVectorIterator;
typedef xxBasevector<mpuExtendedNanoparticle*>::const_iterator  cExtendedNanoparticleVectorIterator;

typedef xxBasevector<mpuExtendedPolymer*>					    ExtendedPolymerVector;
typedef xxBasevector<mpuExtendedPolymer*>::iterator			    ExtendedPolymerVectorIterator;
typedef xxBasevector<mpuExtendedPolymer*>::const_iterator	    cExtendedPolymerVectorIterator;

typedef xxBasevector<pmSendGhostBeadCoords*>					GhostBeadCoordsMsgVector;
typedef xxBasevector<pmSendGhostBeadCoords*>::iterator			GhostBeadCoordsMsgVectorIterator;
typedef xxBasevector<pmSendGhostBeadCoords*>::const_iterator	cGhostBeadCoordsMsgVectorIterator;

typedef xxBasevector<CNanoparticle*>							NanoparticleSequence;
typedef xxBasevector<CNanoparticle*>::iterator				    NanoparticleIterator;
typedef xxBasevector<CNanoparticle*>::const_iterator			cNanoparticleIterator;
typedef xxBasevector<CNanoparticle*>::reverse_iterator		    rNanoparticleIterator;
typedef xxBasevector<CNanoparticle*>::const_reverse_iterator	crNanoparticleIterator;

typedef xxBasevector<CCNTCell*>							        CNTCellVector;
typedef xxBasevector<CCNTCell*>::iterator				        CNTCellIterator;
typedef xxBasevector<CCNTCell*>::const_iterator			        cCNTCellIterator;

typedef xxBasevector<CStressGridCell*>						    StressCellVector;
typedef xxBasevector<CStressGridCell*>::iterator			    StressCellIterator;
typedef xxBasevector<CStressGridCell*>::const_iterator			cStressCellIterator;

typedef xxBasevector<CObservable*>						        ObservableSequence;
typedef xxBasevector<CObservable*>::iterator			        ObservableIterator;
typedef xxBasevector<CObservable*>*						        ObservableSequencePointer;

typedef xxBasevector<CGridObservable*>					        GridObservableSequence;
typedef xxBasevector<CGridObservable*>::iterator		        GridObservableIterator;

typedef xxBasevector<CCurrentState*>					        CurrentStateSequence;
typedef xxBasevector<CCurrentState*>::iterator			        CurrentStateIterator;

typedef xxBasevector<CDensityState*>					        DensityStateSequence;
typedef xxBasevector<CDensityState*>::iterator			        DensityStateIterator;
typedef xxBasevector<CDensityState*>::const_iterator	        cDensityStateIterator;
typedef xxBasevector<CDensityState*>::reverse_iterator          rDensityStateIterator;
typedef xxBasevector<CDensityState*>::const_reverse_iterator    crDensityStateIterator;

typedef xxBasevector<CTimeSeriesData*>							TimeSeriesSequence;
typedef xxBasevector<CTimeSeriesData*>::iterator				TimeSeriesIterator;
typedef xxBasevector<CTimeSeriesData*>::const_iterator			cTimeSeriesIterator;

typedef xxBasevector<CAnalysis*>								AggregateSequence;
typedef xxBasevector<CAnalysis*>::iterator						AggregateIterator;
typedef xxBasevector<CAnalysis*>::const_iterator				cAggregateIterator;
typedef xxBasevector<CAnalysis*>::reverse_iterator				rAggregateIterator;
typedef xxBasevector<CAnalysis*>::const_reverse_iterator		crAggregateIterator;

typedef xxBasevector<CAnalysisData*>							AggregateDataSequence;
typedef xxBasevector<CAnalysisData*>::iterator					AggregateDataIterator;
typedef xxBasevector<CAnalysisData*>::const_iterator			cAggregateDataIterator;

typedef xxBasevector<CAnalysisTool*>							ToolSequence;
typedef xxBasevector<CAnalysisTool*>::iterator					ToolIterator;
typedef xxBasevector<CAnalysisTool*>::const_iterator			cToolIterator;

typedef xxBasevector<IRegionAnalysis*>							RegionSequence;
typedef xxBasevector<IRegionAnalysis*>::iterator				RegionIterator;
typedef xxBasevector<IRegionAnalysis*>::const_iterator			cRegionIterator;
typedef xxBasevector<IRegionAnalysis*>::reverse_iterator		rRegionIterator;
typedef xxBasevector<IRegionAnalysis*>::const_reverse_iterator	crRegionIterator;

#if EnableShadowSimBox == SimACNEnabled
typedef xxBasevector<aeActiveBond*>							    ActiveBondSequence;
typedef xxBasevector<aeActiveBond*>::iterator					ActiveBondIterator;
typedef xxBasevector<aeActiveBond*>::const_iterator			    cActiveBondIterator;
typedef xxBasevector<aeActiveBond*>::reverse_iterator			rActiveBondIterator;
typedef xxBasevector<aeActiveBond*>::const_reverse_iterator	    crActiveBondIterator;

typedef xxBasevector<aeActivePolymer*>							ActivePolymerSequence;
typedef xxBasevector<aeActivePolymer*>::iterator			    ActivePolymerIterator;
typedef xxBasevector<aeActivePolymer*>::const_iterator			cActivePolymerIterator;
typedef xxBasevector<aeActivePolymer*>::reverse_iterator	    rActivePolymerIterator;
typedef xxBasevector<aeActivePolymer*>::const_reverse_iterator	crActivePolymerIterator;

typedef xxBasevector<aevActiveEvent*>							ActiveEventSequence;
typedef xxBasevector<aevActiveEvent*>::iterator					ActiveEventIterator;
typedef xxBasevector<aevActiveEvent*>::const_iterator			cActiveEventIterator;
typedef xxBasevector<aevActiveEvent*>::reverse_iterator			rActiveEventIterator;
typedef xxBasevector<aevActiveEvent*>::const_reverse_iterator	crActiveEventIterator;
#endif

typedef xxBasevector<xxProcess*>								ProcessSequence;
typedef xxBasevector<xxProcess*>::iterator						ProcessIterator;
typedef xxBasevector<xxProcess*>::const_iterator				cProcessIterator;
typedef xxBasevector<xxProcess*>::reverse_iterator				rProcessIterator;
typedef xxBasevector<xxProcess*>::const_reverse_iterator		crProcessIterator;

typedef xxBasevector<xxEvent*>									EventSequence;
typedef xxBasevector<xxEvent*>::iterator						EventIterator;
typedef xxBasevector<xxEvent*>::const_iterator					cEventIterator;
typedef xxBasevector<xxEvent*>::reverse_iterator				rEventIterator;
typedef xxBasevector<xxEvent*>::const_reverse_iterator			crEventIterator;

typedef xxBasevector<xxMessage*>								MessageSequence;
typedef xxBasevector<xxMessage*>::iterator						MessageIterator;
typedef xxBasevector<xxMessage*>::const_iterator				cMessageIterator;
typedef xxBasevector<xxMessage*>::reverse_iterator				rMessageIterator;
typedef xxBasevector<xxMessage*>::const_reverse_iterator		crMessageIterator;

typedef xxBasevector<const CAnalysisObject*>					AnalysisObjectSequence;
typedef xxBasevector<const CAnalysisObject*>::iterator			AnalysisObjectIterator;
typedef xxBasevector<const CAnalysisObject*>::const_iterator	cAnalysisObjectIterator;

typedef xxBasevector<const xxProcessObject*>					ProcessObjectSequence;
typedef xxBasevector<const xxProcessObject*>::iterator			ProcessObjectIterator;
typedef xxBasevector<const xxProcessObject*>::const_iterator	cProcessObjectIterator;

typedef xxBasevector<const xxEventObject*>						EventObjectSequence;
typedef xxBasevector<const xxEventObject*>::iterator			EventObjectIterator;
typedef xxBasevector<const xxEventObject*>::const_iterator		cEventObjectIterator;

typedef xxBasevector<const xxCommandObject*>					CommandObjectSequence;
typedef xxBasevector<const xxCommandObject*>::iterator			CommandObjectIterator;
typedef xxBasevector<const xxCommandObject*>::const_iterator	cCommandObjectIterator;

typedef xxBasevector<CSlice*>							SliceVector;
typedef xxBasevector<CSlice*>::iterator					SliceVectorIterator;
typedef xxBasevector<CSlice*>::const_iterator			cSliceVectorIterator;

typedef xxBasevector<CRow*>								RowVector;
typedef xxBasevector<CRow*>::iterator					RowVectorIterator;
typedef xxBasevector<CRow*>::const_iterator				cRowVectorIterator;

typedef xxBasevector<CCell*>							CellVector;
typedef xxBasevector<CCell*>::iterator					CellVectorIterator;
typedef xxBasevector<CCell*>::const_iterator			cCellVectorIterator;

typedef xxBasevector<CRowProfile*>						RowProfileVector;
typedef xxBasevector<CRowProfile*>::iterator			RowProfileVectorIterator;

typedef xxBasevector<CCellProfile*>						CellProfileVector;
typedef xxBasevector<CCellProfile*>::iterator			CellProfileVectorIterator;

typedef xxBasevector<CCellProfileSet*>					CellProfileSetVector;
typedef xxBasevector<CCellProfileSet*>::iterator		CellProfileSetVectorIterator;

typedef xxBasevector<aaScalarProfile*>					ScalarProfileSequence;
typedef xxBasevector<aaScalarProfile*>::iterator		ScalarProfileIterator;
typedef xxBasevector<aaScalarProfile*>::const_iterator	cScalarProfileIterator;

typedef xxBasevector<aaStressTensorPoint*>				StressVector;
typedef xxBasevector<aaStressTensorPoint*>::iterator	StressVectorIterator;
typedef xxBasevector<aaStressTensorPoint*>::const_iterator	cStressVectorIterator;

typedef xxBasevector<CRaft*>							RaftSequence;
typedef xxBasevector<CRaft*>::iterator					RaftIterator;
typedef xxBasevector<CRaft*>::const_iterator			cRaftIterator;

typedef xxBasevector<tguArgumentType*>					       tguArgumentSequence;
typedef xxBasevector<tguArgumentType*>::iterator			   tguArgumentIterator;
typedef xxBasevector<tguArgumentType*>::const_iterator	       ctguArgumentIterator;
typedef xxBasevector<tguArgumentType*>::reverse_iterator       rtguArgumentIterator;
typedef xxBasevector<tguArgumentType*>::const_reverse_iterator crtguArgumentIterator;


// STL list containers

typedef xxBaselist<CAbstractBead*>								BeadList;
typedef xxBaselist<CAbstractBead*>::iterator					BeadListIterator;
typedef xxBaselist<CAbstractBead*>::const_iterator				cBeadListIterator;
typedef xxBaselist<CAbstractBead*>::reverse_iterator			rBeadListIterator;
typedef xxBaselist<CAbstractBead*>::const_reverse_iterator		crBeadListIterator;

typedef xxBaselist<CBond*>										BondList;
typedef xxBaselist<CBond*>::iterator							BondListIterator;
typedef xxBaselist<CBond*>::const_iterator						cBondListIterator;
typedef xxBaselist<CBond*>::reverse_iterator					rBondListIterator;
typedef xxBaselist<CBond*>::const_reverse_iterator				crBondListIterator;

typedef xxBaselist<CBondPair*>									BondPairList;
typedef xxBaselist<CBondPair*>::iterator						BondPairListIterator;
typedef xxBaselist<CBondPair*>::const_iterator					cBondPairListIterator;
typedef xxBaselist<CBondPair*>::reverse_iterator				rBondPairListIterator;
typedef xxBaselist<CBondPair*>::const_reverse_iterator			crBondPairListIterator;

typedef xxBaselist<CPolymer*>									PolymerList;
typedef xxBaselist<CPolymer*>::iterator							PolymerListIterator;
typedef xxBaselist<CPolymer*>::const_iterator					cPolymerListIterator;
typedef xxBaselist<CPolymer*>::reverse_iterator					rPolymerListIterator;
typedef xxBaselist<CPolymer*>::const_reverse_iterator			crPolymerListIterator;

typedef xxBaselist<CNanoparticle*>								NanoparticleList;
typedef xxBaselist<CNanoparticle*>::iterator					NanoparticleListIterator;
typedef xxBaselist<CNanoparticle*>::const_iterator				cNanoparticleListIterator;
typedef xxBaselist<CNanoparticle*>::reverse_iterator			rNanoparticleListIterator;
typedef xxBaselist<CNanoparticle*>::const_reverse_iterator		crNanoparticleListIterator;

typedef xxBaselist<mpuGhostBead*>								GhostBeadList;
typedef xxBaselist<mpuGhostBead*>::iterator					    GhostBeadListIterator;
typedef xxBaselist<mpuGhostBead*>::const_iterator				cGhostBeadListIterator;
typedef xxBaselist<mpuGhostBead*>::reverse_iterator			    rGhostBeadListIterator;
typedef xxBaselist<mpuGhostBead*>::const_reverse_iterator		crGhostBeadListIterator;

typedef xxBaselist<mpuExtendedBond*>							ExtendedBondList;
typedef xxBaselist<mpuExtendedBond*>::iterator					ExtendedBondListIterator;
typedef xxBaselist<mpuExtendedBond*>::const_iterator			cExtendedBondListIterator;
typedef xxBaselist<mpuExtendedBond*>::reverse_iterator			rExtendedBondListIterator;
typedef xxBaselist<mpuExtendedBond*>::const_reverse_iterator	crExtendedBondListIterator;

typedef xxBaselist<mpuExtendedBondPair*>						    ExtendedBondPairList;
typedef xxBaselist<mpuExtendedBondPair*>::iterator				    ExtendedBondPairListIterator;
typedef xxBaselist<mpuExtendedBondPair*>::const_iterator			cExtendedBondPairListIterator;
typedef xxBaselist<mpuExtendedBondPair*>::reverse_iterator			rExtendedBondPairListIterator;
typedef xxBaselist<mpuExtendedBondPair*>::const_reverse_iterator    crExtendedBondPairListIterator;

typedef xxBaselist<mpuExtendedNanoparticle*>							ExtendedNanoparticleList;
typedef xxBaselist<mpuExtendedNanoparticle*>::iterator					ExtendedNanoparticleListIterator;
typedef xxBaselist<mpuExtendedNanoparticle*>::const_iterator		    cExtendedNanoparticleListIterator;
typedef xxBaselist<mpuExtendedNanoparticle*>::reverse_iterator			rExtendedNanoparticleListIterator;
typedef xxBaselist<mpuExtendedNanoparticle*>::const_reverse_iterator	crExtendedNanoparticleListIterator;

typedef xxBaselist<mpuExtendedNPBond*>							    ExtendedNPBondList;
typedef xxBaselist<mpuExtendedNPBond*>::iterator					ExtendedNPBondListIterator;
typedef xxBaselist<mpuExtendedNPBond*>::const_iterator			    cExtendedNPBondListIterator;
typedef xxBaselist<mpuExtendedNPBond*>::reverse_iterator			rExtendedNPBondListIterator;
typedef xxBaselist<mpuExtendedNPBond*>::const_reverse_iterator	    crExtendedNPBondListIterator;

typedef xxBaselist<mpuExtendedPolymer*>							    ExtendedPolymerList;
typedef xxBaselist<mpuExtendedPolymer*>::iterator				    ExtendedPolymerListIterator;
typedef xxBaselist<mpuExtendedPolymer*>::const_iterator			    cExtendedPolymerListIterator;
typedef xxBaselist<mpuExtendedPolymer*>::reverse_iterator		    rExtendedPolymerListIterator;
typedef xxBaselist<mpuExtendedPolymer*>::const_reverse_iterator	    crExtendedPolymerListIterator;

typedef xxBaselist<pmSendGhostBeadCoords*>                          GhostBeadCoordsMsgList;
typedef xxBaselist<pmSendGhostBeadCoords*>::iterator                GhostBeadCoordsMsgListIterator;
typedef xxBaselist<pmSendGhostBeadCoords*>::const_iterator          cGhostBeadCoordsMsgListIterator;
typedef xxBaselist<pmSendGhostBeadCoords*>::reverse_iterator        rGhostBeadCoordsMsgListIterator;
typedef xxBaselist<pmSendGhostBeadCoords*>::const_reverse_iterator	crGhostBeadCoordsMsgListIterator;

typedef xxBaselist<CBeadChargeWrapper*>							ChargedBeadList;
typedef xxBaselist<CBeadChargeWrapper*>::iterator				ChargedBeadListIterator;
typedef xxBaselist<CBeadChargeWrapper*>::const_iterator			cChargedBeadListIterator;
typedef xxBaselist<CBeadChargeWrapper*>::reverse_iterator		rChargedBeadListIterator;
typedef xxBaselist<CBeadChargeWrapper*>::const_reverse_iterator	crChargedBeadListIterator;

typedef xxBaselist<CCommandTargetNode*>							CommandTargetSequence;
typedef xxBaselist<CCommandTargetNode*>::iterator				CommandTargetIterator;
typedef xxBaselist<CCommandTargetNode*>::const_iterator			cCommandTargetIterator;
typedef xxBaselist<CCommandTargetNode*>::reverse_iterator		rCommandTargetIterator;
typedef xxBaselist<CCommandTargetNode*>::const_reverse_iterator	crCommandTargetIterator;

typedef xxBaselist<CDensityFieldState*>							DensityFieldSequence;
typedef xxBaselist<CDensityFieldState*>::iterator				DensityFieldIterator;
typedef xxBaselist<CDensityFieldState*>::const_iterator			cDensityFieldIterator;
typedef xxBaselist<CDensityFieldState*>::reverse_iterator		rDensityFieldIterator;
typedef xxBaselist<CDensityFieldState*>::const_reverse_iterator	crDensityFieldIterator;


typedef xxBaselist<CForceTarget*>								ForceTargetSequence;
typedef xxBaselist<CForceTarget*>::iterator						ForceTargetIterator;
typedef xxBaselist<CForceTarget*>::const_iterator				cForceTargetIterator;
typedef xxBaselist<CForceTarget*>::reverse_iterator		        rForceTargetIterator;
typedef xxBaselist<CForceTarget*>::const_reverse_iterator	    crForceTargetIterator;

typedef xxBaselist<const xxCommand*>							CommandSequence;
typedef xxBaselist<const xxCommand*>::iterator					CommandIterator;
typedef xxBaselist<const xxCommand*>::const_iterator			cCommandIterator;
typedef xxBaselist<const xxCommand*>::reverse_iterator		    rCommandIterator;
typedef xxBaselist<const xxCommand*>::const_reverse_iterator	crCommandIterator;

typedef xxBaselist<tguCommandGroup*>						    CommandGroupSequence;
typedef xxBaselist<tguCommandGroup*>::iterator					CommandGroupIterator;
typedef xxBaselist<tguCommandGroup*>::const_iterator			cCommandGroupIterator;
typedef xxBaselist<tguCommandGroup*>::reverse_iterator		    rCommandGroupIterator;
typedef xxBaselist<tguCommandGroup*>::const_reverse_iterator	crCommandGroupIterator;

#if EnableShadowSimBox == SimACNEnabled
typedef xxBaselist<aeActiveBond*>								ActiveBondList;
typedef xxBaselist<aeActiveBond*>::iterator						ActiveBondListIterator;
typedef xxBaselist<aeActiveBond*>::const_iterator				cActiveBondListIterator;
typedef xxBaselist<aeActiveBond*>::reverse_iterator				rActiveBondListIterator;
typedef xxBaselist<aeActiveBond*>::const_reverse_iterator		crActiveBondListIterator;

typedef xxBaselist<aeActivePolymer*>							ActivePolymerList;
typedef xxBaselist<aeActivePolymer*>::iterator					ActivePolymerListIterator;
typedef xxBaselist<aeActivePolymer*>::const_iterator			cActivePolymerListIterator;
typedef xxBaselist<aeActivePolymer*>::reverse_iterator			rActivePolymerListIterator;
typedef xxBaselist<aeActivePolymer*>::const_reverse_iterator	crActivePolymerListIterator;

typedef xxBaselist<aevActiveEvent*>								ActiveEventList;
typedef xxBaselist<aevActiveEvent*>::iterator					ActiveEventListIterator;
typedef xxBaselist<aevActiveEvent*>::const_iterator				cActiveEventListIterator;
typedef xxBaselist<aevActiveEvent*>::reverse_iterator			rActiveEventListIterator;
typedef xxBaselist<aevActiveEvent*>::const_reverse_iterator		crActiveEventListIterator;
#endif

// STL map containers

typedef xxBasemap<long,long>							LongLongMap;
typedef xxBasemap<long,long>::iterator					LongLongIterator;
typedef xxBasemap<long,long>::const_iterator			cLongLongIterator;

typedef xxBasemap<long,double>							LongDoubleMap;
typedef xxBasemap<long,double>::iterator				LongDoubleIterator;
typedef xxBasemap<long,double>::const_iterator			cLongDoubleIterator;

typedef xxBasemap<long,zString>							LongStringMap;
typedef xxBasemap<long,zString>::iterator				LongStringIterator;
typedef xxBasemap<long,zString>::const_iterator			cLongStringIterator;

typedef xxBasemap<zString,long>							StringLongMap;
typedef xxBasemap<zString,long>::iterator				StringLongIterator;
typedef xxBasemap<zString,long>::const_iterator			cStringLongIterator;

typedef xxBasemap<zString,double>						StringDoubleMap;
typedef xxBasemap<zString,double>::iterator			    StringDoubleIterator;
typedef xxBasemap<zString,double>::const_iterator		cStringDoubleIterator;

typedef xxBasemap<zString,zString>						StringStringMap;
typedef xxBasemap<zString,zString>::iterator			StringStringIterator;
typedef xxBasemap<zString,zString>::const_iterator		cStringStringIterator;

typedef xxBasemap<long,CBeadType*>						LongBeadTypeMap;
typedef xxBasemap<long,CBeadType*>::iterator			LongBeadTypeIterator;
typedef xxBasemap<long,CBeadType*>::const_iterator		cLongBeadTypeIterator;

typedef xxBasemap<long,CBondType*>						LongBondTypeMap;
typedef xxBasemap<long,CBondType*>::iterator			LongBondTypeIterator;
typedef xxBasemap<long,CBondType*>::const_iterator		cLongBondTypeIterator;

typedef xxBasemap<long,CBondPairType*>					LongBondPairTypeMap;
typedef xxBasemap<long,CBondPairType*>::iterator		LongBondPairTypeIterator;
typedef xxBasemap<long,CBondPairType*>::const_iterator	cLongBondPairTypeIterator;

typedef xxBasemap<long,CPolymerType*>					LongPolymerTypeMap;
typedef xxBasemap<long,CPolymerType*>::iterator			LongPolymerTypeIterator;
typedef xxBasemap<long,CPolymerType*>::const_iterator	cLongPolymerTypeIterator;

typedef xxBasemap<long,CAbstractBead*>                  LongBeadMap;
typedef xxBasemap<long,CAbstractBead*>::iterator        LongBeadIterator;
typedef xxBasemap<long,CAbstractBead*>::const_iterator  cLongBeadIterator;

typedef xxBasemap<long,CBond*>					        LongBondMap;
typedef xxBasemap<long,CBond*>::iterator                LongBondIterator;
typedef xxBasemap<long,CBond*>::const_iterator          cLongBondIterator;

typedef xxBasemap<long,CBondPair*>					    LongBondPairMap;
typedef xxBasemap<long,CBondPair*>::iterator            LongBondPairIterator;
typedef xxBasemap<long,CBondPair*>::const_iterator      cLongBondPairIterator;

typedef xxBasemap<long,CPolymer*>					     LongPolymerMap;
typedef xxBasemap<long,CPolymer*>::iterator			     LongPolymerIterator;
typedef xxBasemap<long,CPolymer*>::const_iterator	     cLongPolymerIterator;

typedef xxBasemap<long,CNanoparticle*>					 LongNanoparticleMap;
typedef xxBasemap<long,CNanoparticle*>::iterator	     LongNanoparticleIterator;
typedef xxBasemap<long,CNanoparticle*>::const_iterator	 cLongNanoparticleIterator;


typedef xxBasemap<long,mpuGhostBead*>				                        LongGhostBeadMap;
typedef xxBasemap<long,mpuGhostBead*>::iterator			                    LongGhostBeadIterator;
typedef xxBasemap<long,mpuGhostBead*>::const_iterator	                    cLongGhostBeadIterator;

typedef xxBasemap<long,mpuExtendedBond*>				                    LongExtendedBondMap;
typedef xxBasemap<long,mpuExtendedBond*>::iterator			                LongExtendedBondIterator;
typedef xxBasemap<long,mpuExtendedBond*>::const_iterator	                cLongExtendedBondIterator;

typedef xxBasemap<long,mpuExtendedNPBond*>				                    LongExtendedNPBondMap;
typedef xxBasemap<long,mpuExtendedNPBond*>::iterator			            LongExtendedNPBondIterator;
typedef xxBasemap<long,mpuExtendedNPBond*>::const_iterator	                cLongExtendedNPBondIterator;

typedef xxBasemap<long,mpuExtendedNanoparticle*>				            LongExtendedNanoparticleMap;
typedef xxBasemap<long,mpuExtendedNanoparticle*>::iterator			        LongExtendedNanoparticleIterator;
typedef xxBasemap<long,mpuExtendedNanoparticle*>::const_iterator	        cLongExtendedNanoparticleIterator;

typedef xxBasemap<long,mpuExtendedPolymer*>				                    LongExtendedPolymerMap;
typedef xxBasemap<long,mpuExtendedPolymer*>::iterator			            LongExtendedPolymerIterator;
typedef xxBasemap<long,mpuExtendedPolymer*>::const_iterator	                cLongExtendedPolymerIterator;

typedef xxBasemap<long,CCommandTargetNode*>									LongTargetMap;
typedef xxBasemap<long,CCommandTargetNode*>::iterator						LongTargetIterator;
typedef xxBasemap<long,CCommandTargetNode*>::const_iterator					cLongTargetIterator;

typedef xxBasemap<zString,CCommandTargetNode*>								StringTargetMap;
typedef xxBasemap<zString,CCommandTargetNode*>::iterator					StringTargetIterator;
typedef xxBasemap<zString,CCommandTargetNode*>::const_iterator				cStringTargetIterator;

typedef xxBasemap<zString,tguArgumentType*>		                            StringArgumentTypeMap;
typedef xxBasemap<zString,tguArgumentType*>::iterator			            StringArgumentTypeIterator;
typedef xxBasemap<zString,tguArgumentType*>::const_iterator	                cStringArgumentTypeIterator;



#if EnableShadowSimBox == SimACNEnabled
typedef xxBasemap<zString, CreateACNCallback>						        StringACNMap;
typedef xxBasemap<zString, CreateACNCallback>::iterator				        StringACNIterator;
typedef xxBasemap<zString, CreateACNCallback>::const_iterator		        cStringACNIterator;

typedef xxBasemap<zString, CreateActiveEventCallback>						StringActiveEventMap;
typedef xxBasemap<zString, CreateActiveEventCallback>::iterator				StringActiveEventIterator;
typedef xxBasemap<zString, CreateActiveEventCallback>::const_iterator		cStringActiveEventIterator;
#endif

typedef xxBasemap<zString, CreateAnalysisCallback>							StringAnalysisMap;
typedef xxBasemap<zString, CreateAnalysisCallback>::iterator				StringAnalysisIterator;
typedef xxBasemap<zString, CreateAnalysisCallback>::const_iterator			cStringAnalysisIterator;

typedef xxBasemap<zString, CreateInitialStateCallback>						StringInitialStateMap;
typedef xxBasemap<zString, CreateInitialStateCallback>::iterator			StringInitialStateIterator;
typedef xxBasemap<zString, CreateInitialStateCallback>::const_iterator		cStringInitialStateIterator;

typedef xxBasemap<zString, CreateCommandCallback>							StringCommandMap;
typedef xxBasemap<zString, CreateCommandCallback>::iterator					StringCommandIterator;
typedef xxBasemap<zString, CreateCommandCallback>::const_iterator			cStringCommandIterator;

typedef xxBasemap<zString, CreateEventCallback>								StringEventMap;
typedef xxBasemap<zString, CreateEventCallback>::iterator					StringEventIterator;
typedef xxBasemap<zString, CreateEventCallback>::const_iterator				cStringEventIterator;

typedef xxBasemap<zString, CreateParallelMessageCallback>					StringParallelMessageMap;
typedef xxBasemap<zString, CreateParallelMessageCallback>::iterator			StringParallelMessageIterator;
typedef xxBasemap<zString, CreateParallelMessageCallback>::const_iterator	cStringParallelMessageIterator;

typedef xxBasemap<zString, CreateProcessCallback>							StringProcessMap;
typedef xxBasemap<zString, CreateProcessCallback>::iterator					StringProcessIterator;
typedef xxBasemap<zString, CreateProcessCallback>::const_iterator			cStringProcessIterator;

typedef xxBasemap<zString, IModifyProcess*>									StringModifiableProcessMap;
typedef xxBasemap<zString, IModifyProcess*>::iterator						StringModifiableProcessIterator;
typedef xxBasemap<zString, IModifyProcess*>::const_iterator					cStringModifiableProcessIterator;

#if EnableShadowSimBox == SimACNEnabled
typedef xxBasemap<zString, aeActiveCellNetwork*>							StringActiveACNMap;
typedef xxBasemap<zString, aeActiveCellNetwork*>::iterator					StringActiveACNIterator;
typedef xxBasemap<zString, aeActiveCellNetwork*>::const_iterator			cStringActiveACNIterator;

typedef xxBasemap<zString, IModifyActiveCellNetwork*>						StringModifiableACNMap;
typedef xxBasemap<zString, IModifyActiveCellNetwork*>::iterator				StringModifiableACNIterator;
typedef xxBasemap<zString, IModifyActiveCellNetwork*>::const_iterator		cStringModifiableACNIterator;

typedef xxBasemap<zString, IACNAccessControl*>								StringAccessibleACNMap;
typedef xxBasemap<zString, IACNAccessControl*>::iterator					StringAccessibleACNIterator;
typedef xxBasemap<zString, IACNAccessControl*>::const_iterator			    cStringAccessibleACNIterator;
#endif

// STL multimap containers

typedef xxBasemultimap<zString,long>			                             StringLongMMap;
typedef xxBasemultimap<zString,long>::iterator					             StringLongMMIterator;
typedef xxBasemultimap<zString,long>::const_iterator			             cStringLongMMIterator;
typedef xxBasemultimap<zString,double>			                             StringDoubleMMap;
typedef xxBasemultimap<zString,double>::iterator					         StringDoubleMMIterator;
typedef xxBasemultimap<zString,double>::const_iterator			             cStringDoubleMMIterator;
typedef xxBasemultimap<zString,zString>			                             StringStringMMap;
typedef xxBasemultimap<zString,zString>::iterator					         StringStringMMIterator;
typedef xxBasemultimap<zString,zString>::const_iterator			             cStringStringMMIterator;

typedef xxBasemultimap<long,long>			                                 LongLongMMap;
typedef xxBasemultimap<long,long>::iterator					                 LongLongMMIterator;
typedef xxBasemultimap<long,long>::const_iterator			                 cLongLongMMIterator;
typedef xxBasemultimap<long,double>			                                 LongDoubleMMap;
typedef xxBasemultimap<long,double>::iterator					             LongDoubleMMIterator;
typedef xxBasemultimap<long,double>::const_iterator			                 cLongDoubleMMIterator;
typedef xxBasemultimap<long,zString>			                             LongStringMMap;
typedef xxBasemultimap<long,zString>::iterator					             LongStringMMIterator;
typedef xxBasemultimap<long,zString>::const_iterator			             cLongStringMMIterator;


typedef xxBasemultimap<long,CAbstractBead*>			                         LongBeadMMap;
typedef xxBasemultimap<long,CAbstractBead*>::iterator					     LongBeadMMIterator;
typedef xxBasemultimap<long,CAbstractBead*>::const_iterator			         cLongBeadMMIterator;
typedef xxBasemultimap<long,CBond*>					                         LongBondMMap;
typedef xxBasemultimap<long,CBond*>::iterator					             LongBondMMIterator;
typedef xxBasemultimap<long,CBond*>::const_iterator			                 cLongBondMMIterator;
typedef xxBasemultimap<long,CBondPair*>				                         LongBondPairMMap;
typedef xxBasemultimap<long,CBondPair*>::iterator					         LongBondPairMMIterator;
typedef xxBasemultimap<long,CBondPair*>::const_iterator			             cLongBondPairMMIterator;
typedef xxBasemultimap<long,CPolymer*>				                         LongPolymerMMap;
typedef xxBasemultimap<long,CPolymer*>::iterator					         LongPolymerMMIterator;
typedef xxBasemultimap<long,CPolymer*>::const_iterator			             cLongPolymerMMIterator;
typedef xxBasemultimap<zString,taEventSourceDecorator*>	                     StringEventSourceMMap;
typedef xxBasemultimap<zString,taEventSourceDecorator*>::iterator			 StringEventSourceIterator;
typedef xxBasemultimap<zString,taEventSourceDecorator*>::const_iterator      cStringEventSourceIterator;
typedef xxBasemultimap<zString,taEventAnalysisDecorator*>	                 StringEventAnalysisMMap;
typedef xxBasemultimap<zString,taEventAnalysisDecorator*>::iterator			 StringEventAnalysisIterator;
typedef xxBasemultimap<zString,taEventAnalysisDecorator*>::const_iterator    cStringEventAnalysisIterator;

typedef xxBasemultimap<long,mpuGhostBead*>				                     LongGhostBeadMMap;
typedef xxBasemultimap<long,mpuGhostBead*>::iterator			             LongGhostBeadMMIterator;
typedef xxBasemultimap<long,mpuGhostBead*>::const_iterator	                 cLongGhostBeadMMIterator;

typedef xxBasemultimap<long,mpuExtendedBond*>				                 LongExtendedBondMMap;
typedef xxBasemultimap<long,mpuExtendedBond*>::iterator			             LongExtendedBondMMIterator;
typedef xxBasemultimap<long,mpuExtendedBond*>::const_iterator	             cLongExtendedBondMMIterator;

typedef xxBasemultimap<long,mpuExtendedNPBond*>				                 LongExtendedNPBondMMap;
typedef xxBasemultimap<long,mpuExtendedNPBond*>::iterator			         LongExtendedNPBondMMIterator;
typedef xxBasemultimap<long,mpuExtendedNPBond*>::const_iterator	             cLongExtendedNPBondMMIterator;

typedef xxBasemultimap<long,mpuExtendedPolymer*>				             LongExtendedPolymerMMap;
typedef xxBasemultimap<long,mpuExtendedPolymer*>::iterator			         LongExtendedPolymerMMIterator;
typedef xxBasemultimap<long,mpuExtendedPolymer*>::const_iterator	         cLongExtendedPolymerMMIterator;



// **********************************************************************

class xxBase  
{
public:
	xxBase();				
	xxBase(const xxBase& oldBase);				
	virtual ~xxBase();


public:			// static constants used in initialisation routines

	static const double m_globalPI;
	static const double m_globalTwoPI;
	static const double m_globalFourPI;
	static const double m_globalPIByTwo;

    static const zString GetFilePrefix();

    static const zString GetAAPrefix();
    static const zString GetASPrefix();
    static const zString GetCDFPrefix();
    static const zString GetCHPrefix();
    static const zString GetCQPrefix();
    static const zString GetCSPrefix();
    static const zString GetDPPrefix();
    static const zString GetDSPrefix();
	static const zString GetEADPrefix();
    static const zString GetHSPrefix();
    static const zString GetISPrefix();
    static const zString GetLSPrefix();
    static const zString GetOSPrefix();
    static const zString GetPSPrefix();
    static const zString GetPSDFPrefix();
    static const zString GetRAPrefix();
    static const zString GetRSPrefix();
	static const zString GetTADSPrefix();

public:

	zString ToString(unsigned long x) const;
	zString ToString(long x) const;
	zString ToString(double x) const;

protected:
	virtual bool ErrorTrace(zString errStr) const;
	virtual void TraceEndl() const;
	virtual void Trace(zString msgStr) const;
	virtual void TraceStringNoEndl(zString msgStr) const;
	virtual void TraceIntNoEndl(long var1) const;
	virtual void TraceInt(zString msgStr, long var1) const;
	virtual void TraceInt2(zString msgStr, long var1, long var2) const;
	virtual void TraceInt3(zString msgStr, long var1, long var2, long var3) const;
	virtual void TraceDouble( zString msgStr, double var1) const;
	virtual void TraceDoubleNoEndl(double var1) const;
	virtual void TraceDouble2(zString msgStr, double var1, double var2) const;
	virtual void TraceVector(zString msgStr, double var1, double var2, double var3) const;


};

#endif // !defined(AFX_XXBASE_H__83457702_3C59_11D3_820E_0060088AD300__INCLUDED_)


