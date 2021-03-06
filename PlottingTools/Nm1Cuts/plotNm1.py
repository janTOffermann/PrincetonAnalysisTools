import ROOT
import sys,os
import numpy,array
import math

filename=sys.argv[1]
tfile=ROOT.TFile(filename)

treename="electronTree"
tree=tfile.Get(treename)

outDir="variablePlots_offlineElID_HLTCuts_Veto_Auto"
suffix="eID"

if not os.path.exists(outDir):
    os.makedirs(outDir)

flatTree=True

varsmap={}
def AssignVarsToMap():
    varsmap["pt"]         = tree.pt
    varsmap["eta"]        = tree.etaSC
    varsmap["rho"]        = tree.rho
    varsmap["sieie"]      = tree.full5x5_sigmaIetaIeta
    varsmap["1_e_1_p"]    = tree.ooEmooP
    varsmap["dEta"]       = math.fabs(tree.dEtaIn)
    varsmap["dPhi"]       = math.fabs(tree.dPhiIn)
    varsmap["combiso_pt"] = tree.relIsoWithEA
    varsmap["missingHits"]= tree.expectedMissingInnerHits
    #varsmap["chi2"]       = tree.hltEleChi2
    varsmap["h_e"]        = tree.hOverE
    varsmap["d0"]         = math.fabs(tree.d0)
    varsmap["dz"]         = math.fabs(tree.dz)
    varsmap["type"]       = tree.sampleType

yieldsTextFile=open(outDir+"/yields.txt","w+")


cutsets={}

cutsets["presel"]={}
cutsets["presel"]["sieie"]      =   [0.018,0.040]
cutsets["presel"]["1_e_1_p"]    =   [0.05 ,0.05 ]
cutsets["presel"]["dEta"]       =   [0.020,0.025]
cutsets["presel"]["dPhi"]       =   [0.10 ,0.10 ]
cutsets["presel"]["combiso_pt"] =   [1.0  ,1.0  ]
cutsets["presel"]["missingHits"]=   [10   ,10   ]
#cutsets["presel"]["chi2"]       =   [10   ,10   ]
cutsets["presel"]["h_e"]        =   [0.20 ,0.20 ]
cutsets["presel"]["d0"]         =   [0.3  ,0.4  ]
cutsets["presel"]["dz"]         =   [0.4  ,0.5  ]

cutsets["preselglobal"]={}
cutsets["preselglobal"]["sieie"]      =   [9999,9999]
cutsets["preselglobal"]["1_e_1_p"]    =   [9999,9999]
cutsets["preselglobal"]["dEta"]       =   [9999,9999]
cutsets["preselglobal"]["dPhi"]       =   [9999,9999]
cutsets["preselglobal"]["combiso_pt"] =   [9999,9999]
cutsets["preselglobal"]["missingHits"]=   [9999,9999]
cutsets["preselglobal"]["h_e"]        =   [9999,9999]
cutsets["preselglobal"]["d0"]         =   [9999,9999]
cutsets["preselglobal"]["dz"]         =   [9999,9999]


cutsets["Veto"]={}
cutsets["Veto"]["sieie"]        =   [0.0118,0.033] 
cutsets["Veto"]["1_e_1_p"]      =   [9999 ,9999 ]  
cutsets["Veto"]["dEta"]         =   [0.0155,0.013] 
cutsets["Veto"]["dPhi"]         =   [9999 ,9999 ]  
cutsets["Veto"]["combiso_pt"]   =   [0.20 ,0.20 ]  
cutsets["Veto"]["missingHits"]  =   [2    ,3    ]  
cutsets["Veto"]["h_e"]          =   [9999 ,0.100]  
cutsets["Veto"]["d0"]           =   [0.12 ,0.30 ]  
cutsets["Veto"]["dz"]           =   [9999 ,9999 ]  


cutsets["Loose"]={}
cutsets["Loose"]["sieie"]=[ 0.0106,  0.0305]
cutsets["Loose"]["1_e_1_p"]=[9999,  9999]
cutsets["Loose"]["dEta"]=[ 0.0102,  0.0100]
cutsets["Loose"]["missingHits"]=[ 2.0000,  2.0000]
cutsets["Loose"]["combiso_pt"]=[ 0.1300,  0.1300]
cutsets["Loose"]["h_e"]=[ 0.0992,  0.0803]
cutsets["Loose"]["dz"]=[ 0.0600,  9999]  
cutsets["Loose"]["dPhi"]=[ 0.0880,  9999]
cutsets["Loose"]["d0"]=[ 0.0300,  0.1240]


cutsets["Medium"]={}
cutsets["Medium"]["sieie"]=[ 0.0102,  0.0290]
cutsets["Medium"]["1_e_1_p"]=[ 0.0295,  0.0355]
cutsets["Medium"]["dEta"]=[ 0.0094,  0.0088]
cutsets["Medium"]["missingHits"]=[ 2.0000,  2.0000]
cutsets["Medium"]["combiso_pt"]=[ 0.0900,  0.0900]
cutsets["Medium"]["h_e"]=[ 0.0530,  0.0593]
cutsets["Medium"]["dz"]=[ 0.0320,  0.1750]
cutsets["Medium"]["dPhi"]=[ 0.0310,  0.0510]
cutsets["Medium"]["d0"]=[ 0.0150,  0.0720]


cutsets["Tight"]={}
cutsets["Tight"]["sieie"]       =   [0.0101,0.0285]
cutsets["Tight"]["1_e_1_p"]     =   [0.013,0.017]  
cutsets["Tight"]["dEta"]        =   [0.0065,0.0090]
cutsets["Tight"]["dPhi"]        =   [0.018,0.026]  
cutsets["Tight"]["combiso_pt"]  =   [0.08 ,0.080]  
cutsets["Tight"]["missingHits"] =   [2    ,2    ]  
cutsets["Tight"]["h_e"]         =   [0.040,0.050]  
cutsets["Tight"]["d0"]          =   [0.010,0.020]  
cutsets["Tight"]["dz"]          =   [0.015,0.13 ]  


cutsets["MediumHLT"]={}
cutsets["MediumHLT"]["sieie"]=      [ 0.0103,  0.0315]
cutsets["MediumHLT"]["1_e_1_p"]=    [ 0.0120,  0.010]
cutsets["MediumHLT"]["dEta"]=       [ 0.0130,  0.014]
cutsets["MediumHLT"]["missingHits"]=[ 2.0000,  2.0]
cutsets["MediumHLT"]["combiso_pt"]= [ 0.1000,  0.1900]
cutsets["MediumHLT"]["h_e"]=        [ 0.0600,  0.065]
cutsets["MediumHLT"]["dz"]=         [ 0.0320,  9999.0000]
cutsets["MediumHLT"]["dPhi"]=       [ 0.0360,  0.055]
cutsets["MediumHLT"]["d0"]=         [ 0.0200,  0.1840]

cutsets["TightHLT"]={}
cutsets["TightHLT"]["sieie"]=      [ 0.0101,  0.0288]
cutsets["TightHLT"]["1_e_1_p"]=    [ 0.0120,  0.010]
cutsets["TightHLT"]["dEta"]=       [ 0.0070,  0.008]
cutsets["TightHLT"]["missingHits"]=[ 2.0000,  2.0]
cutsets["TightHLT"]["combiso_pt"]= [ 0.0800,  0.1000]
cutsets["TightHLT"]["h_e"]=        [ 0.0300,  0.050]
cutsets["TightHLT"]["dz"]=         [ 0.0200,  0.10]
cutsets["TightHLT"]["dPhi"]=       [ 0.0200,  0.025]
cutsets["TightHLT"]["d0"]=         [ 0.0100,  0.0400]





nBins=100
varstocut={}  # 0 keep less than, 1 keept greater than
varstocut["sieie"]      =[0,nBins,.004,cutsets["presel"]["sieie"][0]      ,nBins,.015,cutsets["presel"]["sieie"][1]     ]
varstocut["1_e_1_p"]    =[0,nBins,0  ,cutsets["presel"]["1_e_1_p"][0]      ,nBins,0  ,cutsets["presel"]["1_e_1_p"][1]   ]
varstocut["dEta"]       =[0,nBins,0  ,cutsets["presel"]["dEta"][0]         ,nBins,0  ,cutsets["presel"]["dEta"][1]      ]
varstocut["dPhi"]       =[0,nBins,0  ,cutsets["presel"]["dPhi"][0]         ,nBins,0  ,cutsets["presel"]["dPhi"][1]      ]
varstocut["combiso_pt"] =[0,nBins,0  ,cutsets["presel"]["combiso_pt"][0]   ,nBins,0  ,cutsets["presel"]["combiso_pt"][1]]
varstocut["missingHits"]=[0,10,0  ,10                                   ,10,0  ,10                                ]
#varstocut["chi2"]       =[0,nBins,0  ,cutsets["presel"]["chi2"][0]         ,nBins,0  ,cutsets["presel"]["chi2"][1]      ]
varstocut["h_e"]        =[0,nBins,-0.01  ,0.2                              ,nBins,-0.01  ,0.2                           ]
varstocut["d0"]         =[0,nBins,0.0    ,cutsets["presel"]["d0"][0]       ,nBins,0.0    ,cutsets["presel"]["d0"][1]    ]
varstocut["dz"]         =[0,nBins,0.0    ,cutsets["presel"]["dz"][0]       ,nBins,0.0    ,cutsets["presel"]["dz"][1]    ]


nCats=2

def deltaPhi(phi1,phi2):
    dphi=math.fabs(phi1-phi2)
    
    if dphi>math.pi:
        dphi=2*math.pi - dphi

    return dphi

def deltaR(eta1,phi1,eta2,phi2):
    dphi=deltaPhi(phi1,phi2)
    deta=math.fabs(eta1-eta2)
    dR=math.sqrt(dphi*dphi + deta*deta)
    return dR




absCutVars=["dEta","dPhi","d0","dz"]
def passVarCut(var,index,wp):
    if flatTree:
        varValue=varsmap[var]
    else:
        varValue=varsmap[var][index]

    if var in absCutVars:
        varValue=math.fabs(varValue)
 
    catInd=category(nCats,index)
    if varstocut[var][0]==0:
        if varValue > cutsets[wp][var][catInd]:
            return False
    
    if varstocut[var][0]==1:
        if varValue < cutsets[wp][var][catInd]:
            return False
    
    return True


def passNminus1(vartoplot,index,wp):
    for var in varstocut:
        if vartoplot != var:
            if passVarCut(var,index,wp) is False:
                return False

    return True


def passAnyNminus1(index,wp):
    cutsFailed=[]

    for var in varstocut:
        if passVarCut(var,index,wp) is False:
            cutsFailed.append(var)
        if len(cutsFailed)>1:
            break

    return cutsFailed


def category(nCats,index):
    if flatTree:
        eta=math.fabs(varsmap["eta"])
        pt=varsmap["pt"]
    else:
        eta=math.fabs(varsmap["eta"][index])
        pt=varsmap["pt"][index]

    if nCats is 2:
        return int(eta>1.5)

    if nCats is 4:
        return 2*int(eta) + int(pt<50)
    
    print "nCats is ",nCats,"what is happening?  returning -1"
    return -1

catMap={}
catMap[0]="Barrel"
catMap[1]="Endcaps"




hists={}
kinplots={}

tree.SetBranchStatus("*", 1)
      
nev = tree.GetEntries()
print "loop over ",nev

#mctypes =["WenuH","W","DY","TT","QCD"]
#mccolors=[632,633,634,417,402]

mctypes =["DY","TTReal","TTFake","GJet"]
mccolors=[633,634,417,402]
#mctypes =["DY","TTFake"]
#mccolors=[633,417]
bkgType=mctypes[2]
sigType=mctypes[0]


requireHLT=True
etaRestriction=False
matchL1=False
doSoB=True
makeTree=False

eleHoE_PassNm1        = numpy.zeros(1, dtype=int)
b_eleHoE_PassNm1=tree.Branch( 'eleHoE_PassNm1', eleHoE_PassNm1, 'eleHoE_PassNm1/I' )

#wps=["preselglobal","Veto","Loose","Medium"]
wps=["TightHLT"]

potentialCuts={}  #wp,cat,var

numIterations=1


for wp in wps:
    print wp
    for iteration in range(numIterations):
        if iteration==0:
            originalwp=wp
        else:
            wp=wpPlus1
        wpPlus1=originalwp+"iteration"+str(iteration)
        print wp,wpPlus1
        potentialCuts[wpPlus1]={}
        #if makeTree:
        #    newfile = ROOT.TFile("merged_"+wp+"_"+filename.split("/")[-1],"recreate")
        #    newtree = tree.CloneTree(0)
        
        for var in varstocut:
            potentialCuts[wpPlus1][var]={}
            for cat in catMap:
                potentialCuts[wpPlus1][var][catMap[cat]]=[]
        
        for cat in catMap:
            for mctype in mctypes:
                kinplots[mctype+"_eta_"+catMap[cat]+"_"+wp]=ROOT.TH1F(mctype+"_eta_"+catMap[cat]+"_"+wp,";"+mctype+" #eta "+catMap[cat]+"_"+wp,60,-2.8,2.8)
                kinplots[mctype+"_pt_"+catMap[cat]+"_"+wp]=ROOT.TH1F(mctype+"_pt_"+catMap[cat]+"_"+wp,";"+mctype+" P_{T} "+catMap[cat]+"_"+wp,50,10,160)
                kinplots[mctype+"_rho_"+catMap[cat]+"_"+wp]=ROOT.TH1F(mctype+"_rho_"+catMap[cat]+"_"+wp,";"+mctype+" P_{T} "+catMap[cat]+"_"+wp,80,0,60)
        
        for var in varstocut:
            print var
            for cat in catMap:
                for mctype in mctypes:
                    #print "constructing th1f ",mctype+"_"+catMap[cat]+"_"+var+"_nm1_"+wp
                    hists[mctype+"_"+catMap[cat]+"_"+var+"_nm1_"+wp]=ROOT.TH1F(mctype+"_"+catMap[cat]+"_"+var+"_nm1_"+wp,mctype+"_"+catMap[cat]+"_"+var+"_nm1_"+wp,varstocut[var][1+3*cat],varstocut[var][2+3*cat],varstocut[var][3+3*cat])
                    #print hists[mctype+"_"+catMap[cat]+"_"+var+"_nm1_"+wp].Integral()
         
        for iev in range(0,nev):
        #for iev in range(0,300):
            if iev%100000==0:
                print "event",iev
            tree.GetEntry(iev)
            if flatTree:
                AssignVarsToMap()
            #if tree.nhltele == 0:
            #    continue

            hltindex=0
            hltptpassing=0
            

            if varsmap["type"]==10 and tree.isTrueEle==1:
                mctype="DY"
            elif varsmap["type"]==20 and tree.isTrueEle==1:
                mctype="TTReal"
            elif varsmap["type"]==20 and (tree.isTrueEle==0 or tree.isTrueEle==3):
                mctype="TTFake"
            elif varsmap["type"]==30 and (tree.isTrueEle==0 or tree.isTrueEle==3):
                mctype="GJet"
            else:
                continue
                print "sampleIndex",varsmap["type"],"isTrueEle",tree.isTrueEle
                print "I don't want to live in a world of unknowns!!"


            # preselection            
            if varsmap["pt"]<20 or tree.passConversionVeto!=1 or math.fabs(tree.dz)>1.0 or (math.fabs(varsmap["eta"])<1.566 and math.fabs(varsmap["eta"])>1.4442):
                continue
                

            # Important HLT cuts
            if requireHLT:
                if math.fabs(varsmap["eta"])<1.4442:
                    if varsmap["sieie"]>0.011:
                        continue
                    if varsmap["1_e_1_p"]>0.012:
                        continue
                    if varsmap["h_e"]>0.06:
                        continue
                if math.fabs(varsmap["eta"])>1.566:
                    if varsmap["sieie"]>0.032:
                        continue
                    if varsmap["1_e_1_p"]>0.010:
                        continue
                    if varsmap["h_e"]>0.065:
                        continue
                    if varsmap["missingHits"]>2:
                        continue
                
            cutsFailed=passAnyNminus1(-1,wp)
           
            # changed to 2 so I can see which cuts fail both iso cuts 
            if len(cutsFailed)>2:
                if makeTree:
                    b_eleHoE_PassNm1.Fill()
                continue
           
            #if hltindex >= tree.nhltele:
            #    print hltindex,tree.nhltele
            #    continue
 
            if makeTree:
                if len(cutsFailed)==0:
                    tree.eleHoE_PassNm1[hltindex]=1
                        
                elif len(cutsFailed)==1:
                    if cutsFailed[0]=="h_e":
                        tree.eleHoE_PassNm1[hltindex]=1
                    #elif cutsFailed[0]=="hcaliso_pt":
                    #    tree.hltEleHCalIsoOPt_PassNm1[hltindex]=1
                    #    tree.hltEleBothIsoOPt_PassNm2[hltindex]=1
                    #elif cutsFailed[0]=="ecaliso_pt":
                    #    tree.hltEleECalIsoOPt_PassNm1[iel]=1
                    #    tree.hltEleBothIsoOPt_PassNm2[hltindex]=1
                
                elif len(cutsFailed)==2:
                    #if (cutsFailed[0]=="hcaliso_pt" and cutsFailed[1]=="ecaliso_pt") or (cutsFailed[1]=="hcaliso_pt" and cutsFailed[0]=="ecaliso_pt"):
                    #    tree.hltEleBothIsoOPt_PassNm2[hltindex]=1
                    #else:
                    continue         
                b_eleHoE_PassNm1.Fill()
                
            if len(cutsFailed)==0:
                if flatTree:
                    for var in varstocut:
                        hists[mctype+"_"+catMap[category(nCats,hltindex)]+"_"+var+"_nm1_"+wp].Fill(varsmap[var])
                    kinplots[mctype+"_eta_"+catMap[category(nCats,hltindex)]+"_"+wp].Fill(varsmap['eta'])
                    kinplots[mctype+"_pt_"+catMap[category(nCats,hltindex)]+"_"+wp].Fill(varsmap['pt'])
                    kinplots[mctype+"_rho_"+catMap[category(nCats,hltindex)]+"_"+wp].Fill(varsmap['rho'])
                else:
                    for var in varstocut:
                        hists[mctype+"_"+catMap[category(nCats,hltindex)]+"_"+var+"_nm1_"+wp].Fill(varsmap[var][hltindex],tree.weight)
                    kinplots[mctype+"_eta_"+catMap[category(nCats,hltindex)]+"_"+wp].Fill(tree.hltEleEta[hltindex],tree.weight)
                    kinplots[mctype+"_pt_"+catMap[category(nCats,hltindex)]+"_"+wp].Fill(tree.hltElePt[hltindex],tree.weight)
                    kinplots[mctype+"_rho_"+catMap[category(nCats,hltindex)]+"_"+wp].Fill(tree.hltElePt[hltindex],tree.weight)
        
            if len(cutsFailed)==1:
                if flatTree:
                    hists[mctype+"_"+catMap[category(nCats,hltindex)]+"_"+cutsFailed[0]+"_nm1_"+wp].Fill(varsmap[cutsFailed[0]])
                else:
                    hists[mctype+"_"+catMap[category(nCats,hltindex)]+"_"+cutsFailed[0]+"_nm1_"+wp].Fill(varsmap[cutsFailed[0]][hltindex],tree.weight)
                    
            #if makeTree:
            #    npass=0
            #    for iel in range(0,tree.nhltele):
            #        if newtree.hltEle_Pass[iel]==1:
            #            npass=npass+1
            #            if npass>1:
            #                print "WTF mate 3"
            #    newtree.Fill()
                    
                    
        
        
        for var in varstocut:
            for icat in catMap:
                can=ROOT.TCanvas(catMap[icat]+"_"+var+"_nm1_"+wp,"",700,500)
                if doSoB:
                    hists["sob_"+catMap[icat]+"_"+var+"_nm1_"+wp]=hists[sigType+"_"+catMap[icat]+"_"+var+"_nm1_"+wp].Clone()
                    bkgtemp=hists[bkgType+"_"+catMap[icat]+"_"+var+"_nm1_"+wp].Clone()
                    bkgtemp.Smooth(10)
                    hists["sob_"+catMap[icat]+"_"+var+"_nm1_"+wp].Divide(bkgtemp)
               
                itype=0 
                maxval=0
                for mctype in mctypes:
                    if hists[mctype+"_"+catMap[icat]+"_"+var+"_nm1_"+wp].Integral() > 0:
                        hists[mctype+"_"+catMap[icat]+"_"+var+"_nm1_"+wp].Scale(1/ hists[mctype+"_"+catMap[icat]+"_"+var+"_nm1_"+wp].Integral())
                    hists[mctype+"_"+catMap[icat]+"_"+var+"_nm1_"+wp].SetLineWidth(2)
                    hists[mctype+"_"+catMap[icat]+"_"+var+"_nm1_"+wp].SetLineColor(mccolors[itype])
                    itype=itype+1
                    maxval=max(maxval,hists[mctype+"_"+catMap[icat]+"_"+var+"_nm1_"+wp].GetMaximum())
        
                maxval=maxval*1.2
        
                first=True 
                leg=ROOT.TLegend(0.7,0.6,0.9,0.9)
                leg.SetHeader(catMap[icat])
                for mctype in mctypes:
                    if first is True:    
                        hists[mctype+"_"+catMap[icat]+"_"+var+"_nm1_"+wp].SetMaximum(maxval)
                        hists[mctype+"_"+catMap[icat]+"_"+var+"_nm1_"+wp].SetTitle(";"+var+" "+catMap[icat]+" "+wp)
                        hists[mctype+"_"+catMap[icat]+"_"+var+"_nm1_"+wp].Draw()
                        first=False
                    else:
                        hists[mctype+"_"+catMap[icat]+"_"+var+"_nm1_"+wp].Draw("sames")
                    leg.AddEntry(hists[mctype+"_"+catMap[icat]+"_"+var+"_nm1_"+wp],mctype,"l")
   
                leg.Draw("same") 
                line=ROOT.TLine(cutsets[wp][var][icat],0,cutsets[wp][var][icat], maxval/1.3)
                line.SetLineColor(1)    
                line.SetLineWidth(2)
                line.Draw("same")
         
                can.Update() 
                can.SaveAs(outDir+"/s_b_"+catMap[icat]+"_"+var+"_"+wp+"_"+suffix+".png")
                can.SaveAs(outDir+"/s_b_"+catMap[icat]+"_"+var+"_"+wp+"_"+suffix+".C")
                can.Write()
                can.Clear()
        
                
                if doSoB:
                    hists["sob_"+catMap[icat]+"_"+var+"_nm1_"+wp].SetLineWidth(2)
                    hists["sob_"+catMap[icat]+"_"+var+"_nm1_"+wp].Draw()
                    hists["sob_"+catMap[icat]+"_"+var+"_nm1_"+wp].SetMinimum(0.1)
                    temp=hists["sob_"+catMap[icat]+"_"+var+"_nm1_"+wp].Clone()
                    line=ROOT.TLine(cutsets[wp][var][icat],0,cutsets[wp][var][icat], hists["sob_"+catMap[icat]+"_"+var+"_nm1_"+wp].GetMaximum()/1.3)
                    line.SetLineColor(1)    
                    line.SetLineWidth(2)
                    line.Draw("same")
        
                    hLineVal=[800,200]
                    #[300,15]
                    #[7.5,1.7]
                    #[900,400] 
                    #[250,145]
                    #[50,35]
                    #[7.5,7.5]
                    line2=ROOT.TLine(temp.GetBinLowEdge(1),hLineVal[icat],temp.GetBinLowEdge(temp.GetNbinsX()+1),hLineVal[icat])
                    line2.SetLineColor(633)    
                    line2.SetLineWidth(2)
                    line2.Draw("same")
                    
                    can.SetLogy(1)
                    
                    can.Update() 
                    can.SaveAs(outDir+"/soverb_"+catMap[icat]+"_"+var+"_"+wp+"_"+suffix+".png")
                    can.SaveAs(outDir+"/soverb_"+catMap[icat]+"_"+var+"_"+wp+"_"+suffix+".C")
                    for iBin in range(1,hists["sob_"+catMap[icat]+"_"+var+"_nm1_"+wp].GetNbinsX()):
                        lowEdge=hists["sob_"+catMap[icat]+"_"+var+"_nm1_"+wp].GetBinLowEdge(iBin)
                        binVal=hists["sob_"+catMap[icat]+"_"+var+"_nm1_"+wp].GetBinContent(iBin)
                        if var=="dEta" and lowEdge<0.004:
                            continue
                        if var=="dPhi" and lowEdge<0.004:
                            continue
                        if var=="d0" and lowEdge<0.01:
                            continue
                        if var=="dz" and lowEdge<0.01:
                            continue
                        if var=="1_e_1_p" and lowEdge<0.008:
                            continue
                        if var=="h_e" and lowEdge<0.018:
                            continue
                        if var=="sieie":
                            if icat==0 and lowEdge<0.009:
                                continue
                            if icat==1 and lowEdge<0.027:
                                continue
                        if binVal < hLineVal[icat] and binVal>0.01:
                            potentialCuts[wpPlus1][var][catMap[icat]].append(lowEdge)
                            
        can.SetLogy(0)

#        for wp in wps:
        if doSoB:
            for icat in catMap:
                for var in varstocut:
                    print wp,catMap[icat],var,
                    cutbin=int((cutsets[wp][var][icat]-hists[sigType+"_"+catMap[icat]+"_"+var+"_nm1_"+wp].GetBinLowEdge(1))/hists[sigType+"_"+catMap[icat]+"_"+var+"_nm1_"+wp].GetBinWidth(1))
                    finalbin=hists[sigType+"_"+catMap[icat]+"_"+var+"_nm1_"+wp].GetNbinsX()
        
                    print hists[sigType+"_"+catMap[icat]+"_"+var+"_nm1_"+wp].Integral(cutbin,finalbin),
                    print hists[bkgType+"_"+catMap[icat]+"_"+var+"_nm1_"+wp].Integral(cutbin,finalbin)
        
        for mctype in mctypes:
            print mctype,
        print
#for     wp in wps:
        for cat in catMap:
            yieldsTextFile.write(wp+"\t"+catMap[cat]+"\n")
            for mctype in mctypes:
                print kinplots[mctype+"_eta_"+catMap[cat]+"_"+wp].Integral(),

            
            for ikin in ["pt","eta"]:
                if doSoB:
                    kinplots["soverb_"+ikin+"_"+catMap[cat]+"_"+wp]=kinplots[sigType+"_"+ikin+"_"+catMap[cat]+"_"+wp].Clone()
                    kinplots["soverb_"+ikin+"_"+catMap[cat]+"_"+wp].Divide(kinplots[bkgType+"_"+ikin+"_"+catMap[cat]+"_"+wp])
                    kinplots["soverb_"+ikin+"_"+catMap[cat]+"_"+wp].SetMinimum(0.01)
                
                itype=0 
                maxval=0
                leg=ROOT.TLegend(0.7,0.6,0.9,0.9)
                leg.SetHeader(catMap[cat]+" "+wp)
                for mctype in mctypes:
                    if kinplots[mctype+"_"+ikin+"_"+catMap[cat]+"_"+wp].Integral()>0:
                        if ikin=="eta":
                            yieldsTextFile.write(mctype+"\t"+str(kinplots[mctype+"_"+ikin+"_"+catMap[cat]+"_"+wp].Integral())+"\n")

                        kinplots[mctype+"_"+ikin+"_"+catMap[cat]+"_"+wp].Scale(1/kinplots[mctype+"_"+ikin+"_"+catMap[cat]+"_"+wp].Integral())
                    kinplots[mctype+"_"+ikin+"_"+catMap[cat]+"_"+wp].SetLineWidth(2)
                    kinplots[mctype+"_"+ikin+"_"+catMap[cat]+"_"+wp].SetLineColor(mccolors[itype])
                    itype=itype+1
                    maxval=max(maxval,kinplots[mctype+"_"+ikin+"_"+catMap[cat]+"_"+wp].GetMaximum())
                    leg.AddEntry(kinplots[mctype+"_"+ikin+"_"+catMap[cat]+"_"+wp],mctype,"l")
   
                leg.Draw("same") 
                line=ROOT.TLine(cutsets[wp][var][cat],0,cutsets[wp][var][cat], maxval/1.3)
        
                maxval=maxval*1.2
        
                first=True 
                for mctype in mctypes:

                    if first is True:    
                        kinplots[mctype+"_"+ikin+"_"+catMap[cat]+"_"+wp].SetMaximum(maxval)
                        kinplots[mctype+"_"+ikin+"_"+catMap[cat]+"_"+wp].Draw()
                        first=False
                    else:
                        kinplots[mctype+"_"+ikin+"_"+catMap[cat]+"_"+wp].Draw("sames")
        

                can.Update()
                can.SaveAs(outDir+"/s_b_"+ikin+"_"+catMap[cat]+"_"+wp+"_"+suffix+".png")
                can.SaveAs(outDir+"/s_b_"+ikin+"_"+catMap[cat]+"_"+wp+"_"+suffix+".C")
                can.Clear()

                if doSoB:
                    kinplots["soverb_"+ikin+"_"+catMap[cat]+"_"+wp].SetLineWidth(2)
                    kinplots["soverb_"+ikin+"_"+catMap[cat]+"_"+wp].Draw()
                    can.Update()
                    can.SaveAs(outDir+"/soverb_"+ikin+"_"+catMap[cat]+"_"+wp+"_"+suffix+".png")
                    can.SaveAs(outDir+"/soverb_"+ikin+"_"+catMap[cat]+"_"+wp+"_"+suffix+".C")
                    can.Clear()
            yieldsTextFile.write("\n")
        if makeTree:
            newfile.Write()
            newfile.Close()

        
        cutsets[wpPlus1]={}
        for var in potentialCuts[wpPlus1]:
            iCat=0
            cutsets[wpPlus1][var]=[9999,9999]
            for cat in potentialCuts[wpPlus1][var]:
                iShow=0
                for i in potentialCuts[wpPlus1][var][cat]:
                    cutsets[wpPlus1][var][iCat]=i
                    iShow=iShow+1
                    if iShow>0:
                        break
                iCat=iCat+1

yieldsTextFile.close()
wps=potentialCuts.keys()
wps.sort()

for wp in potentialCuts:
    print wp
    for var in potentialCuts[wp]:
        print var,
        for cat in potentialCuts[wp][var]:
            iShow=0
            print cat,   
            for i in potentialCuts[wp][var][cat]:
                print "{0:.4f}".format(i),
                iShow=iShow+1
                if iShow>6:
                    break
            print
    print

for wp in potentialCuts:
    print "cutsets[\""+wp+"\"]={}"
    for var in potentialCuts[wp]:
        iCat=0
        for cat in potentialCuts[wp][var]:
            iShow=0
            if iCat==0:
                print "cutsets[\""+wp+"\"][\""+var+"\"]=[",
            val=9999
            for i in potentialCuts[wp][var][cat]:
                val=i
                iShow=iShow+1
                if iShow>0:
                    break
            
            if iCat==0:
                print "{0:.4f},".format(val)+" ",
            elif iCat==1:
                print "{0:.4f}]".format(val)
            iCat=iCat+1
    print


outfile=ROOT.TFile("cutsets.root","recreate")
cuttree=ROOT.TTree("cuttree","each entry is a set of cuts")
setname = ROOT.std.string()


nCat=numpy.zeros(1, dtype=int)
cutsetvars={}
cuttree.Branch("nCat",nCat,"nCat/I") 
cuttree.Branch('cutsetname',setname)


for var in varstocut:
    cutsetvars[var]=numpy.zeros(10, dtype=float)
    cuttree.Branch(var,cutsetvars[var],var+"[nCat]/D")


for cutset in cutsets:
    setname.replace(0, ROOT.std.string.npos, cutset)
    for var in cutsets[cutset]:
        nCat[0]=int(len(cutsets[cutset][var]))
        #print "nCat",nCat
        count=0
        for cut in cutsets[cutset][var]:
            #print cutset,var,cut
            cutsetvars[var][count]=cut
            count=count+1
    cuttree.Fill()

cuttree.Write()
outfile.Save()
outfile.Close()


