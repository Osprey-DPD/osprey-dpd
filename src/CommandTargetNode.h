// CommandTargetNode.h: interface for the CCommandTargetNode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMANDTARGETNODE_H__E2D177BA_19B6_4369_9AD8_0CF50E53B54A__INCLUDED_)
#define AFX_COMMANDTARGETNODE_H__E2D177BA_19B6_4369_9AD8_0CF50E53B54A__INCLUDED_


// Forward declarations

class xxDecoratorState;

// Include file for the CCommandTargetNode Factory object so that derived
// classes automatically have access to it; the initial state object so 
// that derived classes can replace their proxy contents with actual pointers;
// and the log error message in case importing a target from a restart state fails.


#include "acfTargetFactory.h"
#include "InitialState.h"
#include "LogRestartStateBuilderError.h"


#include "aaVector.h"
#include "xxBase.h"
#include "ISerialiseInclusiveRestartState.h"

class CCommandTargetNode : public xxBase, public ISerialiseInclusiveRestartState
{
public:
	// ****************************************
	// Construction/Destruction - protected constructor declared below
public:

	virtual ~CCommandTargetNode();


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os,  const CCommandTargetNode& target);
	friend zInStream&  operator>>(zInStream& is,         CCommandTargetNode& target);

	static long GetCommandTargetTotal();
	static void ZeroCommandTargetTotal();

private:

	static long m_CommandTargetTotal;	// Number of command targets created

	// ****************************************
	// PVFs that must be implemented by all instantiated derived classes 
public:

	// Do-nothing function that stops the chain of execution of decorator objects.

	virtual void Execute(long simTime);

    // Non-static function that must be implemented by all derived classes
    // to return the target's type. A static member variable called m_Type
    // and a static member function GetType() must also be provided.

    virtual const zString GetTargetType() const = 0;

    // Implementation of the ISerialiseInclusiveRestartState interface
    // to allow this class to read/write data that can be modified
    // for restarts. Because this class cannot be instantiated, we make
    // these functions PVFs to ensure that all derived classes implement them.

    virtual zInStream& Read(zInStream& is);
    virtual zOutStream& Write(zOutStream& is) const;

	// Functions needed to distinguish node types

	virtual bool IsComposite() const;
	virtual bool IsSimpleBeadTargetInComposite() const;

    // VF used by derived classes that contain beads or other entities to 
    // replace the proxy ids by actual pointers. We provide a do-nothing
    // implementation in this class so that target decorators can ignore
    // the function. We pass in the initial state instance so that the
    // target can get access to bead pointers, and other entities.

    virtual bool ReplaceProxyIds(CInitialState* const pIS);

	// ****************************************
	// Public access functions
public:

	// Functions to access the inner and outer decorator objects. The first two
    // store the string identifiers of the decorators that act as proxies for the
    // decorators when importing targets from a restart state; the second two
    // functions return pointers to the actual decorator instances.

    inline const zString   GetInnerDecoratorName() const {return m_InnerDecoratorName;}
    inline const zString   GetOuterDecoratorName() const {return m_OuterDecoratorName;}

	inline CCommandTargetNode* GetInnerDecorator() const {return m_pInnerDecorator;}
	inline CCommandTargetNode* GetOuterDecorator() const {return m_pOuterDecorator;}

	// Function to count the number of decorator instances wrapping a command target

	long GetDecoratorTotal();

	// Functions to return the wrapped command target, the outermost decorator 
	// instance wrapping the target, and a named decorator instance (if it wraps 
	// the current target)

	CCommandTargetNode* GetActiveCommandTarget();
	CCommandTargetNode* GetOutermostDecorator();
	CCommandTargetNode* GetDecorator(const zString decLabel);


	// ****************************************
	// Functions common to simple and composite targets

    inline long          GetId()                const {return m_id;}
	inline const zString GetLabel()				const {return m_Label;}

    virtual aaVector GetCM() const;

	virtual bool IsBeadTypeInTarget(long type)  const;
	virtual long GetBeadTotalForType(long type)	const;
	virtual long GetBeadTotal()					const;
	virtual long GetFirstBeadType()             const;
	virtual BeadVector GetBeads()				const;

    virtual bool ChangeNamedBeadType(long newType);

	// ****************************************
	// Functions implemented by CCommandTarget

	virtual long GetCurrentBeadType()				const;
	virtual long GetPreviousBeadType(long newType)	const;
	virtual long GetOriginalBeadType()				const;
	virtual bool AddBeadTypePair(long newType, long oldType);
	virtual bool RemoveBeadTypePair(long newType);
	virtual bool ClearBeadTypePairs();

	// ****************************************
	// Functions implemented by CCommandTargetPolymer

	virtual bool IsPolymerTypeInTarget(long type)	const;
	virtual long GetPolymerTotalForType(long type)	const;
	virtual long GetPolymerTotal()					const;
	virtual long GetOriginalPolymerType()			const;
	virtual PolymerVector GetPolymers()				const;
    virtual void AddPolymer(CPolymer* pPolymer);
    virtual void RemovePolymer(CPolymer* pPolymer);
    
	// ****************************************
	// Functions implemented by CCompositeTarget

	virtual long GetTargetTotal() const;
    virtual CommandTargetSequence GetTargets() const;
	virtual bool AddTarget(CCommandTargetNode* const pTarget);
	virtual bool RemoveTarget(const zString label);
	virtual CCommandTargetNode* GetTarget(const zString label)	const;
	virtual CCommandTargetNode* GetTargetRecursively(const zString label) const;

	// ****************************************
	// Functions implemented by the taXXX decorator classes 
	// to apply commands to the targets

	// Functions used by decorator classes to tell the inner object that it
	// has been wrapped by a new object. This allows commands to be chained
	// outwards through all decorator classes. The first two functions
    // store the string identifiers for the decorators as proxies for use
    // when reading targets from a restart state file; the second two
    // set the actual decorator pointers.

	void SetInnerDecoratorName(const zString name);
    void SetOuterDecoratorName(const zString name);

	void SetInnerDecorator(CCommandTargetNode* const pDec);
	void SetOuterDecorator(CCommandTargetNode* const pDec);

	// ****************************************
	// Protected local functions
protected:

	// Protected constructor allows only derived classes to be instantiated.
	// This constructor is used to instantiate the actual command targets.

	CCommandTargetNode(const zString label);

	// Protected constructor that is used to instantiate a decorator class
	// and store a pointer to the inner object.

	CCommandTargetNode(const zString label, CCommandTargetNode* const pDec);

	// ****************************************
	// Implementation

    // Function to allow derived classes to store a pointer to an xxDecoratorState
    // object that serialises their data to file.

    void SetState(xxDecoratorState *pState);

    // Function to serialise data to file

    bool SaveState() const;

	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members

protected:

    // Set of numeric ids used by concrete target classes as proxies for the 
    // actual entities held in the target. It is ignored by all decorator classes.

    zLongVector m_ProxyIds;  

    // Decorator state object to allow target decorators to serialise their
    // data to file. Decorators that do not need to save their data ignore
    // this object.

    xxDecoratorState* m_pState;    

private:

    const long      m_id;           // Target's unique numeric id
	const zString	m_Label;		// Target's unique identifying label

    zString m_InnerDecoratorName;   // Proxy name for the inner decorator
    zString m_OuterDecoratorName;   // Proxy name for the outer decorator

	// Pointers to the previous and next decorators around a command target.
	// For the wrapped command target itself, the inner pointer is always null,
	// and the outer pointer points to the first decorator applied.

	CCommandTargetNode* m_pInnerDecorator;
	CCommandTargetNode* m_pOuterDecorator;

};

#endif // !defined(AFX_COMMANDTARGETNODE_H__E2D177BA_19B6_4369_9AD8_0CF50E53B54A__INCLUDED_)
