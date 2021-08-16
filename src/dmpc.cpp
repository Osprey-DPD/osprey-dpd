/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
/* **********************************************************************
    
   For more information contact the author - jsnano@me.com
   
   **********************************************************************
   Change List
   ***********
   
   08/09/20   Open Source release 1.0.
  
  
   **********************************************************************
   The main() routine defines the entry point for the application.
  
   The code runs on a single processor if the SimMPS flag is disabled, or on
   multiple processors as an MPI job if the flag is enabled. When running on a 
   single processor, the MPI functions are compiled out.
  
   If no arguments are given the program prompts for a single input file id.
   The input file (or CDF = Control Data File) name is of the form: prefix.nnn, 
   where "prefix" is the value of the DefaultFilePrefix constant in SimDefs.h, 
   and nnn is a user-supplied string used to label the output files.
  
   If a sequence of arguments is supplied on the command line they will be used 
   as runIds to identify the control data files for a sequence of simulations. 
   All simulations are independent. The output of a simulation may be used to form 
   the input for a subsequent run. In this case, there must be a distinctly-named 
   control data file for the later run, and the "State" keyword must have the 
   value "restart" and provide values for the runId and time step number for the 
   old configuration to be used as the initial configuration. 
  
   If a runId of an existing simulation is specified the old output files are 
   overwritten.
  
   Error codes
   ***********
  
   0 = Run completed successfully
   1 = Parallel interactive experiment failed
   2 = Serial interactive experiment failed
   3 = Parallel simulation failed: Unable to initialise MPI
   4 = Parallel simulation failed: Unable to finalize MPI
   5 = Parallel batch simulation failed
   
 ********************************************************************** */

#include "StdAfx.h"
#include "SimDefs.h"
#include "Experiment.h"

int main(int argc, char* argv[])
{
    zString runId;
    long errCode = 0;

    // Initialise MPI if running parallel code and abort run if any errors.
    // We define the rank as zero and the number of processors as 0 so that 
    // we can distinguish a parallel run from a serial one.

    int my_rank  = 0;
    
#if SimMPS == SimulationEnabled

    int my_world = 1;

    char stringRunId[1000];  // Maximum size of runId is 1000 chars including terminating space
    if(MPI_Init(&argc, &argv) != MPI_SUCCESS)
    {
        errCode = 3;
        return errCode;
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &my_world);
	
	// Select the error handler mechanism
		
 //   int errorCode;
//	errorCode = MPI_Comm_set_errhandler(MPI_COMM_WORLD, MPI_ERRORS_RETURN);
		
//	std::cout << "Proc " << my_rank << " has set error handling on and has error string length " << MPI_MAX_ERROR_STRING << zEndl;
	
#endif

	if(argc == 1)     // Note that argv[0] = executable file name hence argc >= 1
	{
		// If no runId is provided on the command line prompt the user for it
        // on P0 and send it to the other processors

        if(my_rank == 0)
        {
			#if SimIdentifier == BD
				std::cout << "BD Experiment Code 1.5" << zEndl;
			#elif SimIdentifier == DPD
				std::cout << "DPD Experiment Code 1.5" << zEndl;
			#elif SimIdentifier == MD
				std::cout << "MD Experiment Code 1.5" << zEndl;
			#endif
			std::cout << "Enter a runId (nnn): ";
			std::cin >> runId;

#if SimMPS == SimulationEnabled

            // Send the runId to all other processors
            strcpy(stringRunId, runId.c_str());

            int tag = 0;
            for(int pn = 1; pn < my_world; pn++)
            {
                MPI_Send(stringRunId, strlen(stringRunId)+1, MPI_CHAR, pn, tag, MPI_COMM_WORLD);
            }
        }
        else
        {
            int tag = 0;
            MPI_Status status;
            MPI_Recv(stringRunId, 1000, MPI_CHAR, 0, tag, MPI_COMM_WORLD, &status);
            runId = stringRunId;
        }
		
		if(!runId.empty())
		{
			IExperiment*  pIExpt = CExperiment::Instance("epstd", runId, true);
			if(!pIExpt->Run())
            {
              errCode = 1;
			  // Tell MPI to shut down if the initialisation fails for any reason
			  MPI_Abort(MPI_COMM_WORLD, errCode);
            }
			delete pIExpt;
		}
#else
        }
		
		if(!runId.empty())
		{
			IExperiment*  pIExpt = CExperiment::Instance("epstd", runId, true);
			if(!pIExpt->Run())
            {
              errCode = 2;
            }
			delete pIExpt;
		}
#endif
	}
	else
	{
	    // This case has two or more command line arguments. How we process them depends on whether we are doing parallel or serial run.
		
#if SimMPS == SimulationEnabled
        // Check for a Blue Gene mpirun batch job. There must be two arguments, the second being the runId of the input file.
        // For all other arguments we start a series of serial runs. Note that this means that a run with three or more arguments must be a series 
		// of runs on a single processor.
		
        if(my_rank == 0)
        {
			runId.assign(argv[1]);

            // Send the runId to all other processors
            strcpy(stringRunId, runId.c_str());

            int tag = 0;
            for(int pn = 1; pn < my_world; pn++)
            {
                MPI_Send(stringRunId, strlen(stringRunId)+1, MPI_CHAR, pn, tag, MPI_COMM_WORLD);
            }
        }
        else
        {
            int tag = 0;
            MPI_Status status;
            MPI_Recv(stringRunId, 1000, MPI_CHAR, 0, tag, MPI_COMM_WORLD, &status);
            runId = stringRunId;
        }
			
		IExperiment* pIExpt = CExperiment::Instance("epstd", runId, true);
		if(!pIExpt->Run())
		{
			errCode = 5;
		}
		delete pIExpt;
#else
		// For the serial code, we use a pointer to the simulation object 
		// so that successive simulations are independent objects. 
		// Note that if a failed simulation is followed by a successful one, 
		// the error code is replaced by the successful value.
		// If we reach here in the parallel code, it is an error as we only
		// allow a single run on multiple processors.
		
		for(int i=1; i<argc; i++)
		{
			runId.assign(argv[i]);

			IExperiment* pIExpt = CExperiment::Instance("epstd", runId, true);
			if(!pIExpt->Run())
			{
				errCode = 2;
			}
			delete pIExpt;
		}
#endif
	}

    // Shut down MPI if running parallel code
#if SimMPS == SimulationEnabled
    if(MPI_Finalize() != MPI_SUCCESS)
    {
        errCode = 4;
    }
#endif

	return errCode;
}

