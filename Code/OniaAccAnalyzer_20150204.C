#include <iostream>
#include <TString.h>
#include <TH1.h>
#include <TH2.h>
#include <TTree.h>
#include <TFile.h>
#include <TChain.h>
#include <TNtuple.h>
#include <TMath.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <TROOT.h>
#include <TAxis.h>
#include <cmath>
#include <TLorentzRotation.h>
//### NOTICE ###
// - Basically in this code all value(especially rapidity and eta) means in LAB frame(not CM frame!).

const double shiftvar = -0.47; // conversion constant y=0(CM)==y=-0.47(LAB frame)  
const double m_mu = .105658; // muon mass (GeV)
const double m_kaonplus = .49360; // K+(kaon plus/minus) mass (GeV)

std::string datestring; // for remark of the files

bool passBCut(int pdgId);

//bool CCut_B(int BCode, int BpJpmup_ch, int BpJpmum_ch, int Bp_ch, int BpKp_ch);
bool CCut_B(int BCode, int BpJpmup_Id, int BpJpmum_Id, int BpKpdau0_Id, int BpKpdau1_Id, int BpKp_Id, int Bp_Id);

bool passCut_Jpmu(bool, double muPt, double muEta, double muP);
bool passCut_JpKp(bool isBoosted, double muPt, double muEta, int Kpdau0Code, int Kpdau1Code, double Ptcut);

bool passCut_JpKpdau(int Kpdau0Code, int Kpdau1Code, bool isBoosted, double BpKpdau0_pt, double BpKpdau0_eta, double BpKpdau1_pt, double BpKpdau1_eta, double Ptcut);

//### for B+->J/psi+K+ channel

//void OniaAccAnalyzer_20150204(int optionBinning = 1, int optionA = 1, int optionB = 1, double varmag = 0.0, std::string stringA = "PtYbinBp", bool isBoosted = true, int OniaCode = 521, int Kpdau0Code = -999, int Kpdau1Code = -999, int KpCode = 321, std::string remarkstring = "BanaAcc_IDfilters_intot100M_20150204_default", double dimuptbinmin=0.0, double dimuptbinmax=100.0, double rapbinmin=-2.4, double rapbinmax=2.4, double Bptbinmin=10.0, double Bptbinmax=60.0, double Brapbinmin=-2.4, double Brapbinmax=2.4, int BCode=521, double trkptcut=0.9){

//void OniaAccAnalyzer_20150204(int optionBinning = 1, int optionA = 1, int optionB = 100, double varmag = 0.0, std::string stringA = "PtYbinBp", bool isBoosted = true, int OniaCode = 521, int Kpdau0Code = -999, int Kpdau1Code = -999, int KpCode = 321, std::string remarkstring = "BanaAcc_IDfilters_intot100M_20150204", double dimuptbinmin=0.0, double dimuptbinmax=100.0, double rapbinmin=-2.4, double rapbinmax=2.4, double Bptbinmin=10.0, double Bptbinmax=60.0, double Brapbinmin=-2.4, double Brapbinmax=2.4, int BCode=521, double trkptcut=0.9){

void OniaAccAnalyzer_20150204(int optionBinning = 1, int optionA = 1, int optionB = 200, double varmag = 0.0, std::string stringA = "PtYbinBp", bool isBoosted = true, int OniaCode = 521, int Kpdau0Code = -999, int Kpdau1Code = -999, int KpCode = 321, std::string remarkstring = "BanaAcc_IDfilters_intot100M_20150204", double dimuptbinmin=0.0, double dimuptbinmax=100.0, double rapbinmin=-2.4, double rapbinmax=2.4, double Bptbinmin=10.0, double Bptbinmax=60.0, double Brapbinmin=-2.4, double Brapbinmax=2.4, int BCode=521, double trkptcut=0.9){


	std::string Bx;

	switch (BCode)
	{
		case 511 : {Bx="B^{0}";break;}
		case 521 : {Bx="B^{+}";break;}
		case 531 : {Bx="B_{s}^{0}";break;}
							 break;
	}

	int optionCS;
	std::string stringB;

	if (optionB == 1) {stringB="_nowt_";}

	if (optionB == 100) {stringB="_Toy1stpolPtVar_";}
	if (optionB == 200) {stringB="_Toy2ndpolYvar_";}


	gROOT->Macro("rootlogon_2D.C+");
	//gROOT->Macro("rootlogon.C+");

	gStyle->SetCanvasDefW(800);
	gStyle->SetCanvasDefH(800);

	TFile *f1;
	//char* samplename;
	std::string samplename;


	double minylab;
	double maxylab;
	double mindimumass;
	double maxdimumass;
	double mindtrimass;
	double maxdtrimass;

	//## OniaCode(Bottom) : 1,4,5 - 1S, 2,6,7 - 2S, 3,8,9 - 3S
	if (isBoosted) {
		minylab=-2.4;
		maxylab=1.46;
		if (OniaCode==521) {
			f1 = new TFile("../InputAccFiles/PYTHIA6_BuJpsiK_5TeV_boost_IDfilter_intot100Mevt_MuonAnav5.root");
			samplename="Bu_boosted_";mindimumass=2.6;maxdimumass=3.5;mindtrimass=4.779;maxdtrimass=5.779;}		
/*
		else if (OniaCode==511) {
			f1 = new TFile("PYTHIA6_BdJpsiKstar_5TeV_boost_IDfilter_intot100Mevt_MuonAnav5.root");
			samplename="Bd_boosted_";mindimumass=2.6;maxdimumass=3.5;mindtrimass=4.780;maxdtrimass=5.780;}		
		else if (OniaCode==531) {
			f1 = new TFile("PYTHIA6_BsJpsiPhi_5TeV_boost_IDfilter_intot100Mevt_MuonAnav6.root");
			samplename="Bs_boosted_";mindimumass=2.6;maxdimumass=3.5;mindtrimass=4.967;maxdtrimass=5.967;}
*/
		else {std::cout << "### It is not valid OniaCode, please check again!" << std::endl;return;}
	}
	else {
		minylab=-2.4;
		maxylab=2.4;
		std::cout << "### For non-boosting, we don't have any input file. Please add first!" << std::endl;return;
	}
	datestring = stringA+stringB+samplename+remarkstring;

	TFile *outfile = new TFile(Form("../Results/AccAna_%s.root",datestring.c_str()),"RECREATE");
	std::cout << "datestring.c_str(): " << datestring.c_str() << std::endl;

	TTree *ana2 = (TTree*)f1->Get("DiAna");

	int dkid0_Id, dkid0_ch, dkid0_st;	
	int dkid1_Id, dkid1_ch, dkid1_st;
	double dkid0_pt, dkid0_eta, dkid0_y, dkid0_px, dkid0_py, dkid0_pz, dkid0_phi, dkid0_mass, dkid0_p;
	double dkid1_pt, dkid1_eta, dkid1_y, dkid1_px, dkid1_py, dkid1_pz, dkid1_phi, dkid1_mass, dkid1_p;
	double dimu_pt, dimu_eta, dimu_y, dimu_px, dimu_py, dimu_pz, dimu_phi, dimu_mass, dimu_p;
	int dmom0_Id, dmom0_ch, dmom0_st;	
	int dgmom0_Id, dgmom0_ch, dgmom0_st;
	double dmom0_pt, dmom0_eta, dmom0_y, dmom0_px, dmom0_py, dmom0_pz, dmom0_phi, dmom0_mass, dmom0_p;
	double dgmom0_pt, dgmom0_eta, dgmom0_y, dgmom0_px, dgmom0_py, dgmom0_pz, dgmom0_phi, dgmom0_mass, dgmom0_p;

	TTree *anaB = (TTree*)f1->Get("ExclBAna");

	double Bp_pt, Bp_eta, Bp_rap, Bp_phi;
	int Bp_Id, Bp_st, Bp_ch;
	double BpJp_pt, BpJp_eta, BpJp_rap, BpJp_phi;
	int BpJp_Id, BpJp_st, BpJp_ch;
	double BpKp_pt, BpKp_eta, BpKp_rap, BpKp_phi;
	int BpKp_Id, BpKp_st, BpKp_ch;
	double BpJpmup_pt, BpJpmup_eta, BpJpmup_rap, BpJpmup_phi, BpJpmup_p;
	int BpJpmup_Id, BpJpmup_st, BpJpmup_ch;
	double BpJpmum_pt, BpJpmum_eta, BpJpmum_rap, BpJpmum_phi, BpJpmum_p;
	int BpJpmum_Id, BpJpmum_st, BpJpmum_ch;

	double BpKpdau0_pt, BpKpdau0_eta, BpKpdau0_rap, BpKpdau0_phi, BpKpdau0_p, BpKpdau0_m;
	int BpKpdau0_Id, BpKpdau0_st, BpKpdau0_ch;
	double BpKpdau1_pt, BpKpdau1_eta, BpKpdau1_rap, BpKpdau1_phi, BpKpdau1_p, BpKpdau1_m;
	int BpKpdau1_Id, BpKpdau1_st, BpKpdau1_ch;

	double dtri_pt, dtri_eta, dtri_y, dtri_px, dtri_py, dtri_pz, dtri_phi, dtri_mass, dtri_p;

	using namespace std;

	// Definition of bin

	Int_t nYBins;
	Int_t nPtBins;
	Float_t afYBins[10];
	Float_t afPtBins[10];

	Int_t nMuPtBins = 6;
	Float_t afMuPtBins[7] = {0., 1.5, 3.0, 6.0, 9.0, 20.0, 30.0};
	Int_t nMuEtaBins = 5;
	Float_t afMuEtaBins[6] = {-2.4, -0.94, -0.47, 0, 1.46, 2.4};

	switch (optionBinning)
	{
		// case 0 : 1 bin
		case 0 : {
							 nPtBins = 1;
							 afPtBins[0] = 10.0;
							 afPtBins[1] = 60.0;
							 nYBins = 1;
							 afYBins[0] = -2.4;
							 afYBins[1] = 2.4;
							 break;
						 }
						 // case 1 : YPt binning
		case 1 : {
							 nPtBins = 5;
							 afPtBins[0] = 10.0;
							 afPtBins[1] = 15.0;
							 afPtBins[2] = 20.0;
							 afPtBins[3] = 25.0;
							 afPtBins[4] = 30.0;
							 afPtBins[5] = 60.0;
							 nYBins = 5;
							 afYBins[0] = -2.4;
							 afYBins[1] = -1.465;
							 afYBins[2] = -0.465;
							 afYBins[3] = 0.535;
							 afYBins[4] = 1.470;
							 afYBins[5] = 2.4;
							 break;

						 }
						 // case 2 : rap binning
		case 2 : {
							 nPtBins = 1;
							 afPtBins[0] = 10.0;
							 afPtBins[1] = 60.0;
							 nYBins = 5;
							 afYBins[0] = -2.4;
							 afYBins[1] = -1.465;
							 afYBins[2] = -0.465;
							 afYBins[3] = 0.535;
							 afYBins[4] = 1.470;
							 afYBins[5] = 2.4;
							 break;
						 }
						 // case 3 : pT binning
		case 3 : {
							 nPtBins = 5;
							 afPtBins[0] = 10.0;
							 afPtBins[1] = 15.0;
							 afPtBins[2] = 20.0;
							 afPtBins[3] = 25.0;
							 afPtBins[4] = 30.0;
							 afPtBins[5] = 60.0;
							 nYBins = 1;
							 afYBins[0] = -2.4;
							 afYBins[1] = 2.4;
							 break;
						 }
		case 11 : {
								nPtBins = 3;
								afPtBins[0] = 10.0;
								afPtBins[1] = 15.0;
								afPtBins[2] = 20.0;
								afPtBins[3] = 60.0;
								nYBins = 5;
								afYBins[0] = -2.4;
								afYBins[1] = -1.465;
								afYBins[2] = -0.465;
								afYBins[3] = 0.535;
								afYBins[4] = 1.470;
								afYBins[5] = 2.4;
								break;

							}
							// case 2 : rap binning
		case 12 : {
								nPtBins = 1;
								afPtBins[0] = 10.0;
								afPtBins[1] = 60.0;
								nYBins = 5;
								afYBins[0] = -2.4;
								afYBins[1] = -1.465;
								afYBins[2] = -0.465;
								afYBins[3] = 0.535;
								afYBins[4] = 1.470;
								afYBins[5] = 2.4;
								break;
							}
							// case 3 : pT binning
		case 13 : {
								nPtBins = 3;
								afPtBins[0] = 10.0;
								afPtBins[1] = 15.0;
								afPtBins[2] = 20.0;
								afPtBins[3] = 60.0;
								nYBins = 1;
								afYBins[0] = -2.4;
								afYBins[1] = 2.4;
								break;
							}
		default : {
								nPtBins = 1;
								afPtBins[0] = 10.0;
								afPtBins[1] = 60.0;
								nYBins = 1;
								afYBins[0] = -2.4;
								afYBins[1] = 2.4;
								break;
							}
	}
	std::cout << "nPtBins : " << nPtBins << ", nYBins: " << nYBins << std::endl;

	ana2->SetBranchAddress("dimu_eta",	&dimu_eta);
	ana2->SetBranchAddress("dimu_mass",	&dimu_mass);
	ana2->SetBranchAddress("dimu_p",	&dimu_p);
	ana2->SetBranchAddress("dimu_phi",	&dimu_phi);
	ana2->SetBranchAddress("dimu_pt",	&dimu_pt);
	ana2->SetBranchAddress("dimu_px",	&dimu_px);
	ana2->SetBranchAddress("dimu_py",	&dimu_py);
	ana2->SetBranchAddress("dimu_pz",	&dimu_pz);
	ana2->SetBranchAddress("dimu_y",	&dimu_y);

	ana2->SetBranchAddress("dgmom0_Id",	&dgmom0_Id);
	ana2->SetBranchAddress("dgmom0_ch",	&dgmom0_ch);
	ana2->SetBranchAddress("dgmom0_eta",	&dgmom0_eta);
	ana2->SetBranchAddress("dgmom0_mass",	&dgmom0_mass);
	ana2->SetBranchAddress("dgmom0_p",	&dgmom0_p);
	ana2->SetBranchAddress("dgmom0_phi",	&dgmom0_phi);
	ana2->SetBranchAddress("dgmom0_pt",	&dgmom0_pt);
	ana2->SetBranchAddress("dgmom0_px",	&dgmom0_px);
	ana2->SetBranchAddress("dgmom0_py",	&dgmom0_py);
	ana2->SetBranchAddress("dgmom0_pz",	&dgmom0_pz);
	ana2->SetBranchAddress("dgmom0_st",	&dgmom0_st);
	ana2->SetBranchAddress("dgmom0_y",	&dgmom0_y);

	ana2->SetBranchAddress("dmom0_Id",	&dmom0_Id);
	ana2->SetBranchAddress("dmom0_ch",	&dmom0_ch);
	ana2->SetBranchAddress("dmom0_eta",	&dmom0_eta);
	ana2->SetBranchAddress("dmom0_mass",	&dmom0_mass);
	ana2->SetBranchAddress("dmom0_p",	&dmom0_p);
	ana2->SetBranchAddress("dmom0_phi",	&dmom0_phi);
	ana2->SetBranchAddress("dmom0_pt",	&dmom0_pt);
	ana2->SetBranchAddress("dmom0_px",	&dmom0_px);
	ana2->SetBranchAddress("dmom0_py",	&dmom0_py);
	ana2->SetBranchAddress("dmom0_pz",	&dmom0_pz);
	ana2->SetBranchAddress("dmom0_st",	&dmom0_st);
	ana2->SetBranchAddress("dmom0_y",	&dmom0_y);

	cout << "Entried of tree : " << ana2->GetEntries() << endl;
	std::cout << "datestring.c_str(): " << datestring.c_str() << std::endl;

	anaB->SetBranchAddress("Bp_pt",		&Bp_pt);
	anaB->SetBranchAddress("Bp_rap",	&Bp_rap);
	anaB->SetBranchAddress("Bp_Id",		&Bp_Id);
	anaB->SetBranchAddress("Bp_ch",		&Bp_ch);

	anaB->SetBranchAddress("BpJp_pt",	&BpJp_pt);
	anaB->SetBranchAddress("BpJp_rap",	&BpJp_rap);
	anaB->SetBranchAddress("BpJp_Id",	&BpJp_Id);
	anaB->SetBranchAddress("BpJp_ch",	&BpJp_ch);

	anaB->SetBranchAddress("BpKp_pt",	&BpKp_pt);
	anaB->SetBranchAddress("BpKp_eta",	&BpKp_eta);
	anaB->SetBranchAddress("BpKp_Id",	&BpKp_Id);
	anaB->SetBranchAddress("BpKp_ch",	&BpKp_ch);
	anaB->SetBranchAddress("BpKp_phi",	&BpKp_phi);

	anaB->SetBranchAddress("BpJpmup_pt",	&BpJpmup_pt);
	anaB->SetBranchAddress("BpJpmup_eta",	&BpJpmup_eta);
	anaB->SetBranchAddress("BpJpmup_Id",	&BpJpmup_Id);
	anaB->SetBranchAddress("BpJpmup_ch",	&BpJpmup_ch);
	anaB->SetBranchAddress("BpJpmup_phi",	&BpJpmup_phi);
	anaB->SetBranchAddress("BpJpmup_p",	&BpJpmup_p);

	anaB->SetBranchAddress("BpJpmum_pt",	&BpJpmum_pt);
	anaB->SetBranchAddress("BpJpmum_eta",	&BpJpmum_eta);
	anaB->SetBranchAddress("BpJpmum_Id",	&BpJpmum_Id);
	anaB->SetBranchAddress("BpJpmum_ch",	&BpJpmum_ch);
	anaB->SetBranchAddress("BpJpmum_phi",	&BpJpmum_phi);
	anaB->SetBranchAddress("BpJpmum_p",	&BpJpmum_p);

	anaB->SetBranchAddress("BpKpdau0_pt",	&BpKpdau0_pt);
	anaB->SetBranchAddress("BpKpdau0_eta",	&BpKpdau0_eta);
	anaB->SetBranchAddress("BpKpdau0_Id",	&BpKpdau0_Id);
	anaB->SetBranchAddress("BpKpdau0_ch",	&BpKpdau0_ch);
	anaB->SetBranchAddress("BpKpdau0_phi",	&BpKpdau0_phi);
	anaB->SetBranchAddress("BpKpdau0_p",	&BpKpdau0_p);
	anaB->SetBranchAddress("BpKpdau0_m",	&BpKpdau0_m);

	anaB->SetBranchAddress("BpKpdau1_pt",	&BpKpdau1_pt);
	anaB->SetBranchAddress("BpKpdau1_eta",	&BpKpdau1_eta);
	anaB->SetBranchAddress("BpKpdau1_Id",	&BpKpdau1_Id);
	anaB->SetBranchAddress("BpKpdau1_ch",	&BpKpdau1_ch);
	anaB->SetBranchAddress("BpKpdau1_phi",	&BpKpdau1_phi);
	anaB->SetBranchAddress("BpKpdau1_p",	&BpKpdau1_p);
	anaB->SetBranchAddress("BpKpdau1_m",	&BpKpdau1_m);

	////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////

	TH2F *hNocuYPt = new TH2F(Form("hNocuYPt_%s",stringA.c_str()),"",500,-2.5,2.5,300,0,30);
	TH2F *hDen1YPt = new TH2F(Form("hDen1YPt_%s",stringA.c_str()),"",500,-2.5,2.5,300,0,30);
	TH2F *hDen2YPt = new TH2F(Form("hDen2YPt_%s",stringA.c_str()),"",500,-2.5,2.5,300,0,30);
	TH2F *hNum1YPt = new TH2F(Form("hNum1YPt_%s",stringA.c_str()),"",500,-2.5,2.5,300,0,30);
	TH2F *hNum2YPt = new TH2F(Form("hNum2YPt_%s",stringA.c_str()),"",500,-2.5,2.5,300,0,30);
	TH2F *hAcc1YPt = new TH2F(Form("hAcc1YPt_%s",stringA.c_str()),"",500,-2.5,2.5,300,0,30);
	TH2F *hAcc2YPt = new TH2F(Form("hAcc2YPt_%s",stringA.c_str()),"",500,-2.5,2.5,300,0,30);

	TH2F *hNocuYPtBin = new TH2F(Form("hNocuYPtBin_%s",stringA.c_str()),"",nYBins,afYBins,nPtBins,afPtBins);
	TH2F *hDen1YPtBin = new TH2F(Form("hDen1YPtBin_%s",stringA.c_str()),"",nYBins,afYBins,nPtBins,afPtBins);
	TH2F *hDen2YPtBin = new TH2F(Form("hDen2YPtBin_%s",stringA.c_str()),"",nYBins,afYBins,nPtBins,afPtBins);
	TH2F *hNum1YPtBin = new TH2F(Form("hNum1YPtBin_%s",stringA.c_str()),"",nYBins,afYBins,nPtBins,afPtBins);
	TH2F *hNum2YPtBin = new TH2F(Form("hNum2YPtBin_%s",stringA.c_str()),"",nYBins,afYBins,nPtBins,afPtBins);
	TH2F *hAcc1YPtBin = new TH2F(Form("hAcc1YPtBin_%s",stringA.c_str()),"",nYBins,afYBins,nPtBins,afPtBins);
	TH2F *hAcc2YPtBin = new TH2F(Form("hAcc2YPtBin_%s",stringA.c_str()),"",nYBins,afYBins,nPtBins,afPtBins);

	TH2F *hNocuTriYPt = new TH2F(Form("hNocuTriYPt_%s",stringA.c_str()),"",50,-2.5,2.5,30,0,30);
	TH2F *hDen1TriYPt = new TH2F(Form("hDen1TriYPt_%s",stringA.c_str()),"",50,-2.5,2.5,30,0,30);
	TH2F *hDen2TriYPt = new TH2F(Form("hDen2TriYPt_%s",stringA.c_str()),"",50,-2.5,2.5,30,0,30);
	TH2F *hNum1TriYPt = new TH2F(Form("hNum1TriYPt_%s",stringA.c_str()),"",50,-2.5,2.5,30,0,30);
	TH2F *hNum2TriYPt = new TH2F(Form("hNum2TriYPt_%s",stringA.c_str()),"",50,-2.5,2.5,30,0,30);
	TH2F *hAcc1TriYPt = new TH2F(Form("hAcc1TriYPt_%s",stringA.c_str()),"",50,-2.5,2.5,30,0,30);
	TH2F *hAcc2TriYPt = new TH2F(Form("hAcc2TriYPt_%s",stringA.c_str()),"",50,-2.5,2.5,30,0,30);

	TH2F *hNocuTriYPtBin = new TH2F(Form("hNocuTriYPtBin_%s",stringA.c_str()),"",nYBins,afYBins,nPtBins,afPtBins);
	TH2F *hDen1TriYPtBin = new TH2F(Form("hDen1TriYPtBin_%s",stringA.c_str()),"",nYBins,afYBins,nPtBins,afPtBins);
	TH2F *hDen2TriYPtBin = new TH2F(Form("hDen2TriYPtBin_%s",stringA.c_str()),"",nYBins,afYBins,nPtBins,afPtBins);
	TH2F *hNum1TriYPtBin = new TH2F(Form("hNum1TriYPtBin_%s",stringA.c_str()),"",nYBins,afYBins,nPtBins,afPtBins);
	TH2F *hNum2TriYPtBin = new TH2F(Form("hNum2TriYPtBin_%s",stringA.c_str()),"",nYBins,afYBins,nPtBins,afPtBins);
	TH2F *hAcc1TriYPtBin = new TH2F(Form("hAcc1TriYPtBin_%s",stringA.c_str()),"",nYBins,afYBins,nPtBins,afPtBins);
	TH2F *hAcc2TriYPtBin = new TH2F(Form("hAcc2TriYPtBin_%s",stringA.c_str()),"",nYBins,afYBins,nPtBins,afPtBins);

	TH2F *hNocuYPtBin2 = new TH2F(Form("hNocuYPtBin2_%s",stringA.c_str()),"",1,-10,10,1,0,200);
	TH2F *hDen1YPtBin2 = new TH2F(Form("hDen1YPtBin2_%s",stringA.c_str()),"",1,-10,10,1,0,200);
	TH2F *hDen2YPtBin2 = new TH2F(Form("hDen2YPtBin2_%s",stringA.c_str()),"",1,-10,10,1,0,200);
	TH2F *hNum1YPtBin2 = new TH2F(Form("hNum1YPtBin2_%s",stringA.c_str()),"",1,-10,10,1,0,200);
	TH2F *hNum2YPtBin2 = new TH2F(Form("hNum2YPtBin2_%s",stringA.c_str()),"",1,-10,10,1,0,200);
	TH2F *hAcc1YPtBin2 = new TH2F(Form("hAcc1YPtBin2_%s",stringA.c_str()),"",1,-10,10,1,0,200);
	TH2F *hAcc2YPtBin2 = new TH2F(Form("hAcc2YPtBin2_%s",stringA.c_str()),"",1,-10,10,1,0,200);
	////////////////////////////////////////////////////////////////////////////////////////////////
	TH2F *hNocu_Diffgen_YPt = new TH2F(Form("hNocu_Diffgen_YPt_%s",stringA.c_str()),"",500,-2.5,2.5,300,0,30);
	TH2F *hDen1_Diffgen_YPt = new TH2F(Form("hDen1_Diffgen_YPt_%s",stringA.c_str()),"",500,-2.5,2.5,300,0,30);
	TH2F *hDen2_Diffgen_YPt = new TH2F(Form("hDen2_Diffgen_YPt_%s",stringA.c_str()),"",500,-2.5,2.5,300,0,30);
	TH2F *hNum1_Diffgen_YPt = new TH2F(Form("hNum1_Diffgen_YPt_%s",stringA.c_str()),"",500,-2.5,2.5,300,0,30);
	TH2F *hNum2_Diffgen_YPt = new TH2F(Form("hNum2_Diffgen_YPt_%s",stringA.c_str()),"",500,-2.5,2.5,300,0,30);
	TH2F *hAcc1_Diffgen_YPt = new TH2F(Form("hAcc1_Diffgen_YPt_%s",stringA.c_str()),"",500,-2.5,2.5,300,0,30);
	TH2F *hAcc2_Diffgen_YPt = new TH2F(Form("hAcc2_Diffgen_YPt_%s",stringA.c_str()),"",500,-2.5,2.5,300,0,30);

	TH2F *hNocu_Diffgen_YPtBin = new TH2F(Form("hNocu_Diffgen_YPtBin_%s",stringA.c_str()),"",nYBins,afYBins,nPtBins,afPtBins);
	TH2F *hDen1_Diffgen_YPtBin = new TH2F(Form("hDen1_Diffgen_YPtBin_%s",stringA.c_str()),"",nYBins,afYBins,nPtBins,afPtBins);
	TH2F *hDen2_Diffgen_YPtBin = new TH2F(Form("hDen2_Diffgen_YPtBin_%s",stringA.c_str()),"",nYBins,afYBins,nPtBins,afPtBins);
	TH2F *hNum1_Diffgen_YPtBin = new TH2F(Form("hNum1_Diffgen_YPtBin_%s",stringA.c_str()),"",nYBins,afYBins,nPtBins,afPtBins);
	TH2F *hNum2_Diffgen_YPtBin = new TH2F(Form("hNum2_Diffgen_YPtBin_%s",stringA.c_str()),"",nYBins,afYBins,nPtBins,afPtBins);
	TH2F *hAcc1_Diffgen_YPtBin = new TH2F(Form("hAcc1_Diffgen_YPtBin_%s",stringA.c_str()),"",nYBins,afYBins,nPtBins,afPtBins);
	TH2F *hAcc2_Diffgen_YPtBin = new TH2F(Form("hAcc2_Diffgen_YPtBin_%s",stringA.c_str()),"",nYBins,afYBins,nPtBins,afPtBins);

	TH2F *hNocu_Diffgen_YPtBin2 = new TH2F(Form("hNocu_Diffgen_YPtBin2_%s",stringA.c_str()),"",1,-10,10,1,0,200);
	TH2F *hDen1_Diffgen_YPtBin2 = new TH2F(Form("hDen1_Diffgen_YPtBin2_%s",stringA.c_str()),"",1,-10,10,1,0,200);
	TH2F *hDen2_Diffgen_YPtBin2 = new TH2F(Form("hDen2_Diffgen_YPtBin2_%s",stringA.c_str()),"",1,-10,10,1,0,200);
	TH2F *hNum1_Diffgen_YPtBin2 = new TH2F(Form("hNum1_Diffgen_YPtBin2_%s",stringA.c_str()),"",1,-10,10,1,0,200);
	TH2F *hNum2_Diffgen_YPtBin2 = new TH2F(Form("hNum2_Diffgen_YPtBin2_%s",stringA.c_str()),"",1,-10,10,1,0,200);
	TH2F *hAcc1_Diffgen_YPtBin2 = new TH2F(Form("hAcc1_Diffgen_YPtBin2_%s",stringA.c_str()),"",1,-10,10,1,0,200);
	TH2F *hAcc2_Diffgen_YPtBin2 = new TH2F(Form("hAcc2_Diffgen_YPtBin2_%s",stringA.c_str()),"",1,-10,10,1,0,200);

	TH1F *hNocuY = new TH1F(Form("hNocuY_%s",stringA.c_str()),"",50,-2.5,2.5);
	TH1F *hNocuPt = new TH1F(Form("hNocuPt_%s",stringA.c_str()),"",100,0,100);
	TH1F *hDen1Y = new TH1F(Form("hDen1Y_%s",stringA.c_str()),"",50,-2.5,2.5);
	TH1F *hDen1Pt = new TH1F(Form("hDen1Pt_%s",stringA.c_str()),"",100,0,100);
	TH1F *hNum1Y = new TH1F(Form("hNum1Y_%s",stringA.c_str()),"",50,-2.5,2.5);
	TH1F *hNum1Pt = new TH1F(Form("hNum1Pt_%s",stringA.c_str()),"",100,0,100);
	TH1F *hAcc1Y = new TH1F(Form("hAcc1Y_%s",stringA.c_str()),"",50,-2.5,2.5);
	TH1F *hAcc1Pt = new TH1F(Form("hAcc1Pt_%s",stringA.c_str()),"",100,0,100);

	TH1F *hNocuYBin = new TH1F(Form("hNocuYBin_%s",stringA.c_str()),"",nYBins,afYBins);
	TH1F *hNocuPtBin = new TH1F(Form("hNocuPtBin_%s",stringA.c_str()),"",nPtBins,afPtBins);
	TH1F *hDen1YBin = new TH1F(Form("hDen1YBin_%s",stringA.c_str()),"",nYBins,afYBins);
	TH1F *hDen1PtBin = new TH1F(Form("hDen1PtBin_%s",stringA.c_str()),"",nPtBins,afPtBins);
	TH1F *hNum1YBin = new TH1F(Form("hNum1YBin_%s",stringA.c_str()),"",nYBins,afYBins);
	TH1F *hNum1PtBin = new TH1F(Form("hNum1PtBin_%s",stringA.c_str()),"",nPtBins,afPtBins);
	TH1F *hAcc1YBin = new TH1F(Form("hAcc1YBin_%s",stringA.c_str()),"",nYBins,afYBins);
	TH1F *hAcc1PtBin = new TH1F(Form("hAcc1PtBin_%s",stringA.c_str()),"",nPtBins,afPtBins);

	TH1F *hNocuYBin2 = new TH1F(Form("hNocuYBin2_%s",stringA.c_str()),"",1,-10.,10.);
	TH1F *hNocuPtBin2 = new TH1F(Form("hNocuPtBin2_%s",stringA.c_str()),"",1,0,200.);
	TH1F *hDen1YBin2 = new TH1F(Form("hDen1YBin2_%s",stringA.c_str()),"",1,-10.,10.);
	TH1F *hDen1PtBin2 = new TH1F(Form("hDen1PtBin2_%s",stringA.c_str()),"",1,0,200.);
	TH1F *hNum1YBin2 = new TH1F(Form("hNum1YBin2_%s",stringA.c_str()),"",1,-10.,10.);
	TH1F *hNum1PtBin2 = new TH1F(Form("hNum1PtBin2_%s",stringA.c_str()),"",1,0,200.);
	TH1F *hAcc1YBin2 = new TH1F(Form("hAcc1YBin2_%s",stringA.c_str()),"",1,-10.,10.);
	TH1F *hAcc1PtBin2 = new TH1F(Form("hAcc1PtBin2_%s",stringA.c_str()),"",1,0,200.);

	TH1F *hNocuTriY = new TH1F(Form("hNocuTriY_%s",stringA.c_str()),"",50,-2.5,2.5);
	TH1F *hNocuTriPt = new TH1F(Form("hNocuTriPt_%s",stringA.c_str()),"",14,0,70);
	TH1F *hDen1TriY = new TH1F(Form("hDen1TriY_%s",stringA.c_str()),"",50,-2.5,2.5);
	TH1F *hDen1TriPt = new TH1F(Form("hDen1TriPt_%s",stringA.c_str()),"",14,0,70);
	TH1F *hNum1TriY = new TH1F(Form("hNum1TriY_%s",stringA.c_str()),"",50,-2.5,2.5);
	TH1F *hNum1TriPt = new TH1F(Form("hNum1TriPt_%s",stringA.c_str()),"",14,0,70);
	TH1F *hAcc1TriY = new TH1F(Form("hAcc1TriY_%s",stringA.c_str()),"",50,-2.5,2.5);
	TH1F *hAcc1TriPt = new TH1F(Form("hAcc1TriPt_%s",stringA.c_str()),"",14,0,70);

	TH1F *hNocuTriYBin = new TH1F(Form("hNocuTriYBin_%s",stringA.c_str()),"",nYBins,afYBins);
	TH1F *hNocuTriPtBin = new TH1F(Form("hNocuTriPtBin_%s",stringA.c_str()),"",nPtBins,afPtBins);
	TH1F *hDen1TriYBin = new TH1F(Form("hDen1TriYBin_%s",stringA.c_str()),"",nYBins,afYBins);
	TH1F *hDen1TriPtBin = new TH1F(Form("hDen1TriPtBin_%s",stringA.c_str()),"",nPtBins,afPtBins);
	TH1F *hNum1TriYBin = new TH1F(Form("hNum1TriYBin_%s",stringA.c_str()),"",nYBins,afYBins);
	TH1F *hNum1TriPtBin = new TH1F(Form("hNum1TriPtBin_%s",stringA.c_str()),"",nPtBins,afPtBins);
	TH1F *hAcc1TriYBin = new TH1F(Form("hAcc1TriYBin_%s",stringA.c_str()),"",nYBins,afYBins);
	TH1F *hAcc1TriPtBin = new TH1F(Form("hAcc1TriPtBin_%s",stringA.c_str()),"",nPtBins,afPtBins);

	TH1F *hNocuTriYBin2 = new TH1F(Form("hNocuTriYBin2_%s",stringA.c_str()),"",1,-10.,10.);
	TH1F *hNocuTriPtBin2 = new TH1F(Form("hNocuTriPtBin2_%s",stringA.c_str()),"",1,0,200.);
	TH1F *hDen1TriYBin2 = new TH1F(Form("hDen1TriYBin2_%s",stringA.c_str()),"",1,-10.,10.);
	TH1F *hDen1TriPtBin2 = new TH1F(Form("hDen1TriPtBin2_%s",stringA.c_str()),"",1,0,200.);
	TH1F *hNum1TriYBin2 = new TH1F(Form("hNum1TriYBin2_%s",stringA.c_str()),"",1,-10.,10.);
	TH1F *hNum1TriPtBin2 = new TH1F(Form("hNum1TriPtBin2_%s",stringA.c_str()),"",1,0,200.);
	TH1F *hAcc1TriYBin2 = new TH1F(Form("hAcc1TriYBin2_%s",stringA.c_str()),"",1,-10.,10.);
	TH1F *hAcc1TriPtBin2 = new TH1F(Form("hAcc1TriPtBin2_%s",stringA.c_str()),"",1,0,200.);

	TH2F *hNocuSmuEtaPt = new TH2F(Form("hNocuSmuEtaPt_%s",stringA.c_str()),"",500,-2.5,2.5,300,0,30);
	TH2F *hDen1SmuEtaPt = new TH2F(Form("hDen1SmuEtaPt_%s",stringA.c_str()),"",500,-2.5,2.5,300,0,30);
	TH2F *hDen2SmuEtaPt = new TH2F(Form("hDen2SmuEtaPt_%s",stringA.c_str()),"",500,-2.5,2.5,300,0,30);
	TH2F *hNum1SmuEtaPt = new TH2F(Form("hNum1SmuEtaPt_%s",stringA.c_str()),"",500,-2.5,2.5,300,0,30);
	TH2F *hNum2SmuEtaPt = new TH2F(Form("hNum2SmuEtaPt_%s",stringA.c_str()),"",500,-2.5,2.5,300,0,30);
	TH2F *hAcc1SmuEtaPt = new TH2F(Form("hAcc1SmuEtaPt_%s",stringA.c_str()),"",500,-2.5,2.5,300,0,30);
	TH2F *hAcc2SmuEtaPt = new TH2F(Form("hAcc2SmuEtaPt_%s",stringA.c_str()),"",500,-2.5,2.5,300,0,30);

	TH1F *hNocuSmuEta = new TH1F(Form("hNocuSmuEta_%s",stringA.c_str()),"",50,-2.5,2.5);
	TH1F *hNocuSmuPt = new TH1F(Form("hNocuSmuPt_%s",stringA.c_str()),"",300,0,30);
	TH1F *hDen1SmuEta = new TH1F(Form("hDen1SmuEta_%s",stringA.c_str()),"",50,-2.5,2.5);
	TH1F *hDen1SmuPt = new TH1F(Form("hDen1SmuPt_%s",stringA.c_str()),"",300,0,30);
	TH1F *hNum1SmuEta = new TH1F(Form("hNum1SmuEta_%s",stringA.c_str()),"",50,-2.5,2.5);
	TH1F *hNum1SmuPt = new TH1F(Form("hNum1SmuPt_%s",stringA.c_str()),"",300,0,30);
	TH1F *hDen2SmuEta = new TH1F(Form("hDen2SmuEta_%s",stringA.c_str()),"",50,-2.5,2.5);
	TH1F *hDen2SmuPt = new TH1F(Form("hDen2SmuPt_%s",stringA.c_str()),"",300,0,30);
	TH1F *hNum2SmuEta = new TH1F(Form("hNum2SmuEta_%s",stringA.c_str()),"",50,-2.5,2.5);
	TH1F *hNum2SmuPt = new TH1F(Form("hNum2SmuPt_%s",stringA.c_str()),"",300,0,30);
	TH1F *hAcc1SmuEta = new TH1F(Form("hAcc1SmuEta_%s",stringA.c_str()),"",50,-2.5,2.5);
	TH1F *hAcc1SmuPt = new TH1F(Form("hAcc1SmuPt_%s",stringA.c_str()),"",300,0,30);

	TH1F *hNocuSmuEtaBin = new TH1F(Form("hNocuSmuEtaBin_%s",stringA.c_str()),"",nMuEtaBins,afMuEtaBins);
	TH1F *hNocuSmuPtBin = new TH1F(Form("hNocuSmuPtBin_%s",stringA.c_str()),"",nMuPtBins,afMuPtBins);
	TH1F *hDen1SmuEtaBin = new TH1F(Form("hDen1SmuEtaBin_%s",stringA.c_str()),"",nMuEtaBins,afMuEtaBins);
	TH1F *hDen1SmuPtBin = new TH1F(Form("hDen1SmuPtBin_%s",stringA.c_str()),"",nMuPtBins,afMuPtBins);
	TH1F *hNum1SmuEtaBin = new TH1F(Form("hNum1SmuEtaBin_%s",stringA.c_str()),"",nMuEtaBins,afMuEtaBins);
	TH1F *hNum1SmuPtBin = new TH1F(Form("hNum1SmuPtBin_%s",stringA.c_str()),"",nMuPtBins,afMuPtBins);
	TH1F *hDen2SmuEtaBin = new TH1F(Form("hDen2SmuEtaBin_%s",stringA.c_str()),"",nMuEtaBins,afMuEtaBins);
	TH1F *hDen2SmuPtBin = new TH1F(Form("hDen2SmuPtBin_%s",stringA.c_str()),"",nMuPtBins,afMuPtBins);
	TH1F *hNum2SmuEtaBin = new TH1F(Form("hNum2SmuEtaBin_%s",stringA.c_str()),"",nMuEtaBins,afMuEtaBins);
	TH1F *hNum2SmuPtBin = new TH1F(Form("hNum2SmuPtBin_%s",stringA.c_str()),"",nMuPtBins,afMuPtBins);
	TH1F *hAcc1SmuEtaBin = new TH1F(Form("hAcc1SmuEtaBin_%s",stringA.c_str()),"",nMuEtaBins,afMuEtaBins);
	TH1F *hAcc1SmuPtBin = new TH1F(Form("hAcc1SmuPtBin_%s",stringA.c_str()),"",nMuPtBins,afMuPtBins);

	TH2F *hNocuSmuEtaPtBin = new TH2F(Form("hNocuSmuEtaPtBin_%s",stringA.c_str()),"",nMuEtaBins,afMuEtaBins,nMuPtBins,afMuPtBins);
	TH2F *hDen1SmuEtaPtBin = new TH2F(Form("hDen1SmuEtaPtBin_%s",stringA.c_str()),"",nMuEtaBins,afMuEtaBins,nMuPtBins,afMuPtBins);
	TH2F *hDen2SmuEtaPtBin = new TH2F(Form("hDen2SmuEtaPtBin_%s",stringA.c_str()),"",nMuEtaBins,afMuEtaBins,nMuPtBins,afMuPtBins);
	TH2F *hNum1SmuEtaPtBin = new TH2F(Form("hNum1SmuEtaPtBin_%s",stringA.c_str()),"",nMuEtaBins,afMuEtaBins,nMuPtBins,afMuPtBins);
	TH2F *hNum2SmuEtaPtBin = new TH2F(Form("hNum2SmuEtaPtBin_%s",stringA.c_str()),"",nMuEtaBins,afMuEtaBins,nMuPtBins,afMuPtBins);
	TH2F *hAcc1SmuEtaPtBin = new TH2F(Form("hAcc1SmuEtaPtBin_%s",stringA.c_str()),"",nMuEtaBins,afMuEtaBins,nMuPtBins,afMuPtBins);

	std::cout << "TH1, TH2 definition, datestring.c_str(): " << datestring.c_str() << std::endl;

	TFile* fin_AccDef = new TFile("../InputAccFiles/AccAna_PtYbinBp_nowt_Bu_boosted_BanaAcc_IDfilters_intot100M_20150204_default.root");

	TH1F* hAcc1TriPtBin_AccDef = (TH1F*)fin_AccDef->Get("hAcc1TriPtBin_PtYbinBp");
	TH1F* hAcc1TriYBin_AccDef	= (TH1F*)fin_AccDef->Get("hAcc1TriYBin_PtYbinBp");

	TH1D *hAccCompBin1 = new TH1D("hAccCompBin1","",1000,hAcc1TriPtBin_AccDef->GetBinContent(1)-0.0500,hAcc1TriPtBin_AccDef->GetBinContent(1)+0.0500);
	TH1D *hAccCompBin2 = new TH1D("hAccCompBin2","",1000,hAcc1TriPtBin_AccDef->GetBinContent(2)-0.0500,hAcc1TriPtBin_AccDef->GetBinContent(2)+0.0500);
	TH1D *hAccCompBin3 = new TH1D("hAccCompBin3","",1000,hAcc1TriPtBin_AccDef->GetBinContent(3)-0.0500,hAcc1TriPtBin_AccDef->GetBinContent(3)+0.0500);
	TH1D *hAccCompBin4 = new TH1D("hAccCompBin4","",1000,hAcc1TriPtBin_AccDef->GetBinContent(4)-0.0500,hAcc1TriPtBin_AccDef->GetBinContent(4)+0.0500);
	TH1D *hAccCompBin5 = new TH1D("hAccCompBin5","",1000,hAcc1TriPtBin_AccDef->GetBinContent(5)-0.0500,hAcc1TriPtBin_AccDef->GetBinContent(5)+0.0500);
	TH1D *hAccYCompBin1 = new TH1D("hAccYCompBin1","",1000,hAcc1TriYBin_AccDef->GetBinContent(1)-0.0500,hAcc1TriYBin_AccDef->GetBinContent(1)+0.0500);
	TH1D *hAccYCompBin2 = new TH1D("hAccYCompBin2","",1000,hAcc1TriYBin_AccDef->GetBinContent(2)-0.0500,hAcc1TriYBin_AccDef->GetBinContent(2)+0.0500);
	TH1D *hAccYCompBin3 = new TH1D("hAccYCompBin3","",1000,hAcc1TriYBin_AccDef->GetBinContent(3)-0.0500,hAcc1TriYBin_AccDef->GetBinContent(3)+0.0500);
	TH1D *hAccYCompBin4 = new TH1D("hAccYCompBin4","",1000,hAcc1TriYBin_AccDef->GetBinContent(4)-0.0500,hAcc1TriYBin_AccDef->GetBinContent(4)+0.0500);
	TH1D *hAccYCompBin5 = new TH1D("hAccYCompBin5","",1000,hAcc1TriYBin_AccDef->GetBinContent(5)-0.0500,hAcc1TriYBin_AccDef->GetBinContent(5)+0.0500);

	/*
		 TH1D *hAccCompBin1 = new TH1D("hAccCompBin1","",1000,0.2798-0.0500,0.2798+0.0500);
		 TH1D *hAccCompBin2 = new TH1D("hAccCompBin2","",1000,0.4492-0.0500,0.4492+0.0500);
		 TH1D *hAccCompBin3 = new TH1D("hAccCompBin3","",1000,0.5383-0.0500,0.5383+0.0500);
		 TH1D *hAccCompBin4 = new TH1D("hAccCompBin4","",1000,0.6615-0.0500,0.6615+0.0500);
		 TH1D *hAccCompBin5 = new TH1D("hAccCompBin5","",1000,0.7412-0.0500,0.7412+0.0500);
		 TH1D *hAccYCompBin1 = new TH1D("hAccYCompBin1","",1000,0.3939-0.0500,0.3939+0.0500);
		 TH1D *hAccYCompBin2 = new TH1D("hAccYCompBin2","",1000,0.3608-0.0500,0.3608+0.0500);
		 TH1D *hAccYCompBin3 = new TH1D("hAccYCompBin3","",1000,0.2568-0.0500,0.2568+0.0500);
		 TH1D *hAccYCompBin4 = new TH1D("hAccYCompBin4","",1000,0.3617-0.0500,0.3617+0.0500);
		 TH1D *hAccYCompBin5 = new TH1D("hAccYCompBin5","",1000,0.3892-0.0500,0.3892+0.0500);
	 */
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	hDen1YPt->Sumw2();
	hDen2YPt->Sumw2();
	hNum1YPt->Sumw2();
	hNum2YPt->Sumw2();

	hDen1YPtBin->Sumw2();
	hDen2YPtBin->Sumw2();
	hNum1YPtBin->Sumw2();
	hNum2YPtBin->Sumw2();

	hDen1TriYPt->Sumw2();
	hDen2TriYPt->Sumw2();
	hNum1TriYPt->Sumw2();
	hNum2TriYPt->Sumw2();

	hDen1TriYPtBin->Sumw2();
	hDen2TriYPtBin->Sumw2();
	hNum1TriYPtBin->Sumw2();
	hNum2TriYPtBin->Sumw2();

	hDen1YPtBin2->Sumw2();
	hDen2YPtBin2->Sumw2();
	hNum1YPtBin2->Sumw2();
	hNum2YPtBin2->Sumw2();

	hDen1_Diffgen_YPt->Sumw2();
	hDen2_Diffgen_YPt->Sumw2();
	hNum1_Diffgen_YPt->Sumw2();
	hNum2_Diffgen_YPt->Sumw2();

	hDen1_Diffgen_YPtBin->Sumw2();
	hDen2_Diffgen_YPtBin->Sumw2();
	hNum1_Diffgen_YPtBin->Sumw2();
	hNum2_Diffgen_YPtBin->Sumw2();

	hDen1_Diffgen_YPtBin2->Sumw2();
	hDen2_Diffgen_YPtBin2->Sumw2();
	hNum1_Diffgen_YPtBin2->Sumw2();
	hNum2_Diffgen_YPtBin2->Sumw2();

	hDen1Y->Sumw2();
	hDen1Pt->Sumw2();
	hNum1Y->Sumw2();
	hNum1Pt->Sumw2();

	hDen1TriY->Sumw2();
	hDen1TriPt->Sumw2();
	hNum1TriY->Sumw2();
	hNum1TriPt->Sumw2();

	hDen1YBin->Sumw2();
	hDen1PtBin->Sumw2();
	hNum1YBin->Sumw2();
	hNum1PtBin->Sumw2();

	hDen1YBin2->Sumw2();
	hDen1PtBin2->Sumw2();
	hNum1YBin2->Sumw2();
	hNum1PtBin2->Sumw2();

	hDen1TriYBin->Sumw2();
	hDen1TriPtBin->Sumw2();
	hNum1TriYBin->Sumw2();
	hNum1TriPtBin->Sumw2();

	hDen1TriYBin2->Sumw2();
	hDen1TriPtBin2->Sumw2();
	hNum1TriYBin2->Sumw2();
	hNum1TriPtBin2->Sumw2();

	hDen1SmuEtaPt->Sumw2();
	hDen2SmuEtaPt->Sumw2();
	hNum1SmuEtaPt->Sumw2();
	hNum2SmuEtaPt->Sumw2();

	hDen1SmuEta->Sumw2();
	hDen1SmuPt->Sumw2();
	hNum1SmuEta->Sumw2();
	hNum1SmuPt->Sumw2();

	hDen1SmuEtaPtBin->Sumw2();
	hDen2SmuEtaPtBin->Sumw2();
	hNum1SmuEtaPtBin->Sumw2();
	hNum2SmuEtaPtBin->Sumw2();

	hDen1SmuEtaBin->Sumw2();
	hDen1SmuPtBin->Sumw2();
	hNum1SmuEtaBin->Sumw2();
	hNum1SmuPtBin->Sumw2();

	////////////////////////////////////////////////////////////////////////////////////////////////

	//TFile *ratiofile = new TFile("test_20141001.root");
	//TFile *ratiofile = new TFile("test_20141001_try1k.root");
	//TFile *ratiofile = new TFile("test_20141001_gaus_try10k.root");
	TFile *ratiofile;
/*
	if (optionB==100) ratiofile = new TFile("test_20141031_gaus_try10k.root");
	if (optionB==200) ratiofile = new TFile("test_20141031_gaus_ydep_try10k.root");
	else ratiofile = new TFile("test_20141031_gaus_try10k.root");
*/
	if (optionB==100) ratiofile = new TFile("../Results/RunMCtoy_gaus_wBpluspt_try100k.root");
	if (optionB==200) ratiofile = new TFile("../Results/test_20141031_gaus_ydep_try10k.root");
	else ratiofile = new TFile("../Results/test_20141031_gaus_try10k.root");
	TTree *anar = (TTree*)ratiofile->Get("ditTree");
	double a1,a2;
	if (optionB==100) {
	anar->SetBranchAddress("a1", &a1);
	anar->SetBranchAddress("a2", &a2);
	}
	if (optionB==200) {
	anar->SetBranchAddress("ay1", &a1);
	anar->SetBranchAddress("ay2", &a2);
	}

	Float_t w_Diffgen;

	std::cout << "before check acc. datestring.c_str(): " << datestring.c_str() << std::endl;

	int ifin;
	if (optionB==100 || optionB==200) ifin=anar->GetEntries(); else ifin=1;
	//ifin=10;
	for(int vari=0;vari<ifin;vari++) {
		if (vari%1000==0) std::cout << "### vari : " << vari << " time: " << time(0) << std::endl;
		//std::cout << "### vari : " << vari << " time: " << time(0) << std::endl;
		double vara1=0.0;
		double vara2=0.0;
		if (optionB==100 || optionB==200) {anar->GetEntry(vari);vara1=a1;vara2=a2;}
		hDen1YPt->Reset();
		hDen2YPt->Reset();
		hNum1YPt->Reset();
		hNum2YPt->Reset();

		hDen1YPtBin->Reset();
		hDen2YPtBin->Reset();
		hNum1YPtBin->Reset();
		hNum2YPtBin->Reset();

		hDen1TriYPt->Reset();
		hDen2TriYPt->Reset();
		hNum1TriYPt->Reset();
		hNum2TriYPt->Reset();

		hDen1TriYPtBin->Reset();
		hDen2TriYPtBin->Reset();
		hNum1TriYPtBin->Reset();
		hNum2TriYPtBin->Reset();

		hDen1YPtBin2->Reset();
		hDen2YPtBin2->Reset();
		hNum1YPtBin2->Reset();
		hNum2YPtBin2->Reset();

		hDen1_Diffgen_YPt->Reset();
		hDen2_Diffgen_YPt->Reset();
		hNum1_Diffgen_YPt->Reset();
		hNum2_Diffgen_YPt->Reset();

		hDen1_Diffgen_YPtBin->Reset();
		hDen2_Diffgen_YPtBin->Reset();
		hNum1_Diffgen_YPtBin->Reset();
		hNum2_Diffgen_YPtBin->Reset();

		hDen1_Diffgen_YPtBin2->Reset();
		hDen2_Diffgen_YPtBin2->Reset();
		hNum1_Diffgen_YPtBin2->Reset();
		hNum2_Diffgen_YPtBin2->Reset();

		hDen1Y->Reset();
		hDen1Pt->Reset();
		hNum1Y->Reset();
		hNum1Pt->Reset();

		hDen1TriY->Reset();
		hDen1TriPt->Reset();
		hNum1TriY->Reset();
		hNum1TriPt->Reset();

		hDen1YBin->Reset();
		hDen1PtBin->Reset();
		hNum1YBin->Reset();
		hNum1PtBin->Reset();

		hDen1YBin2->Reset();
		hDen1PtBin2->Reset();
		hNum1YBin2->Reset();
		hNum1PtBin2->Reset();

		hDen1TriYBin->Reset();
		hDen1TriPtBin->Reset();
		hNum1TriYBin->Reset();
		hNum1TriPtBin->Reset();

		hDen1TriYBin2->Reset();
		hDen1TriPtBin2->Reset();
		hNum1TriYBin2->Reset();
		hNum1TriPtBin2->Reset();

		hDen1SmuEtaPt->Reset();
		hDen2SmuEtaPt->Reset();
		hNum1SmuEtaPt->Reset();
		hNum2SmuEtaPt->Reset();

		hDen1SmuEta->Reset();
		hDen1SmuPt->Reset();
		hNum1SmuEta->Reset();
		hNum1SmuPt->Reset();

		hDen1SmuEtaPtBin->Reset();
		hDen2SmuEtaPtBin->Reset();
		hNum1SmuEtaPtBin->Reset();
		hNum2SmuEtaPtBin->Reset();

		hDen1SmuEtaBin->Reset();
		hDen1SmuPtBin->Reset();
		hNum1SmuEtaBin->Reset();
		hNum1SmuPtBin->Reset();

		hAcc1YPt->Reset();
		hAcc1YPtBin->Reset();
		hAcc1TriYPt->Reset();
		hAcc1TriYPtBin->Reset();
		hAcc1YPtBin2->Reset();
		hAcc1_Diffgen_YPt->Reset();
		hAcc1_Diffgen_YPtBin->Reset();
		hAcc1_Diffgen_YPtBin2->Reset();
		hAcc1Y->Reset();
		hAcc1Pt->Reset();
		hAcc1TriY->Reset();
		hAcc1TriPt->Reset();
		hAcc1YBin->Reset();
		hAcc1PtBin->Reset();
		hAcc1YBin2->Reset();
		hAcc1PtBin2->Reset();
		hAcc1TriYBin->Reset();
		hAcc1TriPtBin->Reset();
		hAcc1TriYBin2->Reset();
		hAcc1TriPtBin2->Reset();
		hAcc1SmuEtaPt->Reset();
		hAcc1SmuEta->Reset();
		hAcc1SmuPt->Reset();
		hAcc1SmuEtaPtBin->Reset();
		hAcc1SmuEtaBin->Reset();
		hAcc1SmuPtBin->Reset();

		hNocuYPt->Reset();
		hNocuYPtBin->Reset();
		hNocuTriYPt->Reset();
		hNocuTriYPtBin->Reset();
		hNocuYPtBin2->Reset();
		hNocu_Diffgen_YPt->Reset();
		hNocu_Diffgen_YPtBin->Reset();
		hNocu_Diffgen_YPtBin2->Reset();
		hNocuY->Reset();
		hNocuPt->Reset();
		hNocuTriY->Reset();
		hNocuTriPt->Reset();
		hNocuYBin->Reset();
		hNocuPtBin->Reset();
		hNocuYBin2->Reset();
		hNocuPtBin2->Reset();
		hNocuTriYBin->Reset();
		hNocuTriPtBin->Reset();
		hNocuTriYBin2->Reset();
		hNocuTriPtBin2->Reset();
		hNocuSmuEtaPt->Reset();
		hNocuSmuEta->Reset();
		hNocuSmuPt->Reset();
		hNocuSmuEtaPtBin->Reset();
		hNocuSmuEtaBin->Reset();
		hNocuSmuPtBin->Reset();

		for(int i=0; i<anaB->GetEntries(); i++)
		{
			anaB->GetEntry(i);

			if (!CCut_B(BCode,BpJpmup_Id,BpJpmum_Id,BpKpdau0_Id,BpKpdau1_Id,BpKp_Id,Bp_Id)) continue; 

			TLorentzVector dkid0, dkid1, dimu;
			TLorentzVector dkpd0, dkpd1, dkpc;
			TLorentzVector dtri;

			dkid0.SetPtEtaPhiM(BpJpmup_pt,BpJpmup_eta,BpJpmup_phi,m_mu);
			dkid1.SetPtEtaPhiM(BpJpmum_pt,BpJpmum_eta,BpJpmum_phi,m_mu);
			dimu = dkid0 + dkid1;
			dimu_pt=dimu.Pt(); dimu_eta=dimu.Eta(); dimu_y=dimu.Rapidity(); dimu_px=dimu.Px(); dimu_py=dimu.Py(); dimu_pz=dimu.Pz(); dimu_phi=dimu.Phi(); dimu_mass=dimu.M(); dimu_p=dimu.P();

			if (BCode==521) {
				dkpc.SetPtEtaPhiM(BpKp_pt,BpKp_eta,BpKp_phi,m_kaonplus);
			}
			else {
				dkpd0.SetPtEtaPhiM(BpKpdau0_pt,BpKpdau0_eta,BpKpdau0_phi,BpKpdau0_m);
				dkpd1.SetPtEtaPhiM(BpKpdau1_pt,BpKpdau1_eta,BpKpdau1_phi,BpKpdau1_m);
				dkpc = dkpd0 + dkpd1;
			}
			dtri = dimu + dkpc;
			dtri_pt=dtri.Pt(); dtri_eta=dtri.Eta(); dtri_y=dtri.Rapidity(); dtri_px=dtri.Px(); dtri_py=dtri.Py(); dtri_pz=dtri.Pz(); dtri_phi=dtri.Phi(); dtri_mass=dtri.M(); dtri_p=dtri.P();

			if (optionB == 1) {w_Diffgen = 1;}
			if (optionB == 4) {w_Diffgen = (1/1.93)*TMath::Abs(dimu_y-shiftvar)+0.5;}//y 50% up
			if (optionB == 5) {w_Diffgen = -(1/1.93)*TMath::Abs(dimu_y-shiftvar)+1.5;}//y 50% down
			if (optionB == 41) {w_Diffgen = (1.6/1.93)*TMath::Abs(dimu_y-shiftvar)+0.2;}//y 80% up
			if (optionB == 51) {w_Diffgen = -(1.6/1.93)*TMath::Abs(dimu_y-shiftvar)+1.8;}//y 80% down
			if (optionB == 2) {w_Diffgen = 0.016*dtri_pt+0.44;} //pt 40% up
			if (optionB == 3) {w_Diffgen = -0.016*dtri_pt+1.56;} //pt 40% down
			if (optionB == 21) {w_Diffgen = 0.032*dtri_pt-0.12;}//pt 80% up
			if (optionB == 31) {w_Diffgen = -0.032*dtri_pt+2.12;}//down 80% down
			if (optionB == 100) {w_Diffgen = vara1*dtri_pt+vara2;}
			if (optionB == 200) {w_Diffgen = vara1*pow((dtri_y+0.465),2)+vara2;}

			hNocuYPt->Fill(dimu_y,dimu_pt);
			hNocuYPtBin->Fill(dimu_y,dimu_pt);
			hNocuYPtBin2->Fill(dimu_y,dimu_pt);

			hNocu_Diffgen_YPt->Fill(dimu_y,dimu_pt,w_Diffgen);
			hNocu_Diffgen_YPtBin->Fill(dimu_y,dimu_pt,w_Diffgen);
			hNocu_Diffgen_YPtBin2->Fill(dimu_y,dimu_pt,w_Diffgen);

			hNocuY->Fill(dimu_y);
			hNocuPt->Fill(dimu_pt);
			hNocuYBin->Fill(dimu_y);
			hNocuPtBin->Fill(dimu_pt);
			hNocuYBin2->Fill(dimu_y);
			hNocuPtBin2->Fill(dimu_pt);

			hNocuTriY->Fill(dtri_y,w_Diffgen);
			hNocuTriPt->Fill(dtri_pt,w_Diffgen);
			hNocuTriYPt->Fill(dtri_y,dtri_pt,w_Diffgen);
			hNocuTriYPtBin->Fill(dtri_y,dtri_pt,w_Diffgen);

			hNocuTriYBin->Fill(dtri_y,w_Diffgen);
			hNocuTriPtBin->Fill(dtri_pt,w_Diffgen);
			hNocuTriYBin2->Fill(dtri_y,w_Diffgen);
			hNocuTriPtBin2->Fill(dtri_pt,w_Diffgen);

			hNocuSmuEta->Fill(BpJpmup_eta,w_Diffgen);
			hNocuSmuPt->Fill(BpJpmup_pt,w_Diffgen);
			hNocuSmuEtaPt->Fill(BpJpmup_eta,BpJpmup_pt,w_Diffgen);
			hNocuSmuEta->Fill(BpJpmum_eta,w_Diffgen);
			hNocuSmuPt->Fill(BpJpmum_pt,w_Diffgen);
			hNocuSmuEtaPt->Fill(BpJpmum_eta,BpJpmum_pt,w_Diffgen);

			hNocuSmuEtaBin->Fill(BpJpmup_eta,w_Diffgen);
			hNocuSmuPtBin->Fill(BpJpmup_pt,w_Diffgen);
			hNocuSmuEtaPtBin->Fill(BpJpmup_eta,BpJpmup_pt,w_Diffgen);
			hNocuSmuEtaBin->Fill(BpJpmum_eta,w_Diffgen);
			hNocuSmuPtBin->Fill(BpJpmum_pt,w_Diffgen);
			hNocuSmuEtaPtBin->Fill(BpJpmum_eta,BpJpmum_pt,w_Diffgen);

			if (CCut_B(BCode,BpJpmup_Id,BpJpmum_Id,BpKpdau0_Id,BpKpdau1_Id,BpKp_Id,Bp_Id))
			{
				hDen2YPt->Fill(dimu_y,dimu_pt);
				hDen2YPtBin->Fill(dimu_y,dimu_pt);
				hDen2YPtBin2->Fill(dimu_y,dimu_pt);

				hDen2_Diffgen_YPt->Fill(dimu_y,dimu_pt,w_Diffgen);
				hDen2_Diffgen_YPtBin->Fill(dimu_y,dimu_pt,w_Diffgen);
				hDen2_Diffgen_YPtBin2->Fill(dimu_y,dimu_pt,w_Diffgen);

				hDen2SmuEta->Fill(BpJpmup_eta,w_Diffgen);
				hDen2SmuPt->Fill(BpJpmup_pt,w_Diffgen);
				hDen2SmuEtaPt->Fill(BpJpmup_eta,BpJpmup_pt,w_Diffgen);
				hDen2SmuEta->Fill(BpJpmum_eta,w_Diffgen);
				hDen2SmuPt->Fill(BpJpmum_pt,w_Diffgen);
				hDen2SmuEtaPt->Fill(BpJpmum_eta,BpJpmum_pt,w_Diffgen);

				hDen2SmuEtaBin->Fill(BpJpmup_eta,w_Diffgen);
				hDen2SmuPtBin->Fill(BpJpmup_pt,w_Diffgen);
				hDen2SmuEtaPtBin->Fill(BpJpmup_eta,BpJpmup_pt,w_Diffgen);
				hDen2SmuEtaBin->Fill(BpJpmum_eta,w_Diffgen);
				hDen2SmuPtBin->Fill(BpJpmum_pt,w_Diffgen);
				hDen2SmuEtaPtBin->Fill(BpJpmum_eta,BpJpmum_pt,w_Diffgen);

				bool yn = false;
				//if ( optionA == 77) {
				if (CCut_B(BCode,BpJpmup_Id,BpJpmum_Id,BpKpdau0_Id,BpKpdau1_Id,BpKp_Id,Bp_Id) && Bptbinmin<=dtri_pt && dtri_pt<Bptbinmax && Brapbinmin<= dtri_y && dtri_y<Brapbinmax) {yn = true;}
				//}
				if ( yn == true  )  
				{

					hDen1YPt->Fill(dimu_y,dimu_pt);
					hDen1YPtBin->Fill(dimu_y,dimu_pt);
					hDen1YPtBin2->Fill(dimu_y,dimu_pt);

					hDen1_Diffgen_YPt->Fill(dimu_y,dimu_pt,w_Diffgen);
					hDen1_Diffgen_YPtBin->Fill(dimu_y,dimu_pt,w_Diffgen);
					hDen1_Diffgen_YPtBin2->Fill(dimu_y,dimu_pt,w_Diffgen);

					hNum2YPt->Fill(dimu_y,dimu_pt);
					hNum2YPtBin->Fill(dimu_y,dimu_pt);
					hNum2YPtBin2->Fill(dimu_y,dimu_pt);

					hNum2_Diffgen_YPt->Fill(dimu_y,dimu_pt,w_Diffgen);
					hNum2_Diffgen_YPtBin->Fill(TMath::Abs(dimu_y),dimu_pt,w_Diffgen);
					hNum2_Diffgen_YPtBin2->Fill(TMath::Abs(dimu_y),dimu_pt,w_Diffgen);

					hNum2SmuEta->Fill(BpJpmup_eta,w_Diffgen);
					hNum2SmuPt->Fill(BpJpmup_pt,w_Diffgen);
					hNum2SmuEtaPt->Fill(BpJpmup_eta,BpJpmup_pt,w_Diffgen);
					hNum2SmuEta->Fill(BpJpmum_eta,w_Diffgen);
					hNum2SmuPt->Fill(BpJpmum_pt,w_Diffgen);
					hNum2SmuEtaPt->Fill(BpJpmum_eta,BpJpmum_pt,w_Diffgen);

					hNum2SmuEtaBin->Fill(BpJpmup_eta,w_Diffgen);
					hNum2SmuPtBin->Fill(BpJpmup_pt,w_Diffgen);
					hNum2SmuEtaPtBin->Fill(BpJpmup_eta,BpJpmup_pt,w_Diffgen);
					hNum2SmuEtaBin->Fill(BpJpmum_eta,w_Diffgen);
					hNum2SmuPtBin->Fill(BpJpmum_pt,w_Diffgen);
					hNum2SmuEtaPtBin->Fill(BpJpmum_eta,BpJpmum_pt,w_Diffgen);

					hDen1Y->Fill(dimu_y);
					hDen1Pt->Fill(dimu_pt);
					hDen1YBin->Fill(dimu_y);
					hDen1PtBin->Fill(dimu_pt);
					hDen1YBin2->Fill(dimu_y);
					hDen1PtBin2->Fill(dimu_pt);

					hDen1TriY->Fill(dtri_y,w_Diffgen);
					hDen1TriPt->Fill(dtri_pt,w_Diffgen);
					hDen1TriYPt->Fill(dtri_y,dtri_pt,w_Diffgen);
					hDen1TriYPtBin->Fill(dtri_y,dtri_pt,w_Diffgen);

					hDen1TriYBin->Fill(dtri_y,w_Diffgen);
					hDen1TriPtBin->Fill(dtri_pt,w_Diffgen);
					hDen1TriYBin2->Fill(dtri_y,w_Diffgen);
					hDen1TriPtBin2->Fill(dtri_pt,w_Diffgen);

					hDen1SmuEta->Fill(BpJpmup_eta,w_Diffgen);
					hDen1SmuPt->Fill(BpJpmup_pt,w_Diffgen);
					hDen1SmuEtaPt->Fill(BpJpmup_eta,BpJpmup_pt,w_Diffgen);
					hDen1SmuEta->Fill(BpJpmum_eta,w_Diffgen);
					hDen1SmuPt->Fill(BpJpmum_pt,w_Diffgen);
					hDen1SmuEtaPt->Fill(BpJpmum_eta,BpJpmum_pt,w_Diffgen);

					hDen1SmuEtaBin->Fill(BpJpmup_eta,w_Diffgen);
					hDen1SmuPtBin->Fill(BpJpmup_pt,w_Diffgen);
					hDen1SmuEtaPtBin->Fill(BpJpmup_eta,BpJpmup_pt,w_Diffgen);
					hDen1SmuEtaBin->Fill(BpJpmum_eta,w_Diffgen);
					hDen1SmuPtBin->Fill(BpJpmum_pt,w_Diffgen);
					hDen1SmuEtaPtBin->Fill(BpJpmum_eta,BpJpmum_pt,w_Diffgen);


					bool yn2 = false;
					if ( optionA == 1) {
						if (CCut_B(BCode,BpJpmup_Id,BpJpmum_Id,BpKpdau0_Id,BpKpdau1_Id,BpKp_Id,Bp_Id) && passCut_Jpmu(isBoosted,BpJpmup_pt,BpJpmup_eta,BpJpmup_p) && passCut_Jpmu(isBoosted,BpJpmum_pt,BpJpmum_eta,BpJpmum_p) && passCut_JpKp(isBoosted,BpKp_pt,BpKp_eta,Kpdau0Code,Kpdau1Code,trkptcut) && passCut_JpKpdau(Kpdau0Code,Kpdau1Code,isBoosted,BpKpdau0_pt,BpKpdau0_eta,BpKpdau1_pt,BpKpdau1_eta,trkptcut) && mindimumass<dimu_mass && dimu_mass<maxdimumass && mindtrimass<dtri_mass && dtri_mass<maxdtrimass && Bptbinmin<=dtri_pt && dtri_pt<Bptbinmax && Brapbinmin<= dtri_y && dtri_y<Brapbinmax && BpJpmup_pt>1.5 && BpJpmum_pt>1.5) {yn2 = true;}
					}
					if (yn2 == true)				
					{

						hNum1YPt->Fill(dimu_y,dimu_pt);
						hNum1YPtBin->Fill(dimu_y,dimu_pt);
						hNum1YPtBin2->Fill(dimu_y,dimu_pt);

						hNum1_Diffgen_YPt->Fill(dimu_y,dimu_pt,w_Diffgen);
						hNum1_Diffgen_YPtBin->Fill(dimu_y,dimu_pt,w_Diffgen);
						hNum1_Diffgen_YPtBin2->Fill(dimu_y,dimu_pt,w_Diffgen);

						hNum1Y->Fill(dimu_y);
						hNum1Pt->Fill(dimu_pt);
						hNum1YBin->Fill(dimu_y);
						hNum1PtBin->Fill(dimu_pt);
						hNum1YBin2->Fill(dimu_y);
						hNum1PtBin2->Fill(dimu_pt);

						hNum1TriY->Fill(dtri_y,w_Diffgen);
						hNum1TriPt->Fill(dtri_pt,w_Diffgen);
						hNum1TriYPtBin->Fill(dtri_y,dtri_pt,w_Diffgen);
						hNum1TriYPt->Fill(dtri_y,dtri_pt,w_Diffgen);

						hNum1TriYBin->Fill(dtri_y,w_Diffgen);
						hNum1TriPtBin->Fill(dtri_pt,w_Diffgen);
						hNum1TriYBin2->Fill(dtri_y,w_Diffgen);
						hNum1TriPtBin2->Fill(dtri_pt,w_Diffgen);

						hNum1SmuEta->Fill(BpJpmup_eta,w_Diffgen);
						hNum1SmuPt->Fill(BpJpmup_pt,w_Diffgen);
						hNum1SmuEtaPt->Fill(BpJpmup_eta,BpJpmup_pt,w_Diffgen);
						hNum1SmuEta->Fill(BpJpmum_eta,w_Diffgen);
						hNum1SmuPt->Fill(BpJpmum_pt,w_Diffgen);
						hNum1SmuEtaPt->Fill(BpJpmum_eta,BpJpmum_pt,w_Diffgen);

						hNum1SmuEtaBin->Fill(BpJpmup_eta,w_Diffgen);
						hNum1SmuPtBin->Fill(BpJpmup_pt,w_Diffgen);
						hNum1SmuEtaPtBin->Fill(BpJpmup_eta,BpJpmup_pt,w_Diffgen);
						hNum1SmuEtaBin->Fill(BpJpmum_eta,w_Diffgen);
						hNum1SmuPtBin->Fill(BpJpmum_pt,w_Diffgen);
						hNum1SmuEtaPtBin->Fill(BpJpmum_eta,BpJpmum_pt,w_Diffgen);

					}
				}
			}

		}//for


		hAcc1YPt->Divide(hNum1YPt,hDen1YPt,1,1,"B");
		hAcc2YPt->Divide(hNum2YPt,hDen2YPt,1,1,"B");
		hAcc1YPtBin->Divide(hNum1YPtBin,hDen1YPtBin,1,1,"B");
		hAcc2YPtBin->Divide(hNum2YPtBin,hDen2YPtBin,1,1,"B");
		hAcc1YPtBin2->Divide(hNum1YPtBin2,hDen1YPtBin2,1,1,"B");
		hAcc2YPtBin2->Divide(hNum2YPtBin2,hDen2YPtBin2,1,1,"B");

		hAcc1_Diffgen_YPt->Divide(hNum1_Diffgen_YPt,hDen1_Diffgen_YPt,1,1,"B");
		hAcc2_Diffgen_YPt->Divide(hNum2_Diffgen_YPt,hDen2_Diffgen_YPt,1,1,"B");
		hAcc1_Diffgen_YPtBin->Divide(hNum1_Diffgen_YPtBin,hDen1_Diffgen_YPtBin,1,1,"B");
		hAcc2_Diffgen_YPtBin->Divide(hNum2_Diffgen_YPtBin,hDen2_Diffgen_YPtBin,1,1,"B");
		hAcc1_Diffgen_YPtBin2->Divide(hNum1_Diffgen_YPtBin2,hDen1_Diffgen_YPtBin2,1,1,"B");
		hAcc2_Diffgen_YPtBin2->Divide(hNum2_Diffgen_YPtBin2,hDen2_Diffgen_YPtBin2,1,1,"B");

		hAcc1Y->Divide(hNum1Y,hDen1Y,1,1,"B");
		hAcc1Pt->Divide(hNum1Pt,hDen1Pt,1,1,"B");

		hAcc1TriY->Divide(hNum1TriY,hDen1TriY,1,1,"B");
		hAcc1TriPt->Divide(hNum1TriPt,hDen1TriPt,1,1,"B");
		hAcc1TriYPt->Divide(hNum1TriYPt,hDen1TriYPt,1,1,"B");
		hAcc1TriYPtBin->Divide(hNum1TriYPtBin,hDen1TriYPtBin,1,1,"B");
		hAcc1TriYBin->Divide(hNum1TriYBin,hDen1TriYBin,1,1,"B");
		hAcc1TriPtBin->Divide(hNum1TriPtBin,hDen1TriPtBin,1,1,"B");
		hAcc1TriYBin2->Divide(hNum1TriYBin2,hDen1TriYBin2,1,1,"B");
		hAcc1TriPtBin2->Divide(hNum1TriPtBin2,hDen1TriPtBin2,1,1,"B");

		hAcc1SmuEta->Divide(hNum1SmuEta,hDen1SmuEta,1,1,"B");
		hAcc1SmuPt->Divide(hNum1SmuPt,hDen1SmuPt,1,1,"B");
		hAcc1SmuEtaPt->Divide(hNum1SmuEtaPt,hDen1SmuEtaPt,1,1,"B");

		hAcc1SmuEtaBin->Divide(hNum1SmuEtaBin,hDen1SmuEtaBin,1,1,"B");
		hAcc1SmuPtBin->Divide(hNum1SmuPtBin,hDen1SmuPtBin,1,1,"B");
		hAcc1SmuEtaPtBin->Divide(hNum1SmuEtaPtBin,hDen1SmuEtaPtBin,1,1,"B");

		TH1D *h1DNocuPt = hNocuYPt->ProjectionY();
		TH1D *h1DNocuY = hNocuYPt->ProjectionX();
		TH1D *h1DAcc1Pt = hAcc1YPt->ProjectionY();
		TH1D *h1DAcc1Y = hAcc1YPt->ProjectionX();

		hAcc1YBin->Divide(hNum1YBin,hDen1YBin,1,1,"B");
		hAcc1PtBin->Divide(hNum1PtBin,hDen1PtBin,1,1,"B");
		hAcc1YBin2->Divide(hNum1YBin2,hDen1YBin2,1,1,"B");
		hAcc1PtBin2->Divide(hNum1PtBin2,hDen1PtBin2,1,1,"B");
		if (optionB==100 || optionB==200) {
			hAccCompBin1->Fill(hAcc1TriPtBin->GetBinContent(1));
			hAccCompBin2->Fill(hAcc1TriPtBin->GetBinContent(2));
			hAccCompBin3->Fill(hAcc1TriPtBin->GetBinContent(3));
			hAccCompBin4->Fill(hAcc1TriPtBin->GetBinContent(4));
			hAccCompBin5->Fill(hAcc1TriPtBin->GetBinContent(5));
			hAccYCompBin1->Fill(hAcc1TriYBin->GetBinContent(1));
			hAccYCompBin2->Fill(hAcc1TriYBin->GetBinContent(2));
			hAccYCompBin3->Fill(hAcc1TriYBin->GetBinContent(3));
			hAccYCompBin4->Fill(hAcc1TriYBin->GetBinContent(4));
			hAccYCompBin5->Fill(hAcc1TriYBin->GetBinContent(5));
		}
	}//vari
	outfile->cd();
	if (optionB==100 || optionB==200) {
		hAccCompBin1->Write();
		hAccCompBin2->Write();
		hAccCompBin3->Write();
		hAccCompBin4->Write();
		hAccCompBin5->Write();
		hAccYCompBin1->Write();
		hAccYCompBin2->Write();
		hAccYCompBin3->Write();
		hAccYCompBin4->Write();
		hAccYCompBin5->Write();
	}

	////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////

	TCanvas *c1 = new TCanvas("c1","");

	gStyle->SetPalette(1);
	gStyle->SetOptStat(0);

	hAcc1YPt->GetXaxis()->SetTitle(Form("%s y_{lab}",Bx.c_str()));
	hAcc1YPt->GetYaxis()->SetTitle(Form("%s p_{T} (GeV/c)",Bx.c_str()));
	hAcc1YPt->GetZaxis()->SetRangeUser(0.,1.);
	hAcc1YPt->GetXaxis()->SetRangeUser(-2.5,2.5);

	hAcc1YPt->Draw("colz");
	double nocut = hNocuYPt->GetEntries();
	double den2 = hDen2YPt->GetEntries();
	cout << "*** hAcc1YPt_BtoJPsi" << endl;
	double num = hNum1YPt->GetEntries();
	double den = hDen1YPt->GetEntries();
	cout << "# of Nocut   : " << nocut << endl;
	cout << "# of Den2   : " << den2 << endl;
	cout << "# of Numerator   : " << num << endl;
	cout << "# of Denominator : " << den << endl;
	cout << "Acc(Num/Den)     : " << num/den << endl;
	double accerror = sqrt ( 1/(den*den*num) + (num*num)/(den*den*den*den*den));
	cout << "Error of the acceptance : " << accerror << endl;

	std::cout << "datestring.c_str(): " << datestring.c_str() << std::endl;
	c1->SaveAs(Form("../Results/Acc1YPt_%s.pdf",datestring.c_str()));

	hAcc1TriYPt->GetXaxis()->SetTitle(Form("%s y_{lab}",Bx.c_str()));
	hAcc1TriYPt->GetYaxis()->SetTitle(Form("%s p_{T} (GeV/c)",Bx.c_str()));
	hAcc1TriYPt->GetZaxis()->SetRangeUser(0.,1.);
	hAcc1TriYPt->GetXaxis()->SetRangeUser(-2.5,2.5);

	hAcc1TriYPt->Draw("colz");
	nocut = hNocuTriYPt->GetEntries();
	den2 = hDen2TriYPt->GetEntries();
	cout << "*** hAcc1TriYPt_" << endl;
	num = hNum1TriYPt->GetEntries();
	den = hDen1TriYPt->GetEntries();
	cout << "# of Nocut   : " << nocut << endl;
	cout << "# of Den2   : " << den2 << endl;
	cout << "# of Numerator   : " << num << endl;
	cout << "# of Denominator : " << den << endl;
	cout << "Acc(Num/Den)     : " << num/den << endl;
	accerror = sqrt ( 1/(den*den*num) + (num*num)/(den*den*den*den*den));
	cout << "Error of the acceptance : " << accerror << endl;

	std::cout << "datestring.c_str(): " << datestring.c_str() << std::endl;
	c1->SaveAs(Form("../Results/Acc1TriYPt_%s.pdf",datestring.c_str()));

	hDen1TriYPt->GetXaxis()->SetTitle(Form("%s y_{lab}",Bx.c_str()));
	hDen1TriYPt->GetYaxis()->SetTitle(Form("%s p_{T} (GeV/c)",Bx.c_str()));
	hDen1TriYPt->GetXaxis()->SetRangeUser(-2.5,2.5);

	hDen1TriYPt->Draw("colz");
	c1->SaveAs(Form("../Results/Den1TriYPt_%s.pdf",datestring.c_str()));

	hNum1TriYPt->GetXaxis()->SetTitle(Form("%s y_{lab}",Bx.c_str()));
	hNum1TriYPt->GetYaxis()->SetTitle(Form("%s p_{T} (GeV/c)",Bx.c_str()));
	hNum1TriYPt->GetXaxis()->SetRangeUser(-2.5,2.5);

	hNum1TriYPt->Draw("colz");
	c1->SaveAs(Form("../Results/Num1TriYPt_%s.pdf",datestring.c_str()));

	hNocuYPt->Write();
	hNocuY->Write();
	hNocuPt->Write();

	hNocuYPtBin->Write();
	hNocuYBin->Write();
	hNocuPtBin->Write();

	hNocuYPtBin2->Write();
	hNocuYBin2->Write();
	hNocuPtBin2->Write();

	hNum1Y->Write();
	hDen1Y->Write();
	hAcc1Y->Write();

	hNum1Pt->Write();
	hDen1Pt->Write();
	hAcc1Pt->Write();

	hNum1YPt->Write();
	hDen1YPt->Write();
	hAcc1YPt->Write();

	/////////////
	hNum1YBin->Write();
	hDen1YBin->Write();
	hAcc1YBin->Write();

	hNum1PtBin->Write();
	hDen1PtBin->Write();
	hAcc1PtBin->Write();

	hNum1YPtBin->Write();
	hDen1YPtBin->Write();
	hAcc1YPtBin->Write();

	hNum1YBin2->Write();
	hDen1YBin2->Write();
	hAcc1YBin2->Write();

	hNum1PtBin2->Write();
	hDen1PtBin2->Write();
	hAcc1PtBin2->Write();

	hNum1YPtBin2->Write();
	hDen1YPtBin2->Write();
	hAcc1YPtBin2->Write();

	/////////////////////////////////////////
	hNocuTriY->Write();
	hNocuTriPt->Write();
	hDen1TriY->Write();
	hDen1TriPt->Write();
	hNum1TriY->Write();
	hNum1TriPt->Write();
	hAcc1TriY->Write();
	hAcc1TriPt->Write();

	hNocuTriYPt->Write();
	hNocuTriYPtBin->Write();
	hDen1TriYPt->Write();
	hDen1TriYPtBin->Write();
	hNum1TriYPt->Write();
	hNum1TriYPtBin->Write();
	hAcc1TriYPt->Write();
	hAcc1TriYPtBin->Write();

	hDen1TriYBin->Write();
	hDen1TriPtBin->Write();
	hNum1TriYBin->Write();
	hNum1TriPtBin->Write();
	hAcc1TriYBin->Write();
	hAcc1TriPtBin->Write();

	hDen1TriYBin2->Write();
	hDen1TriPtBin2->Write();
	hNum1TriYBin2->Write();
	hNum1TriPtBin2->Write();
	hAcc1TriYBin2->Write();
	hAcc1TriPtBin2->Write();

	hNocuSmuEta->Write();
	hNocuSmuPt->Write();
	hDen1SmuEta->Write();
	hDen1SmuPt->Write();
	hNum1SmuEta->Write();
	hNum1SmuPt->Write();
	hAcc1SmuEta->Write();
	hAcc1SmuPt->Write();
	hDen2SmuEta->Write();
	hDen2SmuPt->Write();
	hNum2SmuEta->Write();
	hNum2SmuPt->Write();

	hNocuSmuEtaPt->Write();
	hDen1SmuEtaPt->Write();
	hNum1SmuEtaPt->Write();
	hAcc1SmuEtaPt->Write();
	hDen2SmuEtaPt->Write();
	hNum2SmuEtaPt->Write();

	hNocuSmuEtaBin->Write();
	hNocuSmuPtBin->Write();
	hDen1SmuEtaBin->Write();
	hDen1SmuPtBin->Write();
	hNum1SmuEtaBin->Write();
	hNum1SmuPtBin->Write();
	hAcc1SmuEtaBin->Write();
	hAcc1SmuPtBin->Write();
	hDen2SmuEtaBin->Write();
	hDen2SmuPtBin->Write();
	hNum2SmuEtaBin->Write();
	hNum2SmuPtBin->Write();

	hNocuSmuEtaPtBin->Write();
	hDen1SmuEtaPtBin->Write();
	hNum1SmuEtaPtBin->Write();
	hAcc1SmuEtaPtBin->Write();
	hDen2SmuEtaPtBin->Write();
	hNum2SmuEtaPtBin->Write();

	//////////////////////////////////////
	gPad->SetLogy(1);

	hDen1TriPt->SetMarkerColor(kAzure-3);
	hDen1TriPt->GetXaxis()->SetRangeUser(0.,100.);
	hDen1TriPt->GetXaxis()->SetTitle(Form("%s p_{T} (GeV/c)",Bx.c_str()));
	hDen1TriPt->GetYaxis()->SetTitle("denominator entries");
	hDen1TriPt->Draw();
	c1->SaveAs(Form("../Results/Den1TriPt_%s.pdf",datestring.c_str()));

	gPad->SetLogy(1);

	hNum1TriPt->SetMarkerColor(kAzure-3);
	hNum1TriPt->GetXaxis()->SetTitle(Form("%s p_{T} (GeV/c)",Bx.c_str()));
	hNum1TriPt->GetYaxis()->SetTitle("numerator entries");
	hNum1TriPt->Draw();
	c1->SaveAs(Form("../Results/Num1TriPt_%s.pdf",datestring.c_str()));

	gPad->SetLogy(0);

	hAcc1TriPt->SetMarkerColor(kAzure-3);
	hAcc1TriPt->GetXaxis()->SetRangeUser(0.,100.);
	hAcc1TriPt->GetYaxis()->SetRangeUser(0.,1.);
	hAcc1TriPt->GetXaxis()->SetTitle(Form("%s p_{T} (GeV/c)",Bx.c_str()));
	hAcc1TriPt->GetYaxis()->SetTitle("Acceptance");
	hAcc1TriPt->Draw();
	c1->SaveAs(Form("../Results/Acc1TriPt_%s.pdf",datestring.c_str()));

	gPad->SetLogy(0);

	hDen1TriY->SetMarkerColor(kAzure-3);
	hDen1TriY->GetXaxis()->SetRangeUser(-2.5,2.5);
	hDen1TriY->GetXaxis()->SetTitle(Form("%s y_{lab}",Bx.c_str()));
	hDen1TriY->GetYaxis()->SetTitle("denominator entries");
	hDen1TriY->Draw();
	c1->SaveAs(Form("../Results/Den1TriY_%s.pdf",datestring.c_str()));

	gPad->SetLogy(0);

	hNum1TriY->SetMarkerColor(kAzure-3);
	hNum1TriY->GetXaxis()->SetRangeUser(-2.5,2.5);
	hNum1TriY->GetXaxis()->SetTitle(Form("%s y_{lab}",Bx.c_str()));
	hNum1TriY->GetYaxis()->SetTitle("numerator entries");
	hNum1TriY->Draw();
	c1->SaveAs(Form("../Results/Num1TriY_%s.pdf",datestring.c_str()));

	gPad->SetLogy(0);

	hAcc1TriY->SetMarkerColor(kAzure-3);
	hAcc1TriY->GetXaxis()->SetRangeUser(-2.5,2.5);
	hAcc1TriY->GetYaxis()->SetRangeUser(0.,1.);
	hAcc1TriY->GetXaxis()->SetTitle(Form("%s y_{lab}",Bx.c_str()));
	hAcc1TriY->GetYaxis()->SetTitle("Acceptance");
	hAcc1TriY->Draw();
	c1->SaveAs(Form("../Results/Acc1TriY_%s.pdf",datestring.c_str()));

	gPad->SetLogy(0);

	hAcc1TriPtBin->SetMarkerColor(kAzure-3);
	hAcc1TriPtBin->SetMarkerSize(2);
	hAcc1TriPtBin->GetXaxis()->SetRange(1,nPtBins);
	hAcc1TriPtBin->GetYaxis()->SetRangeUser(0.,1.);
	hAcc1TriPtBin->GetXaxis()->SetTitle(Form("%s p_{T} (GeV/c)",Bx.c_str()));
	hAcc1TriPtBin->GetYaxis()->SetTitle("Acceptance");
	gStyle->SetEndErrorSize(5);
	hAcc1TriPtBin->Draw("E1 L");
	c1->SaveAs(Form("../Results/Acc1TriPtBin_%s.pdf",datestring.c_str()));

	gPad->SetLogy(0);

	hAcc1TriYBin->SetMarkerColor(kAzure-3);
	hAcc1TriYBin->SetMarkerSize(2);
	//hAcc1TriYBin->GetXaxis()->SetRange(1,4);
	hAcc1TriYBin->GetYaxis()->SetRangeUser(0.,1.);
	hAcc1TriYBin->GetXaxis()->SetTitle(Form("%s y_{lab}",Bx.c_str()));
	hAcc1TriYBin->GetYaxis()->SetTitle("Acceptance");
	hAcc1TriYBin->Draw("E1 L");
	c1->SaveAs(Form("../Results/Acc1TriYBin_%s.pdf",datestring.c_str()));

	////////////////////////////////////////////////////////////////////
	TH2F *pty_gen_jpsi = (TH2F*)hNocuYPt->Clone();
	TH2F *pty_acc_jpsi = (TH2F*)hAcc1YPt->Clone();
	pty_gen_jpsi->SetName("pty_gen_jpsi");
	pty_acc_jpsi->SetName("pty_acc_jpsi");
	pty_gen_jpsi->Write();
	pty_acc_jpsi->Write();

	TH1F *pt_gen_jpsi = (TH1F*)hNocuPt->Clone();
	TH1F *pt_acc_jpsi = (TH1F*)hAcc1Pt->Clone();
	pt_gen_jpsi->SetName("pt_gen_jpsi");
	pt_acc_jpsi->SetName("pt_acc_jpsi");
	pt_gen_jpsi->Write();
	pt_acc_jpsi->Write();

	TH1F *y_gen_jpsi = (TH1F*)hNocuY->Clone();
	TH1F *y_acc_jpsi = (TH1F*)hAcc1Y->Clone();
	y_gen_jpsi->SetName("y_gen_jpsi");
	y_acc_jpsi->SetName("y_acc_jpsi");
	y_gen_jpsi->Write();
	y_acc_jpsi->Write();

	///////////////////////////////////////////
	gStyle->SetPaintTextFormat("1.3f");
	///////////////////////////////////////////////////////////////////
	hNocuTriYPtBin->GetXaxis()->SetTitle(Form("%s y_{lab}",Bx.c_str()));
	hNocuTriYPtBin->GetYaxis()->SetTitle(Form("%s p_{T} (GeV/c)",Bx.c_str()));
	hNocuTriYPtBin->SetMarkerSize(1);
	hNocuTriYPtBin->Draw("colz text e");
	c1->SaveAs(Form("../Results/hNocuTriYPtBin_%s.pdf",datestring.c_str()));

	hDen1TriYPtBin->GetXaxis()->SetTitle(Form("%s y_{lab}",Bx.c_str()));
	hDen1TriYPtBin->GetYaxis()->SetTitle(Form("%s p_{T} (GeV/c)",Bx.c_str()));
	hDen1TriYPtBin->SetMarkerSize(1);
	hDen1TriYPtBin->Draw("colz text e");
	c1->SaveAs(Form("../Results/hDen1TriYPtBin_%s.pdf",datestring.c_str()));

	hNum1TriYPtBin->GetXaxis()->SetTitle(Form("%s y_{lab}",Bx.c_str()));
	hNum1TriYPtBin->GetYaxis()->SetTitle(Form("%s p_{T} (GeV/c)",Bx.c_str()));
	hNum1TriYPtBin->SetMarkerSize(1);
	hNum1TriYPtBin->Draw("colz text e");
	c1->SaveAs(Form("../Results/hNum1TriYPtBin_%s.pdf",datestring.c_str()));

	gStyle->SetPaintTextFormat("1.3f");

	hAcc1TriYPtBin->GetXaxis()->SetTitle(Form("%s y_{lab}",Bx.c_str()));
	hAcc1TriYPtBin->GetYaxis()->SetTitle(Form("%s p_{T} (GeV/c)",Bx.c_str()));
	hAcc1TriYPtBin->GetZaxis()->SetRangeUser(0.,1.);
	hAcc1TriYPtBin->SetMarkerSize(2);
	hAcc1TriYPtBin->Draw("colz text e");
	cout << "*** hAcc1TriYPtBin_BtoJPsi_20110404_5M" << endl;
	num = hNum1TriYPtBin->Integral();
	den = hDen1TriYPtBin->Integral();
	cout << "# of Numerator   : " << num << endl;
	cout << "# of Denominator : " << den << endl;
	cout << "Acc(Num/Den)     : " << num/den << endl;
	accerror = sqrt ( 1/(den*den*num) + (num*num)/(den*den*den*den*den));
	cout << "Error of the acceptance : " << accerror << endl;

	c1->SaveAs(Form("../Results/hAcc1TriYPtBin_%s.pdf",datestring.c_str()));

	outfile->Close();
}
bool passBCut(int pdgId)
{
	if(TMath::Abs(pdgId) == 511 || TMath::Abs(pdgId) == 521 || TMath::Abs(pdgId) == 531) {return true;}

	else {return false;}
}

bool CCut_B(int BCode, int BpJpmup_Id, int BpJpmum_Id, int BpKpdau0_Id, int BpKpdau1_Id, int BpKp_Id, int Bp_Id) {

	if (!(BpJpmup_Id*BpJpmum_Id<0)) return false;

	if (BCode==521 && BpKpdau0_Id==-999 && BpKpdau1_Id==-999 && TMath::Abs(BpKp_Id)==321 && TMath::Abs(Bp_Id)==521) return true;
	else if (BCode==511 && TMath::Abs(BpKpdau0_Id)==321 && TMath::Abs(BpKpdau1_Id)==211 && BpKpdau0_Id*BpKpdau1_Id<0 && TMath::Abs(BpKp_Id)==313 && TMath::Abs(Bp_Id)==511) return true;
	else if (BCode==531 && TMath::Abs(BpKpdau0_Id)==321 && TMath::Abs(BpKpdau1_Id)==321 && BpKpdau0_Id*BpKpdau1_Id<0 && TMath::Abs(BpKp_Id)==333 && TMath::Abs(Bp_Id)==531) return true;
	else {return false;}     
}

bool passCut_Jpmu(bool isBoosted, double muPt, double muEta, double muP) {
	double mietalimit;
	double pletalimit;

	//std::cout << "muPt:" << muPt << ",muEta:" << muEta << ",muP:" << muP << std::endl;
	mietalimit=-2.4;pletalimit=2.4;

	if(muEta<mietalimit || muEta>=pletalimit)

	{return false;}
	else if(TMath::Abs(muEta)>=2.2 && TMath::Abs(muEta)<2.4 && muPt<=0.8)
	{return false;}
	else if(TMath::Abs(muEta)>=1.3 && TMath::Abs(muEta)<2.2 && muP<=2.9)
	{return false;}
	else if(TMath::Abs(muEta)<1.3 && muPt<=3.3)
	{return false;}

	else {//std::cout << "##### passCut_Jpmu is PASSED!" << std::endl;
		return true;}
}
bool passCut_JpKp(bool isBoosted, double muPt, double muEta, int Kpdau0Code, int Kpdau1Code, double Ptcut) {

	if (!(Kpdau0Code==-999 && Kpdau1Code==-999)) return true;

	if (muPt<=Ptcut) return false;
	double mietalimit;
	double pletalimit;

	mietalimit=-2.4;pletalimit=2.4;
	if(muEta<mietalimit || muEta>=pletalimit)
	{return false;}
	else {//std::cout << "######### passCut_JpKp is PASSED!" << std::endl;
		return true;}
}

bool passCut_JpKpdau(int Kpdau0Code, int Kpdau1Code, bool isBoosted, double BpKpdau0_pt, double BpKpdau0_eta, double BpKpdau1_pt, double BpKpdau1_eta, double Ptcut) {

	if (Kpdau0Code==-999 && Kpdau1Code==-999) return true;

	if (BpKpdau0_eta<-2.4 || BpKpdau0_eta>=2.4) return false;
	if (BpKpdau1_eta<-2.4 || BpKpdau1_eta>=2.4) return false;	
	if (BpKpdau0_pt<=Ptcut || BpKpdau1_pt<=Ptcut) return false;
	return true;	
}


