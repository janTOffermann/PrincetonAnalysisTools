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
    bool sel=false;
    if( *d["Vtype"]==3 ) sel=true;
    return sel;
}

bool VHbbAnalysis::Analyze(){
    bool sel=false;

    if(debug>1000) {
        std::cout<<"selecting bjets"<<std::endl;
    }
    std::pair<int,int> bjets=HighestPtBJets(0.5);
   
    // there aren't two acceptable jets
    if(bjets.first==-1 || bjets.second==-1) return sel;
    *in["hJetInd1"]=bjets.first;
    *in["hJetInd2"]=bjets.second;

    if(debug>1000) {
        std::cout<<"found two bjets with pt "
            <<d["Jet_pt"][bjets.first]<<" "
            <<d["Jet_pt"][bjets.second]<<" "
            <<std::endl;
    }
 
    //check if event passes any event class
    if(WmunuHbbSelection()) {
        sel=true;
        *in["eventClass"]=0;
    } else if(WenuHbbSelection()) {
        sel=true;
        *in["eventClass"]=1;
    }

    if(debug>1000) {
        std::cout<<"selecting event"<<std::endl;
    }

    return sel;
}

void VHbbAnalysis::FinishEvent(){
    *in["sampleIndex"] = cursample->sampleNum;
    *f["weight"] = cursample->intWeight;
    
    *f["Vtype_f"] = (float) *d["Vtype"];
    //*f["absDeltaPullAngle"] = fabs(*f["deltaPullAngle"]);
    *f["naLeptonsPassingCuts"] = 0.;
    //for(int j=0;j<*in["naLeptons"]&&j<100;j++){
    //    //if(f["aLepton_pt"][j]>15. && fabs(f["aLepton_eta"][j])<2.5 && f["aLepton_pfCombRelIso"][j]<0.15) *f["naLeptonsPassingCuts"] += 1.;
    //    if(f["aLepton_pt"][j]>15. && fabs(f["aLepton_eta"][j])<2.5 && f["aLeptons_eleCutIdCSA14_25ns_v1"][j]) *f["naLeptonsPassingCuts"] += 1.;
    //}
    //*f["naJetsPassingCuts"] = 0.;
    //*f["highestCSVaJet"] = 0.0;
    //*f["minDeltaRaJet"] = 9999.0;
    //for(int j=0;j<*in["naJets"];j++){
    //    if(f["aJet_pt"][j]>20. && fabs(f["aJet_eta"][j])<4.5) *f["naJetsPassingCuts"] += 1;
    //}       
    
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
    int selectedInd=-1;
    bool selectEvent=false;
    float highestPt=-99;
    if(debug>1000){
        std::cout<<"*in[\"nselLeptons\"] "<<*in["nselLeptons"]<<std::endl;
        std::cout<<"d[\"selLeptons_pt\"][0] "<<d["selLeptons_pt"][0]<<std::endl;
        std::cout<<"in[\"selLeptons_pdgId\"] "<<in["selLeptons_pdgId"][0]<<std::endl;
        std::cout<<"d[\"selLeptons_relIso03\"] "<<d["selLeptons_relIso03"][0]<<std::endl;
        std::cout<<"*d[\"met_pt\"] "<<*d["met_pt"]<<std::endl;
    }

    // there is only one selected electron for Vtype == 3 which is the electron tag
    // FIXME add configurable cuts
    if(d["selLeptons_pt"][0]> 40 
        && abs(in["selLeptons_pdgId"][0])==11 
        && d["selLeptons_relIso03"][0]<1.0
        && *d["met_pt"] > 45){
        *d["elMetDPhi"]=abs(EvalDeltaPhi(d["selLeptons_phi"][0],*d["met_phi"]));
        
        if(*d["elMetDPhi"]<3.1415926536/4){
            selectEvent=true;
        }
    }

    return selectEvent;
}


bool VHbbAnalysis::WmunuHbbSelection(){
    bool sel=false;
    //if(1) sel=true;
    return sel;
}


std::pair<int,int> VHbbAnalysis::HighestPtBJets(float minCSV){
    std::pair<int,int> pair(-1,-1);

    for(int i=0; i<*in["nJet"]; i++){
        if(in["Jet_puId"][i] == 1
            && d["Jet_pt"][i]>30
            && d["Jet_btagCSV"][i]>minCSV) {
            if( pair.first == -1 ) {
                pair.first = i;
            } else {
                // pt of this jet is larger than both jets
                // bump 1st to 2nd and put this jet in 1st
                if(d["Jet_pt"][pair.first]<d["Jet_pt"][i]){
                    pair.second=pair.first;
                    pair.first=i;
                } else if( pair.second == -1 ){
                    pair.second = i;
                // pt of this jet is between the two
                // replace 2nd jet
                } else if(d["Jet_pt"][pair.second]<d["Jet_pt"][i]){
                    pair.second=i;
                }
                // else the jet is lower pt than both the selected jets
            }
        }
    }

    return pair;
}

