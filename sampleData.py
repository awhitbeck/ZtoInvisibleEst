from ROOT import *

fGJets400 = TFile("yields_GJets400HT600_RA2b.root")
fGJets600 = TFile("yields_GJets600HTinf_RA2b.root")

GJetsData = { fGJets400 : { "weight" : 0.075604015773398 , "name" : "GJets400" } ,
              fGJets600 : { "weight" : 0.0261385093009007, "name" : "GJets600" } }

fZinv400 = TFile("yields_ZJetsToNuNu400HT600_RA2b.root")
fZinv600 = TFile("yields_ZJetsToNuNu600HTinf_RA2b.root")

ZinvData = { fZinv400 : { "weight" : 0.0144160859953796  , "name" : "Zinv400" } , 
             fZinv600 : { "weight" : 0.00489467814129328 , "name" : "Zinv600" } }

