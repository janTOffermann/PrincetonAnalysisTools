from ROOT import *
import sys
from math import sqrt,pow

#ch = sys.argv[1]
ifilename = sys.argv[1]
ifile = TFile(ifilename)
#ifilename = ifilename[ifilename.find("hist"):]
channel = ifilename[ifilename.find("hist"):].replace("hists_","").replace(".root","")

hs = ifile.Get("BDT_%s_WH" % channel)
hbkg = ifile.Get("BDT_%s_Bkg" % channel)

#ifile = TFile("mlfit.root","r")
#hs = ifile.Get("shapes_prefit/%s/total_signal" % ch) 
#hbkg = ifile.Get("shapes_fit_s/%s/total_background" % ch)


nbins = hs.GetNbinsX()

sb = 0.
print channel
#print ch
for i in range(1,nbins+1):
    s = hs.GetBinContent(i)
    b = hbkg.GetBinContent(i)
    if (b > 0. and s > 0.001):
        bin_sb = s/sqrt(b)
        #bin_sb = sqrt(2*((s+b)*log(1+(s/b)) - s ))
    else: bin_sb = 0.
    print "Bin %i: %f %f %f" % (i,s,b,bin_sb)
    sb += pow(bin_sb,2)
sb = sqrt(sb)
print sb
sb = pow(sb,2)

#ifilename2 = ifilename.replace("Wmn","Wen")
#channel2 = channel.replace("Wmn","Wen")
#ifile2 = TFile(ifilename2)

#hs2 = ifile2.Get("BDT_%s_WH" % channel2)
#hbkg2 = ifile2.Get("BDT_%s_Bkg" % channel2)

#print channel2
#for i in range(1,nbins+1):
#    s = hs2.GetBinContent(i)
#    b = hbkg2.GetBinContent(i)
#    if (b > 0. and s > 0.):
#        #bin_sb = s/sqrt(b)
#        bin_sb = sqrt(2*((s+b)*log(1+(s/b)) - s ))
#    else: bin_sb = 0.
#    print "Bin %i: %f %f %f" % (i,s,b,bin_sb)
#    sb += pow(bin_sb,2)
#sb = sqrt(sb)
#print sb
