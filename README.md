# AccSysStudy
Study on acceptance systematics for Bana analysis

## Notice for the storage
All the executive codes are in /Code

## Prerequisite
This study is started from dsigma/dpt or dsigma/dy from data and MC.
Please prepare input files refering root files in /Inputfileforreweighting.

## Step 1. prerequisite - get the differential cross sections of MC and data exclusively

Already resulted files were copied from B analysis directory and stored in /Inputfileforreweighting.

## Step 2. Get the ratio of differential cross sections of Data over MC

* usage
	* for Bplus (pt and y binning) : root -l -b -q FakeInput_Bplus.C+
	* for Bzero (pt binning) : root -l -b -q FakeInput_Bzero.C+

* resulted file : FunctionsReweighting_Bplus(Bzero).root

## Step 3. 

