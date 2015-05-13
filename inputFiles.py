from os import walk
from ROOT import *

redirector = "root://cmsxrootd.fnal.gov///store/user/awhitbe1/PHYS14productionV10/"
baseDir = "/eos/uscms/store/user/awhitbe1/PHYS14productionV10/"

inputTag = {"Gjets400":"GJets_HT-400to600",
            "Gjets600":"GJets_HT-600toInf",
            "Zinv400":"ZJetsToNuNu_HT-400to600",
            "Zinv600":"ZJetsToNuNu_HT-600toInf",
            "DYJets400":"DYJetsToLL_M-50_HT-400to600",
            "DYJets600":"DYJetsToLL_M-50_HT-600toInf",
            "QCD500":"QCD_HT-500To1000",
            "QCD1000":"QCD_HT_1000ToInf",
            "TTJets":"TTJets_MSDecaysCKM",
            "WJets400":"WJetsToLNu_HT-400to600",
            "WJets600":"WJetsToLNu_HT-600toInf",

            "T1bbbb_1000-900":"SMS-T1bbbb_2J_mGl-1000_mLSP-900",
            "T1bbbb_1500-100":"SMS-T1bbbb_2J_mGl-1500_mLSP-100",
            "T1qqqq_1000-800":"SMS-T1qqqq_2J_mGl-1000_mLSP-800",
            "T1qqqq_1500-100":"SMS-T1qqqq_2J_mGl-1400_mLSP-100",
            "T1tttt_1200-800":"SMS-T1tttt_2J_mGl-1200_mLSP-800",
            "T1tttt_1500-100":"SMS-T1tttt_2J_mGl-1500_mLSP-100"

}

chains = {}
for key in inputTag :
    chains[key] = TChain("TreeMaker2/PreSelection")
           
# ---------------------------------------|
# s: is the sample unumerator that you   |
# want to add the files to               |
# ---------------------------------------|
# directory: is the directory that the   |
# files reside in                        |
# ---------------------------------------|
# tag: the tag to be used to filter files|
# ---------------------------------------|
def addToChain( key , directory ):

    for (dirpath , dirnames , filenames) in walk(directory) : 
        for f in filenames : 
            if f.find(inputTag[key]) == -1 : continue
            chains[key].Add("{0}{1}".format(redirector,f))
            #print "{0}{1}".format(redirector,f)
                           

def initChains():

    for key in chains :
        print key
        addToChain( key , baseDir )
    

    
