// cgcSetArgumentsTo2dRealLatticeVector.h: interface for the cgcSetArgumentsTo2dRealLatticeVector class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGCSETARGUMENTSTO2DREALLATTICEVECTOR_H__C0F559B4_0F08_4D7C_B0F5_836BCF8144BF__INCLUDED_)
#define AFX_CGCSETARGUMENTSTO2DREALLATTICEVECTOR_H__C0F559B4_0F08_4D7C_B0F5_836BCF8144BF__INCLUDED_



#include "xxCommand.h"

class cgcSetArgumentsTo2dRealLatticeVector : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: protected constructors declared below
public:

	virtual ~cgcSetArgumentsTo2dRealLatticeVector();
	
	// ****************************************
	// Global functions, static member functions and variables
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an xxCommand pointer,
	// respond to the SimBox's request to execute and return the name of the command.

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const = 0;

	virtual const zString GetCommandType() const = 0;

    virtual const xxCommand* GetCommand() const = 0;

	virtual bool IsDataValid(const CInputData& riData) const;

    virtual bool Pack(const tguArgumentSequence& vArguments);

	// ****************************************
	// Public access functions
	
	inline const zString GetCommandGroupName()	 const {return m_GroupName;};
    inline long          GetXCommandIndex()      const {return m_XCommandIndex;}
    inline long          GetYCommandIndex()      const {return m_YCommandIndex;}
	inline const zString GetXArgName()	         const {return m_XArgName;};
	inline const zString GetYArgName()	         const {return m_YArgName;};
    inline long          GetXMax()               const {return m_XMax;}
    inline long          GetYMax()               const {return m_YMax;}
    inline double        GetXOrigin()            const {return m_XOrigin;}
    inline double        GetYOrigin()            const {return m_YOrigin;}
    inline double        GetLX()                 const {return m_LX;}
    inline double        GetLY()                 const {return m_LY;}

	// ****************************************
	// Protected local functions
protected:

	// Protected constructor ensures that only subclasses can be instantiated

    cgcSetArgumentsTo2dRealLatticeVector(long executionTime);
	cgcSetArgumentsTo2dRealLatticeVector(const cgcSetArgumentsTo2dRealLatticeVector& oldCommand);

	cgcSetArgumentsTo2dRealLatticeVector(long executionTime, const zString groupName, 
                         long cmdIndexX, long cmdIndexY, const zString argNameX ,const zString argNameY,
                         long xmax, long ymax, double xorigin, double yorigin, double lx, double ly);


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

	zString      m_GroupName;	  // Command group name
    long         m_XCommandIndex; // Index of the command whose argument is the X component
    long         m_YCommandIndex; // Index of the command whose argument is the Y component
    zString      m_XArgName;      // Placeholder name for command's X argument
    zString      m_YArgName;      // Placeholder name for command's Y argument

    long         m_XMax;          // Max number of X lattice points
    long         m_YMax;          // Max number of Y lattice points

    double       m_XOrigin;       // X component of lattice origin
    double       m_YOrigin;       // Y component of lattice origin
    double       m_LX;            // Length of X component of lattice 
    double       m_LY;            // Length of Y component of lattice 



};

#endif // !defined(AFX_CGCSETARGUMENTSTO2DREALLATTICEVECTOR_H__C0F559B4_0F08_4D7C_B0F5_836BCF8144BF__INCLUDED_)
