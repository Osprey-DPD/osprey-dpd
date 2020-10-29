/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// InclusiveRestartState.cpp: implementation of the CInclusiveRestartState class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "InclusiveRestartState.h"
#include "ISimBox.h"
#include "AnalysisState.h"
#include "CurrentState.h"
#include "InitialState.h"
#include "RestartState.h"
#include "Polymer.h"
#include "Bead.h"
#include "CommandTargetNode.h"

#include "LogRestartStateBuilderError.h"  
#include "LogRestartStateBuilderWarning.h" 

// STL using declarations

using std::ios_base;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Constructor that is used when reading a restart state from a file. 
// Note that it does not have access to any simulation objects except the 
// initial state instance, so all the data that are required to define
// the restarted run must be stored there.

CInclusiveRestartState::CInclusiveRestartState(long currentTime, zString runId, CInitialState& riState) : xxState(xxBase::GetRSPrefix() + runId + ".con." + ToString(currentTime) + ".dat", false, currentTime, runId),
                                               m_pISimBox(0),
                                               m_paState(0),
                                               m_riState(riState),
                                               m_bNonBeadRestartState(false),
                                               m_bLogWarningMessages(false)
{

}

// Constructor that is used when writing a new restart state to a file.
// Instances created using this constructor receive data from all simulation
// objects that want to persist their data, and it writes the relevant data
// to the restart file.

CInclusiveRestartState::CInclusiveRestartState(long currentTime, zString runId, bool IOFlag, bool bLogWarnings, 
                                               const ISimBox* const pISimBox, 
                                               CAnalysisState* paState, 
                                               CInitialState& riState) : xxState(xxBase::GetRSPrefix() + runId + ".con." + ToString(currentTime) + ".dat", IOFlag, currentTime, runId),
                                               m_pISimBox(pISimBox),
                                               m_paState(paState),
                                               m_riState(riState),
                                               m_bNonBeadRestartState(false),
                                               m_bLogWarningMessages(bLogWarnings)
{

}

// Constructor used in cases when only the non-bead data may need to be written 
// to the restart file for debug purposes. We pass in a flag that indicates not to call
// the standard restart state's Serialize function. Apart from this flag, the constructor
// is identical to the previous one.

CInclusiveRestartState::CInclusiveRestartState(long currentTime, zString runId, bool bNonBeadState, bool IOFlag, bool bLogWarnings, 
                                               const ISimBox* const pISimBox, 
                                               CAnalysisState* paState, 
                                               CInitialState& riState) : xxState(xxBase::GetRSPrefix() + runId + ".con." + ToString(currentTime) + ".dat", IOFlag, currentTime, runId),
                                               m_pISimBox(pISimBox),
                                               m_paState(paState),
                                               m_riState(riState),
                                               m_bNonBeadRestartState(bNonBeadState),
                                               m_bLogWarningMessages(bLogWarnings)
{

}

CInclusiveRestartState::~CInclusiveRestartState()
{

}

// Function to read/write the bead coordinate data and other information from a 
// previously-saved CInclusiveRestartState. If an error occurs during I/O, the
// fuction returns false so that the run can be aborted. This is important as the
// user may have modified the original restart file and made it invalid.
//
// This class first calls the original CRestartState's Serialise() function to
// read/write the bead coordinate data, and then serialises the dynamic data
// stored in various classes that implement the ISerialiseInclusiveRestartState
// interface. See the relevant classes for details of their data. Here we list
// only the order in which the data is saved.
//
// InitialState data
// *****************
// This data includes the bead-bead,bond, bondpair and polymer data, as well
// as dynamically-created polymerised bonds and command targets and their decorators.
//
// CurrentState data
// *****************
//  This class' data is stored in static members, so we access them from here.
//  This is important because it may be possible for a restart to be required
//  before a current state object has been created, and we don't actually need
//  an instance to access the class data.
//
//  List of bead id and display id (one per line, only
//  for those beads whose status has been modified by command, and whose
//  data are stored in the CCurrentState static members). Bead visibility status
//  is written out with the target data by which the status was modified.
//
//
// Target data
// ***********
//
//  For all command targets, the following data are output:
//
//  Target name, inner decorator name, outer decorator name, is-composite flag
//
//  For simple targets, the following additional data, that depends on the type
//  of the target, are written out:
//
//  Bead targets: 
//       original bead type 
//       current bead type
//       set of (new type, old type) pairs
//       list of (bead id, visibility) status pairs
//
//  Polymer targets:
//       original polymer type
//       list of polymer id's
//
//  ACNFreeBond targets:
//       original polymer type
//       ACN name
//
//  ACNFreePolymer targets:
//       original polymer type
//       ACN name
//
//  For composite targets, we identify their contained targets by name:
//       list of the names of contained targets (which may themselves be composite)
//
// Note that the initial state object is responsible for reading target data
// as no targets have been created at the time the inclusive restart state is being
// read in. We cannot therefore define a loop over targets here.

bool CInclusiveRestartState::Serialize()
{
    // If this is a debug restart state, we don't create a CRestartState instance,
    // but just write all non-bead coordinate data to the file.

    if(m_bNonBeadRestartState)
    {
        m_riState.Write(m_outStream);

        CommandTargetSequence targets = m_pISimBox->GetSimBox()->GetCommandTargets();

        m_outStream << m_pISimBox->GetSimBox()->GetCommandTargetNodeTotal();

        for(cCommandTargetIterator iterTarget=targets.begin(); iterTarget!=targets.end(); iterTarget++)
        {
            (*iterTarget)->Write(m_outStream);
        }

        if(!m_riState.IsRestartStateValid())
        {
            CLogRestartStateBuilderError* pMsg = new CLogRestartStateBuilderError(0, "Unable to write non-bead data inclusive restart state");
            return false;
        }
    }
    else 
    {
        // Create an instance of the original restart state, and tell it to serialise
        // its data. If that succeeds, then serialise the dynamic data. Note that the
        // original CRestartState class must allow bead types to be modifiable so that
        // command targets can be properly implemented.

	    CRestartState rState(GetCurrentTime(), GetRunId(), IsFileWritable(), m_bLogWarningMessages, m_riState);
    
        if(rState.Serialize())
        {
	        if(IsFileWritable())	
	        {
	            // **********************************************************************
                // Write dynamic data to a restart file: we have to move the stream
                // position to the end of the file to avoid overwriting data already 
                // there. We then write a keyword to allow the inclusive restart data
                // to be read in again taking the tellg()/seekg() bug into account.

    //            m_outStream.seekp(0, ios_base::end);

                m_outStream.seekp(rState.GetCurrentWritePos());
                m_outStream << "inclusive" << zEndl;
                m_riState.Write(m_outStream);

                // Now the display id data for those particular beads that have been
                // modified. We first write out the size of the map so that the reading
                // routine can check it gets the correct number of entries.
                // Note that bead visibility data are written out under the 
                // targets that were used to change their visibility status.

    //            m_outStream << CCurrentState::GetBeadDisplayIdMapSize() << zEndl;
    //            for(cLongLongIterator citerLong=CCurrentState::GetMap().begin(); citerLong!=CCurrentState::GetMap().end(); citerLong++)
    //           {
    //                m_outStream << citerLong->first << " " << citerLong->second << zEndl;
    //            }


                // Now we iterate over all command targets and write out their data; we
                // precede the list of targets with their number so that the reading
                // routine can tell if there are any targets to import. The targets'
                // Write() function includes an initial blank line, so we don't add 
                // one here. But we have to add a final endline to ensure the reading 
                // routine does not get an eof error on input if no targets are defined.

                CommandTargetSequence targets = m_pISimBox->GetSimBox()->GetCommandTargets();
 
                m_outStream << m_pISimBox->GetSimBox()->GetCommandTargetNodeTotal();

                for(cCommandTargetIterator iterTarget=targets.begin(); iterTarget!=targets.end(); iterTarget++)
                {
                    (*iterTarget)->Write(m_outStream);
                }

                m_outStream << zEndl;

                if(!m_riState.IsRestartStateValid())
                {
                    CLogRestartStateBuilderError* pMsg = new CLogRestartStateBuilderError(0, "Unable to write inclusive restart state");
                    return false;
                }
	        }
	        else
	        {
	            // **********************************************************************
		        // Read dynamic data from a restart file and overwrite the default values
                // in all classes that implement the ISerialiseInclusiveRestartState interface.
                // The CInitialState object is responsible for reading ALL data and 
                // propagating it to the other objects via the CSimState.
                // Because the original restart state object has already read in the 
                // bead coordinates, we move the stream position after that data.
                //
                // Then we try and read another field to see if this is an inclusive 
                // restart state; if this succeeds, we reset the stream position to 
                // allow the field to be read in by the initial state; if it fails, 
                // this must be a coordinates-only restart, and we issue a warning 
                // message and skip the inclusive restart state data. Note that if 
                // any beads in a coordinates-only restart state have had their type 
                // modified the restarted run will not run correctly, because
                // the number of beads of each type will be set from the CDF and will not 
                // match the number of each type actually read in from the restart file.

                m_inStream.seekg(rState.GetCurrentReadPos());

                zString nextValue;
                m_inStream >> nextValue;

                if(!m_inStream.eof())
                {
                    // Inclusive restart state file - read remaining data.
                    // Because of the bug in tellg()/seekg() we have to manually
                    // move the read position to the data following the "inclusive"
                    // keyword. We iterate over each line in the file looking for the
                    // keyword and then start reading the remaining data there.
                    // If the restart state was produced under the same OS as the current
                    // job we don't have to do this.

                    if(nextValue != "inclusive")
                    {
                        // Read position is corrupt, so manually search for the keyword
                        zString line;
                        m_inStream.seekg(0, ios_base::beg);

                        while(!m_inStream.eof() && line != "inclusive")
                        {
                            m_inStream >> line;
                        }

                        // Check for the keyword not being found
                        if(!m_inStream.good() || m_inStream.eof())
                        {
                            CLogRestartStateBuilderError* pMsg = new CLogRestartStateBuilderError(0, "Unable to read inclusive restart state (has user modified unchangeable data?)");
                            return false;
                        }
                    }

                    // Finally read in the inclusive restart data
                    m_riState.Read(m_inStream);

                    if(!m_riState.IsRestartStateValid())
                    {
                        CLogRestartStateBuilderError* pMsg = new CLogRestartStateBuilderError(0, "Unable to read inclusive restart state (has user modified unchangeable data?)");
                        return false;
                    }
                }
                else
                {
                    // Coordinates-only restart state file - check bead types and issue
                    // an error if the restart file contains any beads with types larger
                    // than expected from the original CDF; otherwise, issue a warning
                    // that the restart file is coordinates only, but proceed with the run.

                    if(rState.IsUnexpectedBeadTypeFound())
                    {
                        CLogRestartStateBuilderError* pMsg = new CLogRestartStateBuilderError(0, "Unable to read coordinates-only restart state (unexpected bead type found)");
                        return false;
                    }
                    else
                    {
                        CLogRestartStateBuilderWarning* pMsg = new CLogRestartStateBuilderWarning(0, "Expected an inclusive restart state, but found bead coordinates only: simulation proceeding without targets or polymerised molecules");
                    }
                }
	        }
        }
        else
        {
            // Error in the CRestartState data, bead coordinates may have been corrupted
            if(IsFileWritable())
            {
                CLogRestartStateBuilderError* pMsg = new CLogRestartStateBuilderError(GetCurrentTime(), "Unable to write bead coordinates");
            }
            else
            {
                CLogRestartStateBuilderError* pMsg = new CLogRestartStateBuilderError(0, "Unable to read bead coordinates (is restart file corrupt?");
            }

            return false;
        }
    }

	return true;
}

