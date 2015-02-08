from ROOT import *
from sampleData import GJetsData, ZinvData

gROOT.ProcessLine(".L ~/tdrstyle.C")
gROOT.ProcessLine("setTDRStyle()")

def getHisto( sampleDict , tag ) :

    histo = None

    for iFile in sampleDict :
        temp = TH1F( iFile.Get(tag) )
        temp.SetNameTitle(str(sampleDict[iFile]["name"]+"_"+tag) , str(sampleDict[iFile]["name"]+"_"+tag) )
        #print sampleDict[iFile]["name"],tag
        #print temp.GetBinContent(1),"+/-",temp.GetBinError(1)
        temp.Sumw2()
        temp.Scale(sampleDict[iFile]["weight"])
        #print temp.GetBinContent(1),"+/-",temp.GetBinError(1)
        if histo == None : 
            histo = TH1F( temp )
        else :
            histo.Add(temp)

    #print "total:"
    #print histo.GetBinContent(1),"+/-",histo.GetBinError(1)
    return histo 

def computeEffic( numTag = "" , denomTag = ""):

    #print "numTag:",numTag,"denomTag",denomTag

    GJets_denom = getHisto(GJetsData,denomTag)
    GJets_denom.SetNameTitle("GJets_"+denomTag , "GJets_"+denomTag)
    
    GJets_num = getHisto(GJetsData,numTag)
    GJets_num.SetNameTitle("GJets_"+numTag , "GJets_"+numTag)
    
    #print "num: ",GJets_num.GetBinContent(1),"+/-",GJets_num.GetBinError(1)
    #print "denom: ",GJets_denom.GetBinContent(1),"+/-",GJets_denom.GetBinError(1)

    eff = TH1F(GJets_num)
    eff.SetNameTitle( "GJets_"+numTag+"_"+denomTag , "GJets_"+numTag+"_"+denomTag )

    eff.Divide(GJets_num,GJets_denom,1.,1.,"B")
    
    #print "eff: ",eff.GetBinContent(1),"+/-",eff.GetBinError(1)

    eff.SetLineWidth(2)
    eff.SetLineColor(1)
    eff.SetLineStyle(1)
    eff.SetMarkerColor(1)
    eff.SetMarkerStyle(8)

    return eff

def getHisto2D( sampleDict , tag ) :

    histo = None

    for iFile in sampleDict :
        temp = TH2F( iFile.Get(tag) )
        temp.SetNameTitle(str(sampleDict[iFile]["name"]+"_"+tag) , str(sampleDict[iFile]["name"]+"_"+tag) )
        temp.Sumw2()
        temp.Scale(sampleDict[iFile]["weight"])
        if histo == None : 
            histo = TH2F( temp )
        else :
            histo.Add(temp)

    return histo 

def getHisto2D( sampleDict , tag ) :

    histo = None

    for iFile in sampleDict :
        temp = TH2F( iFile.Get(tag) )
        temp.SetNameTitle(str(sampleDict[iFile]["name"]+"_"+tag) , str(sampleDict[iFile]["name"]+"_"+tag) )
        temp.Sumw2()
        temp.Scale(sampleDict[iFile]["weight"])
        if histo == None : 
            histo = TH2F( temp )
        else :
            histo.Add(temp)

    return histo 

def computeEfficNJets( numTag = "" , denomTag = ""):

    #print "numTag:",numTag,"denomTag",denomTag

    GJets_denom = getHisto2D(GJetsData,denomTag+"_njets")
    GJets_denom.SetNameTitle("GJets_"+denomTag+"_njets" , "GJets_"+denomTag+"_njets")
    
    GJets_num = getHisto2D(GJetsData,numTag+"_njets")
    GJets_num.SetNameTitle("GJets_"+numTag+"_njets" , "GJets_"+numTag+"_njets")
    
    eff = TH2F(GJets_num)
    eff.SetNameTitle( "GJets_"+numTag+"_"+denomTag+"_njets" , "GJets_"+numTag+"_"+denomTag+"_njets" )

    eff.Divide(GJets_num,GJets_denom,1.,1.,"B")

    effVec = []

    for y in range(1,eff.GetNbinsY()+2) : 
        effVec.append( TH1F("GJets_"+numTag+"_"+denomTag+"_njets_"+str(y),"GJets_"+numTag+"_"+denomTag+"_njets_"+str(y) , 6 , 3.5 + .15 * (y-2) , 9.5 + .15 * (y-2) ) )
        #print len(effVec)
        for x in range(1,eff.GetNbinsX()+2):

            #print "y:",y,"x:",x,eff.GetBinContent(x,y),"+/-",eff.GetBinError(x,y)
            effVec[y-1].SetBinContent( x , eff.GetBinContent(x,y) )
            effVec[y-1].SetBinError( x , eff.GetBinError(x,y) )
            

    return effVec

def getHisto2D( sampleDict , tag ) :

    histo = None

    for iFile in sampleDict :
        temp = TH2F( iFile.Get(tag) )
        temp.SetNameTitle(str(sampleDict[iFile]["name"]+"_"+tag) , str(sampleDict[iFile]["name"]+"_"+tag) )
        temp.Sumw2()
        temp.Scale(sampleDict[iFile]["weight"])
        if histo == None : 
            histo = TH2F( temp )
        else :
            histo.Add(temp)

    return histo 

def computeEfficBTags( numTag = "" , denomTag = ""):

    #print "numTag:",numTag,"denomTag",denomTag

    GJets_denom = getHisto2D(GJetsData,denomTag+"_btags")
    GJets_denom.SetNameTitle("GJets_"+denomTag+"_btags" , "GJets_"+denomTag+"_btags")
    
    GJets_num = getHisto2D(GJetsData,numTag+"_btags")
    GJets_num.SetNameTitle("GJets_"+numTag+"_btags" , "GJets_"+numTag+"_btags")
    
    eff = TH2F(GJets_num)
    eff.SetNameTitle( "GJets_"+numTag+"_"+denomTag+"_btags" , "GJets_"+numTag+"_"+denomTag+"_btags" )

    eff.Divide(GJets_num,GJets_denom,1.,1.,"B")

    effVec = []

    for y in range(1,eff.GetNbinsY()+2) : 
        effVec.append( TH1F("GJets_"+numTag+"_"+denomTag+"_btags_"+str(y),"GJets_"+numTag+"_"+denomTag+"_btags_"+str(y) , 4 , -0.5 + .15 * (y-2) , 3.5 + .15 * (y-2) ) )
        #print len(effVec)
        for x in range(1,eff.GetNbinsX()+2):

            #print "y:",y,"x:",x,eff.GetBinContent(x,y),"+/-",eff.GetBinError(x,y)
            effVec[y-1].SetBinContent( x , eff.GetBinContent(x,y) )
            effVec[y-1].SetBinError( x , eff.GetBinError(x,y) )
            

    return effVec

configDict = [ { "name" : "Acceptance" , "numTag" : "accepted" , "denomTag" : "gen" } ,
               { "name" : "ID" , "numTag" : "IDed" , "denomTag" : "accepted" } ,
               { "name" : "Isolation" , "numTag" : "ISOed" , "denomTag" : "IDed" } ,
               { "name" : "Gen-match" , "numTag" : "Matched" , "denomTag" : "ISOed" } 
               ]

can = TCanvas("can","can",500,500)

for i in configDict : 

    eff = computeEffic( i["numTag"] , i["denomTag"] )
    eff.GetXaxis().SetTitle("i^{th} Bin")
    eff.GetYaxis().SetTitle(i["name"]+" Efficiency")
    eff.GetYaxis().SetRangeUser(0,1)
    
    eff.Draw()
    can.SaveAs(i["name"]+"_efficiency_RA2bBins.eps")
    can.SaveAs(i["name"]+"_efficiency_RA2bBins.png")
    can.SaveAs(i["name"]+"_efficiency_RA2bBins.pdf")

    eff = computeEffic( i["numTag"]+"_HT" , i["denomTag"]+"_HT" )
    eff.GetXaxis().SetTitle("H_{T} [GeV]")
    eff.GetYaxis().SetTitle(i["name"]+" Efficiency")
    eff.GetYaxis().SetRangeUser(0,1)
    eff.GetXaxis().SetNdivisions(505)
    
    eff.Draw()
    can.SaveAs(i["name"]+"_efficiency_HT.eps")
    can.SaveAs(i["name"]+"_efficiency_HT.png")
    can.SaveAs(i["name"]+"_efficiency_HT.pdf")

    eff = computeEffic( i["numTag"]+"_MHT" , i["denomTag"]+"_MHT" )
    eff.GetXaxis().SetTitle("H_{T}^{miss}")
    eff.GetYaxis().SetTitle(i["name"]+" Efficiency")
    eff.GetYaxis().SetRangeUser(0,1)
    eff.GetXaxis().SetNdivisions(505)
    
    eff.Draw()
    can.SaveAs(i["name"]+"_efficiency_MHT.eps")
    can.SaveAs(i["name"]+"_efficiency_MHT.png")
    can.SaveAs(i["name"]+"_efficiency_MHT.pdf")

    eff2D = computeEfficNJets( i["numTag"] , i["denomTag"])
    colors = [ 1 , 4 , 6 ]
    for MHT in range( len(eff2D) ) :
        eff2D[MHT].GetYaxis().SetTitle(i["name"]+" Efficiency")
        eff2D[MHT].GetXaxis().SetTitle("n_{jets}")
        eff2D[MHT].GetYaxis().SetRangeUser(0,1)
        eff2D[MHT].SetLineColor(colors[MHT])
        eff2D[MHT].SetLineWidth(2)
        eff2D[MHT].SetLineStyle(1)
        eff2D[MHT].SetMarkerStyle(8)
        eff2D[MHT].SetMarkerColor(colors[MHT])
        if MHT == 0 :
            eff2D[MHT].Draw("e1")
        else : 
            eff2D[MHT].Draw("e1,SAME")
            
    can.SaveAs(i["name"]+"_efficiency_njets.eps")
    can.SaveAs(i["name"]+"_efficiency_njets.png")
    can.SaveAs(i["name"]+"_efficiency_njets.pdf")

    eff2Db = computeEfficBTags( i["numTag"] , i["denomTag"])

    for MHT in range( len(eff2Db) ) :
        eff2Db[MHT].GetYaxis().SetTitle(i["name"]+" Efficiency")
        eff2Db[MHT].GetXaxis().SetTitle("n_{b-tags}")
        eff2Db[MHT].GetYaxis().SetRangeUser(0,1)
        eff2Db[MHT].SetLineColor(colors[MHT])
        eff2Db[MHT].SetLineWidth(2)
        eff2Db[MHT].SetLineStyle(1)
        eff2Db[MHT].SetMarkerStyle(8)
        eff2Db[MHT].SetMarkerColor(colors[MHT])
        if MHT == 0 :
            eff2Db[MHT].Draw("e1")
        else : 
            eff2Db[MHT].Draw("e1,SAME")
            
    can.SaveAs(i["name"]+"_efficiency_btags.eps")
    can.SaveAs(i["name"]+"_efficiency_btags.png")
    can.SaveAs(i["name"]+"_efficiency_btags.pdf")


