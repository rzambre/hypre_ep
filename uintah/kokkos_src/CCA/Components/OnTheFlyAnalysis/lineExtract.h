/*
 * The MIT License
 *
 * Copyright (c) 1997-2017 The University of Utah
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */


#ifndef Packages_Uintah_CCA_Components_ontheflyAnalysis_lineExtract_h
#define Packages_Uintah_CCA_Components_ontheflyAnalysis_lineExtract_h
#include <CCA/Components/OnTheFlyAnalysis/AnalysisModule.h>
#include <CCA/Ports/Output.h>
#include <Core/Grid/Variables/VarTypes.h>
#include <Core/Grid/Variables/CCVariable.h>
#include <Core/Grid/Variables/SFCXVariable.h>
#include <Core/Grid/Variables/SFCYVariable.h>
#include <Core/Grid/Variables/SFCZVariable.h>
#include <Core/Grid/GridP.h>
#include <Core/Grid/LevelP.h>

#include <map>
#include <vector>

namespace Uintah {
  

/**************************************

CLASS
   lineExtract
   
GENERAL INFORMATION

   lineExtract.h

   Todd Harman
   Department of Mechanical Engineering
   University of Utah

   Center for the Simulation of Accidental Fires and Explosions (C-SAFE)
  

KEYWORDS
   lineExtract

DESCRIPTION
   Long description...
  
WARNING
  
****************************************/
  class lineExtract : public AnalysisModule {
  public:
    lineExtract(ProblemSpecP& prob_spec,
                    SimulationStateP& sharedState,
		      Output* dataArchiver);
    lineExtract();
                    
    virtual ~lineExtract();
   
    virtual void problemSetup(const ProblemSpecP& prob_spec,
                              const ProblemSpecP& restart_prob_spec,
                              GridP& grid,
                              SimulationStateP& sharedState);
                              
    virtual void outputProblemSpec(ProblemSpecP& ps){};
                              
    virtual void scheduleInitialize(SchedulerP& sched,
                                    const LevelP& level);
                                    
    virtual void scheduleRestartInitialize(SchedulerP& sched,
                                           const LevelP& level){};
                                    
    virtual void restartInitialize();
                                    
    virtual void scheduleDoAnalysis(SchedulerP& sched,
                                    const LevelP& level);
   
    virtual void scheduleDoAnalysis_preReloc(SchedulerP& sched,
                                    const LevelP& level) {};
                                      
  private:

    void initialize(const ProcessorGroup*, 
                    const PatchSubset* patches,
                    const MaterialSubset*,
                    DataWarehouse*,
                    DataWarehouse* new_dw);
                    
    void doAnalysis(const ProcessorGroup* pg,
                    const PatchSubset* patches,
                    const MaterialSubset*,
                    DataWarehouse*,
                    DataWarehouse* new_dw);
                    
    void createFile(std::string& filename, FILE*& fp);
    
    void createDirectory(std::string& lineName, std::string& levelIndex);
                    
    
    // general labels
    class lineExtractLabel {
    public:
      VarLabel* lastWriteTimeLabel;
      VarLabel* fileVarsStructLabel;
    };
    
    lineExtractLabel* ps_lb;

    struct line{
      std::string  name;
      Point   startPt;
      Point   endPt;
      double  stepSize;
      int loopDir;    // direction to loop over
    };
    
    
       
    //__________________________________
    // global constants
    double d_writeFreq; 
    double d_startTime;
    double d_stopTime;
    std::vector<VarLabel*> d_varLabels;
    std::vector<int> d_varMatl;
    SimulationStateP d_sharedState;
    std::vector<line*> d_lines;
    Output* d_dataArchiver;
    ProblemSpecP d_prob_spec;
    const Material* d_matl;
    MaterialSet* d_matl_set;
    std::set<std::string> d_isDirCreated;
    
    MaterialSubset* d_zero_matl;
    
  
  };
}

#endif
