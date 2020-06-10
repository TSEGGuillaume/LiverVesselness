# LiverVesselness

Repository of work submitted to ICPR 2020:

**Vesselness Filters: A Survey with Benchmarks Applied to Liver Imaging**

The  implementations of the seven methods are available and results presented on the paper can be reproduced.

Moreover the run of the methods can be done without any installation with this online demonstration:

[http://ipol-geometry.loria.fr/~kerautre/ipol_demo/LiverVesselnessIPOLDemo](http://ipol-geometry.loria.fr/~kerautre/ipol_demo/LiverVesselnessIPOLDemo/)

## Dependancies
- Benchmark (C/C++)

ITK > 5.0 : [Github page](https://github.com/InsightSoftwareConsortium/ITK)
recommanded options : Module_Thickness3D, ITK_USE_FFTWF, ITK_USE_FFTWD

Jsoncpp : [Github page](https://github.com/open-source-parsers/jsoncpp)
Boost > 1.46.0 : needs program_option and filesystem

- scripts ( python 3.5+)

Pandas : (https://pandas.pydata.org/)
Numpy : (https://numpy.org/)
Matplotlib : (https://matplotlib.org/)

## Benchmark
### Purpose

This benchmarking tool was created to compare Vesselness filters in a common application.

It is decomposed in 3 steps :

1) Parameters files creation
This step uses python scripts to generate the algorithms parameters used by the benchmark.
2) Computing metrics
For each parameters sets the according vesselness will be produced and a metrics summed up in a csv file.
This step is implemented in C/C++ as 3D data are expensive to process. 
3) Analysing results
The raw metrics are analysed using python scripts to compute things such as ROC curves, mean metrics, etc.


### Features
- Custom vesselness can be added.
- metrics : confusions matrix (TP,FP,TN,FN), sensitivity, specificity, precision, accuracy, Dice, MCC
- Provides results of the benchmark into a CSV file
- Provides scripts for Algorithm's sets of parameters generation.
- Seven vesselness already available (Sato, Frangi, Meijering, OOF(|lambda_1 + lambda_2|), Jerman, Zhang, RORPO)
- supports all medical formats read by ITK (.mhd,.nii, etc.) as well as DICOM series
- Computes metrics on 3 different areas of interest (an organ, the vessels neighbourhood and the vessels bifurcation)
### Usage

The benchmark is launched using a settings file in Json format using the option --settingsFile (or -s)

```
./Benchmark --settingsFile settingFile.json
```

#### Benchmark Settings
The settings file options are the following :

- name : name of the benchmark. This name is used as the Benchmark folder name as well as the results csv file name.
- path : path where you want the benchmark folder to be created ( see Benchmark Hierarchy for more details )
- inputVolumesList : path to the .txt file listing the data used
- algorithmSets : path to the json file enumerating benchmarked algorithms and their parameters
- maskType : The type of mask used; it can be "Organ","DilatedVessels","Bifurcations or "" (for No masks). * 
- nbThresholds : Metrics are computed between the thresholded output of the filters and the groundtruth. This parameter controls the number of thresholds we want to make. This parameter also controls the amount of points when computing the ROC curve of a filter output.
- removeResultsVolumes : true/false if set to true, the output volumes are discarded after metrics are computed. This option is useful on big benchmark or computers with low disk space. 

Here is an example :
```
{
    "Settings":{
	"name":"newBenchark",
	"path":"/home/path/WhereIWant/My/BenchmarkToBe",
	"inputVolumesList":"fileLists/ircad_10_and_11.txt",
	"algorithmSets":"paramSets/minimal.json",
	"maskType":"",
	"nbThresholds":10,
	"removeResultsVolumes":false
    }
}
```

For existing methods parameters, please refers to vesselness executables documentation and reference papers.

*Masks can greatly beneficiate some methods that relies on gobal information such that the biggest eigen value in the whole image, or a Kmean in a specific area.

### Benchmark Hierarchy

Building Livervesselness will create a build directory with a bin folder containing the Benchmark executable as well as all the 7 vesselness executables. 

A full benchmark will create the following hierarchy :
```
path/newBenchmark
	|_ csv
	|	|_ newBenchmark.csv
	|   |_ newBenchmark_dilatedVessels.csv
	|   |_ newBenchmark_bifurcations.csv
	|_ data1
	|	|_ ouputVesselness1.nii
	|	|_ ouputVesselness2.nii
	|	|_outputVesselness3.nii
	|_ data2
	|_ ouputVesselness1.nii
	|	|_ ouputVesselness2.nii
	|	|_outputVesselness3.nii
```

### Input Volumes list
For a given data, for instance liver CTs, The input volume list requires an input volume, several masks and a groundtruth. The supported types are listed in the table below:

| Volume      | Other (.mhd,.nii,etc.) | Dicom |
|-------------|------------------------|-------|
| input volume| double                 | int_16|
| masks       | uint_8                 | uint_8|
| ground truth| uint_8                 | uint_8|

The input volume list is a .txt file listing all necessary volumes for the benchmark in the following order :

- A unique ID/Name
- The path to the raw data
- The path to the organ mask
- The path to the bifuracations mask
- The path to the dilated vessels mask
- The path to the vessels groundtruth 

Note that the unique ID will be created to create a folder containing all vesselness for a given sequence. This correspond to data1 and data2 in the previous section.

Example :
```
3Dircadb1.10 // ID of the first sequence 
/DATA/ircad_iso_111/3Dircadb1.10/patientIso.nii // input data
/DATA/ircad_iso_111/3Dircadb1.10/liverMaskIso.nii // 1rst mask 
/DATA/ircad_iso_111/3Dircadb1.10/bifurcationsMaskIso.nii // 2nd mask 
/DATA/ircad_iso_111/3Dircadb1.10/dilatedVesselsMaskIso.nii // 3rd mask
/DATA/ircad_iso_111/3Dircadb1.10/vesselsIso.nii // groundtruth
3Dircadb1.11 // ID of the second sequence
/DATA/ircad_iso_111/3Dircadb1.11/patientIso.nii // input data
/DATA/ircad_iso_111/3Dircadb1.11/liverMaskIso.nii // 1rst mask 
/DATA/ircad_iso_111/3Dircadb1.11/bifurcationsMaskIso.nii // 2nd mask 
/DATA/ircad_iso_111/3Dircadb1.11/dilatedVesselsMaskIso.nii // 3rd mask 
/DATA/ircad_iso_111/3Dircadb1.11/vesselsIso.nii // groundtruth 
...
```
### methods Parameters

A parameter file lists all vesselness filters and sets of parameters that will be used during the benchmark.

each instance of parameters is defined by :

- The name of the vesselness function (i.e the name of the vesselness executable)
- The name of the output.
- The list of arguments of the vesselness executable.

Here is an example :
```
{
 "Antiga" :
    [
	 {
	    "Output":"antiga.nii",
	    "Arguments":[
		{"sigmaMin":"2.4"},
		{"sigmaMax":"2.6"},
		{"nbSigmaSteps":"4"},
		{"alpha":"0.7"},
		{"beta":"0.1"},
		{"gamma":"5"}
	    ]
	 }
    ],
	"Jerman" :
    [
	 {
	    "Output":"jerman.nii",
	    "Arguments":[
		{"sigmaMin":"2.0"},
		{"sigmaMax":"2.2"},
		{"nbSigmaSteps":"5"},
		{"tau":"0.75"}
	    ]
	 },
	 {
	    "Output":"jerman.nii",
	    "Arguments":[
		{"sigmaMin":"1.0"},
		{"sigmaMax":"3.0"},
		{"nbSigmaSteps":"3"},
		{"tau":"0.2"}
	    ]
	 }
    ]
}
```

Internally, the benchmark will produce a command line according to the parameters and launch the according vesselness.
Currently the command line is created using the boost options format. The Antiga parameters instance above produces the following command line :
```
./Antiga --input OneInputFromVolumeList --ouput antiga.nii --sigmaMin 2.4 --sigmaMax 2.6 --nbSigmaSteps 4 --alpha 0.7 --beta 0.1 --gamma 5
```

Note that a parameter file can be composed of one or several vesselness with one or several instances of parameters.
### Adding a custom vesselness

As stated before, the benchmark is designed to use stand alone vesselness functions.
To add your own, 2 things are required.
1) your executable is placed next to the Benchmark executable so that it can be called using bash command ./YourVesselnessName.
2) all parameters of your executable are called using --option. Also --input and --ouput are compulsory parameters.

So that the minimal call for your vesselness is : ./YourVesselness --input inputVolume.nii --output outputVolume.nii

Once this is done, you can create your custom json parameter file for your own benchmark.

### Analyse scripts
The scripts provided are used to find the mean of the best MCC, Dice and ROC dist as well as computing the ROC curves once the metrics are computed. 

- parsing csv
This script ouputs the best set of parameter and threshold according to the Best MCC, best Dice and best Roc Dist for each pair (volumes,parameter set) of the benchmark. The outputs are distributed into 3 CSV files, one per metric.
usage

```
python3 parseCSV.py pathToCSV/myResultCSVFile.csv

```
- analysing results (means per parameter sets)

This script computes the mean and standard deviation of the best MCC, DICE and ROC dist for each parameters sets and summarize it in a csv file.
The script needs the csv file from the benchmark as unique input as it will determine the others from it's name.
Moreover, you can choose to save the ROC curves for visualization.

```
python3 parseCSV.py pathToCSV/myResultCSVFile.csv 1 

output example :
ParameterSet ,MCC   ,MCC_std ,Dice,  Dice_std ,ROC_dist ,ROC_dist_std
antiga1.nii  ,0.347 ,0.021   ,0.356 ,0.021    ,0.609    ,0.145
antiga2.nii  ,0.418 ,0.030   ,0.435 ,0.024    ,0.345    ,0.161
antiga3.nii  ,0.340 ,0.028   ,0.352 ,0.024    ,0.338    ,0.106
meijering.nii,0.000 ,0.000   ,0.064 ,0.025    ,1.000    ,0.000


```
