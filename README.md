# AccSysStudy
Study on acceptance systematics for Bana analysis

## Notice for the storage

* All the executive codes are in /Code.

* All the resulted files would be stored in /Results, if this directory is not made, please make direcotry here.

* In step 4, you will spend many hours depending on the number of toy.(more than 8 hours for 50k,maybe?)

## Prerequisite
This study is started from dsigma/dpt or dsigma/dy from data and MC.
Please prepare input files refering root files in /Inputfileforreweighting.

## Step 0. AUTIMATIC script : just run ./autoBanaAccSys.sh

## Step 1. Prerequisite - get the differential cross sections of MC and data exclusively

Already resulted files were copied from B analysis directory and stored in /Inputfileforreweighting.

## Step 2. Get the ratio of differential cross sections of Data over MC

* usage
	* for Bplus (pt and y binning) : root -l -b -q FakeInput_Bplus.C+
	* for Bzero (pt binning) : root -l -b -q FakeInput_Bzero.C+

* resulted file : FunctionsReweighting_Bplus(Bzero).root

## Step 3. Run gaussian random generator and get the various set of weighting function

* running mechanism
	* In step 2, we have the cenral value of ratio and related error in each bin. In this code, we will varied all the central value in each bin within error. And variation is followed by gaussian function in which the central value and error is set as the mean and sigma.
	* And every each event, we can get the weighting function from fitting with randomly produced points. So for example if the number of randomly generated events is 100k, you can get the fitting functions with 100k caried parameter sets.

* usage
	* for pt binning : root -l -b -q 'RunMCtoy_gaus_allB_pt.C+(0(1),100)'
		* parameter : which source? - 0(Bplus), 1(Bzero), how many trials of toy? unit : k - ex. 100k => just enter 100 

## Step 4. Run toy MC

* Now we would run the acceptance analyzer with different set of weighting function. For example, the number of toy event is 100k, we could get the 100k set of acceptance results.

* In acceptance code, please add following parts. (For you, I will upload the example acceptance code soon.)

	* Call root file with toy study results. 
  TFile *ratiofile = new TFile("../Results/RunMCtoy_gaus_wBpluspt_try100k.root");

	* Call parameters of each fitting function. 
  TTree *anar = (TTree*)ratiofile->Get("ditTree"); 
  double a1,a2; 
  anar->SetBranchAddress("a1", &a1); 
  anar->SetBranchAddress("a2", &a2); 

	* For loop for acceptance calculation, please cover the old loop with "vari" for loop.  
  for(int vari=0;vari<anar->GetEntries();vari++) { 
    if (vari%1000==0) std::cout << "### vari : " << vari << " time: " << time(0) << std::endl; 
    anar->GetEntry(vari); 
    double vara1=a1; 
    double vara2=a2; 

	* In the part of setting "w_Diffgen", please add set that with parameters from RunMCtoy root file. 
  if (optionB == 100) {w_Diffgen = vara1*dtri_pt+vara2;} 

	* For loop for acceptance calculation, please cover the old loop with "vari" for loop. 
  } 
