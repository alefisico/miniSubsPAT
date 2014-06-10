#!/bin/bash 

####################################################
###
###  Simple bash script to copy a hadronizer and send jobs to crab.
###
###  Alejandro Gomez Espinosa
###  gomez@physics.rutgers.edu
###
###  How to run: 
###  source createJobs_condorLPC.sh
###  (If it is an executable (chmod +x createJobs_condorLPC.sh):
###  ./createJobs_condorLPC.sh 
###
###  You must change the PARAMETERS according with your needs. Initially is the only part that you should modify.
###
####################################################

######################################
### PARAMETERS
#####################################

user=${USER}
stop1=100	## You can use this parameters later to make everything simpler. Now I am not using them at all
stop2=250	## You can use this parameters later to make everything simpler. Now I am not using them at all

numJobs=400					#### The more, the better
totalNumberEvents=100000			#### if you don't know exactly the number, give a big number, you will end up with empty root files but it's ok
Energy=13000

FWR_Dir=/uscms_data/d3/algomez/Substructure/miniPAT/CMSSW_5_3_12/src/	## Where your environment is
Main_Dir=${FWR_Dir}/miniPAT/miniSubsPAT/prod/			# Main Dir
Name=RPVSt100tojj_8TeV_HT500
#Name=RPVSt${stop1}tojj_8TeV_HT500		 						# Example how to use the stop1 parameters
input_Dir=/eos/uscms/store/user/algomez/RPVSt100tojj_8TeV_HT500/RPVSt100tojj_8TeV_HT500_9/aodsim/		# input directory, where your aod files are.
run_File=miniSubsPAT_cfg.py

#####################################################
#### Here is where the code starts.. 
#### Initially you shouldn't modify this part
#####################################################
echo " Creating directories..."
####### Working directory
Working_Dir=${Main_Dir}/${Name}	
if [ -d $Working_Dir ]; then
	rm -rf $Working_Dir
	mkdir -p $Working_Dir
else
	mkdir -p $Working_Dir
fi
cd $Working_Dir/


##############################################
##### Create the python file 
##############################################
echo " Creating Hadronizer file... "
namePythonFile=${Name}_miniSubsPAT_cfg.py
namePythonFileCFI=${Name}_miniSubsPAT_cfi.py
if [ -f $namePythonFile ]; then
	rm -rf $namePythonFile
fi

if [ -f $namePythonFileCFI ]; then
	rm -rf $namePythonFileCFI
fi

cp ${Main_Dir}/${run_File} ${namePythonFile} 

echo "import FWCore.ParameterSet.Config as cms

myfilelist = cms.untracked.vstring()
myfilelist.extend(         [ " >> ${namePythonFileCFI}

for file in `ls ${input_Dir}`
do
	echo "'file:${input_Dir}/${file}'," >> ${namePythonFileCFI}
done

echo ']
)
source = cms.Source ("PoolSource", fileNames=myfilelist) ' >> ${namePythonFileCFI}

cp ${namePythonFileCFI} ${Main_Dir}../python/
sed -i 's/miniSubsPAT_cfi/'"${Name}"'_miniSubsPAT_cfi/' ${namePythonFile}

echo ' #############################################################'
echo ' ##### Compiling again                                  ######'
echo ' #############################################################'
cd ${FWR_Dir}
scramv1 b -j8
cd ${Working_Dir}


echo ' #############################################################'
echo ' ########### TESTING BEFORE SENDING TO CRAB          #########'
echo ' #############################################################'

cmsRun ${namePythonFile} outputFile=test.root maxEvents=1

echo ' #############################################################'
echo ' ########### END OF TEST, IF NO ERROR CONTINUE       #########'
echo ' #############################################################'

########################################################
######### Small file with the commands for condor
########################################################
########################################################
echo 'Creating run file....'
runFile=${Working_Dir}/run_${Name}.sh
if [ -f ${runFile} ]; then 
	rm -rf ${runFile}
fi
echo '#!/bin/sh

export LC_ALL="en_US.UTF-8"
export SCRAM_ARCH="slc5_amd64_gcc462"
export COIN_FULL_INDIRECT_RENDERING=1
source /uscmst1/prod/sw/cms/setup/shrc prod

#---------------------------------------------------------------

cd '${FWR_Dir}'
eval `scramv1 runtime -sh`
cd '${Working_Dir}'
cmsRun '${namePythonFile}' outputFile=$1 maxEvents=$2 skip=$3
'>> ${runFile}

echo 'Creating condor file....'
condorFile=${Working_Dir}/condor_${Name}.jdl
if [ -f ${condorFile} ]; then 
	rm -rf ${condorFile}
fi
echo 'Universe = vanilla
Executable = '${runFile}'
Requirements = Memory >= 199 &&OpSys == "LINUX"&& (Arch != "DUMMY" )&& Disk > 1000000
Should_Transfer_Files = YES
WhenToTransferOutput = ON_EXIT' >> ${condorFile}

for ((job=1;job<= $numJobs-1;job++ ))
do
	eventsPerJob=$((${totalNumberEvents}/${numJobs}))
	events=$((1+(${job}-1)*${eventsPerJob}))
	echo "Output = ${Working_Dir}/condorlog_${Name}__${job}.stdout" >> ${condorFile}
	echo "Error  = ${Working_Dir}/condorlog_${Name}__${job}.stderr" >>  ${condorFile}
	echo "Log    = ${Working_Dir}/condorlog_${Name}__.stdlog" >>  ${condorFile}
	echo "Arguments = /eos/uscms/store/user/${user}/${Name}/${Name}_miniPAT_${job} ${eventsPerJob} ${events}" >>  ${condorFile}
	echo -e "Queue\n" >>  ${condorFile}
done

#################################
##### To make it run
#################################
echo '

To run:
cd '${Name}'
condor_submit '${condorFile}'

Have a nice day :D '

