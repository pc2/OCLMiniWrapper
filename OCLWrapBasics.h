#ifndef OCLWRAPBASICS_H
#define OCLWRAPBASICS_H

#include <vector>

#include <CL/cl.hpp>
#include "Events.h"
#include "OCLErrorToException.h"
#include "WorkDimensions.h"

namespace OCLWRAP {

    inline Events EnqueueSingle(cl_command_queue queue, cl_kernel kernel, const Events& wait_for){
        cl_event ev;
        cl_int error = clEnqueueTask(
            queue, kernel, wait_for.size(), wait_for.data(),&ev
        );
        OCLErrorToException(error);
        return Events(ev);        
    }


    inline Events EnqueueRange(cl_command_queue queue, cl_kernel kernel, const WorkDimensions& dims, const Events& wait_for){
        cl_event ev;
        cl_int error = clEnqueueNDRangeKernel(
            queue
            , kernel
            , dims.dimension_count
            , nullptr//dims.offset.data() | Must be null following the guide
            , dims.size.data()
            , dims.get_local_size()
            , wait_for.size()
            , wait_for.data(),&ev
        );
        OCLErrorToException(error);
        return Events(ev);
    }


    template<typename T> inline void simpleKernelArg(cl_kernel kernel,int nr, T t){
        cl_int error = clSetKernelArg(kernel,nr,sizeof(T),&t); 
        OCLErrorToException(error);
    }


}





#endif 