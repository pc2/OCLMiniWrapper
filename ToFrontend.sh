#!/bin/bash
# My first script

#scp -r kernels fe-1.cc.pc2.uni-paderborn.de:hpcg_opencl

#rsync -r -v --update kernels erikm@fe-1.cc.pc2.uni-paderborn.de:hpcg_opencl

#scp -r MyBuildRules fe-1.cc.pc2.uni-paderborn.de:hpcg_opencl

#rsync -r -v --update MyBuildRules erikm@fe-1.cc.pc2.uni-paderborn.de:hpcg_opencl

#scp -r src fe-1.cc.pc2.uni-paderborn.de:hpcg_opencl

#rsync -r -v --update src erikm@fe-1.cc.pc2.uni-paderborn.de:hpcg_opencl

rsync -r -v --update . erikm@fe-1.cc.pc2.uni-paderborn.de:NoiseReducingWrapper --exclude ToFrontend.sh