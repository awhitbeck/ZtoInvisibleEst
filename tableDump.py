from ROOT import *

gROOT.ProcessLine(".L ~/tdrstyle.C")
gROOT.ProcessLine("setTDRStyle()")

def computeProduct( prod , histoA , histoB ):

    for i in range( prod.GetNbinsX() ):

        print i,histoA.GetBinContent(i+1),histoB.GetBinContent(i+1)

        product = histoA.GetBinContent(i+1) / 1e-10
        prod.SetBinContent( i+1 , product )
        if histoA.GetBinContent(i+1) == 0 :
            relErrorA = 0.0
        else : 
            relErrorA = histoA.GetBinError(i+1) / histoA.GetBinContent(i+1)            
        if histoB.GetBinContent(i+1) == 0 :
            relErrorB = 0.0
        else :
            relErrorB = histoB.GetBinError(i+1) / histoB.GetBinContent(i+1)
        error = ratio * sqrt( relErrorA*relErrorA + relErrorB*relErrorB )
        prod.SetBinError( i+1 , error )
        
    return prod

def computeEfficiency( eff , histoA , histoB ):

    for i in range( eff.GetNbinsX() ):

        print i,histoA.GetBinContent(i+1),histoB.GetBinContent(i+1)

        if histoB.GetBinContent(i+1) == 0 :
            ratio = histoA.GetBinContent(i+1) / 1e-10
        else :
            ratio = histoA.GetBinContent(i+1) / histoB.GetBinContent(i+1)
        eff.SetBinContent( i+1 , ratio )
        if histoA.GetBinContent(i+1) == 0 :
            relErrorA = 0.0
        else : 
            relErrorA = histoA.GetBinError(i+1) / histoA.GetBinContent(i+1)            
        if histoB.GetBinContent(i+1) == 0 :
            relErrorB = 0.0
        else :
            relErrorB = histoB.GetBinError(i+1) / histoB.GetBinContent(i+1)
        error = ratio * sqrt( relErrorA*relErrorA + relErrorB*relErrorB )
        eff.SetBinError( i+1 , error )
        
    return eff

def computeRatio( R , histoA , histoB ):

    for i in range( R.GetNbinsX() ):

        if histoB.GetBinContent(i+1) == 0 :
            ratio = histoA.GetBinContent(i+1) / 1e-10
        else :
            ratio = histoA.GetBinContent(i+1) / histoB.GetBinContent(i+1)
        R.SetBinContent( i+1 , ratio )
        if histoA.GetBinContent(i+1) == 0 :
            relErrorA = 0.0
        else : 
            relErrorA = histoA.GetBinError(i+1) / histoA.GetBinContent(i+1)            
        if histoB.GetBinContent(i+1) == 0 :
            relErrorB = 0.0
        else :
            relErrorB = histoB.GetBinError(i+1) / histoB.GetBinContent(i+1)
        error = ratio * sqrt( relErrorA*relErrorA + relErrorB*relErrorB )
        print i,histoA.GetBinContent(i+1),"+/-",relErrorA,histoB.GetBinContent(i+1),"+/-",relErrorB
        print "ratio:",ratio
        print "error:",error

        R.SetBinError( i+1 , error )
        
    return R

def combineSamples(total , histoA , weightA , histoB , weightB ):

    for i in range(total.GetNbinsX()):
        weightedSum = histoA.GetBinContent(i+1) * weightA + histoB.GetBinContent(i+1)*weightB
        total.SetBinContent(i+1, weightedSum)
        errorA = sqrt(histoA.GetBinContent(i+1))
        errorB = sqrt(histoB.GetBinContent(i+1))
        error = sqrt( weightA*weightA*errorA*errorA + weightB*weightB*errorB*errorB )
        total.SetBinError(i+1, error) 

    return total

fGJets400 = TFile("yields_GJets400HT600_RA2b.root")
fGJets600 = TFile("yields_GJets600HTinf_RA2b.root")
fZinv400 = TFile("yields_ZJetsToNuNu400HT600_RA2b.root")
fZinv600 = TFile("yields_ZJetsToNuNu600HTinf_RA2b.root")

Zinv400 = TH1F( fZinv400.Get("gen") )
Zinv400.SetNameTitle("Zinv400","Zinv400")
Zinv600 = TH1F( fZinv600.Get("gen") )
Zinv600.SetNameTitle("Zinv600","Zinv600")

Zinv = TH1F( Zinv400 )
Zinv.SetNameTitle("Zinv","Zinv")
combineSamples(Zinv,Zinv400,0.0144160859953796,Zinv600,0.00489467814129328)

GJets400 = TH1F( fGJets400.Get("gen") )
GJets400.SetNameTitle("GJets400","GJets400")
GJets600 = TH1F( fGJets600.Get("gen") )
GJets600.SetNameTitle("GJets600","GJets600")

GJets = TH1F( GJets400 )
GJets.SetNameTitle("GJets","GJets")
combineSamples(GJets,GJets400,0.075604015773398,GJets600,0.0261385093009007)

GJets.SetLineWidth(2)
GJets.SetLineStyle(2)
GJets.SetLineColor(6)
GJets.DrawNormalized("hist")    

Zinv.SetLineWidth(2)
Zinv.SetLineStyle(1)
Zinv.SetLineColor(4)
Zinv.DrawNormalized("hist,same")

GJets_acc400 = TH1F( fGJets400.Get("accepted") )
GJets_acc400.SetNameTitle("GJets_acc400","GJets_acc400")
GJets_acc600 = TH1F( fGJets600.Get("accepted") )
GJets_acc600.SetNameTitle("GJets_acc600","GJets_acc600")

GJets_acc = TH1F( fGJets400.Get("accepted") )
GJets_acc.SetNameTitle("GJets_acc","GJets_acc")
combineSamples(GJets_acc,GJets_acc400,0.075604015773398,GJets_acc600,0.0261385093009007)

GJets_id400 = TH1F( fGJets400.Get("IDed") )
GJets_id400.SetNameTitle("GJets_id400","GJets_id400")
GJets_id600 = TH1F( fGJets600.Get("IDed") )
GJets_id600.SetNameTitle("GJets_id600","GJets_id600")

GJets_id = TH1F( fGJets400.Get("IDed") )
GJets_id.SetNameTitle("GJets_id","GJets_id")
combineSamples(GJets_id,GJets_id400,0.075604015773398,GJets_id600,0.0261385093009007)

GJets_iso400 = TH1F( fGJets400.Get("ISOed") )
GJets_iso400.SetNameTitle("GJets_iso400","GJets_iso400")
GJets_iso600 = TH1F( fGJets600.Get("ISOed") )
GJets_iso600.SetNameTitle("GJets_iso600","GJets_iso600")

GJets_iso = TH1F( fGJets400.Get("ISOed") )
GJets_iso.SetNameTitle("GJets_id","GJets_iso")
combineSamples(GJets_iso,GJets_iso400,0.075604015773398,GJets_iso600,0.0261385093009007)

effAcc = TH1F(GJets_acc)
effAcc.SetNameTitle("effAcc","effAcc")
effAcc.Divide(GJets_acc,GJets,1.,1.,"B")
#computeEfficiency( effAcc , GJets_acc , GJets )

effID = TH1F(GJets_id)
effID.SetNameTitle("effID","effID")
effID.Divide(GJets_id,GJets_acc,1.,1.,"B")
#computeEfficiency( effID , GJets_id , GJets_acc )

effISO = TH1F(GJets_iso)
effISO.SetNameTitle("effISO","effISO")
effISO.Divide(GJets_iso,GJets_id,1.,1.,"B")
#computeEfficiency( effISO , GJets_iso , GJets_id )

### inflate GJets_iso errors to match 5/fb data...
for iBin in range( GJets_iso.GetNbinsX() ) :
    GJets_iso.SetBinError( iBin , sqrt( GJets_iso.GetBinContent(iBin) ) )

can1 = TCanvas("can2","can2",500,500)
effAcc.GetYaxis().SetRangeUser(0,1.5)
effAcc.Draw("hist,e1")
effID.Draw("hist,e1,SAME")
effISO.Draw("hist,e1,SAME")

can2 = TCanvas("can1","can1",500,500)
gPad.SetLogy()
GJets.Draw("hist,e1")
GJets_acc.Draw("hist,e1,SAME")
GJets_id.Draw("hist,e1,SAME")
GJets_iso.Draw("hist,e1,SAME")

print "======Z/gamma ratio======="
ZgamRatio = TH1F(GJets)
ZgamRatio.SetNameTitle("ZgamRatio","ZgamRatio")
computeRatio(ZgamRatio,Zinv,GJets)
ZgamRatio.GetXaxis().SetTitle("i^{th} Bin")
ZgamRatio.GetYaxis().SetTitle("Z/#gamma Ratio")
for i in range(Zinv.GetNbinsX()):
    print i,":",Zinv.GetBinContent(i),"/",GJets.GetBinContent(i),"=",ZgamRatio.GetBinContent(i)

ZgamRatio.GetYaxis().SetRangeUser(0,.5)
ZgamRatio.SetMarkerStyle(8)
ZgamRatio.SetLineColor(1)
ZgamRatio.SetLineStyle(1)
ZgamRatio.SetLineWidth(2)

can3 = TCanvas("can3","can3",500,500)
ZgamRatio.Draw("e1")

can3.SaveAs("ZgamRatio.png")
can3.SaveAs("ZgamRatio.eps")
can3.SaveAs("ZgamRatio.pdf")

##==============================
## Z/gamma ratio versus njets
##==============================
GJets400_njets = TH2F( fGJets400.Get("gen_njets") )
GJets400_njets.SetNameTitle("GJets400_njets",";n_{jets};Events")
GJets400_njets.Sumw2()
GJets600_njets = TH2F( fGJets600.Get("gen_njets") )
GJets600_njets.SetNameTitle("GJets600_njets",";n_{jets};Events")
GJets600_njets.Sumw2()
GJets_njets2D = TH2F( GJets400_njets )
GJets_njets2D.SetNameTitle("GJets_njets2D",";n_{jets};Events")
GJets_njets2D.Scale(0.075604015773398)
GJets_njets2D.Add(GJets600_njets,0.0261385093009007)
GJets_njets2D.SetLineColor(4)
GJets_njets2D.SetLineWidth(2)
GJets_njets2D.SetLineStyle(1)

GJets_njets = TH1F("GJets_njets",";n_{jets};Events",6,3.5,9.5)
for xBin in range( 1 , GJets.GetNbinsX() + 1 ) :
    GJets_njets.SetBinContent( xBin , GJets_njets2D.GetBinContent(xBin,3) + GJets_njets2D.GetBinContent(xBin,2) + GJets_njets2D.GetBinContent(xBin,1) ) 
    GJets_njets.SetBinError( xBin , sqrt( pow( GJets_njets2D.GetBinError( xBin , 1 ) , 2 ) + pow( GJets_njets2D.GetBinError( xBin , 2 ) , 2 ) + pow( GJets_njets2D.GetBinError( xBin , 3 ) , 2 ) ) )

GJets_njets.SetLineColor(4)
GJets_njets.SetLineWidth(2)
GJets_njets.SetLineStyle(1)

Zinv400_njets = TH2F( fZinv400.Get("gen_njets") )
Zinv400_njets.SetNameTitle("Zinv400_njets",";n_{jets};Events")
Zinv400_njets.Sumw2()
Zinv600_njets = TH2F( fZinv600.Get("gen_njets") )
Zinv600_njets.SetNameTitle("Zinv600_njets",";n_{jets};Events")
Zinv600_njets.Sumw2()
Zinv_njets2D = TH2F( Zinv400_njets )
Zinv_njets2D.SetNameTitle("Zinv_njets2D",";n_{jets};Events")
Zinv_njets2D.Scale(0.0144160859953796)
Zinv_njets2D.Add(Zinv600_njets,0.00489467814129328)
Zinv_njets2D.SetLineColor(6)
Zinv_njets2D.SetLineWidth(2)
Zinv_njets2D.SetLineStyle(2)

Zinv_njets = TH1F("Zinv_njets",";n_{jets};Events",6,3.5,9.5)
for xBin in range( 1 , Zinv.GetNbinsX() + 1 ) :
    Zinv_njets.SetBinContent( xBin , Zinv_njets2D.GetBinContent(xBin,3) + Zinv_njets2D.GetBinContent(xBin,2) + Zinv_njets2D.GetBinContent(xBin,1) ) 
    Zinv_njets.SetBinError( xBin , sqrt( pow( Zinv_njets2D.GetBinError( xBin , 1 ) , 2 ) + pow( Zinv_njets2D.GetBinError( xBin , 2 ) , 2 ) + pow( Zinv_njets2D.GetBinError( xBin , 3 ) , 2 ) ) )

Zinv_njets.SetLineColor(6)
Zinv_njets.SetLineWidth(2)
Zinv_njets.SetLineStyle(2)

ZgamR_njets = TH1F( Zinv_njets )
ZgamR_njets.SetNameTitle("ZgamR_njets",";n_{jets};Events")
ZgamR_njets.Divide( GJets_njets )
ZgamR_njets.SetMarkerStyle(8)
ZgamR_njets.SetLineColor(1)

for xBin in range( 1 , Zinv_njets.GetNbinsX() + 1 ) :
    print "xBin:",xBin
    print "Zinv:",Zinv_njets.GetBinContent(xBin),"+/-",Zinv_njets.GetBinError(xBin),"GJets:",GJets_njets.GetBinContent(xBin),"+/-",GJets_njets.GetBinError(xBin)
    print "ratio:",ZgamR_njets.GetBinContent(xBin),"+/-",ZgamR_njets.GetBinError(xBin)

can7 = TCanvas("can7","can7",500,500)
njetsPad = TPad("njetsPad","MHTPad",0,0.3,1,1)
ZgamR_njetsPad = TPad("ZgamR_njetsPad","ZgamR_MHTPad",0,0,1,0.3)
njetsPad.SetBottomMargin(0)
ZgamR_njetsPad.SetTopMargin(0)
ZgamR_njetsPad.SetBottomMargin(.3)
ZgamR_njets.GetXaxis().SetTitleSize(.12)
ZgamR_njets.GetYaxis().SetTitleSize(.12)
ZgamR_njets.GetYaxis().SetTitleOffset(.5)
ZgamR_njets.GetXaxis().SetLabelSize(.10)
ZgamR_njets.GetYaxis().SetLabelSize(.10)
ZgamR_njets.GetYaxis().SetNdivisions(505)
njetsPad.Draw()
ZgamR_njetsPad.Draw()
njetsPad.SetLogy()
njetsPad.cd()
GJets_njets.Draw("hist,e1")
Zinv_njets.Draw("hist,e1,SAME")

ZgamR_njets.GetYaxis().SetRangeUser(.1,.3)

ZgamR_njetsPad.cd()
ZgamR_njets.Draw("e1")

can7.SaveAs("ZgamRatio_njets.eps")
can7.SaveAs("ZgamRatio_njets.png")
can7.SaveAs("ZgamRatio_njets.pdf")

##==============================
## Z/gamma ratio versus btags
##==============================
GJets400_btags = TH2F( fGJets400.Get("gen_btags") )
GJets400_btags.SetNameTitle("GJets400_btags",";n_{jets};Events")
GJets400_btags.Sumw2()
GJets600_btags = TH2F( fGJets600.Get("gen_btags") )
GJets600_btags.SetNameTitle("GJets600_btags",";n_{btags};Events")
GJets600_btags.Sumw2()
GJets_btags2D = TH2F( GJets400_btags )
GJets_btags2D.SetNameTitle("GJets_btags2D",";n_{btags};Events")
GJets_btags2D.Scale(0.075604015773398)
GJets_btags2D.Add(GJets600_btags,0.0261385093009007)
GJets_btags2D.SetLineColor(4)
GJets_btags2D.SetLineWidth(2)
GJets_btags2D.SetLineStyle(1)

GJets_btags = TH1F("GJets_btags",";n_{btags};Events",4,-0.5,3.5)
for xBin in range( 1 , GJets.GetNbinsX() + 1 ) :
    GJets_btags.SetBinContent( xBin , GJets_btags2D.GetBinContent(xBin,3) + GJets_btags2D.GetBinContent(xBin,2) + GJets_btags2D.GetBinContent(xBin,1) ) 
    GJets_btags.SetBinError( xBin , sqrt( pow( GJets_btags2D.GetBinError( xBin , 1 ) , 2 ) + pow( GJets_btags2D.GetBinError( xBin , 2 ) , 2 ) + pow( GJets_btags2D.GetBinError( xBin , 3 ) , 2 ) ) )

GJets_btags.SetLineColor(4)
GJets_btags.SetLineWidth(2)
GJets_btags.SetLineStyle(1)

Zinv400_btags = TH2F( fZinv400.Get("gen_btags") )
Zinv400_btags.SetNameTitle("Zinv400_btags",";n_{btags};Events")
Zinv400_btags.Sumw2()
Zinv600_btags = TH2F( fZinv600.Get("gen_btags") )
Zinv600_btags.SetNameTitle("Zinv600_btags",";n_{btags};Events")
Zinv600_btags.Sumw2()
Zinv_btags2D = TH2F( Zinv400_btags )
Zinv_btags2D.SetNameTitle("Zinv_btags2D",";n_{btags};Events")
Zinv_btags2D.Scale(0.0144160859953796)
Zinv_btags2D.Add(Zinv600_btags,0.00489467814129328)
Zinv_btags2D.SetLineColor(6)
Zinv_btags2D.SetLineWidth(2)
Zinv_btags2D.SetLineStyle(2)

Zinv_btags = TH1F("Zinv_btags",";n_{btags};Events",4,-0.5,3.5)
for xBin in range( 1 , Zinv.GetNbinsX() + 1 ) :
    Zinv_btags.SetBinContent( xBin , Zinv_btags2D.GetBinContent(xBin,3) + Zinv_btags2D.GetBinContent(xBin,2) + Zinv_btags2D.GetBinContent(xBin,1) ) 
    Zinv_btags.SetBinError( xBin , sqrt( pow( Zinv_btags2D.GetBinError( xBin , 1 ) , 2 ) + pow( Zinv_btags2D.GetBinError( xBin , 2 ) , 2 ) + pow( Zinv_btags2D.GetBinError( xBin , 3 ) , 2 ) ) )

Zinv_btags.SetLineColor(6)
Zinv_btags.SetLineWidth(2)
Zinv_btags.SetLineStyle(2)

ZgamR_btags = TH1F( Zinv_btags )
ZgamR_btags.SetNameTitle("ZgamR_btags",";n_{btags};Events")
ZgamR_btags.Divide( GJets_btags )
ZgamR_btags.SetNdivisions(504)
ZgamR_btags.SetMarkerStyle(8)
ZgamR_btags.SetLineColor(1)

for xBin in range( 1 , Zinv_btags.GetNbinsX() + 1 ) :
    print "xBin:",xBin
    print "Zinv:",Zinv_btags.GetBinContent(xBin),"+/-",Zinv_btags.GetBinError(xBin),"GJets:",GJets_btags.GetBinContent(xBin),"+/-",GJets_btags.GetBinError(xBin)
    print "ratio:",ZgamR_btags.GetBinContent(xBin),"+/-",ZgamR_btags.GetBinError(xBin)

can8 = TCanvas("can8","can8",500,500)
btagsPad = TPad("btagsPad","MHTPad",0,0.3,1,1)
ZgamR_btagsPad = TPad("ZgamR_btagsPad","ZgamR_MHTPad",0,0,1,0.3)
btagsPad.SetBottomMargin(0)
ZgamR_btagsPad.SetTopMargin(0)
ZgamR_btagsPad.SetBottomMargin(.3)
ZgamR_btags.GetXaxis().SetTitleSize(.12)
ZgamR_btags.GetYaxis().SetTitleSize(.12)
ZgamR_btags.GetYaxis().SetTitleOffset(.5)
ZgamR_btags.GetXaxis().SetLabelSize(.10)
ZgamR_btags.GetYaxis().SetLabelSize(.10)
ZgamR_btags.GetYaxis().SetNdivisions(505)
btagsPad.Draw()
ZgamR_btagsPad.Draw()
btagsPad.SetLogy()
btagsPad.cd()
GJets_btags.Draw("hist,e1")
Zinv_btags.Draw("hist,e1,SAME")

ZgamR_btags.GetYaxis().SetRangeUser(.1,.3)

ZgamR_btagsPad.cd()
ZgamR_btags.Draw("e1")

can8.SaveAs("ZgamRatio_btags.eps")
can8.SaveAs("ZgamRatio_btags.png")
can8.SaveAs("ZgamRatio_btags.pdf")

##==============================
## Z/gamma ratio versus MHT
##==============================
GJets400_MHT = TH1F( fGJets400.Get("gen_MHT") )
GJets400_MHT.SetNameTitle("GJets400_MHT",";H_{T}^{miss};Events")
GJets400_MHT.Sumw2()
GJets600_MHT = TH1F( fGJets600.Get("gen_MHT") )
GJets600_MHT.SetNameTitle("GJets600_MHT",";H_{T}^{miss};Events")
GJets600_MHT.Sumw2()
GJets_MHT = TH1F( GJets400_MHT )
GJets_MHT.SetNameTitle("GJets_MHT",";H_{T}^{miss};Events")
GJets_MHT.Scale(0.075604015773398)
GJets_MHT.Add(GJets600_MHT,0.0261385093009007)
GJets_MHT.SetLineColor(4)
GJets_MHT.SetLineWidth(2)
GJets_MHT.SetLineStyle(1)

Zinv400_MHT = TH1F( fZinv400.Get("gen_MHT") )
Zinv400_MHT.SetNameTitle("Zinv400_MHT",";H_{T}^{miss};Events")
Zinv400_MHT.Sumw2()
Zinv600_MHT = TH1F( fZinv600.Get("gen_MHT") )
Zinv600_MHT.SetNameTitle("Zinv600_MHT",";H_{T}^{miss};Events")
Zinv600_MHT.Sumw2()
Zinv_MHT = TH1F( Zinv400_MHT )
Zinv_MHT.SetNameTitle("Zinv_MHT",";H_{T}^{miss};Events")
Zinv_MHT.Scale(0.0144160859953796)
Zinv_MHT.Add(Zinv600_MHT,0.00489467814129328)
Zinv_MHT.SetLineColor(6)
Zinv_MHT.SetLineWidth(2)
Zinv_MHT.SetLineStyle(2)

ZgamR_MHT = TH1F( Zinv_MHT )
ZgamR_MHT.SetNameTitle("ZgamR_MHT",";H_{T}^{miss};Events")
ZgamR_MHT.Divide( GJets_MHT )
ZgamR_MHT.SetMarkerStyle(8)
ZgamR_MHT.SetLineColor(1)

can5 = TCanvas("can5","can5",500,500)
MHTPad = TPad("MHTPad","MHTPad",0,0.3,1,1)
ZgamR_MHTPad = TPad("ZgamR_MHTPad","ZgamR_MHTPad",0,0,1,0.3)
MHTPad.SetBottomMargin(0)
ZgamR_MHTPad.SetTopMargin(0)
ZgamR_MHTPad.SetBottomMargin(.3)
ZgamR_MHT.GetXaxis().SetTitleSize(.12)
ZgamR_MHT.GetYaxis().SetTitleSize(.12)
ZgamR_MHT.GetYaxis().SetTitleOffset(.5)
ZgamR_MHT.GetXaxis().SetLabelSize(.10)
MHTPad.Draw()
ZgamR_MHTPad.Draw()
MHTPad.SetLogy()
MHTPad.cd()
GJets_MHT.Draw("hist,e1")
Zinv_MHT.Draw("hist,e1,SAME")

ZgamR_MHT.GetYaxis().SetRangeUser(0,.5)

ZgamR_MHTPad.cd()
ZgamR_MHT.Draw("e1")

can5.SaveAs("ZgamRatio_MHT.eps")
can5.SaveAs("ZgamRatio_MHT.png")
can5.SaveAs("ZgamRatio_MHT.pdf")

##==============================
## Z/gamma ratio versus HT
##==============================
GJets400_HT = TH1F( fGJets400.Get("gen_HT") )
GJets400_HT.SetNameTitle("GJets400_HT",";H_{T};Events")
GJets400_HT.Sumw2()
GJets600_HT = TH1F( fGJets600.Get("gen_HT") )
GJets600_HT.SetNameTitle("GJets600_HT",";H_{T};Events")
GJets600_HT.Sumw2()
GJets_HT = TH1F( GJets400_HT )
GJets_HT.SetNameTitle("GJets_HT",";H_{T};Events")
GJets_HT.Scale(0.075604015773398)
GJets_HT.Add(GJets600_HT,0.0261385093009007)
GJets_HT.SetLineColor(4)
GJets_HT.SetLineWidth(2)
GJets_HT.SetLineStyle(1)

Zinv400_HT = TH1F( fZinv400.Get("gen_HT") )
Zinv400_HT.SetNameTitle("Zinv400_HT",";H_{T};Events")
Zinv400_HT.Sumw2()
Zinv600_HT = TH1F( fZinv600.Get("gen_HT") )
Zinv600_HT.SetNameTitle("Zinv600_HT",";H_{T};Events")
Zinv600_HT.Sumw2()
Zinv_HT = TH1F( Zinv400_HT )
Zinv_HT.SetNameTitle("Zinv_HT",";H_{T};Events")
Zinv_HT.Scale(0.0144160859953796)
Zinv_HT.Add(Zinv600_HT,0.00489467814129328)
Zinv_HT.SetLineColor(6)
Zinv_HT.SetLineWidth(2)
Zinv_HT.SetLineStyle(2)

ZgamR_HT = TH1F( Zinv_HT )
ZgamR_HT.SetNameTitle("ZgamR_HT",";H_{T};Events")
ZgamR_HT.Divide( GJets_HT )
ZgamR_HT.SetMarkerStyle(8)
ZgamR_HT.SetLineColor(1)

can6 = TCanvas("can6","can6",500,500)
HTPad = TPad("HTPad","HTPad",0,0.3,1,1)
ZgamR_HTPad = TPad("ZgamR_HTPad","ZgamR_HTPad",0,0,1,0.3)
HTPad.SetBottomMargin(0)
ZgamR_HTPad.SetTopMargin(0)
ZgamR_HTPad.SetBottomMargin(.3)
ZgamR_HT.GetXaxis().SetTitleSize(.12)
ZgamR_HT.GetYaxis().SetTitleSize(.12)
ZgamR_HT.GetYaxis().SetTitleOffset(.5)
ZgamR_HT.GetXaxis().SetLabelSize(.10)
HTPad.Draw()
ZgamR_HTPad.Draw()
HTPad.SetLogy()
HTPad.cd()
GJets_HT.Draw("hist,e1")
Zinv_HT.Draw("hist,e1,SAME")

ZgamR_HT.GetYaxis().SetRangeUser(0,.5)

ZgamR_HTPad.cd()
ZgamR_HT.Draw("e1")

can6.SaveAs("ZgamRatio_HT.eps")
can6.SaveAs("ZgamRatio_HT.png")
can6.SaveAs("ZgamRatio_HT.pdf")
##==============================

Zestimate = TH1F(GJets_iso)
Zestimate.SetNameTitle("Zestimate","Zestimate")
Zestimate.GetXaxis().SetTitle("i^{th} Bin")
Zestimate.GetYaxis().SetTitle("Events")
Zestimate.SetLineColor(6)
Zestimate.SetLineStyle(1)
Zestimate.Multiply( GJets_iso , ZgamRatio , 1. , 1. );
Zestimate.Divide( Zestimate , effAcc , 1. , 1. );
Zestimate.Divide( Zestimate , effID , 1. , 1. );
Zestimate.Divide( Zestimate , effISO , 1. , 1. );

can4 = TCanvas("can4","can4",500,500);
closurePad = TPad("closurePad","closurePad",0,0.3,1,1);
closRatioPad = TPad("closRatioPad","closRatioPad",0,0,1,0.3);
closurePad.SetBottomMargin(0);
closRatioPad.SetTopMargin(0);
closRatioPad.SetBottomMargin(.3);
closurePad.Draw();
closRatioPad.Draw();
closurePad.SetLogy();
closurePad.cd();

Zinv.GetYaxis().SetTitle("Events")
Zinv.Draw("hist,e1")
Zestimate.SetMarkerStyle(8)
Zestimate.SetLineColor(1)
Zestimate.SetLineWidth(2)
Zestimate.Draw("e1,SAME")


closRatio = TH1F( Zestimate );
closRatio.Divide( Zestimate , Zinv , 1. , 1. );
closRatioPad.cd();
closRatio.GetYaxis().SetTitle("Est./MC");
closRatio.GetYaxis().SetTitleOffset(0.6);
closRatio.GetYaxis().SetTitleSize(0.1);
closRatio.GetYaxis().SetLabelSize(0.08);
closRatio.GetXaxis().SetTitleOffset(0.9);
closRatio.GetXaxis().SetTitleSize(0.15);
closRatio.GetXaxis().SetLabelSize(0.13);
closRatio.SetMarkerStyle(8);

closRatio.Draw("p,e1");

can4.SaveAs("closureTest.eps")
can4.SaveAs("closureTest.pdf")
can4.SaveAs("closureTest.png")

bin       = ["$i^{th}$ Bin & "]
vZinv  = ["$Z\\to(\\nu\\nu)$ & "]
vGJets = ["$\gamma$+Jets (gen) & "]
vGJets_acc = ["$\gamma$+Jets (Acc.) & "]
vGJets_id  = ["$\gamma$+Jets (ID) & "]
vGJets_iso = ["$\gamma$+Jets (ISO) & "]
veffAcc    = ["$\epsilon_{acc}$ & "]
veffID     = ["$\epsilon_{ID}$ & "]
veffISO    = ["$\epsilon_{iso}$ & "]
vZgamRatio = ["$Z/\gamma$ Ratio & "]
vZestimate = ["Data-driven $Z\\to(\\nu\\nu)$ Est. \\\\"]
vColumnBreak = [" & " ]
vEndLine   = [" \\\\ "]
histoVec = { Zinv : vZinv , 
             GJets : vGJets , 
             #GJets_acc : vGJets_acc , 
             #GJets_id : vGJets_id , 
             GJets_iso : vGJets_iso , 
             effAcc : veffAcc , 
             effID : veffID , 
             effISO : veffISO , 
             ZgamRatio : vZgamRatio , 
             Zestimate : vZestimate ,
             "columnBreak" : vColumnBreak , 
             "endLine" : vEndLine 
             }

for i in range(Zinv.GetNbinsX()):

    bin.append( str( i+1 ) + " & ")

    for h in histoVec :
        if( h == "columnBreak" ):
            histoVec[h].append(" & ")
            continue 

        if( h == "endLine" ):
            histoVec[h].append(" \\\\ ")
            continue 

        if( h.GetBinError(i+1) != 0 ):
            histoVec[h].append( str( round( h.GetBinContent(i+1) , -int(floor(log10(h.GetBinError(i+1))))) ) + "$\pm$"  + str( round( h.GetBinError(i+1) , -int(floor(log10(h.GetBinError(i+1))))+1)) )
        else :
            histoVec[h].append( str( round( h.GetBinContent(i+1) , 0 ) ) + "$\pm$" + str( round( h.GetBinError(i+1) , 0 ) ) )

    #vZinv.append( str(Zinv.GetBinContent(i+1)) + " \pm " + str(Zinv.GetBinContent(i+1)) )
    #vGJets.append( str(GJets.GetBinContent(i+1)) )
    #vGJets_acc.append( str(GJets_acc.GetBinContent(i+1)) )
    #vGJets_id.append( str(GJets_id.GetBinContent(i+1)) )
    #vGJets_iso.append( str(GJets_iso.GetBinContent(i+1)) )
    #veffAcc.append( str(effAcc.GetBinContent(i+1)) )
    #veffID.append( str(effID.GetBinContent(i+1)) )
    #veffISO.append( str(effISO.GetBinContent(i+1)) )
    #vZgamRatio.append( str(ZgamRatio.GetBinContent(i+1)) )
    #vZestimate.append( str(Zestimate.GetBinContent(i+1)) )

from print_table import *

table = [ bin , 
          vColumnBreak , 
          vGJets_iso , 
          vColumnBreak , 
          vZgamRatio , 
          vColumnBreak , 
          vZestimate , 
          vColumnBreak , 
          vZinv , 
          vEndLine 
          ]

table_ratios = [ bin , 
                 vColumnBreak , 
                 veffAcc , 
                 vColumnBreak , 
                 veffID , 
                 vColumnBreak , 
                 veffISO , 
                 vEndLine 
                 ] 

print_table( zip(*table) )

print_table( zip(*table_ratios) )
