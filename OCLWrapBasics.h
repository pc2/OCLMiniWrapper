#ifndef OCLWRAPBASICS_H
#define OCLWRAPBASICS_H

#include <vector>
#include <CL/cl.hpp>
#include "Events.h"
#include "OCLErrorToException.h"
#include "WorkDimensions.h"

namespace OCLWRAP {

    inline Events EnqueueRange(cl_command_queue queue, cl_kernel kernel, const WorkDimensions& dims, const Events& wait_for){
        cl_event ev;
        cl_int error = clEnqueueNDRangeKernel(
            queue
            , kernel
            , dims.dimension_count
            , nullptr//dims.offset.data() | Must always be null following the usage guide
            , dims.size.data()
            , dims.get_local_size()
            , wait_for.size()
            , wait_for.data(),&ev
        );
        OCLErrorToException(error);
        return Events(ev);
    }

    inline Events EnqueueSingle(cl_command_queue queue, cl_kernel kernel, const Events& wait_for){
        WorkDimensions dims{1};
        dims.implementation_chooses_local_distribution();
        return EnqueueRange(queue,kernel,dims,wait_for);        
    }

    //on error shere make sure your T is really identical to the value required by the kernel.
    template<typename T> inline void simpleKernelArg(cl_kernel kernel, int nr, T t){
        cl_int error = clSetKernelArg(kernel,nr,sizeof(T),&t); 
        OCLErrorToException(error);
    }

    inline void finish(cl_command_queue queue){
        cl_int error = clFinish(queue); 
        OCLErrorToException(error);
    }

}





#endif 