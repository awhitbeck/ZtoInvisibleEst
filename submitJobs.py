ZJetsToNuNu400 = { "tag" : "ZJetsToNuNu400HT600" ,
                "fileName" : "/eos/uscms/store/user/awhitbe1/PHYS14productionV1/PHYS14.ZJetsToNuNu_HT-400to600_Tune4C_13TeV-madgraph-tauola_*_RA2AnalysisTree.root" ,
                "color" : 1 ,
                "weight" : 0.0144160859953796 
                }

ZJetsToNuNu600 = { "tag" : "ZJetsToNuNu600HTinf" ,
                "fileName" : "/eos/uscms/store/user/awhitbe1/PHYS14productionV1/PHYS14.ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola_*_RA2AnalysisTree.root" ,
                "color" : 1 ,
                "weight" : 0.00489467814129328
                }
       
GJets400 = { "tag" : "GJets400HT600" ,  
          "fileName" : "/eos/uscms/store/user/awhitbe1/PHYS14productionV1/PHYS14.GJets_HT-400to600_Tune4C_13TeV-madgraph-tauola_*_RA2AnalysisTree.root",
          "color" : 2 ,
          "weight" : 0.075604015773398
          }

GJets600 = { "tag" : "GJets600HTinf" ,  
          "fileName" : "/eos/uscms/store/user/awhitbe1/PHYS14productionV1/PHYS14.GJets_HT-600toInf_Tune4C_13TeV-madgraph-tauola_*_RA2AnalysisTree.root",
          "color" : 2 ,
          "weight" : 0.0261385093009007
          }

DYJetsToLL400 = { "tag" : "DYJetsToLL400HT600" ,
               "fileName" : "/eos/uscms/store/user/awhitbe1/PHYS14productionV1/PHYS14.DYJetsToLL_M-50_HT-400to600_Tune4C_13TeV-madgraph-tauola_*_RA2AnalysisTree.root",
               "color" : 3 ,
               "weight" : 0.00697667875133437
               }

DYJetsToLL600 = { "tag" : "DYJetsToLL600HTinf" ,
               "fileName" : "/eos/uscms/store/user/awhitbe1/PHYS14productionV1/PHYS14.DYJetsToLL_M-50_HT-600toInf_Tune4C_13TeV-madgraph-tauola_*_RA2AnalysisTree.root",
               "color" : 3 ,
               "weight" : 0.00245163560886522
               }

QCD500 = { "tag" : "QCD500HT1000" ,
        "fileName" : "/eos/uscms/store/user/awhitbe1/PHYS14productionV1/PHYS14.QCD_HT-500To1000_13TeV-madgraph_*_RA2AnalysisTree.root",
        "color" : 4 , 
        "weight" : 91.5014519097249 
        }

QCD1000 = { "tag" : "QCD1000HTinf" ,
        "fileName" : "/eos/uscms/store/user/awhitbe1/PHYS14productionV1/PHYS14.QCD_HT_1000ToInf_13TeV-madgraph_*_RA2AnalysisTree.root",
        "color" : 4 , 
        "weight" : 0.971004839254987
        }

TTJets = { "tag" : "TTJets" , 
           "fileName" : "/eos/uscms/store/user/awhitbe1/PHYS14productionV1/PHYS14.TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_*_RA2AnalysisTree.root",
           "color" : 5 ,
           "weight" : 0.170560620344058
           }

WJetsToLNu400 = { "tag" : "WJetsToLNu400HT600" ,
               "fileName" : "/eos/uscms/store/user/awhitbe1/PHYS14productionV1/PHYS14.WJetsToLNu_HT-400to600_Tune4C_13TeV-madgraph-tauola_*_RA2AnalysisTree.root" ,
               "color" : 6 ,
               "weight" : 0.0646704609151318
               }

WJetsToLNu600 = { "tag" : "WJetsToLNu600HTinf" ,
               "fileName" : "/eos/uscms/store/user/awhitbe1/PHYS14productionV1/PHYS14.WJetsToLNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola_*_RA2AnalysisTree.root" ,
               "color" : 6 ,
               "weight" : 0.0213969707986519
               }

samples = [ ZJetsToNuNu400 , ZJetsToNuNu600 , GJets400 , GJets600 , DYJetsToLL400 , DYJetsToLL600 , QCD500 , QCD1000 , TTJets , WJetsToLNu400 , WJetsToLNu600 ]

bashFile = """
#!/bin/bash

date

ls

source /cvmfs/cms.cern.ch/cmsset_default.sh
workerNodeDir=`pwd`
echo $workerNodeDir
cd {0}
eval `scram runtime -sh`
cd -

#### include command for running code below
{1}

ls
"""

condorSubFile = """
universe = vanilla
Executable            = run_{0}.sh
Requirements          = Memory >= 199 &&OpSys == "LINUX"&& (Arch != "DUMMY" )&& Disk > 1000000
Should_Transfer_Files = YES
transfer_input_files=.rootlogon,RgammaZ.C,ZgammaRatio.C,Zmass.C,effArea.cc,isoComparison.C,photonAcceptance.C,photon.cc,photonEfficiency.C,plotArbitraryVar.C,scanEfficiency.C,stackZmumuControlPlots.C,ZnunuYields.C,stackedControlPlots.C,stdPhotonControlPlots.C
WhenToTransferOutput  = ON_EXIT_OR_EVICT
Output = out_{0}.stdout
Error  = out_{0}.stderr
Log    = out_{0}.log
Notification    = Error
notify_user     = awhitbe1@FNAL.GOV
Queue 1
"""

Code = { "controlPlots" : "root -l -b -q 'stdPhotonControlPlots.C+(\"{0}\",\"{1}\",{2})'" ,
         "efficiencies" : "root -l -b -q 'photonEfficiency.C+(\"{0}\",\"{1}\",{2})'" ,
         "scanEfficiencies" : "root -l -b -q scanEfficiency.C" , 
         "closure" : "" 
         }

import os
from optparse import OptionParser

# define options
parser = OptionParser()
parser.add_option("-k", "--codeKey", dest="codeKey", default="",
                                    help="key for selecting code out of dictionary (controlPlots,efficiencies,closure)")

parser.add_option("-f", "--files", dest="files", default="all",
                                    help="key for selecting code out of dictionary (controlPlots,efficiencies,closure)")

parser.add_option("-s","--submit",dest="submit", default=False,action="store_true",
                                       help="submit jobs to condor once they are configured")

parser.add_option("-b","--CMSSWBASE",dest="CMSSW_BASE", default="",
                                       help="location of CMSSW_BASE")

(options, args) = parser.parse_args()

print options.CMSSW_BASE
print options.codeKey

if options.CMSSW_BASE == "" or options.codeKey == "" : 
    raise Exception("need to provide -b and -k options")


for sample in samples : 

    if sample["tag"] == options.files or options.files == "all" : 

        print sample["tag"]
        print Code[options.codeKey].format( sample["fileName"] , sample["tag"] , sample["color"] )
        
        bashFile_ = open("run_"+options.codeKey+"_"+sample["tag"]+".sh" , "w" )
        bashFile_.write(bashFile.format( options.CMSSW_BASE , Code[options.codeKey].format( sample["fileName"] , sample["tag"] , sample["color"] ) ))
        bashFile_.close()
        
        condorSubFile_ = open("condor_submit_"+options.codeKey+"_"+sample["tag"] , "w" )
        condorSubFile_.write(condorSubFile.format( options.codeKey+"_"+sample["tag"] ))
        condorSubFile_.close()
        
        if ( options.submit ) :
            os.system("condor_submit condor_submit_"+options.codeKey+"_"+sample["tag"])

        if ( options.codeKey == "scanEfficiencies" ) : break
