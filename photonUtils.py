from math import *
from ROOT import *

def photonCR( tree , event ) :

    tree.GetEntry( event )
    
    NJets = getattr(tree,"NJetsclean")
    #print "NJets:",NJets
    if( NJets < 4 ): return False

    HT = getattr(tree,"HTclean")
    #print "HT:",HT
    if( HT < 500. ): return False

    MHT = getattr(tree,"MHTclean")
    #print "MHT:",MHT
    if( MHT < 200. ): return False

    ### for now I am removing this to increase the statistics!!!
    ### also, we need the cleaned version of these variables!!
    ############################################################
    #DeltaPhi1 = getattr( tree , "DeltaPhi1" ) 
    #DeltaPhi2 = getattr( tree , "DeltaPhi2" ) 
    #DeltaPhi3 = getattr( tree , "DeltaPhi3" ) 
    #if( DeltaPhi1 < 0.5 or 
    #    DeltaPhi2 < 0.5 or 
    #    DeltaPhi3 < 0.3 ): return False

    NumPhotons = getattr(tree,"NumPhotons")
    #print "NumPhotons:",NumPhotons
    if( NumPhotons != 1 ): return False

    NumLeptons = getattr(tree,"Leptons")
    #print "NumLeptons:",NumLeptons
    if( NumLeptons != 0 ): return False

    #### there should be somethings for the jet cleaning done here
    
    return True

def promptPhoton( tree , event , photonIndex = -1 ) :

    tree.GetEntry( event )
    
    genParticles = getattr( tree , "genParticles"             )
    genPDGid     = getattr( tree , "genParticles_PDGid"       )
    genParent    = getattr( tree , "genParticles_parentPDGid" )

    photon = []
    if photonIndex < 0 :
        photon   = getattr( tree , "bestPhoton"               )
    else :
        photons  = getattr( tree , "photonCands"              ) 
        photon.append( photons[photonIndex] )

    pionCount = 0
    promptPhoton = 0 

    for i in range( len( genParticles ) ): 

        if genPDGid[i] != 22 : continue
        
        dPhi = abs( genParticles[i].Phi() - photon[0].Phi() )
        if dPhi > 3.1415 : dPhi = dPhi - 3.1415
        dEta = abs( genParticles[i].Eta() - photon[0].Eta() )
        dR = sqrt( dPhi*dPhi + dEta*dEta ) 
        if dR < 0.2 and abs(genParent[i]) > 100 : 
            #print " --- gen particles"
            #print " --- PDG ID:",genPDGid[i],"parent PDG ID",genParent[i]
            pionCount = pionCount + 1
        if dR < 0.2 and abs(genParent[i]) < 100 : 
            promptPhoton = promptPhoton + 1
    
    #print "pions:",pionCount,"prompt photons:",promptPhoton
    
    if pionCount > 0 and promptPhoton == 0 : return False
    if pionCount == 0 and promptPhoton == 0 : return False
    if pionCount == 0 and promptPhoton > 0 : return True

def passID( tree , iEvt , iPhoton ) :
    
    tree.GetEntry(iEvt)

    hasPixelSeed  = getattr(tree,"photon_hasPixelSeed")
    if hasPixelSeed[iPhoton] : return False

    isEB          = getattr(tree,"photon_isEB")
    if isEB : 
        hOverE        = getattr(tree,"photon_hadTowOverEM")
        if hOverE[iPhoton] > 0.028 : return False
        sigmaIetaIeta = getattr(tree,"photon_sigmaIetaIeta")
        if sigmaIetaIeta[iPhoton] > 0.0107 : return False
    else :
        hOverE        = getattr(tree,"photon_hadTowOverEM")
        if hOverE[iPhoton] > 0.093 : return False
        sigmaIetaIeta = getattr(tree,"photon_sigmaIetaIeta")
        if sigmaIetaIeta[iPhoton] > 0.0272 : return False

    return True

def passIsolation( tree , iEvt , iPhoton ) :

    tree.GetEntry(iEvt)

    photonCands   = getattr(tree,"photonCands")
    isEB          = getattr(tree,"photon_isEB")
    
    if isEB :
        chargedIso    = getattr(tree,"photon_pfChargedIsoRhoCorr")
        if chargedIso[iPhoton] > 2.67 : return False
        #  NOTE: gammaIso and neutral Iso have been switched to fix a bug in the treemaker!!!!!                                                                                                 
        gammaIso      = getattr(tree,"photon_pfGammaIsoRhoCorr")
        if gammaIso[iPhoton] > 7.23+exp(0.0028*photonCands[iPhoton].Pt()+0.5408) : return False
        neutralIso    = getattr(tree,"photon_pfNeutralIsoRhoCorr")
        if neutralIso[iPhoton] > 2.11+0.0014*photonCands[iPhoton].Pt() : return False

    else :        
        chargedIso    = getattr(tree,"photon_pfChargedIsoRhoCorr")
        if chargedIso[iPhoton] > 1.79 : return False
        #  NOTE: gammaIso and neutral Iso have been switched to fix a bug in the treemaker!!!!!
        gammaIso      = getattr(tree,"photon_pfGammaIsoRhoCorr")
        if gammaIso[iPhoton] > 8.89+0.01725*photonCands[iPhoton].Pt() : return False
        neutralIso    = getattr(tree,"photon_pfNeutralIsoRhoCorr")
        if neutralIso[iPhoton] > 3.09+0.0091*photonCands[iPhoton].Pt() : return False

    return True

def getSigmaIetaIetaDistributions( tree , tag , weight , splitNonPrompt ):

    promptHisto = TH1F("promptHisto_{0}".format(tag),"promptHisto_{0}".format(tag),200,0,0.05)
    promptHisto.Sumw2()
    promptHisto.GetXaxis().SetTitle("#sigma_{i#etai#eta}")
    promptHisto.GetXaxis().SetNdivisions(505)
    promptHisto.GetYaxis().SetTitle("Events")

    nonPromptHisto = TH1F("nonPromptHisto_{0}".format(tag),"nonPromptHisto_{0}".format(tag),200,0,0.05)
    nonPromptHisto.Sumw2()
    nonPromptHisto.GetXaxis().SetTitle("#sigma_{i#etai#eta}")
    nonPromptHisto.GetXaxis().SetNdivisions(505)
    nonPromptHisto.GetYaxis().SetTitle("Events")

    numEvents = tree.GetEntries()
    for i in range( numEvents ) : 

        tree.GetEntry(i)

        #if i > 1000 : break

        if i % 10000 == 0 : print i,"/",numEvents

        sigmaIetaIeta = getattr(tree,"photon_sigmaIetaIeta")
        photonCands   = getattr(tree,"photonCands")
        hasPixelSeed  = getattr(tree,"photon_hasPixelSeed")
        hOverE        = getattr(tree,"photon_hadTowOverEM")
        isEB          = getattr(tree,"photon_isEB")
        chargedIso    = getattr(tree,"photon_pfChargedIsoRhoCorr")
        neutralIso    = getattr(tree,"photon_pfNeutralIsoRhoCorr")
        gammaIso      = getattr(tree,"photon_pfGammaIsoRhoCorr")
        
        for p in range( len( photonCands ) ) : 

            #print "----- photon",p,"------"
            #print "sigmaIetaIeta:",sigmaIetaIeta[p]
            #print "hasPixelSeed:",hasPixelSeed[p]
            #print "hOverE:",hOverE[p]
            #print "isEB:",isEB[p]
            #print "chargedIso:",chargedIso[p]
            #print "neutralIso:",neutralIso[p]
            #print "gammaIso:",gammaIso[p]
                
            #### check whether photon candidates
            #### pass all ID and ISO cuts except
            #### the shower shape cut.  If so, 
            #### fill that photon into histograms
            if hasPixelSeed[p] : continue
            if photonCands[p].Pt() < 100. : continue
            if isEB[p] : 
                if hOverE[p] > 0.028 : continue
                if chargedIso[p] > 2.67 : continue
                #  NOTE: gammaIso and neutral Iso have been switched to fix a bug in the treemaker!!!!!
                if gammaIso[p] > 7.23+exp(0.0028*photonCands[p].Pt()+0.5408) : continue
                if neutralIso[p] > 2.11+0.0014*photonCands[p].Pt() : continue
            else :
                continue
                if hOverE[p] > 0.093 : continue
                if chargedIso[p] > 1.79 : continue
                #  NOTE: gammaIso and neutral Iso have been switched to fix a bug in the treemaker!!!!!
                if gammaIso[p] > 8.89+0.01725*photonCands[p].Pt() : continue
                if neutralIso[p] > 3.09+0.0091*photonCands[p].Pt() : continue

            if splitNonPrompt :
                is_prompt = promptPhoton( tree , i , p ) 
            else : 
                is_prompt = True
            
            if is_prompt : 
                promptHisto.Fill(sigmaIetaIeta[p],weight)
                
            if not is_prompt : 
                nonPromptHisto.Fill(sigmaIetaIeta[p],weight)

    return {"prompt":promptHisto,
            "nonPrompt":nonPromptHisto}
        
def getNJetBinsDistributions( tree , tag , weight ) : 
    
    promptHisto = TH1F("promptHisto_{0}".format(tag),"promptHisto_{0}".format(tag),3,
0.5,3.5)
    promptHisto.Sumw2()
    promptHisto.GetXaxis().SetTitle("RA2 NJets bins")
    promptHisto.GetXaxis().SetNdivisions(503)
    promptHisto.GetYaxis().SetTitle("Events")

    nonPromptHisto = TH1F("nonPromptHisto_{0}".format(tag),"nonPromptHisto_{0}".format(tag),3,0.5,3.5)
    nonPromptHisto.Sumw2()
    nonPromptHisto.GetXaxis().SetTitle("RA2 NJets bins")
    nonPromptHisto.GetXaxis().SetNdivisions(503)
    nonPromptHisto.GetYaxis().SetTitle("Events")

    numEvents = tree.GetEntries()
    for i in range( numEvents ) : 

        tree.GetEntry( i )

        if i % 10000 == 0 : print i,"/",numEvents
        if photonCR( tree , i ) :
            print "event:",i,"passed"
            is_prompt = promptPhoton( tree , i ) 
            NJets = getattr(tree,"NJetsclean")
            if is_prompt : 
                if NJets >= 4 and NJets <=6 : 
                    promptHisto.Fill(1,weight)
                if NJets >=7 and NJets <= 8 : 
                    promptHisto.Fill(2,weight)
                if NJets >= 9 :
                    promptHisto.Fill(3,weight)
            if not is_prompt : 
                if NJets >= 4 and NJets <=6 : 
                    nonPromptHisto.Fill(1,weight)
                if NJets >=7 and NJets <= 8 : 
                    nonPromptHisto.Fill(2,weight)
                if NJets >= 9 :
                    nonPromptHisto.Fill(3,weight)

    return {"prompt":promptHisto,
            "nonPrompt":nonPromptHisto}

