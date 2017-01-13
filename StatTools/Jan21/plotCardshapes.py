import ROOT
import sys

ROOT.gStyle.SetOptStat(0)

#SF_TT = 0.99
#SF_Wj0b = 1.17
#SF_Wj1b = 1.65
#SF_Wj2b = 1.65
#SF_Wj1b = 2.38
#SF_Wj2b = 1.07

SF_TT = 1.0
SF_Wj0b = 1.0
SF_Wj1b = 1.0
SF_Wj2b = 1.0

channel = sys.argv[1]
ifile = ROOT.TFile("hists_%s.root" % channel)
data = ifile.Get("BDT_%s_data_obs" % channel)
tt = ifile.Get("BDT_%s_TT" % channel)
stop = ifile.Get("BDT_%s_s_Top" % channel)
wj0b = ifile.Get("BDT_%s_Wj0b" % channel)
wj1b = ifile.Get("BDT_%s_Wj1b" % channel)
wj2b = ifile.Get("BDT_%s_Wj2b" % channel)
wh = ifile.Get("BDT_%s_WH" % channel)
qcd = ifile.Get("BDT_%s_QCD" % channel)
bkg = ifile.Get("BDT_%s_Bkg" % channel)
vvhf = ifile.Get("BDT_%s_VVHF" % channel)
vvlf = ifile.Get("BDT_%s_VVLF" % channel)

stop.SetLineColor(432)
tt.SetLineColor(600)
wj0b.SetLineColor(418)
wj1b.SetLineColor(410)
wj2b.SetLineColor(416)
wh.SetLineColor(808)
qcd.SetLineColor(616)
vvhf.SetLineColor(922)
vvlf.SetLineColor(921)

#vvhf.SetLineWidth(3)
#vvlf.SetLineWidth(3)
wh.SetLineWidth(3)

#tt.Scale(SF_TT)
#wj0b.Scale(SF_Wj0b)
#wj1b.Scale(SF_Wj1b)
#wj2b.Scale(SF_Wj2b)
wh.Scale(30.0)

stack = ROOT.THStack("stack","stack")
stack.Add(stop)
stack.Add(wj0b)
stack.Add(wj1b)
stack.Add(wj2b)
stack.Add(tt)
stack.Add(vvlf)
stack.Add(vvhf)
#stack.Add(qcd)

#vvlf.Scale(10.)
#vvhf.Scale(10.)
stackvv = ROOT.THStack("stackvv","stackvv")
stackvv.Add(vvlf)
stackvv.Add(vvhf)
#bkg.SetMaximum(1.3*max(bkg.GetMaximum(),wh.GetMaximum()))
#bkg.SetMinimum(0.)
#stack.SetMaximum(1.3*stack.GetMaximum())
#stack.SetMinimum(0.)

#bkg.SetTitle(channel)
#bkg.GetXaxis().SetTitle("Mjj [GeV]")

canv = ROOT.TCanvas("canv","canv")
stack.Draw("hist")
#print "Chi2/NDOF = ",data.Chi2Test( stack.GetHistogram() , "UWCHI2/NDF")
#print "Total bkg: ",stack.GetHistogram().Integral()
stack.Draw("ep same")
stack.SetTitle(channel)
#stack.GetXaxis().SetTitle("Mjj [GeV]")
#data.Draw("ep same")
#print "Total data: ",data.Integral()
#bkg.Draw("hist")
#bkg.Draw("ep same")
#stackvv.Draw("hist same")
#stackvv.Draw("ep same")
wh.Draw("hist same")
wh.Draw("ep same")

leg = ROOT.TLegend(0.7,0.7,0.9,0.9)
leg.AddEntry(stop,"Single Top")
leg.AddEntry(wj0b,"Wj0b")
leg.AddEntry(wj1b,"Wj1b")
leg.AddEntry(wj2b,"Wj2b")
leg.AddEntry(tt,"TT")
leg.AddEntry(vvlf,"VVLF")
leg.AddEntry(vvhf,"VVHF")
#leg.AddEntry(vvlf,"VVLF X 10")
#leg.AddEntry(vvhf,"VVHF X 10")
leg.AddEntry(wh,"WH X 30")
#leg.AddEntry(data,"Data")
#leg.AddEntry(bkg,"Bkg")
leg.Draw("same")

#print "Data yield: ",data.Integral()
#print "MC yield: ",stack.GetHistogram().Integral()

#raw_input()
canv.SetLogy(True)
canv.SaveAs("%s_prefit.pdf" % channel)
canv.SaveAs("%s_prefit.png" % channel)
canv.SaveAs("%s_prefit.C" % channel)
ifile.Close()

