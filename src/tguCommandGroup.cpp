/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// tguCommandGroup.cpp: implementation of the tguCommandGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "tguCommandGroup.h"
#include "tguConstantType.h"
#include "tguSequenceType.h"
#include "tguLatticeComponentType.h"
#include "tguRectangularLatticeType.h"
#include "tguTriangularLatticeType.h"
#include "tguCommands.h"
#include "ISimState.h"
#include "SimBox.h"
#include "xxCommand.h"
#include "LogcgtCommandInstantiationFailed.h"
#include "LogcgtCommandPackingFailed.h"
#include "LogcgtExecuteCommandGroupFailed.h"
#include "LogcgtUnrecognisedCommand.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

tguCommandGroup::tguCommandGroup(const zString name) : m_Name(name), m_bValid(true), m_bActive(true),
                                                       m_Time(0), m_Total(0), m_Period(0),
                                                       m_pCommandSet(0)
{
    m_Commands.clear();
    m_CommandStatus.clear();

    m_mIntegerArgumentMap.clear();
    m_mRealArgumentMap.clear();
    m_mStringArgumentMap.clear();
    m_mArgumentTypeMap.clear();
    m_mStringSourceArgMap.clear();
    m_mStringVariableArgMap.clear();
    m_mmCommandArgumentMap.clear();
    m_mmArgumentArgumentMap.clear();
}

// Copy constructor to create a clone of the group. All its contained commands and
// member variables are duplicated in the new group, and the copy may be modified
// and destroyed independently of the original.
//
// The name of the new group is the old one with a unique integer appended to it.

tguCommandGroup::tguCommandGroup(const tguCommandGroup& oldGroup) : xxBase(oldGroup), 
                                    m_Name(oldGroup.m_Name+"1"),
                                    m_bValid(oldGroup.m_bValid),
                                    m_bActive(oldGroup.m_bActive),
                                    m_Time(oldGroup.m_Time),
                                    m_Total(oldGroup.m_Total),
                                    m_Period(oldGroup.m_Period),
                                    m_pCommandSet(0)
{
    // We expect that the m_pCommandSet member is empty except for the brief period
    // when the group is being executed. Hence, we set it empty here.

    // Clear all containers before filling them with copies from the old group

    m_Commands.clear();
    m_CommandStatus.clear();

    m_mIntegerArgumentMap.clear();
    m_mRealArgumentMap.clear();
    m_mStringArgumentMap.clear();
    m_mArgumentTypeMap.clear();
    m_mStringSourceArgMap.clear();
    m_mStringVariableArgMap.clear();
    m_mmCommandArgumentMap.clear();
    m_mmArgumentArgumentMap.clear();

}


tguCommandGroup::~tguCommandGroup()
{
  // Delete the command set if it still exists

    if(m_pCommandSet)
    {
        delete m_pCommandSet;
        m_pCommandSet = 0;
    }

    // Delete the tguArgumentType-derived instances if any exist, and then
    // empty the map. Note that we do not erase the entries as this 
    // invalidates the iterator. There is no need to do this as only the
    // stored value needs to be explicitly deleted.

    if(!m_mStringVariableArgMap.empty())
    {
        StringArgumentTypeIterator iterArg = m_mStringVariableArgMap.begin(); 

        while(iterArg != m_mStringVariableArgMap.end())
        {
            delete iterArg->second;
            ++iterArg;
        }
        m_mStringVariableArgMap.clear();
    }
}

// Public function to return the name of the cmdNo-th command in the group.
// This function is one-indexed as that is expected by the user; we have to 
// zero-base the index before accessing the group's command container.
// Note that an empty string is return if the index is out of bounds; this 
// should be checked for in the calling routine.

const zString tguCommandGroup::GetCommandName(long cmdNo) const
{
    return GetCommandNameFromZeroIndex(--cmdNo);
}

// Public function to set the execution status of the group as a whole.
// When the group is active, execution of its commands is controlled by 
// their own execution status flag, when the group is inactive, no execution
// is allowed, Toggling the group's execution status repeatedly simply 
// switches between these states.

void tguCommandGroup::SetActive(bool bActive)
{
    m_bActive = bActive;
}

// ****************************************
// Execution profiles for the group
//
// Function that returns true if the command should be executed at the current time,
// otherwise it returns false. We use a separate function from the Execute() 
// function so that we can distinguish a valid command group whose execution time 
// is in the future from an invalid or expired group.

bool tguCommandGroup::TimeToExecute(long currentTime) const
{
    return (currentTime == m_Time);
}

// Function used by the SimBox to repeatedly execute a command group.
// The function TimeToExecute() must be called before this one to indicate
// if the command group should be executed. If the number of remaining executions 
// is non-zero and the current time matches the next execution time of the group,
// the group executes its payload of commands. If the payload executes
// successfully, the function returns true to allow it to be executed again;
// if there is any error, it returns false so that the SimBox to remove the group from the active 
// groups container when the number of executions is reduced to zero.
//
// This function must not be called from a currently executing command as it
// adds its payload to the front of the ISimState command queue and will result
// in an infinite loop if an executing command invokes it.

bool tguCommandGroup::Execute(ISimState* const pISimState)
{
    if(IsActive())
    {
        // Create a new command set instance to hold the concrete command instances,
        // and add the commands to it

        m_pCommandSet = new tguCommands();

        unsigned long cmdIndex = 0;

        while(IsValid() && cmdIndex < m_Commands.size())
        {
            if(!InstantiateCommand(cmdIndex++))
            {
                // If any command fails to instantiate, issue a log message here 
                // as the SimBox cannot distinguish between a failed execution
                // and no more executions for a group. In both cases the group 
                // will be removed from the active groups in the SimBox

                CLogcgtExecuteCommandGroupFailed* pMsg = new CLogcgtExecuteCommandGroupFailed(m_Time, GetName());
                SetValid(false);
            }
        }   
        
        // If all the commands were instantiated correctly, execute the set and
        // delete it ready for the next execution.

        if(IsValid())
        {
            m_pCommandSet->ExecuteCommandsImmediately(pISimState);
            delete m_pCommandSet;
            m_pCommandSet = 0;

            // Decrement the number of executions, set the next execution time
            // and return true if there are still some executions left:

            --m_Total;
            m_Time += m_Period;

            if(m_Total > 0)
                return true;
            else
                return false;
        }
        else
            return false;
    }
    else
    {
        return true;
    }
}

// This profile creates a single instance of each command and packages them
// into a set that is executed at the current time only. We instantiate each 
// command in the group and, if it succeeds, store it in a newly-created
// tguCommands instance. Once all of the
// commands have been instantiated, we tell the tguCommands instance to pass them to 
// the ISimState. If any command fails to be instantiated, the whole set is ignored.
// Hence, a group will only be executed if all its commands are valid. The command
// group's validity is set outside of this function, so that if the group is invalid,
// this function will do nothing.
//
// Note that if the group is inactive, this function does nothing but still returns
// true.
//
// This function can be used from within a currently-executing command as it adds
// the payload to the ISimState after all other commands with the same current
// execution time.

bool tguCommandGroup::ExecuteOnce(ISimState* const pISimState)
{    
    if(IsActive())
    {
        // First set the time at which the group is to execute: this is now!

        m_Time = pISimState->GetCurrentTime();

        // Now create a new command set instance to hold the concrete command instances,
        // and add the commands to it

        m_pCommandSet = new tguCommands();

        unsigned long cmdIndex = 0;

        while(IsValid() && cmdIndex < m_Commands.size())
        {
            if(!InstantiateCommand(cmdIndex++))
            {
                SetValid(false);
            }
        }   
        
        // If all the commands were instantiated correctly, execute the set and
        // delete it ready for the next execution.

        if(IsValid())
        {
            m_pCommandSet->ExecuteCommands(pISimState);
            delete m_pCommandSet;
            m_pCommandSet = 0;
        }

        return IsValid();
    }
    else
    {
        return true;
    }
}

// This profile sets the group up to be executed a fixed number of times (total)
// with successive executions separated by a fixed number of time steps (period).
// If the parameters are not valid, or the groups has its active state toggled off,
// the function returns false, otherwise it returns true. 
//
// Note that this design assumes that single commands (such as the one that results
// in the call to this function) are executed after command groups; this allows
// the group to add its payload to the simulations' command set and still have them
// executed in the current time step. However, this also requires that the first
// execution of the group's commands be in the future when this command is executed.
// Hence, the command group's sequence execution starts one period after the command
// is executed to make the group active.

bool tguCommandGroup::ExecuteSequence(ISimState* const pISimState, long total, long period)
{
    if(IsActive() && total > 0 && period >= 0)
    {
        m_Total  = total;
        m_Period = period;
        m_Time   = pISimState->GetCurrentTime() + m_Period;
 
        return true;
    }
    else
    {
        return false;
    }
}

// Function to crate a new instance of the index-th active command in the group, 
// pack its arguments with the current values and store it in the group's current
// tguCommands instance. If the command has been rendered inactive, we ignore it but
// still return true so that the rest of the commands in the group can be executed.
// If the instantation and packing are successful, we return true, 
// otherwise we return false.
//
// NOTE The argument "index" must be zero-based here.

bool tguCommandGroup::InstantiateCommand(unsigned long index)
{
    bool bValid = true; // return value that shows if command instantiates ok
    zString argSource = "constant";  // Error identifer for the automatic generation of an argument

    if(IsCommandActive(index))
    {
        const zString  cmdName = GetCommandNameFromZeroIndex(index);

        xxCommand *pCommand = acfCommandFactory::Instance()->Create(cmdName, m_Time);

        // Ensure the command type actually exists, otherwise return false

        if(pCommand)
        {
            // Now assemble the arguments to the command from the various containers
            // for integers, reals and strings, and those arguments whose values 
            // are copied from a different command/argument.

            const long argTotal = GetArgumentTotalForCommand(index);

            zString hashedArgName = "";
            zString argName = "";
            zString argType = "";

            // Local storage for the argument values
            long     integerValue   = 0;
            double   realValue      = 0.0;
            zString  stringValue    = "";

            // and iterators for the maps
            StringLongIterator    iterIntegerArg;
            StringDoubleIterator  iterRealArg;
            StringStringIterator  iterStringArg;


            zString hashedCmdName = GetHashedCommandName(index);

            StringStringMMIterator iterCmdArg = m_mmCommandArgumentMap.find(hashedCmdName);

            StringSequence argNames;
            argNames.clear();

            tguArgumentSequence vArguments;
            vArguments.clear();

            if(iterCmdArg!=m_mmCommandArgumentMap.end())
            {
                while(bValid && iterCmdArg != m_mmCommandArgumentMap.upper_bound(hashedCmdName))
                {
                    argName = iterCmdArg->second;
 
                    hashedArgName = GetHashedArgumentName(index, argName);
                    argType       = GetArgumentTypeFromHashedName(hashedArgName);

                    argNames.push_back(argName);

                    // First see if the argument has its value determined from
                    // a sequence, a lattice or is linked to that of another argument, 
                    // and, if so, update its value in the appropriate typed map 
                    // before retrieving its value.

                    if(IsArgumentFromSequence(hashedArgName))
                    {
                        if(!SetArgumentFromSequence(hashedArgName))
                        {
                            argSource = "sequence";
                            bValid = false;
                        }
                    }
                    else if(IsArgumentFromLatticeVector(hashedArgName))
                    {
                        if(!SetArgumentFromLatticeVector(hashedArgName))
                        {
                            argSource = "lattice vector";
                            bValid = false;
                        }
                    }
                    else if(IsArgumentLinked(hashedArgName))
                    {
                        if(!SetArgumentFromLinkedValue(hashedArgName))
                        {
                            argSource = "linked";
                            bValid = false;
                        }
                    }

                    if(argType == "integer")
                    {
                        iterIntegerArg = m_mIntegerArgumentMap.find(hashedArgName);
                        if(iterIntegerArg != m_mIntegerArgumentMap.end())
                        {
                            integerValue = iterIntegerArg->second;
                            vArguments.push_back(new tguConstantType(integerValue));
                        }
                        else
                        {
                            bValid = false;
                        }
                    }
                    else if(argType == "real")
                    {
                        iterRealArg = m_mRealArgumentMap.find(hashedArgName);
                        if(iterRealArg != m_mRealArgumentMap.end())
                        {
                            realValue = iterRealArg->second;
                            vArguments.push_back(new tguConstantType(realValue));
                        }
                        else
                        {
                            bValid = false;
                        }
                    }
                    else if(argType == "string")
                    {
                        iterStringArg = m_mStringArgumentMap.find(hashedArgName);
                        if(iterStringArg != m_mStringArgumentMap.end())
                        {
                            stringValue = iterStringArg->second;
                            vArguments.push_back(new tguConstantType(stringValue));
                        }
                        else
                        {
                            bValid = false;
                        }
                    }
                    else
                    {
                        bValid = false;
                    }

                    iterCmdArg++;
                }
            }

            // Finally pack the empty command instance with its arguments and add it
            // to the local container instance

            if(bValid && pCommand->Pack(vArguments))
            {
                m_pCommandSet->AddCommand(pCommand);

                // Now delete the tguArgumentType instances used to pass arguments
                // to the command to avoid a memory leak. We don't need to empty
                // the container as it is a local variable.

                for(tguArgumentIterator iterArg=vArguments.begin(); iterArg!=vArguments.end(); iterArg++)
                {
                    delete *iterArg;
                }
                return true;
            }
            else
            {
                // Issue a log message that the command failed to be instantiated,
                // and distinguish between a failure here and a failure to pack
                // the command's arguments.

                if(bValid)
                {
                    CLogcgtCommandPackingFailed* pMsg1 = new CLogcgtCommandPackingFailed(m_Time, GetName(), cmdName, argNames);
                }
                else
                {
                    CLogcgtCommandInstantiationFailed* pMsg2 = new CLogcgtCommandInstantiationFailed(m_Time, GetName(), cmdName, 
                                                               argName, argType, argSource);
                }


                // We also have to delete the created arguments here even though 
                // the command failed to execute.
                for(tguArgumentIterator iterArg=vArguments.begin(); iterArg!=vArguments.end(); iterArg++)
                {
                    delete *iterArg;
                }
                return false;
            }
        }
        else
        {
            // Issue a log message that the command is unrecognised
            CLogcgtUnrecognisedCommand* pMsg3 = new CLogcgtUnrecognisedCommand(m_Time, GetName(), cmdName);
            return false;
        }
    }
    else
    {
        return true;
    }
}

// ****************************************
// Functions that add/remove/modify commands in the group

// Function to add a command to the group. We store the command's name and a 
// boolean flag showing that it is active (commands are active by default when 
// first added). Commands are indexed in this container by the order in which 
// they are added, and they cannot be removed. However, their active status 
// can be toggled on or off any number of times (using the ToggleCommandExecution 
// function).  But note that the same command may appear many times in the group, 
// so this container may contain the same commands in any order. Also note that 
// we make no check that the string passed in really is a valid command name.

// NOTE. The containers are zero-indexed, but all user-accessible functions
// are one-indexed for the commands in the group. We indicate this by using
// "cmdNo" for the one-indexed arguments, and "index" for zero-indexed arguments.

void tguCommandGroup::AddCommand(const zString cmdName, StringSequence argNames)
{
    // Add the command to the commands container and a flag showing the command's
    // active status to the status container. It is crucial that both of these
    // containers have the same size as some functions only check the size
    // of one or the other.

    m_Commands.push_back(cmdName);
    m_CommandStatus.push_back(true);

    // Now we add the command's arguments to the command multimap so that they
    // can be retrieved using the command's hashed name. This map determines
    // the order of the arguments used in calling a command class' Pack() function,
    // hence, it must be the same order that the command class expects.

    const long index = m_Commands.size()-1;
    const zString hashedCmdName = GetHashedCommandName(index);

    for(StringSequenceIterator is=argNames.begin(); is!=argNames.end(); is++)
    {
        const zString argName = *is;

        if(!IsCommandArgumentInMultimap(hashedCmdName, argName))
        {
            AddCommandArgumentToMultimap(hashedCmdName, argName);
        }
    }
}

// Function to toggle the execution status of a given command in the group.
// This function allows commands to be effectively removed from/added to the group
// without actually deleting them as this would screw up the command indexing.
// The return value is the new execution status.

bool tguCommandGroup::ToggleCommandExecution(long cmdNo)
{
    --cmdNo;   // Make the index zero-based before accessing containers!

    if(IsCommandIndexValid(cmdNo))
    {
        m_CommandStatus.at(cmdNo) = !(m_CommandStatus.at(cmdNo));
    }

    return m_CommandStatus.at(cmdNo);
}

// ****************************************
// Functions that modify the arguments of commands in the group according to their type.
// We use the order in which the commands were added to the group (cmdNo) to identify
// which command the arguments belong to. If commands are allowed to be removed
// from a group, the user must manually keep track of the new numbering.
//
// Two maps are required; the first stores all the argument names in a multimap using
// the command index as the key. This allows all arguments to a specified command
// to be retrieved. Second, a type-specific map is used to hold the value of each
// argument using the hashed argument name as the key. The helper function 
// GetHashedArgumentName() defines the hash used. This can be modified later if needed.
// If the command already has an argument with the specified name, its value is 
// replaced with the new value, otherwise the argument is added to the maps with 
// the given value. This allows the same commands to be used for adding new arguments
// and modifying their values.
//
// Arguments may have their value linked to that of another argument in another
// (or the same) command. We remove such a link before setting the argument's
// value to a constant or sequential value.

void tguCommandGroup::SetArgumentToInteger(long cmdNo, const zString argName, long argValue)
{
    --cmdNo;   // Make the index zero-based before accessing containers!

    zString cmdName = GetCommandNameFromZeroIndex(cmdNo);
    zString hashedCmdName = GetHashedCommandName(cmdNo);
    zString hashedArgName = GetHashedArgumentName(cmdNo, argName);

    // If the argument is linked to a source argument, delete the link first

    RemoveLinkFromArgArgMap(hashedArgName);

    // If the argument is being set from a sequence, delete the sequence

    RemoveArgumentFromArgumentTypeMap(hashedArgName);

    // Now insert the type of the argument and its value into the appropriate 
    // typed map using the hashed name as the key, removing it first if it is
    // already in there

    RemoveArgumentFromTypedMap(hashedArgName);
    SetValid(AddArgumentToMap(hashedArgName, argValue));
}

void tguCommandGroup::SetArgumentToReal(long cmdNo, const zString argName, double argValue)
{
    --cmdNo;   // Make the index zero-based before accessing containers!
    zString cmdName = GetCommandNameFromZeroIndex(cmdNo);
    zString hashedCmdName = GetHashedCommandName(cmdNo);
    zString hashedArgName = GetHashedArgumentName(cmdNo, argName);

    // If the argument is linked to a source argument, delete it first

    RemoveLinkFromArgArgMap(hashedArgName);

    // If the argument is being set from a sequence, delete the sequence

    RemoveArgumentFromArgumentTypeMap(hashedArgName);

    // Now insert the type of the argument and its value into the appropriate 
    // typed map using the hashed name as the key unless it already appears, 
    // in which case just replace the value. 

    RemoveArgumentFromTypedMap(hashedArgName);
    SetValid(AddArgumentToMap(hashedArgName, argValue));
}

void tguCommandGroup::SetArgumentToString(long cmdNo, const zString argName, const zString argValue)
{
    --cmdNo;   // Make the index zero-based before accessing containers!
    zString cmdName = GetCommandNameFromZeroIndex(cmdNo);
    zString hashedCmdName = GetHashedCommandName(cmdNo);
    zString hashedArgName = GetHashedArgumentName(cmdNo, argName);

    // If the argument is linked to a source argument, delete it first

    RemoveLinkFromArgArgMap(hashedArgName);

    // If the argument is being set from a sequence, delete the sequence

    RemoveArgumentFromArgumentTypeMap(hashedArgName);

    // Now insert the type of the argument and its value into the appropriate 
    // typed map using the hashed name as the key unless it already appears, 
    // in which case just replace the value. 

    RemoveArgumentFromTypedMap(hashedArgName);
    SetValid(AddArgumentToMap(hashedArgName, argValue));
}

// This function links one argument in a command equal to that of 
// another command/argument pair. If the second argument is changed by command,
// the first argument is automatically updated with the new value. This continues
// until the user issues a command to change the linkage of the argument to a 
// different command/argument pair or a constant or a sequence.
//
// Note that the source argument's type must already be accessible using the 
// GetArgumentTypeFromHashedName() as we don't yet know the type of the destination
// argument.
//
// Two maps are required: the first is a multimap that uses the hashed source argument
// as the key and has a pair for every argument that is to be updated with the
// source argument's value (as many arguments can be linked to a given argument, 
// this is a one-to-many relation); the second is a normal map that takes the 
// hashed destination argument into the hashed source argument (this is one-to-one as 
// an argument can only be linked to one source argument). This is used in the
// InstantiateCommand() function to check if a given argument is to be set from 
// a source argument or one of the type maps.

void tguCommandGroup::SetArgumentToArgument(long cmdNo, const zString argName, long cmdNoSource, const zString argNameSource)
{
    --cmdNo;   // Make the index zero-based before accessing containers!
    --cmdNoSource;   

    // Create the hashed command/arg names

    zString hashedCmdName       = GetHashedCommandName(cmdNo);
    zString hashedArgName       = GetHashedArgumentName(cmdNo, argName);
    zString hashedSourceArgName = GetHashedArgumentName(cmdNoSource, argNameSource);

    // Add the argument's type, which is the same as its source argument, to the
    // type map

    m_mArgumentTypeMap.insert(zPairSS(hashedArgName, GetArgumentTypeFromHashedName(hashedSourceArgName)));

    // Next remove the argument from its typed map if it is already in there; this
    // occurs if the argument has already been given a constant or sequential value.
    // If the argument is not in the map, nothing happens.
    
    RemoveArgumentFromTypedMap(hashedArgName);

    // Now create a link between the hashed source argument name
    // and the hashed destination argument name so the latter is updated whenever
    // the former changes. The helper function looks for, and deletes, any 
    // existing linkage if it is found before creating the new one.

    AddLinkToArgArgMap(hashedArgName, hashedSourceArgName);
}

// Overloaded functions to set an argument in a command to a value taken from a 
// predefined sequence of integers, reals or strings. The argument's initial value 
// is specified, and an increment, and its value on subsequent invocations of the 
// InstantiateCommand() function is taken from the sequence:
//
//  value(1) = initial value
//  value(n) = value(n-1) + increment
//
// No upper limit is specified on the size of the sequence, as the execution 
// of the command group determines how many values are needed. The string
// sequence is slightly different from the other two as the counter is simply
// appended to the root string value. Note that negative or zero values are 
// allowed for the sequence; if the increment is zero, the argument will simply
// have a constant value.
//
// Note that if a subsequent command sets the sequenced argument to a constant 
// value or links it to another argument or creates a new sequence, the existing 
// sequence is deleted.

void tguCommandGroup::SetArgumentToIntegerSequence(long cmdNo, const zString argName, long initial, long delta)
{
    --cmdNo;   // Make the index zero-based before accessing containers!

    // Create the hashed command/arg names

    zString hashedCmdName       = GetHashedCommandName(cmdNo);
    zString hashedArgName       = GetHashedArgumentName(cmdNo, argName);

    m_mArgumentTypeMap.insert(zPairSS(hashedArgName, "integer"));

    // If the argument is linked to a source argument, delete the link first as it
    // now takes its values from the sequence instead.

    RemoveLinkFromArgArgMap(hashedArgName);

    // Create a new tguArgumentSequence instance and pass it the argument's 
    // initial value and increment so that it can turn each value in the sequence.
    // Then insert the pair composed of the hashed argument name and sequence
    // object into the argument type map (the add function takes care of removing
    // any previous entry, and deleting the previous tguArgumentType-derived instance)
    // before inserting the new one).

    tguSequenceType* pSequence = new tguSequenceType(initial, delta);

    AddArgumentToArgumentTypeMap(hashedArgName, pSequence);

    // The only remaining step is to ensure that when the containing command is
    // instantiated, it retrieves a value for this argument from the sequence
    // object and inserts it into the appropriate typed map so that it will be
    // assigned to the argument later. This is done in the SetArgumentFromSequence()
    // function, which also takes care of removing any existing value from the
    // typed map before inserting the new one.
}

void tguCommandGroup::SetArgumentToRealSequence(long cmdNo, const zString argName, double initial, double delta)
{
    --cmdNo;   // Make the index zero-based before accessing containers!

    // Create the hashed command/arg names

    zString hashedCmdName       = GetHashedCommandName(cmdNo);
    zString hashedArgName       = GetHashedArgumentName(cmdNo, argName);

    m_mArgumentTypeMap.insert(zPairSS(hashedArgName, "real"));

    // If the argument is linked to a source argument, delete the link first as it
    // now takes its values from the sequence instead.

    RemoveLinkFromArgArgMap(hashedArgName);

    // Create a new tguArgumentSequence instance and pass it the argument's 
    // initial value and increment so that it can turn each value in the sequence.
    // Then insert the pair composed of the hashed argument name and sequence
    // object into the argument type map (the add function takes care of removing
    // any previous entry, and deleting the previous tguArgumentType-derived instance)
    // before inserting the new one).

    tguSequenceType* pSequence = new tguSequenceType(initial, delta);

    AddArgumentToArgumentTypeMap(hashedArgName, pSequence);

    // The only remaining step is to ensure that when the containing command is
    // instantiated, it retrieves a value for this argument from the sequence
    // object and inserts it into the appropriate typed map so that it will be
    // assigned to the argument later. This is done in the SetArgumentFromSequence()
    // function, which also takes care of removing any existing value from the
    // typed map before inserting the new one.
}

// Function to set an argument's value to a sequence of strings. The user provides
// a root string (alphanumeric beginning with a letter), and the sequence simply
// appends the integers 1,2,3,... to the root. Note that the counter is one-indexed.

void tguCommandGroup::SetArgumentToStringSequence(long cmdNo, const zString argName, const zString root)
{
    --cmdNo;   // Make the index zero-based before accessing containers!

    // Create the hashed command/arg names

    zString hashedCmdName       = GetHashedCommandName(cmdNo);
    zString hashedArgName       = GetHashedArgumentName(cmdNo, argName);

    m_mArgumentTypeMap.insert(zPairSS(hashedArgName, "string"));

    // If the argument is linked to a source argument, delete the link first as it
    // now takes its values from the sequence instead.

    RemoveLinkFromArgArgMap(hashedArgName);

    // Create a new tguArgumentSequence instance and pass it the argument's 
    // initial value and increment so that it can turn each value in the sequence.
    // Then insert the pair composed of the hashed argument name and sequence
    // object into the argument type map (the add function takes care of removing
    // any previous entry, and deleting the previous tguArgumentType-derived instance)
    // before inserting the new one).

    tguSequenceType* pSequence = new tguSequenceType(root);

    AddArgumentToArgumentTypeMap(hashedArgName, pSequence);

    // The only remaining step is to ensure that when the containing command is
    // instantiated, it retrieves a value for this argument from the sequence
    // object and inserts it into the appropriate typed map so that it will be
    // assigned to the argument later. This is done in the SetArgumentFromSequence()
    // function, which also takes care of removing any existing value from the
    // typed map before inserting the new one.
}

// Function to set the values of two arguments (not necessarily from the same command)
// to the components of a vector from a 2d triangular lattice. We specify the
// origin of the lattice, the components of the primitive vectors, the number of
// points in each dimension and the two argument/command pairs to be modified.
// Then an instance of tguRTriangularLatticeType is created and wrapped by two
// instances of tguLatticeComponentType that bind the X and Y components to the
// two arguments. Thereafter, the arguments receive values from each lattice vector
// in sequence (working along X then Y) as required.

void tguCommandGroup::SetArgumentsTo2dTriangularLatticeVector(long xCmdNo, long yCmdNo, const zString xArgName, const zString yArgName,
                                                               long xmax, long ymax, double xorigin, double yorigin, 
                                                               double lx, double ly)
{
    --xCmdNo;   // Make the indices zero-based before accessing containers!
    --yCmdNo;   

    // Create the hashed command/arg names

    zString hashedXCmdName = GetHashedCommandName(xCmdNo);
    zString hashedYCmdName = GetHashedCommandName(yCmdNo);
    zString hashedXArgName = GetHashedArgumentName(xCmdNo, xArgName);
    zString hashedYArgName = GetHashedArgumentName(yCmdNo, yArgName);

    // Add the arguments' types to the argument type map. Note that we 
    // assume their types are the same and real numbers, this must be 
    // verified in the calling routine.

    m_mArgumentTypeMap.insert(zPairSS(hashedXArgName, "real"));
    m_mArgumentTypeMap.insert(zPairSS(hashedYArgName, "real"));

    // If either of the arguments is linked to a source argument, delete 
    // the link first as it now takes its values from the lattice instead.

    RemoveLinkFromArgArgMap(hashedXArgName);
    RemoveLinkFromArgArgMap(hashedYArgName);

    // Create a new tguTriangularLatticeType instance and pass it the data needed
    // to define the lattice: origin, primitive vector lengths, and maximum
    // number of lattice points in each dimension. Note that this class handles
    // both 2d and 3d lattices; we use the 2d constructor by passing in only
    // X and Y values.

    tguTriangularLatticeType* pLattice = new tguTriangularLatticeType(xmax, ymax, xorigin, yorigin, lx, ly);
    
    // Now wrap this object in two instances of tguLatticeComponentType 
    // to bind the X and Y components of a generated vector to each argument.
    // Note that the components are zero-indexed.

    tguLatticeComponentType* pXComp = new tguLatticeComponentType(pLattice, 0);
    tguLatticeComponentType* pYComp = new tguLatticeComponentType(pLattice, 1);

    // Now insert the pair composed of the hashed argument name and lattice
    // object into the argument type map (the add function takes care of removing
    // any previous entry, and deleting the previous tguArgumentType-derived instance)
    // before inserting the new one).

    AddArgumentToArgumentTypeMap(hashedXArgName, pXComp);
    AddArgumentToArgumentTypeMap(hashedYArgName, pYComp);
}


// Function to set the values of two arguments (not necessarily from the same command)
// to the components of a vector from a 2d rectangular lattice. We specify the
// origin of the lattice, the components of the primitive vectors, the number of
// points in each dimension and the two argument/command pairs to be modified.
// Then an instance of tguRectangularLatticeType is created and wrapped by two
// instances of tguLatticeComponentType that bind the X and Y components to the
// two arguments. Thereafter, the arguments receive values from each lattice vector
// in sequence (working along X then Y) as required.

void tguCommandGroup::SetArgumentsTo2dRectangularLatticeVector(long xCmdNo, long yCmdNo, const zString xArgName, const zString yArgName,
                                                               long xmax, long ymax, double xorigin, double yorigin, 
                                                               double lx, double ly)
{
    --xCmdNo;   // Make the indices zero-based before accessing containers!
    --yCmdNo;   

    // Create the hashed command/arg names

    zString hashedXCmdName = GetHashedCommandName(xCmdNo);
    zString hashedYCmdName = GetHashedCommandName(yCmdNo);
    zString hashedXArgName = GetHashedArgumentName(xCmdNo, xArgName);
    zString hashedYArgName = GetHashedArgumentName(yCmdNo, yArgName);

    // Add the arguments' types to the argument type map. Note that we 
    // assume their types are the same and real numbers, this must be 
    // verified in the calling routine.

    m_mArgumentTypeMap.insert(zPairSS(hashedXArgName, "real"));
    m_mArgumentTypeMap.insert(zPairSS(hashedYArgName, "real"));

    // If either of the arguments is linked to a source argument, delete 
    // the link first as it now takes its values from the lattice instead.

    RemoveLinkFromArgArgMap(hashedXArgName);
    RemoveLinkFromArgArgMap(hashedYArgName);

    // Create a new tguRectangularLatticeType instance and pass it the data needed
    // to define the lattice: origin, primitive vector lengths, and maximum
    // number of lattice points in each dimension. Note that this class handles
    // both 2d and 3d lattices; we use the 2d constructor by passing in only
    // X and Y values.

    tguRectangularLatticeType* pLattice = new tguRectangularLatticeType(xmax, ymax, xorigin, yorigin, lx, ly);
    
    // Now wrap this object in two instances of tguLatticeComponentType 
    // to bind the X and Y components of a generated vector to each argument.
    // Note that the components are zero-indexed.

    tguLatticeComponentType* pXComp = new tguLatticeComponentType(pLattice, 0);
    tguLatticeComponentType* pYComp = new tguLatticeComponentType(pLattice, 1);

    // Now insert the pair composed of the hashed argument name and lattice
    // object into the argument type map (the add function takes care of removing
    // any previous entry, and deleting the previous tguArgumentType-derived instance)
    // before inserting the new one).

    AddArgumentToArgumentTypeMap(hashedXArgName, pXComp);
    AddArgumentToArgumentTypeMap(hashedYArgName, pYComp);
}

// Function to set the values of three arguments (not necessarily from the same 
// commands) to the components of a vector from a 3d rectangular lattice. 
// See the above 2d function for details.

void tguCommandGroup::SetArgumentsTo3dRectangularLatticeVector(long xCmdNo, long yCmdNo, long zCmdNo, const zString xArgName, const zString yArgName, const zString zArgName,
                                                  long xmax, long ymax, long zmax, double xorigin, double yorigin, double zorigin, double lx, double ly, double lz)
{
    --xCmdNo;   // Make the indices zero-based before accessing containers!
    --yCmdNo;   
    --zCmdNo;   

    // Create the hashed command/arg names

    zString hashedXCmdName = GetHashedCommandName(xCmdNo);
    zString hashedYCmdName = GetHashedCommandName(yCmdNo);
    zString hashedZCmdName = GetHashedCommandName(zCmdNo);
    zString hashedXArgName = GetHashedArgumentName(xCmdNo, xArgName);
    zString hashedYArgName = GetHashedArgumentName(yCmdNo, yArgName);
    zString hashedZArgName = GetHashedArgumentName(zCmdNo, zArgName);

    // Add the arguments' types to the argument type map. Note that we 
    // assume their types are the same and real numbers, this must be 
    // verified in the calling routine.

    m_mArgumentTypeMap.insert(zPairSS(hashedXArgName, "real"));
    m_mArgumentTypeMap.insert(zPairSS(hashedYArgName, "real"));
    m_mArgumentTypeMap.insert(zPairSS(hashedZArgName, "real"));

    // If any of the arguments is linked to a source argument, delete 
    // the link first as it now takes its values from the lattice instead.

    RemoveLinkFromArgArgMap(hashedXArgName);
    RemoveLinkFromArgArgMap(hashedYArgName);
    RemoveLinkFromArgArgMap(hashedZArgName);

    // Create a new tguRectangularLatticeType instance and pass it the data needed
    // to define the lattice: origin, primitive vector lengths, and maximum
    // number of lattice points in each dimension. Note that this class handles
    // both 2d and 3d lattices; we use the 3d constructor by passing in 
    // X,Y and Z values.

    tguRectangularLatticeType* pLattice = new tguRectangularLatticeType(xmax, ymax, zmax, xorigin, yorigin, zorigin, lx, ly, lz);
    
    // Now wrap this object in three instances of tguLatticeComponentType 
    // to bind the X, Y and Z components of a generated vector to each argument.
    // Note that the components are zero-indexed.

    tguLatticeComponentType* pXComp = new tguLatticeComponentType(pLattice, 0);
    tguLatticeComponentType* pYComp = new tguLatticeComponentType(pLattice, 1);
    tguLatticeComponentType* pZComp = new tguLatticeComponentType(pLattice, 2);

    // Now insert the pair composed of the hashed argument name and lattice
    // object into the argument type map (the add function takes care of removing
    // any previous entry, and deleting the previous tguArgumentType-derived instance)
    // before inserting the new one).

    AddArgumentToArgumentTypeMap(hashedXArgName, pXComp);
    AddArgumentToArgumentTypeMap(hashedYArgName, pYComp);
    AddArgumentToArgumentTypeMap(hashedZArgName, pZComp);
}

// ****************************************
// Private helper functions to access the various containers used to store
// the commands and their arguments.

// Helper function to set the validity status of the commands in the group.
// In order to see if any command fails, we only change the group's
// validity state if it is currently true and the argument is false. Otherwise,
// the state is unchanged.

void tguCommandGroup::SetValid(bool bValid)
{
    if(IsValid())
    {
        m_bValid = bValid;
    }
}

// Function to show whether a command index is valid for the group.
// This function can be used by other classes, and assumes that the cmdNo
// is one-based.

bool tguCommandGroup::IsCommandInGroup(long cmdNo) const
{
    return IsCommandIndexValid(--cmdNo);
}

// Function to check that the containers of commands and their status
// flags have the same size and that the specified index is within their range.
//
// NOTE The argument "index" must be zero-based here.

bool tguCommandGroup::IsCommandIndexValid(unsigned long index) const
{
    return (m_Commands.size() == m_CommandStatus.size() && index < m_Commands.size());
}

// Helper function to indicate if the specified command is active or not.
// If the index is a valid, i.e., points to an existing command, we return the
// command's status, otherwise we just return false.
//
// NOTE The argument "index" must be zero-based here.

bool tguCommandGroup::IsCommandActive(unsigned long index) const
{ 
    if(IsCommandIndexValid(index))
    {
        return m_CommandStatus.at(index);
    }
    else
    {
        return false;
    }
}

// Helper functions to add/remove a command to/from the active set. We just set 
// the flag in the status container to true/false to indicate that the 
// corresponding command should be executed/ignored. Note that these functions
// assume that the status container already contains an entry for the command
// being modified.
//
// NOTE The argument "index" must be zero-based here.

void tguCommandGroup::AddCommandToActiveSet(unsigned long index)
{
    if(IsCommandIndexValid(index))
    {
        m_CommandStatus.at(index) = true;
    }
}

void tguCommandGroup::RemoveCommandFromActiveSet(unsigned long index)
{
    if(IsCommandIndexValid(index))
    {
        m_CommandStatus.at(index) = false;
    }
}

// Helper function to return the name of a command in the group given
// its zero-based index.

const zString tguCommandGroup::GetCommandNameFromZeroIndex(unsigned long index) const
{
    if(IsCommandIndexValid(index))
    {
        return m_Commands.at(index);
    }
    else
    {
        return "";
    }
}

// Helper function to return the hashed name of a command. This function may
// be changed in the future, but it currently concatenates the command
// name and its position in the command group's container:
//
// hashedName = command name + zero-based index

const zString tguCommandGroup::GetHashedCommandName(unsigned long index) const
{
    zString cmdName = GetCommandNameFromZeroIndex(index);

    zString hashedName = cmdName+ToString(index);

    return hashedName;
}

bool tguCommandGroup::GetCommandStatus(unsigned long index) const
{
    if(IsCommandIndexValid(index))
    {
        return m_CommandStatus.at(index);
    }
    else
        return false;
}

// Helper function to return the number of arguments for the given command.
// 
// NOTE The argument "index" must be zero-based here.

long tguCommandGroup::GetArgumentTotalForCommand(unsigned long index) const
{
    zString cmdName       = GetCommandNameFromZeroIndex(index);
    zString hashedCmdName = cmdName+ToString(index);

    // Retrieve all values that have the key "hashedCmdName" from the multimap

    long total = 0;
    cStringStringMMIterator citer = m_mmCommandArgumentMap.find(hashedCmdName);

    if(citer!=m_mmCommandArgumentMap.end())
    {
        while(citer != m_mmCommandArgumentMap.upper_bound(hashedCmdName))
        {
            total++;
            citer++;
        }
    }

    return total;
}

// Helper function to return the hashed name of a command's argument. This function
// may be updated if needed, but currently it uses the following hash:
//
// hashedName = command name + zero-based index + argument placeholder name
//
// e.g., If the second command in the group is:  CountBeadTypeInTarget  var1 var2 
// and var1 is a string and var2 is an integer. Then the hashed names of its 
// arguments are:   
//
// CountBeadTypeInTarget2var1, CountBeadTypeInTarget2var2
//
// Note that this depends on the index of the commands not changing between invocations
// of the hash function. If commands are later allowed to be removed from the group,
// this function may need to be modified.

const zString tguCommandGroup::GetHashedArgumentName(unsigned long index, const zString argName)
{
    zString cmdName = GetCommandNameFromZeroIndex(index);

    zString hashedName = cmdName+ToString(index)+argName;

    return hashedName;
}

// Helper function to return the type of an argument given its hashed named.
// See the "SetArgumentToInteger()" function for the hash function.

const zString tguCommandGroup::GetArgumentTypeFromHashedName(const zString key) const
{
    if(m_mArgumentTypeMap.find(key) != m_mArgumentTypeMap.end())
        return m_mArgumentTypeMap.find(key)->second;
    else
        return "";
}

// Helper function to add an argument's placeholder name to the command argument 
// map.  Any number of arguments may be accessed using the same key as we 
// store them in a multimap. The key is the hashed command name and the value 
// is the argument's plaintext name. We first check that the argument name is 
// not already in the map; if it is, we ignore the insert.

void tguCommandGroup::AddCommandArgumentToMultimap(const zString key, const zString argName)
{
    if(!IsCommandArgumentInMultimap(key, argName))
    {
        m_mmCommandArgumentMap.insert(zPairSS(key, argName));
    }
}

// Helper functions to add a command's argument to the map appropriate to its type,
// and to store the type in a separate map accessed using the same key.
// The key used to access the argument is the hash returned by GetHashedArgumentName(),
// but here it is just treated as a string.

bool tguCommandGroup::AddArgumentToMap(const zString key, long value)
{
    m_mArgumentTypeMap.insert(zPairSS(key, "integer"));
    m_mIntegerArgumentMap.insert(zPairSL(key, value));

    return (m_mIntegerArgumentMap.find(key) != m_mIntegerArgumentMap.end());
}

bool tguCommandGroup::AddArgumentToMap(const zString key, double value)
{
    m_mArgumentTypeMap.insert(zPairSS(key, "real"));
    m_mRealArgumentMap.insert(zPairSD(key, value));

    return (m_mRealArgumentMap.find(key) != m_mRealArgumentMap.end());
}

bool tguCommandGroup::AddArgumentToMap(const zString key, const zString value)
{
    m_mArgumentTypeMap.insert(zPairSS(key, "string"));
    m_mStringArgumentMap.insert(zPairSS(key, value));

    return (m_mStringArgumentMap.find(key) != m_mStringArgumentMap.end());
}

// Helper function to store a link between a source argument and a destination argument
// so that the latter has its value updated whenvever the former value changes.
// We first call another function to remove any existing linkage before creating
// the new one. If either of the hashed names is empty, we leave the map alone.

void tguCommandGroup::AddLinkToArgArgMap(const zString hashedDestArgName, const zString hashedSourceArgName)
{
    if(!hashedDestArgName.empty() && !hashedSourceArgName.empty())
    {
        RemoveLinkFromArgArgMap(hashedDestArgName);

        // Create a link from the destination argument to the source argument in a
        // one-to-one map.

        m_mStringSourceArgMap.insert(zPairSS(hashedDestArgName, hashedSourceArgName));

        // Create a link from the source argument to the destination argument in a
        // one-to-many multimap.

        m_mmArgumentArgumentMap.insert(zPairSS(hashedSourceArgName, hashedDestArgName));

    /*
        StringStringMMIterator iterArg = m_mmArgumentArgumentMap.find(hashedSourceArgName);

        while(iterArg != m_mmArgumentArgumentMap.upper_bound(hashedSourceArgName))
        {
            const zString hashedArgName = iterArg->second;
            zString argType = GetArgumentTypeFromHashedName(hashedArgName);
            std::cout << "found link from destination argument " << hashedArgName << " of type " << argType << " to source argument " << hashedSourceArgName << " in group " << GetName() << zEndl;
            iterArg++;
        }
    */
    }
}

// Helper function to store a link between an argument and an instance derived from
// the tguArgumentType class. This allows the argument to be assigned values that
// are taken from a sequence or vector. We first remove any previous entry with 
// the same key, and then insert the new pair using the argument's hashed name as 
// the key. If the key is empty or the tguArgumentType pointer is not valid, 
// we leave the map alone.

void tguCommandGroup::AddArgumentToArgumentTypeMap(const zString key, tguArgumentType* pArgumentType)
{
    if(!key.empty() && pArgumentType)
    {
        RemoveArgumentFromArgumentTypeMap(key);

        m_mStringVariableArgMap.insert(zPairSVAT(key, pArgumentType));
    }
}

// Helper function to see if an argument whose value is to be linked to that of 
// another argument is already linked to a third argument. If it is, the old entries
// are removed.
//
// Note that we used variables with names like "oldSourceArgName", but these are 
// actually the hashed versions of the argument names.

void tguCommandGroup::RemoveLinkFromArgArgMap(const zString destArgName)
{
    StringStringIterator iterDestArg = m_mStringSourceArgMap.find(destArgName);

    if(iterDestArg != m_mStringSourceArgMap.end())
    {
        // Destination argument is already linked, so remove the linkage before
        // creating the new one.

        const zString oldSourceArgName = iterDestArg->second;

        m_mStringSourceArgMap.erase(iterDestArg);

        // Now we have to iterate over the multimap to find the right entry for the 
        // source argument name and delete it. Note that there may be many destination
        // arguments all being set from the same source argument, so we cannot just
        // delete the first one returned, we have to check the value of the key.

        bool bNotFound = true;
        StringStringMMIterator iterOldSourceArg = m_mmArgumentArgumentMap.find(oldSourceArgName);

        while(bNotFound && iterOldSourceArg != m_mmArgumentArgumentMap.upper_bound(oldSourceArgName))
        {
            const zString oldDestArgName = iterOldSourceArg->second;

            if(oldDestArgName == destArgName)
            {
                bNotFound = false;
                m_mmArgumentArgumentMap.erase(iterOldSourceArg);
            }
            else
            {
                iterOldSourceArg++;
            }
        }
    }
}

// Helper function to remove a command/argument pair from the multimap.
// The key is the command's hashed name and the value in the map
// is the argument's plaintext name.

void tguCommandGroup::RemoveCommandArgumentFromMultimap(const zString key, const zString argName)
{
    bool bNotFound = true;
    StringStringMMIterator iterCmdArg = m_mmCommandArgumentMap.find(key);

    while(bNotFound && iterCmdArg != m_mmCommandArgumentMap.upper_bound(key))
    {
        const zString cmdArgName = iterCmdArg->second;

        if(cmdArgName == argName)
        {
            bNotFound = false;
            m_mmCommandArgumentMap.erase(iterCmdArg);
        }
        else
        {
            iterCmdArg++;
        }
    }
}

// Helper function to remove an argument from the constant value map
// according to its type. We use the hashed argument name as the key;
// if the type is not recognised, the function does nothing.

void tguCommandGroup::RemoveArgumentFromTypedMap(const zString key)
{
    const zString argType = GetArgumentTypeFromHashedName(key);

    if(argType == "integer")
    {
        m_mIntegerArgumentMap.erase(key);
    }
    else if(argType == "real")
    {
        m_mRealArgumentMap.erase(key);
    }
    else if(argType == "string")
    {
        m_mStringArgumentMap.erase(key);
    }
}

// Function to remove an entry from the tguArgumentType map. This holds pairs
// composed of a hashed argument name and a tguArgumentType-derived class instance.
// If the entry is found, we delete the tguArgumentType-derived instance before
// erasing the entry otherwise we get a memory leak.

void tguCommandGroup::RemoveArgumentFromArgumentTypeMap(const zString key)
{
    StringArgumentTypeIterator iterArg = m_mStringVariableArgMap.find(key);

    if(iterArg != m_mStringVariableArgMap.end())
    {
        delete iterArg->second;
        m_mStringVariableArgMap.erase(key);
    }
}

// Helper function to see if a given argument has its value set from that of
// another command's argument/

bool tguCommandGroup::IsArgumentLinked(const zString key) const
{
    return (m_mStringSourceArgMap.find(key) != m_mStringSourceArgMap.end());
}

// Helper function to see if a given argument has its value set from a sequence
// of values. If it does, there will be an entry in the map that contains
// pairs composed of the hashed argument name and tguArgumentType-derived class instances.

bool tguCommandGroup::IsArgumentFromSequence(const zString key) const
{
    cStringArgumentTypeIterator citerArg = m_mStringVariableArgMap.find(key);

    if(citerArg != m_mStringVariableArgMap.end())
    {
        tguSequenceType* const pType = dynamic_cast<tguSequenceType*>(citerArg->second);

        if(pType)
            return true;
        else
            return false;
    }
    else
        return false;
}

// Helper function to see if a given argument has its value set from a 
// lattice of values. We allow both 2d and 3d lattices, but not 1d as this is
// the same as a sequence and has already been allowed for. Note that the
// type of the object stored is tguLatticeComponentType that wraps the actual
// tguLatticeVectorType.

bool tguCommandGroup::IsArgumentFromLatticeVector(const zString key) const
{
    cStringArgumentTypeIterator citerArg = m_mStringVariableArgMap.find(key);

    if(citerArg != m_mStringVariableArgMap.end())
    {
        tguLatticeComponentType* const pType = dynamic_cast<tguLatticeComponentType*>(citerArg->second);

        if(pType)
            return true;
        else
            return false;
    }
    else
        return false;
}

// Helper function to see if a given argument has its value set from a random
// vector of values. We allow 1d, 2d and 3d vectors.

bool tguCommandGroup::IsArgumentFromRandomVector(const zString key) const
{
    return false;
}

// Helper function to set an argument's value equal to that of another argument.
// It only requires the hashed name of the destination argument. It returns false 
// if the link cannot be found,  or the source and destination types are not the same, 
// otherwise it returns true. When it retrieves the source argument's value it inserts
// it into the simple map for the destination argument's type. It does not modify
// the command argument multimap as it assumes the destination argument has already
// been inserted into this map. It only sets the argument's numeric or string value.
//
// Note that the source argument itself may have its value set from a link as well,
// so we call this function recursively.

bool tguCommandGroup::SetArgumentFromLinkedValue(const zString key)
{
    bool bValid = false;  // Flag showing if link worked

    if(IsArgumentLinked(key))
    {
        StringStringIterator iterDestArg = m_mStringSourceArgMap.find(key);
        const zString sourceArgName = iterDestArg->second;

        const zString destType   = GetArgumentTypeFromHashedName(key);
        const zString sourceType = GetArgumentTypeFromHashedName(sourceArgName);
            
        if(destType == sourceType)
        {
            // If the source argument itself is linked, update its value first
            // and check that it succeeds before setting the current argument.

            if(!IsArgumentLinked(sourceArgName) || SetArgumentFromLinkedValue(sourceArgName))
            {
                if(destType == "integer")
                {
                    StringLongIterator iterArg;
                    iterArg = m_mIntegerArgumentMap.find(sourceArgName);
                    if(iterArg != m_mIntegerArgumentMap.end())
                    {
                        const long value = iterArg->second;

                        RemoveArgumentFromTypedMap(key);
                        SetValid(AddArgumentToMap(key, value));
                        bValid = true;
                    }
                }
                else if(destType == "real")
                {
                    StringDoubleIterator iterArg;
                    iterArg = m_mRealArgumentMap.find(sourceArgName);
                    if(iterArg != m_mRealArgumentMap.end())
                    {
                        const double value = iterArg->second;
                        RemoveArgumentFromTypedMap(key);
                        SetValid(AddArgumentToMap(key, value));
                        bValid = true;
                    }
                }
                else if(destType == "string")
                {
                    StringStringIterator  iterArg;
                    iterArg = m_mStringArgumentMap.find(sourceArgName);
                    if(iterArg != m_mStringArgumentMap.end())
                    {
                        const zString value = iterArg->second;
                        RemoveArgumentFromTypedMap(key);
                        SetValid(AddArgumentToMap(key, value));
                        bValid = true;
                    }
                }
            }
        }
    }

    return bValid;
}

// Helper function to set an argument's value from a sequence of values of the
// appropriate type. It retrieves the tguArgumentType-derived instance that
// is associated with the argument (using its hashed name as the key) and 
// inserts the next value in the sequence into the argument's typed map. 
// If the sequence exists, and the value is copied, the function returns true, 
// otherwise it returns false.

bool tguCommandGroup::SetArgumentFromSequence(const zString key)
{
    bool bFound = false;

    cStringArgumentTypeIterator citerArg = m_mStringVariableArgMap.find(key);

    if(citerArg != m_mStringVariableArgMap.end())
    {
        tguSequenceType* const pSequence = dynamic_cast<tguSequenceType*>(citerArg->second);

        if(pSequence)
        {
            bFound = true;
            const zString argType = GetArgumentTypeFromHashedName(key);

            if(argType == "integer")
            {
                long value = 0;
                pSequence->GetNextValue(&value);

                // Remove any previous value for this argument from its typed map,
                // and insert the new value
                RemoveArgumentFromTypedMap(key);
                SetValid(AddArgumentToMap(key, value));
            }
            else if(argType == "real")
            {
                double value = 0.0;
                pSequence->GetNextValue(&value);

                // Remove any previous value for this argument from its typed map,
                // and insert the new value
                RemoveArgumentFromTypedMap(key);
                SetValid(AddArgumentToMap(key, value));
            }
            else if(argType == "string")
            {
                zString value = "";
                pSequence->GetNextValue(&value);

                // Remove any previous value for this argument from its typed map,
                // and insert the new value
                RemoveArgumentFromTypedMap(key);
                SetValid(AddArgumentToMap(key, value));
            }
            else
            {
                // This case should never occur, but if it does we reject the value
                bFound = false;
            }
        }
    }

    return bFound;
}

// Helper function to set the value of an argument from one component
// of a vector taken from a lattice. The type of lattice is hidden
// in the tguLatticeComponentType objects.

bool tguCommandGroup::SetArgumentFromLatticeVector(const zString key)
{
    bool bFound = false;

    cStringArgumentTypeIterator citerArg = m_mStringVariableArgMap.find(key);

    if(citerArg != m_mStringVariableArgMap.end())
    {
        tguLatticeComponentType* const pComponent = dynamic_cast<tguLatticeComponentType*>(citerArg->second);

        if(pComponent)
        {
            bFound = true;
            const zString argType = GetArgumentTypeFromHashedName(key);

            if(argType == "real")
            {
                double value = 0.0;
                pComponent->GetNextValue(&value);

                // Remove any previous value for this argument from its typed map,
                // and insert the new value
                RemoveArgumentFromTypedMap(key);
                SetValid(AddArgumentToMap(key, value));
            }
            else
            {
                // This case should never occur, but if it does we reject the value
                bFound = false;
            }
        }
    }

    return bFound;
}

// Helper function to return true if a command's argument is already in the
// map from command name to argument names. The key is the hashed command name.
// We retrieve all the pairs that use the key and check their values to see if
// any of them match the desired hashed argument name.

bool tguCommandGroup::IsCommandArgumentInMultimap(const zString key, const zString argName) const
{
    bool bFound = false;

    cStringStringMMIterator citerArg = m_mmCommandArgumentMap.find(key);

    if(citerArg != m_mmCommandArgumentMap.end())
    {
        while(!bFound && citerArg != m_mmCommandArgumentMap.upper_bound(key))
        {
            const zString value = citerArg->second;

            if(value == argName)
            {
                bFound = true;
            }

            citerArg++;
        }
    }

    return bFound;
}

