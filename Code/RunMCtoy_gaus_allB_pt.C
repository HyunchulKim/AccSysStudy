#include <stdio.h>
#include <math.h>
#include <TTree.h>
#include <TH1.h>
#include <TROOT.h>
#include <TFile.h>
#include <TF1.h>

//#include <Windows.h>

// sub-functions
double gaussianRandom(double average, double stdev);
void getfit();

const int nBins = 5;
//double ptBins[nBins+1] = {10,15,20,25,30,60};
const int nBins_Bplus = 5;
double ptBins_Bplus[nBins_Bplus+1] = {10,15,20,25,30,60};
const int nBins_Bzero = 3;
double ptBins_Bzero[nBins_Bzero+1] = {10,15,20,60};

double Rat[nBins];
double RatErr[nBins];
double RatToy[nBins];

TFile* fIn;

double a1, a2;

TTree* ditTree;

//TH1D* hfit = new TH1D("hfit","",nBins,ptBins);
TH1D* hfit;

TH1D* htest = new TH1D("htest","",200,0.00,2.00);
TF1* flinear = new TF1("flinear","[0]*x+[1]",10.0,60.0);
TH1D*hReweightDataOverMC_Pt;

void RunMCtoy_gaus_allB_pt(int option, int numToy) {

	std::string rmk;
	int nBinsv;
	// input file for option
	if (option==0) {fIn = new TFile("../Results/FunctionsReweighting_Bplus.root","read");rmk="Bplus";nBinsv=nBins_Bplus;hfit=new TH1D("hfit","",nBins_Bplus,ptBins_Bplus);} //Bplus, pt dependence
	if (option==1) {fIn = new TFile("../Results/FunctionsReweighting_Bzero.root","read");rmk="Bzero";nBinsv=nBins_Bzero;hfit=new TH1D("hfit","",nBins_Bzero,ptBins_Bzero);} //Bzero, pt dependence

	TFile* fOut = new TFile(Form("../Results/RunMCtoy_gaus_w%spt_try%dk.root",rmk.c_str(),numToy),"RECREATE");
	//TH1D*hReweightDataOverMC_pt=(TH1D*)foutMCPt->Get("hReweightDataOverMC_pt");
	hReweightDataOverMC_Pt=(TH1D*)fIn->Get("hReweightDataOverMC_Pt");

	std::cout << "Input source : " << rmk.c_str() << " ################" << std::endl;
for (int i=0;i<nBinsv;i++) {
   Rat[i]=hReweightDataOverMC_Pt->GetBinContent(i+1);
   RatErr[i]=hReweightDataOverMC_Pt->GetBinError(i+1);
   std::cout << "Bin " << i << " - " << Rat[i] << " + error " << RatErr[i] << std::endl;
}
	std::cout << std::string(50,'-') << std::endl;

	ditTree = new TTree("ditTree","Test tree");
	ditTree->Branch("a1",&a1,"a1/D");
	ditTree->Branch("a2",&a2,"a2/D");

	srand((unsigned int)time(NULL));
	for (int i = 0; i < 1000*numToy; i++){
		hfit->Clear();
		for (int count=0;count<nBinsv;count++) {
			RatToy[count]=gaussianRandom(Rat[count],RatErr[count]);
			printf("%.17f\n", RatToy[count]);
			hfit->SetBinContent(count+1,RatToy[count]);
			if (count==0) htest->Fill(RatToy[count]);
		}
		getfit();
	}
	fOut->Write();
	fOut->Close();
}

double gaussianRandom(double average, double stdev) {
	double v1, v2, s, temp;
	do {
		v1 =  2 * ((double) rand() / RAND_MAX) - 1;      // -1.0 ~ 1.0 까지의 값
		v2 =  2 * ((double) rand() / RAND_MAX) - 1;      // -1.0 ~ 1.0 까지의 값
		s = v1 * v1 + v2 * v2;
	} while (s >= 1 || s == 0);

	s = sqrt( (-2 * log(s)) / s );
	temp = v1*s;
	temp = (stdev*temp)+average;
	return temp;
}

void getfit() {
	hfit->Fit("flinear");
	a1=flinear->GetParameter(0);
	a2=flinear->GetParameter(1);
	ditTree->Fill();
	return;
}
