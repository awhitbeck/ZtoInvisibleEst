from ROOT import *
from labels import *
from photonUtils import *

gROOT.ProcessLine(".L ~/tdrstyle.C")
gROOT.ProcessLine("setTDRStyle()")

tree = TChain("TreeMaker2/PreSelection")
tree.Add("~/eos/PhotonTriggerStudies/Spring15.GJets_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_*_RA2AnalysisTree.root")

photon90_PFHT500_HT = TEfficiency("photon90_PFHT500_HT","photon90_PFHT500_HT",30,100,700)
photon90_PFHT500_pT = TEfficiency("photon90_PFHT500_pT","photon90_PFHT500_pT",50,75,125)
photon90 = TEfficiency("photon90","photon90",50,75,125)

nEvents = tree.GetEntries()

for iEvt in range( nEvents ) :

    #if iEvt > 100000 : break

    if iEvt % 10000 == 0 : print iEvt,"of",nEvents

    tree.GetEntry( iEvt )

    HT         = getattr( tree , "HT"          )
    HTclean    = getattr( tree , "HTclean"     )
    MHTclean   = getattr( tree , "MHTclean"    )
    NJetsclean = getattr( tree , "NJetsclean"  )
    triggerBit = getattr( tree , "PassTrigger" )
    photons    = getattr( tree , "photonCands" )
    bestPhoton = getattr( tree , "bestPhoton"  )
    deltaPhi1  = getattr( tree , "DeltaPhi1clean")
    deltaPhi2  = getattr( tree , "DeltaPhi2clean")
    deltaPhi3  = getattr( tree , "DeltaPhi3clean")

    # bit 13: photon90
    # bit 14: photon75
    # btt 15: photon90_PFHT500
    
    bestPhotonIndex = -1 

    for iPhoton in range( len( photons ) ) :
        if passID( tree , iEvt , iPhoton ) and passIsolation( tree , iEvt , iPhoton ) : 
            bestPhotonIndex = iPhoton
            break

    if bestPhotonIndex == -1 : continue
    if deltaPhi1 < 0.5 or deltaPhi2 < 0.5 or deltaPhi3 < 0.3 : continue
    #if MHTclean < 200. : continue

    ### in current configuration bit 14 is the relative
    if triggerBit[14] : #and MHTclean > 200. and NJetsclean > 3 ) :             
        if HTclean > 500.  :
            photon90.Fill( triggerBit[13] , photons[bestPhotonIndex].Pt() )
            photon90_PFHT500_pT.Fill( triggerBit[15] , photons[bestPhotonIndex].Pt() )
        if photons[bestPhotonIndex].Pt() > 100. :
            photon90_PFHT500_HT.Fill( triggerBit[15] , HTclean )
  
#####################################
#####################################
#####################################
#####################################

canHT = TCanvas("canHT","canHT",500,500)

photon90_PFHT500_HT.SetMarkerStyle(8)
photon90_PFHT500_HT.SetMarkerColor(1)
photon90_PFHT500_HT.SetTitle(";HT [GeV];Efficiency")
photon90_PFHT500_HT.Draw()

legHT = TLegend(.5,.2,.9,.4)
legHT.SetFillColor(0)
legHT.SetBorderSize(0)
legHT.AddEntry(photon90_PFHT500_HT,"HLT_Photon90_PFHT500_v1","p")

legHT.Draw()

CMSlabel.Draw()
setLumi(-1.)
SqrtSlumi.Draw()

canHT.SaveAs("Photon90efficienciesVersusHT_Pt100_deltaPhiCuts_MHT200.eps")
canHT.SaveAs("Photon90efficienciesVersusHT_Pt100_deltaPhiCuts_MHT200.pdf")
canHT.SaveAs("Photon90efficienciesVersusHT_Pt100_deltaPhiCuts_MHT200.png")

#####################################
#####################################
#####################################
#####################################
#####################################

canPt = TCanvas("canPt","canPt",500,500)

photon90.Draw()
gPad.Update()
photon90graph = photon90.GetPaintedGraph()
#photon90graph.GetXaxis().SetRangeUser(0.,200.)

photon90_PFHT500_pT.Draw()
gPad.Update()
photon90_PFHT500_pTgraph = photon90_PFHT500_pT.GetPaintedGraph()

photon90graph.SetMarkerStyle(8)
photon90graph.SetMarkerColor(2)
photon90graph.SetTitle(";p_{T,#gamma} [GeV];Efficiency")
photon90graph.GetXaxis().SetNdivisions(505)
photon90graph.Draw("Ap")

photon90_PFHT500_pTgraph.SetMarkerStyle(8)
photon90_PFHT500_pTgraph.SetMarkerColor(1)
photon90_PFHT500_pTgraph.Draw("SAME,p")

leg = TLegend(.5,.2,.9,.4)
leg.SetFillColor(0)
leg.SetBorderSize(0)
leg.AddEntry(photon90graph,"HLT_Photon90_v1","p")
leg.AddEntry(photon90_PFHT500_pTgraph,"HLT_Photon90_PFHT500_v1","p")

leg.Draw()

CMSlabel.Draw()
setLumi(-1.)
SqrtSlumi.Draw()

canPt.SaveAs("Photon90efficienciesVersusPt_HTclean500_deltaPhiCuts_MHT200.eps")
canPt.SaveAs("Photon90efficienciesVersusPt_HTclean500_deltaPhiCuts_MHT200.pdf")
canPt.SaveAs("Photon90efficienciesVersusPt_HTclean500_deltaPhiCuts_MHT200.png")
