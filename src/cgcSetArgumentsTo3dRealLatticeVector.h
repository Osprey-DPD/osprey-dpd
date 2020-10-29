// cgcSetArgumentsTo3dRealLatticeVector.h: interface for the cgcSetArgumentsTo3dRealLatticeVector class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGCSETARGUMENTSTO3DREALLATTICEVECTOR_H__B1D8B713_6F9E_41DE_ADD7_6ED36D83C1D5__INCLUDED_)
#define AFX_CGCSETARGUMENTSTO3DREALLATTICEVECTOR_H__B1D8B713_6F9E_41DE_ADD7_6ED36D83C1D5__INCLUDED_



#include "xxCommand.h"

class cgcSetArgumentsTo3dRealLatticeVector : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: protected constructors declared below
public:

	virtual ~cgcSetArgumentsTo3dRealLatticeVector();
	
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
    inline long          GetZCommandIndex()      const {return m_ZCommandIndex;}
	inline const zString GetXArgName()	         const {return m_XArgName;};
	inline const zString GetYArgName()	         const {return m_YArgName;};
	inline const zString GetZArgName()	         const {return m_ZArgName;};
    inline long          GetXMax()               const {return m_XMax;}
    inline long          GetYMax()               const {return m_YMax;}
    inline long          GetZMax()               const {return m_ZMax;}
    inline double        GetXOrigin()            const {return m_XOrigin;}
    inline double        GetYOrigin()            const {return m_YOrigin;}
    inline double        GetZOrigin()            const {return m_ZOrigin;}
    inline double        GetLX()                 const {return m_LX;}
    inline double        GetLY()                 const {return m_LY;}
    inline double        GetLZ()                 const {return m_LZ;}

	// ****************************************
	// Protected local functions
protected:

	// Protected constructor ensures that only subclasses can be instantiated

    cgcSetArgumentsTo3dRealLatticeVector(long executionTime);
	cgcSetArgumentsTo3dRealLatticeVector(const cgcSetArgumentsTo3dRealLatticeVector& oldCommand);

	cgcSetArgumentsTo3dRealLatticeVector(long executionTime, const zString groupName, 
                         long cmdIndexX, long cmdIndexY, long cmdIndexZ, 
                         const zString argNameX, const zString argNameY, const zString argNameZ,
                         long xmax, long ymax, long zmax,
                         double xorigin, double yorigin, double zorigin, 
                         double lx, double ly, double lz);


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
    long         m_ZCommandIndex; // Index of the command whose argument is the Z component
    zString      m_XArgName;      // Placeholder name for command's X argument
    zString      m_YArgName;      // Placeholder name for command's Y argument
    zString      m_ZArgName;      // Placeholder name for command's Z argument

    long         m_XMax;          // Max number of X lattice points
    long         m_YMax;          // Max number of Y lattice points
    long         m_ZMax;          // Max number of Z lattice points

    double       m_XOrigin;       // X component of lattice origin
    double       m_YOrigin;       // Y component of lattice origin
    double       m_ZOrigin;       // Z component of lattice origin
    double       m_LX;            // Length of X component of lattice 
    double       m_LY;            // Length of Y component of lattice 
    double       m_LZ;            // Length of Z component of lattice 

};

#endif // !defined(AFX_CGCSETARGUMENTSTO3DREALLATTICEVECTOR_H__B1D8B713_6F9E_41DE_ADD7_6ED36D83C1D5__INCLUDED_)
