# AccSysStudy
Study on acceptance systematics for Bana analysis

## Notice for the storage

* All the executive codes are in /Code

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


