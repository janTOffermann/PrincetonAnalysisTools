//
//  Written by Chris Palmer 
//
//  VHbb analysis
//  Inheriting from Analysis Manager
//

#include "VHbbAnalysis.h"


// initialize parameters
VHbbAnalysis::VHbbAnalysis(){
    if(debug>10) std::cout<<"Constructing VHbbAnalysis"<<std::endl;
}

// remove any 'new'
VHbbAnalysis::~VHbbAnalysis(){
    if(debug>10) std::cout<<"Deconstructing VHbbAnalysis"<<std::endl;
}

//probably want to do bdtSetup here
void VHbbAnalysis::InitAnalysis(){
    //SetupBDT();a//FIXME need to update BDT
    return;
}

//get a few entries and then preselect
//if sel, then analyzeevent
//default to false in the future
bool VHbbAnalysis::Preselection(){
    //return true; // for the moment don't impose any preselection
    bool sel=true;
    //if( *d["Vtype"]==3 ) sel=true;
    //if( *d["Vtype"]>=0 && *d["Vtype"]<=4) sel=true;
    // Preselect for two jets and one lepton which pass some minimum pt threshold
    int nPreselJets = 0;
    for (int i=0; i < *in["nJet"]; i++) {
        if (d["Jet_pt"][i] > *f["JetPtPresel"]) nPreselJets++;
    }
    int nPreselLep = 0;
    for (int i=0; i < *in["nselLeptons"]; i++) {
        if (d["selLeptons_pt"][i] > *f["LepPtPresel"]) nPreselLep++;
    }
    //for (int i=0; i < *in["naLeptons"]; i++) {
    //    if (d["aLeptons_pt"][i] > *f["LepPtPresel"]) nPreselLep++;
    //}
    if (nPreselJets < 2 || nPreselLep < 1) sel = false;
    return sel;
}

bool VHbbAnalysis::Analyze(){
    bool sel=true;
    bool doCutFlow = bool(*f["doCutFlow"]);
    *in["cutFlow"] = 0;

    if(debug>1000 && doCutFlow) {
        std::cout<<"Running cutflow"<<std::endl;
    }

    if(debug>1000) {
        std::cout<<"cutting on V and H pt"<<std::endl;
    }

    // We should really make this cut only after calculating H_pt ourselves
    //if(*d["V_pt"] < *f["vptcut"] || *d["H_pt"] < *f["hptcut"]) sel = false;
    //if (sel) *in["cutFlow"] += 1;

    if(debug>1000) {
        std::cout<<"selecting bjets"<<std::endl;
    }
    //std::pair<int,int> bjets=HighestPtBJets();
    std::pair<int,int> bjets=HighestCSVBJets();
 
    if (bjets.first == -1) {
        sel = false;
        *in["hJetInd1"] = 0;
    }
    else *in["hJetInd1"] = bjets.first;
    if (bjets.second == -1) {
         sel = false;
        *in["hJetInd2"] = 1;
    }
    else *in["hJetInd2"] = bjets.second;

   /* *in["nHJetsMatched"] = 0;
    if(d["hJets_pt"][0] == d["Jet_pt"][*in["hJetInd1"]]) *in["nHJetsMatched"] += 1;
    if(d["hJets_pt"][1] == d["Jet_pt"][*in["hJetInd2"]]) *in["nHJetsMatched"] += 1; 
    
    if(debug>10000) {
        if(*in["nHJetsMatched"]<2) {
            std::cout<<"nHJetsMatched = "<<*in["nHJetsMatched"]<<std::endl;
            std::cout<<"hJets_pt[0] = "<<d["hJets_pt"][0]<<std::endl;
            std::cout<<"hJets_pt[1] = "<<d["hJets_pt"][1]<<std::endl;
            std::cout<<"lead jet1 pt = "<<d["Jet_pt"][*in["hJetInd1"]]<<std::endl;
            std::cout<<"sublead jet2 pt = "<<d["Jet_pt"][*in["hJetInd2"]]<<std::endl;
        }
    }*/

    /*// For now use higgs bjet selection from step 2 ntuples
    if(d["hJets_btagCSV"][0] < *f["j1ptCSV"] || d["hJets_btagCSV"][1] < *f["j2ptCSV"]) return false;
    if(d["hJets_pt"][0] < *f["j1ptCut"] || d["hJets_pt"][1] < *f["j2ptCut"]) return false;
    */

    /*// Cut on the bjets that we select
    if(d["Jet_btagCSV"][*in["hJetInd1"]] < *f["j1ptCSV"] || d["Jet_btagCSV"][*in["hJetInd2"]] < *f["j2ptCSV"]) sel = false;
    if(d["Jet_pt"][*in["hJetInd1"]] < *f["j1ptCut"] || d["Jet_pt"][*in["hJetInd2"]] < *f["j2ptCut"]) sel = false; 
    */
    if (sel) *in["cutFlow"] += 1; // selected jets

    if(debug>1000) {
        std::cout<<"nJet = "<<*in["nJet"]<<std::endl;
        std::cout<<"hJetInd1 = "<<*in["hJetInd1"]<<std::endl;
        std::cout<<"hJetInd2 = "<<*in["hJetInd2"]<<std::endl;
        std::cout<<"found two bjets with pt and CSV "
            <<d["Jet_pt"][*in["hJetInd1"]]<<" "
            <<d["Jet_btagCSV"][*in["hJetInd1"]]<<" "
            <<d["Jet_pt"][*in["hJetInd2"]]<<" "
            <<d["Jet_btagCSV"][*in["hJetInd2"]]<<" "
            <<std::endl;
    }
    
    // Reconstruct Higgs
    TLorentzVector HJ1,HJ2,Hbb;
    HJ1.SetPtEtaPhiM(d["Jet_pt"][*in["hJetInd1"]], d["Jet_eta"][*in["hJetInd1"]], d["Jet_phi"][*in["hJetInd1"]], d["Jet_mass"][*in["hJetInd1"]]);
    HJ2.SetPtEtaPhiM(d["Jet_pt"][*in["hJetInd2"]], d["Jet_eta"][*in["hJetInd2"]], d["Jet_phi"][*in["hJetInd2"]], d["Jet_mass"][*in["hJetInd2"]]);
    Hbb = HJ1 + HJ2;
    *d["H_pt"] = Hbb.Pt();
    if (*d["H_pt"] < *f["hptcut"]) sel=false;
    if (sel) *in["cutFlow"] += 1; // pT(jj) cut
    
    if (*d["met_pt"] < *f["metcut"]) sel = false;
    if (sel) *in["cutFlow"] += 1; // met cut
    
    //check if event passes any event class
    *in["lepInd"] = -1;
    *in["isWmunu"] = 0;
    *in["isWenu"] = 0;
    if(WmunuHbbSelection()) {
        *in["isWmunu"] = 1;
        *in["eventClass"]=0;
        *in["lepInd"] = *in["muInd"];
    } 
    if(WenuHbbSelection()) {
        *in["isWenu"] = 1;
        if (!(*in["isWmunu"])) {
            *in["eventClass"]=1000;
            *in["lepInd"] = *in["elInd"];
        }
    }
    if (*in["isWmunu"] == 0 && *in["isWenu"] == 0) sel = false;
    if (sel) *in["cutFlow"] += 1; // lepton selection
    
    if (*in["lepInd"] == -1) {
        // not Wenu or Wmunu, use preselected lepton
        *in["lepInd"] = 0;
    }

    if(debug>1000) {
        std::cout<<"cutting on dphi(lep, met)"<<std::endl;
    }
    *d["lepMetDPhi"]=fabs(EvalDeltaPhi(d["selLeptons_phi"][*in["lepInd"]],*d["met_phi"]));
    if (*in["isWmunu"] && *d["lepMetDPhi"] > *f["muMetDPhiCut"]) sel = false;
    else if (*in["isWenu"] && *d["lepMetDPhi"] > *f["elMetDPhiCut"]) sel = false;
    if (sel) *in["cutFlow"] += 1;   


    TLorentzVector W,Lep, MET, GenBJ1, GenBJ2, GenBJJ;
    // Reconstruct W
    MET.SetPtEtaPhiM(*d["met_pt"], 0., *d["met_phi"], 0.); // Eta/M don't affect calculation of W.pt and W.phi
    Lep.SetPtEtaPhiM(d["selLeptons_pt"][*in["lepInd"]], d["selLeptons_eta"][*in["lepInd"]], d["selLeptons_phi"][*in["lepInd"]], d["selLeptons_mass"][*in["lepInd"]]); 
    W = MET + Lep; 
    *d["V_pt"] = W.Pt(); // uncomment this line if we want to recalculate W.pt ourselves


    *f["Lep_HJ1_dPhi"] = Lep.DeltaPhi(HJ1);
    *f["Lep_HJ2_dPhi"] = Lep.DeltaPhi(HJ2);
    *f["HJ1_HJ2_dPhi"] = HJ1.DeltaPhi(HJ2);

    /*// Find the closest lepton to each of the selected jets and try to reconstruct the tops from ttbar
    double minDRHJ1 = 999;
    double minDRHJ2 = 999;
    double minSecondDRHJ2 = 999;
    int HJ1ClosestLeptonIndex = -1;
    int HJ2ClosestLeptonIndex = -1;
    int HJ2SecondClosestLeptonIndex = -1; // keep track of this in case the same lepton is closest to both jets

    // look in aleptons too FIXME
    for (int i=0; i<*in["nselLeptons"]; i++) {
        TLorentzVector l;
        l.SetPtEtaPhiM(d["selLeptons_pt"][i], d["selLeptons_eta"][i], d["selLeptons_phi"][i], d["selLeptons_mass"][i] );
        double d1 = l.DeltaR(HJ1);
        double d2 = l.DeltaR(HJ2);
        if (d1 <= minDRHJ1) {
            minDRHJ1 = d1;
            HJ1ClosestLeptonIndex = i;
        }
        if (d2 <= minDRHJ2) {
            minSecondDRHJ2 = minDRHJ2;
            HJ2SecondClosestLeptonIndex = HJ2ClosestLeptonIndex;
            minDRHJ2 = d2;
            HJ2ClosestLeptonIndex = i;
        }
        else if (d2 <= minSecondDRHJ2) {
            minSecondDRHJ2 = d2;
            HJ2SecondClosestLeptonIndex = i;
        }
    }
    if (HJ1ClosestLeptonIndex == HJ2ClosestLeptonIndex) {
        // oops, the same lepton is closest to both jets
        HJ2ClosestLeptonIndex = HJ2SecondClosestLeptonIndex;
    }
    if (HJ1ClosestLeptonIndex!=-1) {
        TLorentzVector Lep1, Top1;
        Lep1.SetPtEtaPhiM(d["selLeptons_pt"][HJ1ClosestLeptonIndex], d["selLeptons_eta"][HJ1ClosestLeptonIndex], d["selLeptons_phi"][HJ1ClosestLeptonIndex], d["selLeptons_mass"][HJ1ClosestLeptonIndex]);
        Top1 = Lep1 + HJ1;
        *f["Top1_mass"] = Top1.M();
    }
    else {
        *f["Top1_mass"] = -999;
    }
    if (HJ2ClosestLeptonIndex!=-1) {
        TLorentzVector Lep2, Top2;
        Lep2.SetPtEtaPhiM(d["selLeptons_pt"][HJ2ClosestLeptonIndex], d["selLeptons_eta"][HJ2ClosestLeptonIndex], d["selLeptons_phi"][HJ2ClosestLeptonIndex], d["selLeptons_mass"][HJ2ClosestLeptonIndex]); 
        Top2 = Lep2 + HJ2;
        *f["Top2_mass"] = Top2.M();
    }
    else {
        *f["Top2_mass"] = -999;
    }*/
    TLorentzVector Jet_bestCSV;
    int Jet_bestCSV_index = -1;
    Jet_bestCSV_index = HighestCSVBJets().first;
    if (Jet_bestCSV_index != -1) {
        Jet_bestCSV.SetPtEtaPhiM(d["Jet_pt"][Jet_bestCSV_index],d["Jet_eta"][Jet_bestCSV_index],d["Jet_phi"][Jet_bestCSV_index],d["Jet_mass"][Jet_bestCSV_index]);
        *f["Top1_mass_bestCSV"] = GetRecoTopMass(Jet_bestCSV);
    }
    else *f["Top1_mass_bestCSV"] = -999;
    
    TLorentzVector Jet_highestPt;
    int Jet_highestPt_index = -1;
    Jet_highestPt_index = HighestPtBJets().first;
    if (Jet_highestPt_index != -1) {
        Jet_highestPt.SetPtEtaPhiM(d["Jet_pt"][Jet_highestPt_index],d["Jet_eta"][Jet_highestPt_index],d["Jet_phi"][Jet_highestPt_index],d["Jet_mass"][Jet_highestPt_index]);
        *f["Top1_mass_highestPt"] = GetRecoTopMass(Jet_highestPt);
    }
    else *f["Top1_mass_highestPt"] = -999;

    *f["Top1_mass_fromLepton"] = GetRecoTopMass(Lep, false); // construct top mass from closest jet to lepton

    // Let's try to reconstruct the second top from a hadronically decaying W
    double min1 = 999.;
    double min2 = 999.;
    int jetInd1 = -1;
    int jetInd2 = -1;
    for (int i=0; i<*in["nJet"]; i++) {
        if (i==*in["hJetInd1"] || i==*in["hJetInd2"]) continue;
        // make some basic jet id/kinematic cut
        if (d["Jet_pt"][i] < 30 || in["Jet_puId"][i]==0) continue;
        //if (in["Jet_mcMatchId"][i] == 0) continue;
        TLorentzVector j;
        j.SetPtEtaPhiM(d["Jet_pt"][i], d["Jet_eta"][i], d["Jet_phi"][i], d["Jet_mass"][i] );
        double DR = j.DeltaR(HJ2);
        if (DR <= min1) {
            min2 = min1;
            jetInd2 = jetInd1;
            min1 = DR;
            jetInd1 = i;
        }
        else if (DR <= min2) {
            min2 = DR;
            jetInd2 = i;
        }
    }
    *f["HJ2_minJetDR1"] = min1;
    *f["HJ2_minJetDR2"] = min2;
    if (jetInd1!=-1 && jetInd2!=-1) {
         TLorentzVector WJet1, WJet2;
         WJet1.SetPtEtaPhiM(d["Jet_pt"][jetInd1], d["Jet_eta"][jetInd1], d["Jet_phi"][jetInd1], d["Jet_mass"][jetInd1]);
         WJet2.SetPtEtaPhiM(d["Jet_pt"][jetInd2], d["Jet_eta"][jetInd2], d["Jet_phi"][jetInd2], d["Jet_mass"][jetInd2]);
         TLorentzVector Top_hadW = WJet1 + WJet2 + HJ2;
         *f["Top2_mass_hadW"] = Top_hadW.M();
         *f["HJ2_WJet1_dPhi"] = HJ2.DeltaPhi(WJet1);
         *f["HJ2_WJet2_dPhi"] = HJ2.DeltaPhi(WJet2);
         *f["HJ2_WJet1_dEta"] = fabs(HJ2.Eta() - WJet1.Eta());
         *f["HJ2_WJet2_dEta"] = fabs(HJ2.Eta() - WJet2.Eta());
     }
     else {
         *f["Top2_mass_hadW"] = -999;
         *f["HJ2_WJet1_dPhi"] = -999;
         *f["HJ2_WJet2_dPhi"] = -999;
         *f["HJ2_WJet1_dEta"] = -999;
         *f["HJ2_WJet2_dEta"] = -999;
     }

    // Compare gen kinematics for b jets for signal vs. ttbar
    if (*in["nGenBQuarkFromHafterISR"] > 1) {
        // signal event
        GenBJ1.SetPtEtaPhiM(d["GenBQuarkFromHafterISR_pt"][0], d["GenBQuarkFromHafterISR_eta"][0], d["GenBQuarkFromHafterISR_phi"][0], d["GenBQuarkFromHafterISR_mass"][0]);
        GenBJ2.SetPtEtaPhiM(d["GenBQuarkFromHafterISR_pt"][1], d["GenBQuarkFromHafterISR_eta"][1], d["GenBQuarkFromHafterISR_phi"][1], d["GenBQuarkFromHafterISR_mass"][1]);
    }        
    else if (*in["nGenBQuarkFromTop"] == 2){
        // ttbar event 
        GenBJ1.SetPtEtaPhiM(d["GenBQuarkFromTop_pt"][0], d["GenBQuarkFromTop_eta"][0], d["GenBQuarkFromTop_phi"][0], d["GenBQuarkFromTop_mass"][0]);
        GenBJ2.SetPtEtaPhiM(d["GenBQuarkFromTop_pt"][1], d["GenBQuarkFromTop_eta"][1], d["GenBQuarkFromTop_phi"][1], d["GenBQuarkFromTop_mass"][1]);
    }
    else {
       // let's not worry about these for now since ttbar is so dominant
       GenBJ1.SetPtEtaPhiM(0, 0, 0, 0);
       GenBJ2.SetPtEtaPhiM(0, 0, 0, 0);
    }

    *f["GenBJ1_pt"] = GenBJ1.Pt();
    *f["GenBJ1_eta"] = GenBJ1.Eta();
    *f["GenBJ1_phi"] = GenBJ1.Phi();
    *f["GenBJ1_mass"] = GenBJ1.M();
    *f["GenBJ2_pt"] = GenBJ2.Pt();
    *f["GenBJ2_eta"] = GenBJ2.Eta();
    *f["GenBJ2_phi"] = GenBJ2.Phi();
    *f["GenBJ2_mass"] = GenBJ2.M();

    GenBJJ = GenBJ1 + GenBJ2;
    *f["GenBJJ_pt"] = GenBJJ.Pt();
    *f["GenBJJ_eta"] = GenBJJ.Eta();
    *f["GenBJJ_phi"] = GenBJJ.Phi();
    *f["GenBJJ_mass"] = GenBJJ.M();
    *f["GenBJJ_dPhi"] = GenBJ2.DeltaPhi(GenBJ1);
    *f["GenBJJ_dR"] = GenBJ2.DeltaR(GenBJ1);
    *f["GenBJJ_dEta"] = fabs(GenBJ1.Eta() - GenBJ2.Eta());

    TLorentzVector GenLep1, GenLep2; // closest gen lep to either jet1 or jet2. Sometimes these could be the same lepton.
    double minDR1 = 999;
    double minDR2 = 999;
    double minSecDR2 = 999; //second closest gen lepton to second jet
    int GenLepIndex1 = -1; // index of the lepton closest to jet 1
    int GenLepIndex2 = -1; // index of the lepton closest to jet 2
    int GenLepSecIndex2 = -1; // index of the lepton second closest to jet 2
    for (int i=0; i<*in["nGenLep"]; i++) {
       TLorentzVector gl;
       gl.SetPtEtaPhiM(d["GenLep_pt"][i], d["GenLep_eta"][i], d["GenLep_phi"][i], d["GenLep_mass"][i] );
       double DR1 = gl.DeltaR(GenBJ1);
       double DR2 = gl.DeltaR(GenBJ2);
       if (DR1 <= minDR1) {
           minDR1 = DR1;
           GenLepIndex1 = i;
       }
       if (DR2 <= minDR2) {
           minSecDR2 = minDR2;
           GenLepSecIndex2 = GenLepIndex2;
           minDR2 = DR2;
           GenLepIndex2 = i;
       } 
       else if (DR2 < minSecDR2) {
           minSecDR2 = DR2;
           GenLepSecIndex2 = i;
       }
    }  
    if (GenLepIndex1 == GenLepIndex2) {
        // don't allow us to use the same lepton for each jet
        GenLepIndex2 = GenLepSecIndex2;
    }
        
    *in["GenLepIndex1"] = GenLepIndex1;
    *in["GenLepIndex2"] = GenLepIndex2;
    
    if (GenLepIndex1!=-1) {
        GenLep1.SetPtEtaPhiM(d["GenLep_pt"][GenLepIndex1], d["GenLep_eta"][GenLepIndex1], d["GenLep_phi"][GenLepIndex1], d["GenLep_mass"][GenLepIndex1] );
        *f["GenLep_GenBJ1_dR"] = GenLep1.DeltaR(GenBJ1);
        *f["GenLep_GenBJ1_dEta"] = fabs(GenLep1.Eta() - GenBJ1.Eta());
        *f["GenLep_GenBJ1_dPhi"] = GenLep1.DeltaPhi(GenBJ1);
        
        // try to reconstruct the top mass, although we've lost the neutrino so it will be shifted left
        TLorentzVector GenTop1 = GenLep1 + GenBJ1;
        *f["GenTop1_mass"] = GenTop1.M();
    }
    else { 
        *f["GenLep_GenBJ1_dR"] = -999;
        *f["GenLep_GenBJ1_dEta"] = -999;
        *f["GenLep_GenBJ1_dPhi"] = -999;
        *f["GenTop1_mass"] = -999;
    }
    if (GenLepIndex2!=-1) {
        GenLep2.SetPtEtaPhiM(d["GenLep_pt"][GenLepIndex2], d["GenLep_eta"][GenLepIndex2], d["GenLep_phi"][GenLepIndex2], d["GenLep_mass"][GenLepIndex2] );
        *f["GenLep_GenBJ2_dR"] = GenLep2.DeltaR(GenBJ2);
        *f["GenLep_GenBJ2_dEta"] = (GenLep2.Eta(), GenBJ2.Eta());
        *f["GenLep_GenBJ2_dPhi"] = GenLep2.DeltaPhi(GenBJ2);

        // try to reconstruct the top mass, although we've lost the neutrino so it will be shifted left
        TLorentzVector GenTop2 = GenLep2 + GenBJ2; 
        *f["GenTop2_mass"] = GenTop2.M();
    } 
    else {
        *f["GenLep_GenBJ2_dR"] = -999;
        *f["GenLep_GenBJ2_dEta"] = -999;
        *f["GenLep_GenBJ2_dPhi"] = -999;
        *f["GenTop2_mass"] = -999;
    }
    
 
    // Now we can calculate whatever we want (transverse) with W and H four-vectors
    *d["HVdPhi"] = Hbb.DeltaPhi(W);
    *f["H_mass_step2"] = *d["H_mass"];
    *d["H_mass"] = Hbb.M(); // mass window cut? regression applied in FinishEvent
    *d["H_pt"] = Hbb.Pt();
 
    int nSelectedJetsMatched = 0; // count the number (0, 1, 2) of selected jets matched to the real bottom quarks
    // Match Jets with Gen B Jets from Higgs/Tops
    for (int i=0; i<*in["nJet"]; i++) {
        TLorentzVector GenHJ1, GenHJ2, Jet;
        //GenHJ1.SetPtEtaPhiM(d["GenBQuarkFromHafterISR_pt"][0],d["GenBQuarkFromHafterISR_eta"][0],d["GenBQuarkFromHafterISR_phi"][0],d["GenBQuarkFromHafterISR_mass"][0]);
        //GenHJ2.SetPtEtaPhiM(d["GenBQuarkFromHafterISR_pt"][1],d["GenBQuarkFromHafterISR_eta"][1],d["GenBQuarkFromHafterISR_phi"][1],d["GenBQuarkFromHafterISR_mass"][1]);
        Jet.SetPtEtaPhiM(d["Jet_pt"][i], d["Jet_eta"][i], d["Jet_phi"][i], d["Jet_mass"][i]);
      
        //double dR1 = Jet.DeltaR(GenHJ1);
        //double dR2 = Jet.DeltaR(GenHJ2);
        double dR1 = Jet.DeltaR(GenBJ1);
        double dR2 = Jet.DeltaR(GenBJ2);
        d["Jet_genHJetMinDR"][i] = min(dR1, dR2);
        if(dR1 <= dR2) d["Jet_genHJetIndex"][i] = 1;
        else d["Jet_genHJetIndex"][i] = 2;

        if (i == *in["hJetInd1"]) {
            *f["hJet1_matchedMinDR"] = d["Jet_genHJetMinDR"][i];
        }
        else if (i == *in["hJetInd2"]) {
            *f["hJet2_matchedMinDR"] = d["Jet_genHJetMinDR"][i];
        }
    }
    
    // Some Gen W variables
    TLorentzVector GenW;
    if (*in["nGenVbosons"]>0 && fabs(in["GenVbosons_pdgId"][0])==24) {
        GenW.SetPtEtaPhiM(d["GenVbosons_pt"][0], d["GenVbosons_eta"][0], d["GenVbosons_phi"][0], d["GenVbosons_mass"][0]);
        *f["GenW_GenBJJ_dPhi"] = GenW.DeltaPhi(GenBJJ);
        *f["GenW_GenBJJ_dEta"] = fabs(GenW.Eta() - GenBJJ.Eta());
    }
    else {
    
        *f["GenW_GenBJJ_dPhi"] = -999;
        *f["GenW_GenBJJ_dEta"] = -999;
    }
    
    // count the number of additional leptons and jets, then cut on this number
    int nAddJet = 0;
    int nAddJet20 = 0;
    int nAddJet25 = 0;
    int nAddJet30 = 0;
    int nAddJet35 = 0;
    int nAddJet40 = 0;
    int nAddJet45 = 0;
    int nAddJet50 = 0;
    int nAddJet203p5 = 0;
    int nAddJet202p5 = 0;
    int nAddJet253p5 = 0;
    int nAddJet252p5 = 0;
    int nAddJet303p5 = 0;
    int nAddJet302p5 = 0;
    int nAddJet353p5 = 0;
    int nAddJet352p5 = 0;
    int nAddJet403p5 = 0;
    int nAddJet402p5 =0;
    int nAddJet453p5 = 0;
    int nAddJet452p5 =0;
    int nAddJet503p5 = 0;
    int nAddJet502p5 =0;
    int nAddJet_puid = 0;
    int nAddJet20_puid = 0;
    int nAddJet25_puid = 0;
    int nAddJet30_puid = 0;
    int nAddJet35_puid = 0;
    int nAddJet40_puid = 0;
    int nAddJet45_puid = 0;
    int nAddJet50_puid = 0;
    int nAddJet203p5_puid = 0;
    int nAddJet202p5_puid = 0;
    int nAddJet253p5_puid = 0;
    int nAddJet252p5_puid = 0;
    int nAddJet303p5_puid = 0;
    int nAddJet302p5_puid = 0;
    int nAddJet353p5_puid = 0;
    int nAddJet352p5_puid = 0;
    int nAddJet403p5_puid = 0;
    int nAddJet402p5_puid =0;
    int nAddJet453p5_puid = 0;
    int nAddJet452p5_puid =0;
    int nAddJet503p5_puid = 0;
    int nAddJet502p5_puid = 0;
    int nAddLep = 0;          
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>20 && fabs(d["Jet_eta"][i])<4.5 && in["Jet_id"][i]>0) {
            nAddJet++;
        }  
    }           
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>20 && fabs(d["Jet_eta"][i])<4.5 && in["Jet_id"][i]>0) {
            nAddJet20++;
        }  
    }           
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>25 && fabs(d["Jet_eta"][i])<4.5 && in["Jet_id"][i]>0) {
            nAddJet25++;
        }  
    }           
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>30 && fabs(d["Jet_eta"][i])<4.5 && in["Jet_id"][i]>0) {
            nAddJet30++;
        }  
    }           
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>35 && fabs(d["Jet_eta"][i])<4.5 && in["Jet_id"][i]>0) {
            nAddJet35++;
        }  
    }           
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>40 && fabs(d["Jet_eta"][i])<4.5 && in["Jet_id"][i]>0) {
            nAddJet40++;
        }  
    }           
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>45 && fabs(d["Jet_eta"][i])<4.5 && in["Jet_id"][i]>0) {
            nAddJet45++;
        }  
    }           
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>50 && fabs(d["Jet_eta"][i])<4.5 && in["Jet_id"][i]>0) {
            nAddJet50++;
        }  
    }           
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>20 && fabs(d["Jet_eta"][i])<3.5 && in["Jet_id"][i]>0) {
            nAddJet203p5++;
        }  
    }           
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>20 && fabs(d["Jet_eta"][i])<2.5 && in["Jet_id"][i]>0) {
            nAddJet202p5++;
        }  
    }           
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>25 && fabs(d["Jet_eta"][i])<3.5 && in["Jet_id"][i]>0) {
            nAddJet253p5++;
        }  
    }           
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>25 && fabs(d["Jet_eta"][i])<2.5 && in["Jet_id"][i]>0) {
            nAddJet252p5++;
        }  
    }           
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>30 && fabs(d["Jet_eta"][i])<3.5 && in["Jet_id"][i]>0) {
            nAddJet303p5++;
        }  
    }           
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>30 && fabs(d["Jet_eta"][i])<2.5 && in["Jet_id"][i]>0) {
            nAddJet302p5++;
        }  
    }           
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>35 && fabs(d["Jet_eta"][i])<3.5 && in["Jet_id"][i]>0) {
            nAddJet353p5++;
        }  
    }           
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>35 && fabs(d["Jet_eta"][i])<2.5 && in["Jet_id"][i]>0) {
            nAddJet352p5++;
        }  
    }           
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>40 && fabs(d["Jet_eta"][i])<3.5 && in["Jet_id"][i]>0) {
            nAddJet403p5++;
        }  
    }           
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>40 && fabs(d["Jet_eta"][i])<2.5 && in["Jet_id"][i]>0) {
            nAddJet402p5++;
        }  
    }           
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>45 && fabs(d["Jet_eta"][i])<3.5 && in["Jet_id"][i]>0) {
            nAddJet453p5++;
        }  
    }           
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>45 && fabs(d["Jet_eta"][i])<2.5 && in["Jet_id"][i]>0) {
            nAddJet452p5++;
        }  
    }           
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>50 && fabs(d["Jet_eta"][i])<3.5 && in["Jet_id"][i]>0) {
            nAddJet503p5++;
        }  
    }           
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>50 && fabs(d["Jet_eta"][i])<2.5 && in["Jet_id"][i]>0) {
            nAddJet502p5++;
        }  
    }          

    
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>20 && fabs(d["Jet_eta"][i])<4.5 && in["Jet_puId"][i]>0) {
            nAddJet_puid++;
        }  
    }
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>20 && fabs(d["Jet_eta"][i])<4.5 && in["Jet_puId"][i]>0) {
            nAddJet20_puid++;
        }
    }
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>25 && fabs(d["Jet_eta"][i])<4.5 && in["Jet_puId"][i]>0) {
            nAddJet25_puid++;
        }
    }           
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>30 && fabs(d["Jet_eta"][i])<4.5 && in["Jet_puId"][i]>0) {
            nAddJet30_puid++;
        }  
    }           
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>35 && fabs(d["Jet_eta"][i])<4.5 && in["Jet_puId"][i]>0) {
            nAddJet35_puid++;
        }  
    }           
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>40 && fabs(d["Jet_eta"][i])<4.5 && in["Jet_puId"][i]>0) {
            nAddJet40_puid++;
        }  
    }           
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>45 && fabs(d["Jet_eta"][i])<4.5 && in["Jet_puId"][i]>0) {
            nAddJet45_puid++;
        }  
    }           
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>50 && fabs(d["Jet_eta"][i])<4.5 && in["Jet_puId"][i]>0) {
            nAddJet50_puid++;
        }  
    }       
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>20 && fabs(d["Jet_eta"][i])<3.5 && in["Jet_puId"][i]>0) {
            nAddJet203p5_puid++;
        }
    }
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>20 && fabs(d["Jet_eta"][i])<2.5 && in["Jet_puId"][i]>0) {
            nAddJet202p5_puid++;
        }
    }
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>25 && fabs(d["Jet_eta"][i])<3.5 && in["Jet_puId"][i]>0) {
            nAddJet253p5_puid++;
        }
    }
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>25 && fabs(d["Jet_eta"][i])<2.5 && in["Jet_puId"][i]>0) {
            nAddJet252p5_puid++;
        }
    }    
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>30 && fabs(d["Jet_eta"][i])<3.5 && in["Jet_puId"][i]>0) {
            nAddJet303p5_puid++;
        }  
    }           
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>30 && fabs(d["Jet_eta"][i])<2.5 && in["Jet_puId"][i]>0) {
            nAddJet302p5_puid++;
        }  
    }           
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>35 && fabs(d["Jet_eta"][i])<3.5 && in["Jet_puId"][i]>0) {
            nAddJet353p5_puid++;
        }  
    }           
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>35 && fabs(d["Jet_eta"][i])<2.5 && in["Jet_puId"][i]>0) {
            nAddJet352p5_puid++;
        }  
    }           
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>40 && fabs(d["Jet_eta"][i])<3.5 && in["Jet_puId"][i]>0) {
            nAddJet403p5_puid++;
        }  
    }           
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>40 && fabs(d["Jet_eta"][i])<2.5 && in["Jet_puId"][i]>0) {
            nAddJet402p5_puid++;
        }  
    }           
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>45 && fabs(d["Jet_eta"][i])<3.5 && in["Jet_puId"][i]>0) {
            nAddJet453p5_puid++;
        }  
    }           
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>45 && fabs(d["Jet_eta"][i])<2.5 && in["Jet_puId"][i]>0) {
            nAddJet452p5_puid++;
        }  
    }           
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>50 && fabs(d["Jet_eta"][i])<3.5 && in["Jet_puId"][i]>0) {
            nAddJet503p5_puid++;
        }  
    }           
    for(int i=0; i < *in["nJet"]; i++) {
        if(i == *in["hJetInd1"] || i == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][i]>50 && fabs(d["Jet_eta"][i])<2.5 && in["Jet_puId"][i]>0) {
            nAddJet502p5_puid++;
        }  
    }           
 
    *in["nAddJets"] = nAddJet;
    *in["nAddJets20"] = nAddJet20;
    *in["nAddJets25"] = nAddJet25;
    *in["nAddJets30"] = nAddJet30;
    *in["nAddJets35"] = nAddJet35;
    *in["nAddJets40"] = nAddJet40;
    *in["nAddJets45"] = nAddJet45;
    *in["nAddJets50"] = nAddJet50;
    *in["nAddJets203p5"] = nAddJet203p5;
    *in["nAddJets202p5"] = nAddJet202p5; 
    *in["nAddJets253p5"] = nAddJet253p5;
    *in["nAddJets252p5"] = nAddJet252p5; 
    *in["nAddJets303p5"] = nAddJet303p5;
    *in["nAddJets302p5"] = nAddJet302p5; 
    *in["nAddJets353p5"] = nAddJet353p5;
    *in["nAddJets352p5"] = nAddJet352p5;
    *in["nAddJets403p5"] = nAddJet403p5;
    *in["nAddJets402p5"] = nAddJet402p5;
    *in["nAddJets453p5"] = nAddJet453p5;
    *in["nAddJets452p5"] = nAddJet452p5;
    *in["nAddJets503p5"] = nAddJet503p5;
    *in["nAddJets502p5"] = nAddJet502p5;
    
    *in["nAddJets_puid"] = nAddJet_puid;
    *in["nAddJets20_puid"] = nAddJet20_puid;
    *in["nAddJets25_puid"] = nAddJet25_puid;
    *in["nAddJets30_puid"] = nAddJet30_puid;
    *in["nAddJets35_puid"] = nAddJet35_puid;
    *in["nAddJets40_puid"] = nAddJet40_puid;
    *in["nAddJets45_puid"] = nAddJet45_puid;
    *in["nAddJets50_puid"] = nAddJet50_puid;
    *in["nAddJets203p5_puid"] = nAddJet203p5_puid;
    *in["nAddJets202p5_puid"] = nAddJet202p5_puid; 
    *in["nAddJets253p5_puid"] = nAddJet253p5_puid;
    *in["nAddJets252p5_puid"] = nAddJet252p5_puid; 
    *in["nAddJets303p5_puid"] = nAddJet303p5_puid;
    *in["nAddJets302p5_puid"] = nAddJet302p5_puid; 
    *in["nAddJets353p5_puid"] = nAddJet353p5_puid;
    *in["nAddJets352p5_puid"] = nAddJet352p5_puid;
    *in["nAddJets403p5_puid"] = nAddJet403p5_puid;
    *in["nAddJets402p5_puid"] = nAddJet402p5_puid;
    *in["nAddJets453p5_puid"] = nAddJet453p5_puid;
    *in["nAddJets452p5_puid"] = nAddJet452p5_puid;
    *in["nAddJets503p5_puid"] = nAddJet503p5_puid;
    *in["nAddJets502p5_puid"] = nAddJet502p5_puid;

    // count additional leptons (check both collections, which are exclusive)
    for (int i=0; i<*in["nselLeptons"]; i++) {
        if (i == *in["lepInd"]) continue; // don't look at the lepton we've selected from the W
        if (d["selLeptons_pt"][i]>15 && fabs(d["selLeptons_eta"][i])<2.5 && d["selLeptons_relIso03"][i]<0.1) {
            nAddLep++;
        }
    }
    for (int i=0; i<*in["naLeptons"]; i++) {
        if (d["aLeptons_pt"][i]>15 && fabs(d["aLeptons_eta"][i])<2.5 && d["aLeptons_relIso03"][i]<0.1) {
            nAddLep++;
        }
    } 

    *in["nAddLeptons"] = nAddLep;
    if (nAddLep>= *f["nAddLeptonsCut"]) sel = false;
    if (sel) *in["cutFlow"] += 1; // additional lepton veto

    if(nAddJet252p5_puid >= *f["nAddJetsCut"]) sel = false;
    if (sel) *in["cutFlow"] += 1; // additional jet veto
    
    if(fabs(*d["HVdPhi"]) < *f["HVDPhiCut"]) sel = false;
     if (sel) *in["cutFlow"] += 1; // dPhi(jj,W) cut

    // let's do this for now in WorkspaceAnalysis since it's fast to rerun there
    // mass window, use parameterized mean/sigma from jet mass res. study
    double mean = 104.7 + 0.055 * (*d["H_pt"]);
    double sigma = 20.9* exp(-0.0024*(*d["H_pt"]));

    // mass window cut
    if (*d["H_mass"] < (mean - 2*sigma) || *d["H_mass"] > (mean + 2*sigma)) sel=false;
    //if (*d["H_mass"] < 100 || *d["H_mass"] > 150) sel=false;
    if (sel) *in["cutFlow"] += 1; 

    if(debug>1000) {
        std::cout<<"selecting event"<<std::endl;
    }

    if (doCutFlow) return true; // keep all preselected events for cutflow
    else return sel;
}

void VHbbAnalysis::FinishEvent(){
   
    //if (bool(*f["doCutFlow"])) {
    //    ofile->cd();
    //    outputTree->Fill();
    //    return;
    //} 
    // General use variables
    *in["sampleIndex"] = cursample->sampleNum;
   
    // Split WJets and ZJets samples by jet parton flavor
    if (cursample->doJetFlavorSplit) {
        int nBJets = 0;
        //std::cout<<fabs(d["hJets_mcFlavour"][0])<<std::endl;
        if (fabs(in["Jet_mcFlavour"][*in["hJetInd1"]]) == 5) nBJets++;
        if (fabs(in["Jet_mcFlavour"][*in["hJetInd2"]]) == 5) nBJets++;
        *in["sampleIndex"] = (*in["sampleIndex"]*100 + nBJets);
    } 

    // Split by boost category
    if(*in["isWenu"]) {
        if(*d["V_pt"] >= 100 && *d["V_pt"] < 150) *in["eventClass"] += 2;
        else if(*d["V_pt"] >= 150) *in["eventClass"] += 1;
        else *in["eventClass"] += 3;
    }
    else if(*in["isWmunu"]) {
        if(*d["V_pt"] >= 100 && *d["V_pt"] < 130) *in["eventClass"] += 3;
        else if(*d["V_pt"] >= 130 && *d["V_pt"] < 180) *in["eventClass"] += 2;
        else if(*d["V_pt"] >= 180) *in["eventClass"] += 1;
        else *in["eventClass"] += 4;
    }

    // compare our Higgs jet selection with step 2 
    *d["hJets_pt_0_step2"] = d["hJets_pt"][0];
    *d["hJets_pt_1_step2"] = d["hJets_pt"][1];
    *d["hJets_btagCSV_0_step2"] = d["hJets_btagCSV"][0];
    *d["hJets_btagCSV_1_step2"] = d["hJets_btagCSV"][1];

    *f["weight"] = cursample->intWeight;
    //*f["weight"] = 1.0; // HACK FIXME
    *f["Vtype_f"] = (float) *d["Vtype"];
    //*f["absDeltaPullAngle"] = fabs(*f["deltaPullAngle"]);
    *f["selLeptons_pt_0"] = d["selLeptons_pt"][*in["lepInd"]];
    *f["selLeptons_eta_0"] = d["selLeptons_eta"][*in["lepInd"]];
    *f["selLeptons_phi_0"] = d["selLeptons_phi"][*in["lepInd"]];
    *in["selLeptons_pdgId_0"] = in["selLeptons_pdgId"][*in["lepInd"]];    
    *in["selLeptons_eleCutIdCSA14_25ns_v1_0"] = in["selLeptons_eleCutIdCSA14_25ns_v1"][*in["lepInd"]];
    *in["selLeptons_tightId_0"] = in["selLeptons_tightId"][*in["lepInd"]];
    *f["selLeptons_relIso03_0"] = d["selLeptons_relIso03"][*in["lepInd"]];  
  
    // electron ID variables
    *f["selLeptons_eleSieie_0"] = d["selLeptons_eleSieie"][*in["lepInd"]];
    *f["selLeptons_eleDEta_0"] = d["selLeptons_eleDEta"][*in["lepInd"]];
    *f["selLeptons_eleDPhi_0"] = d["selLeptons_eleDPhi"][*in["lepInd"]];
    *f["selLeptons_eleHoE_0"] = d["selLeptons_eleHoE"][*in["lepInd"]];
    *f["selLeptons_eleMissingHits_0"] = d["selLeptons_eleMissingHits"][*in["lepInd"]];
    *f["selLeptons_eleChi2_0"] = d["selLeptons_eleChi2"][*in["lepInd"]];
     
 
    if (*in["nGenLep"] == 0) {
        // gen lep is originally a tau?
        *f["selLeptons_genLepDR_0"] = -1;
    }
    else {
        TLorentzVector GenLep, El;
        GenLep.SetPtEtaPhiM(d["GenLep_pt"][0],d["GenLep_eta"][0],d["GenLep_phi"][0],d["GenLep_mass"][0]);
        El.SetPtEtaPhiM(d["selLeptons_pt"][*in["lepInd"]], d["selLeptons_eta"][*in["lepInd"]], d["selLeptons_phi"][*in["lepInd"]], d["selLeptons_mass"][*in["lepInd"]]);
        *f["selLeptons_genLepDR_0"] = El.DeltaR(GenLep);

        
    }
 

    *f["naLeptonsPassingCuts"] = *in["nAddLeptons"]; // no need to calculate this twice
    *f["naJetsPassingCuts"] = *in["nAddJets"];
    *f["highestCSVaJet"] = 0.0;
    *f["minDeltaRaJet"] = 9999.0; // FIXME add correct initialization
    for(int j=0;j<*in["nJet"];j++){
        if (j == *in["hJetInd1"] || j == *in["hJetInd2"]) continue;
        if(d["Jet_pt"][j]>20 && fabs(d["Jet_eta"][j])<2.5 && d["Jet_btagCSV"][j]>*f["highestCSVaJet"]) *f["highestCSVaJet"] = d["Jet_btagCSV"][j];
      }       
    
    // Reconstruct Higgs and W and recalculate variables ourselves
    TLorentzVector MET,Lep,W,HJ1,HJ2,Hbb;
    MET.SetPtEtaPhiM(*d["met_pt"], 0., *d["met_phi"], 0.); // Eta/M don't affect calculation of W.pt and W.phi
    Lep.SetPtEtaPhiM(d["selLeptons_pt"][*in["lepInd"]], d["selLeptons_eta"][*in["lepInd"]], d["selLeptons_phi"][*in["lepInd"]], d["selLeptons_mass"][*in["lepInd"]]);
    W = MET + Lep;

    HJ1.SetPtEtaPhiM(d["Jet_pt"][*in["hJetInd1"]], d["Jet_eta"][*in["hJetInd1"]], d["Jet_phi"][*in["hJetInd1"]], d["Jet_mass"][*in["hJetInd1"]]);
    HJ2.SetPtEtaPhiM(d["Jet_pt"][*in["hJetInd2"]], d["Jet_eta"][*in["hJetInd2"]], d["Jet_phi"][*in["hJetInd2"]], d["Jet_mass"][*in["hJetInd2"]]);
    Hbb = HJ1 + HJ2;
    
    // We already calculate these in Analyze()
    //*d["H_mass"] = Hbb.M();
    //*d["H_pt"] = Hbb.Pt();
    //*d["V_pt"] = W.Pt();
    //*d["HVdPhi"] = Hbb.DeltaPhi(W);
    
    // Set variables used by the BDT
    *f["H_mass_f"] = (float) *d["H_mass"];
    *f["H_pt_f"] = (float) *d["H_pt"];
    *f["V_pt_f"] = (float) *d["V_pt"];
    *f["hJets_btagCSV_0"] = (float) d["Jet_btagCSV"][*in["hJetInd1"]];
    *f["hJets_btagCSV_1"] = (float) d["Jet_btagCSV"][*in["hJetInd2"]]; 
    *f["HVdPhi_f"] = (float) *d["HVdPhi"];
    *f["H_dEta"] = fabs(d["Jet_eta"][*in["hJetInd1"]] - d["Jet_eta"][*in["hJetInd2"]]);
   
    *f["hJets_mt_0"] = HJ1.Mt();
    *f["hJets_mt_1"] = HJ2.Mt();
    *f["H_dR"] = (float) HJ1.DeltaR(HJ2);   
    *f["absDeltaPullAngle"] = 0.; //FIXME what is this in the new ntuples??
    *f["hJet_ptCorr_0"] = (float) d["Jet_pt"][*in["hJetInd1"]];
    *f["hJet_ptCorr_1"] = (float) d["Jet_pt"][*in["hJetInd2"]];
    *f["met_sumEt_f"] = (float) *d["met_sumEt"]; // is this the right variable??
 
    for (unsigned int i=0; i<bdtInfos.size(); i++) {

       BDTInfo tmpBDT = bdtInfos[i];
       if(debug>5000) {
           PrintBDTInfoValues(tmpBDT);
           std::cout<<"BDT evaulates to: "<<tmpBDT.reader->EvaluateMVA(tmpBDT.bdtname)<<std::endl;
       }
       *f[tmpBDT.bdtname] = tmpBDT.reader->EvaluateMVA(tmpBDT.bdtname);
    }

    if(jet1EnergyRegressionIsSet && jet2EnergyRegressionIsSet) {
        *f["hJets_pt_0"] = float(d["Jet_pt"][*in["hJetInd1"]]);
        //*f["hJets_rawPt_0"] = float(d["hJets_rawPt"][0]);
        *f["hJets_eta_0"] = float(d["Jet_eta"][*in["hJetInd1"]]);
        //*f["hJets_mt_0"] = 0.; // FIXME
        *f["hJets_leadTrackPt_0"] = float(d["Jet_leadTrackPt"][*in["hJetInd1"]]);
        *f["hJets_leptonPtRel_0"] = float(d["Jet_leptonPtRel"][*in["hJetInd1"]]);
        *f["hJets_leptonPt_0"] = float(d["Jet_leptonPt"][*in["hJetInd1"]]);
        *f["hJets_leptonDeltaR_0"] = float(d["Jet_leptonDeltaR"][*in["hJetInd1"]]);
        *f["hJets_chEmEF_0"] = float(d["Jet_chEmEF"][*in["hJetInd1"]]);
        *f["hJets_chHEF_0"] = float(d["Jet_chHEF"][*in["hJetInd1"]]);
        *f["hJets_neHEF_0"] = float(d["Jet_neHEF"][*in["hJetInd1"]]);
        *f["hJets_neEmEF_0"] = float(d["Jet_neEmEF"][*in["hJetInd1"]]);
        *f["hJets_chMult_0"] = float(d["Jet_chMult"][*in["hJetInd1"]]);
        //*f["hJets_puId_0"] = float(d["hJets_puId"][0]);
        *f["hJets_vtx3DVal_0"] = 0.;
        *f["hJets_vtxNtracks_0"] = 0.;
        *f["hJets_vtxMass_0"] = float(d["Jet_vtxMass"][*in["hJetInd1"]]);
        *f["hJets_vtxPt_0"] = float(d["Jet_vtxPt"][*in["hJetInd1"]]);
        *f["hJets_vtx3DVal_0"] = float(d["Jet_vtx3DVal"][*in["hJetInd1"]]);
        *f["hJets_vtxNtracks_0"] = float(d["Jet_vtxNtracks"][*in["hJetInd1"]]);

        *f["hJets_pt_1"] = float(d["Jet_pt"][*in["hJetInd2"]]);
        //*f["hJets_rawPt_1"] = float(d["hJets_rawPt"][1]);
        *f["hJets_eta_1"] = float(d["Jet_eta"][*in["hJetInd2"]]);
        //*f["hJets_mt_1"] = 0.; // FIXME
        *f["hJets_leadTrackPt_1"] = float(d["Jet_leadTrackPt"][*in["hJetInd2"]]);
        *f["hJets_leptonPtRel_1"] = float(d["Jet_leptonPtRel"][*in["hJetInd2"]]);
        *f["hJets_leptonPt_1"] = float(d["Jet_leptonPt"][*in["hJetInd2"]]);
        *f["hJets_leptonDeltaR_1"] = float(d["Jet_leptonDeltaR"][*in["hJetInd2"]]);
        *f["hJets_chEmEF_1"] = float(d["Jet_chEmEF"][*in["hJetInd2"]]);
        *f["hJets_chHEF_1"] = float(d["Jet_chHEF"][*in["hJetInd2"]]);
        *f["hJets_neHEF_1"] = float(d["Jet_neHEF"][*in["hJetInd2"]]);
        *f["hJets_neEmEF_1"] = float(d["Jet_neEmEF"][*in["hJetInd2"]]);
        *f["hJets_chMult_1"] = float(d["Jet_chMult"][*in["hJetInd2"]]);
        //*f["hJets_puId_1"] = float(d["hJets_puId"][1]);
        *f["hJets_vtx3DVal_1"] = 0.;
        *f["hJets_vtxNtracks_1"] = 0.;
        *f["hJets_vtxMass_1"] = float(d["Jet_vtxMass"][*in["hJetInd2"]]);
        *f["hJets_vtx3DVal_1"] = float(d["Jet_vtx3DVal"][*in["hJetInd2"]]);
        *f["hJets_vtxPt_1"] = float(d["Jet_vtxPt"][1]);
        *f["hJets_vtxNtracks_1"] = float(d["Jet_vtxNtracks"][*in["hJetInd2"]]);
        
        if(debug>10000) {
            std::cout<<"Evaluating the Jet Energy Regression..."<<std::endl;
            PrintBDTInfoValues(jet1EnergyRegression);
            PrintBDTInfoValues(jet2EnergyRegression);
        }
        double r1Pt = jet1EnergyRegression.reader->EvaluateRegression(jet1EnergyRegression.bdtname)[0];
        double r2Pt = jet2EnergyRegression.reader->EvaluateRegression(jet2EnergyRegression.bdtname)[0];

        *f["Jet1_regWeight"] = r1Pt/(*f["hJets_pt_0"]);
        *f["Jet2_regWeight"] = r2Pt/(*f["hJets_pt_1"]);

        TLorentzVector hJ1_reg = TLorentzVector();
        TLorentzVector hJ2_reg = TLorentzVector();
 
        *f["Jet1_pt_reg"] = r1Pt;
        *f["Jet2_pt_reg"] = r2Pt;
        hJ1_reg.SetPtEtaPhiM(r1Pt, *f["hJets_eta_0"], d["hJets_phi"][0], d["hJets_mass"][0]);
        hJ2_reg.SetPtEtaPhiM(r2Pt, *f["hJets_eta_1"], d["hJets_phi"][1], d["hJets_mass"][1]);
       
        //*f["hJets_mt_0"] = hJ1_reg.Mt();
        //*f["hJets_mt_1"] = hJ2_reg.Mt();
    
        TLorentzVector H_vec_reg = hJ1_reg + hJ2_reg;
        *f["H_mass_reg"] = H_vec_reg.M();
        *f["H_pt_reg"] = H_vec_reg.Pt();
        *f["H_eta_reg"] = H_vec_reg.Eta();
        *f["H_phi_reg"] = H_vec_reg.Phi();
        *f["H_dR_reg"] = hJ1_reg.DeltaR(hJ2_reg);
        *f["H_dPhi_reg"] = hJ1_reg.DeltaPhi(hJ2_reg);
        *f["H_dEta_reg"] = fabs(hJ1_reg.Eta() - hJ2_reg.Eta() );
     }

    ofile->cd();
    outputTree->Fill();
    return;
}

void VHbbAnalysis::TermAnalysis(){
    if(debug>10) std::cout<<"START TermAnalysis()"<<std::endl;
    ofile->cd();
    outputTree->Write();
    ofile->Close();
    if(debug>10) std::cout<<"DONE TermAnalysis()"<<std::endl;
    return;
}

bool VHbbAnalysis::WenuHbbSelection(){
    bool selectEvent=true;
    if(debug>1000){
        std::cout<<"Running Wenu selections"<<std::endl;
        std::cout<<"*in[\"nselLeptons\"] "<<*in["nselLeptons"]<<std::endl;
        std::cout<<"d[\"selLeptons_pt\"][0] "<<d["selLeptons_pt"][0]<<std::endl;
        std::cout<<"in[\"selLeptons_pdgId\"] "<<in["selLeptons_pdgId"][0]<<std::endl;
        std::cout<<"d[\"selLeptons_relIso03\"] "<<d["selLeptons_relIso03"][0]<<std::endl;
        std::cout<<"*d[\"met_pt\"] "<<*d["met_pt"]<<std::endl;
    }
    
    // there is only one selected electron for Vtype == 3 which is the electron tag
    // FIXME add configurable cuts
    *in["elInd"] = -1;
    float elMaxPt = 0; // max pt of the electrons we select
    for (int i =0; i<*in["nselLeptons"]; i++) {
        if(fabs(in["selLeptons_pdgId"][i])==11 
            && d["selLeptons_pt"][i]      > *f["eptcut"] 
            && fabs(d["selLeptons_eta"][i]) < *f["eletacut"]
            && d["selLeptons_relIso03"][i]< *f["erelisocut"]
            && in["selLeptons_eleCutIdCSA14_25ns_v1"][i] >= *f["elidcut"]
            ){
            if (d["selLeptons_pt"][i] > elMaxPt) {
                elMaxPt = d["selLeptons_pt"][i];
                *in["elInd"] = i;
            }
        }
    }
    if (*in["elInd"] == -1) selectEvent = false;

    return selectEvent;
}

bool VHbbAnalysis::WmunuHbbSelection(){
    
    bool selectEvent=true;
    if(debug>1000){
        std::cout<<"Running Wmunu selections"<<std::endl;
        std::cout<<"*in[\"nselLeptons\"] "<<*in["nselLeptons"]<<std::endl;
        std::cout<<"d[\"selLeptons_pt\"][0] "<<d["selLeptons_pt"][0]<<std::endl;
        std::cout<<"in[\"selLeptons_pdgId\"] "<<in["selLeptons_pdgId"][0]<<std::endl;
        std::cout<<"d[\"selLeptons_relIso03\"] "<<d["selLeptons_relIso03"][0]<<std::endl;
        std::cout<<"*d[\"met_pt\"] "<<*d["met_pt"]<<std::endl;
    }
    
    // there is only one selected electron for Vtype == 3 which is the electron tag
    // FIXME add configurable cuts

    *in["muInd"] = -1;
    float muMaxPt = 0; // max pt of the muons we select 
    for (int i=0; i<*in["nselLeptons"]; i++) {
        if(fabs(in["selLeptons_pdgId"][i])==13 
            && d["selLeptons_pt"][i]      > *f["muptcut"]
            && fabs(d["selLeptons_eta"][i]) < *f["muetacut"]
            && d["selLeptons_relIso03"][i]< *f["murelisocut"]
            && in["selLeptons_tightId"][i] >= *f["muidcut"]
            ){
            if (d["selLeptons_pt"][i] > muMaxPt) {
                muMaxPt = d["selLeptons_pt"][i];
                *in["muInd"] = i;
            }
        }
    }
    if (*in["muInd"] == -1) selectEvent = false;

    return selectEvent;
}

std::pair<int,int> VHbbAnalysis::HighestPtBJets(){
    std::pair<int,int> pair(-1,-1);

    for(int i=0; i<*in["nJet"]; i++){
        if(in["Jet_puId"][i] == 1
            && d["Jet_pt"][i]>*f["j1ptCut"]
            && d["Jet_btagCSV"][i]>*f["j1ptCSV"]) {
            if( pair.first == -1 ) {
                pair.first = i;
            } else if(d["Jet_pt"][pair.first]<d["Jet_pt"][i]){
                pair.first = i;
            }
        }
        /*if( pair.first == -1) {
            pair.first = i;
        }
        else if(d["Jet_pt"][pair.first]<d["Jet_pt"][i]){
            pair.first = i;
        }*/
    }
   
    for(int i=0; i<*in["nJet"]; i++){
        if(i==pair.first) continue;
        if(in["Jet_puId"][i] == 1
            && d["Jet_pt"][i]>*f["j2ptCut"]
            && d["Jet_btagCSV"][i]>*f["j2ptCSV"]) {
            if( pair.second == -1 ) {
                pair.second = i;
            } else if(d["Jet_pt"][pair.second]<d["Jet_pt"][i]){
                pair.second = i;
            }
        }
        /*if (pair.second == -1) {
            pair.second = i;
        }
        else if(d["Jet_pt"][pair.second]<d["Jet_pt"][i]){
            pair.second = i;
        }*/
    }

    return pair;
}


std::pair<int,int> VHbbAnalysis::HighestCSVBJets(){
    std::pair<int,int> pair(-1,-1);

    for(int i=0; i<*in["nJet"]; i++){
        if(in["Jet_puId"][i] == 1
            && d["Jet_pt"][i]>*f["j1ptCut"]
            && d["Jet_btagCSV"][i]>*f["j1ptCSV"]) {
            if( pair.first == -1 ) {
                pair.first = i;
            } else if(d["Jet_btagCSV"][pair.first]<d["Jet_btagCSV"][i]){
                pair.first = i;
            }
        }
    }

    for(int i=0; i<*in["nJet"]; i++){
        if(i==pair.first) continue;
        if(in["Jet_puId"][i] == 1
            && d["Jet_pt"][i]>*f["j2ptCut"]
            && d["Jet_btagCSV"][i]>*f["j2ptCSV"]) {
            if( pair.second == -1 ) {
                pair.second = i;
            } else if(d["Jet_btagCSV"][pair.second]<d["Jet_btagCSV"][i]){
                pair.second = i;
            }
        }
    }

    return pair;
}

double VHbbAnalysis::GetRecoTopMass(TLorentzVector Obj, bool isJet) {

    // Try to reconstruct the top in ttbar with leptonic W decay
    // if isJet is true, construct top as given jet + closest lepton
    // if isJet is false, construct top as given lepton + closest lepton
    double Top_mass = -999; // return value

    double minDR = 999;
    int ObjClosestIndex = -1; // index of closest lepton if isJet, closet jet otherwise

    TLorentzVector Obj2; // closest lepton if isJet, closest jet otherwise
    if (isJet) {
        // look in aleptons too FIXME
        // find the closest lepton to the given jet
        for (int i=0; i<*in["nselLeptons"]; i++) {
            TLorentzVector l;
            l.SetPtEtaPhiM(d["selLeptons_pt"][i], d["selLeptons_eta"][i], d["selLeptons_phi"][i], d["selLeptons_mass"][i] );
            double d1 = l.DeltaR(Obj);
            if (d1 <= minDR) {
                minDR = d1;
                ObjClosestIndex = i;
             }
        }
        if (ObjClosestIndex!=-1) {
            Obj2.SetPtEtaPhiM(d["selLeptons_pt"][ObjClosestIndex], d["selLeptons_eta"][ObjClosestIndex], d["selLeptons_phi"][ObjClosestIndex], d["selLeptons_mass"][ObjClosestIndex]);
        }
        else return -999;
    }
    else {
        // find closest jet to the given lepton
        for (int i=0; i<*in["nJet"]; i++) {
            if (d["Jet_pt"][i] < 30 || d["Jet_btagCSV"][i] < 0.5) continue; // only consider jets with some minimal preselection
            TLorentzVector j;
            j.SetPtEtaPhiM(d["Jet_pt"][i], d["Jet_eta"][i], d["Jet_phi"][i], d["Jet_mass"][i] );
            double d1 = j.DeltaR(Obj);
            if (d1 <= minDR) {
                minDR = d1;
                ObjClosestIndex = i;
             }
        }
        if (ObjClosestIndex!=-1) {
            Obj2.SetPtEtaPhiM(d["Jet_pt"][ObjClosestIndex], d["Jet_eta"][ObjClosestIndex], d["Jet_phi"][ObjClosestIndex], d["Jet_mass"][ObjClosestIndex]);
        }
        else return -999;
    }

    
    TLorentzVector Top = Obj + Obj2;
    return Top.M();
}

