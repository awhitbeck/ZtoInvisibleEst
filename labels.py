from ROOT import TPaveText

CMSlabel = TPaveText(.14,.98,.5,.95,"NDC")
SqrtSlumi = TPaveText(.67,.98,.97,.95,"NDC")

def setLumi( lumi = 10. ):
    SqrtSlumi.Clear()
    if lumi < 0. :
        SqrtSlumi.AddText("#sqrt{s}=13 TeV")
    else : 
        SqrtSlumi.AddText("L={0}".format(lumi)+" fb^{-1}, #sqrt{s}=13 TeV")

def drawLabels():
    CMSlabel.Draw()
    SqrtSlumi.Draw()

CMSlabel.AddText("CMS #it{Preliminary}")
CMSlabel.SetTextAlign(11)
CMSlabel.SetFillColor(0)
CMSlabel.SetBorderSize(0)


setLumi()
SqrtSlumi.SetTextAlign(31)
SqrtSlumi.SetFillColor(0)
SqrtSlumi.SetBorderSize(0)



