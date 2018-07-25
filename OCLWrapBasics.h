#ifndef OCLWRAPBASICS_H
#define OCLWRAPBASICS_H
/**
 * Contains basic functions that are equivalent to the most commonly needed functionality, but commonly take fewer arguments.
 * They also provide built-in error handling, so it's not possible to forget.
 * 
 * @author: Erik Messerli (2018)
 * */
#include <vector>
#include <CL/cl.hpp>
#include "Events.h"
#include "OCLErrorToException.h"
#include "WorkDimensions.h"

namespace OCLWRAP {
    /**
     * Enqueues an NDRange or SWI kernel (work dimensions contains only one entry).
     * The kernel may wait for an Events object and will also return one to be waited on.
     * 
     * TODO: [[nodiscard]] can be added to the function to warn people should they forget to grab the Event to wait on.
     * */
    inline Events enqueueRange(cl_command_queue queue, cl_kernel kernel, const WorkDimensions& dims, const Events& wait_for){
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

    /**
     * Enqueue an SWI kernel. No need to pass any WorkDimensions.
     * */
    inline Events enqueueSingle(cl_command_queue queue, cl_kernel kernel, const Events& wait_for){
        WorkDimensions dims{1};
        dims.implementation_chooses_local_distribution();
        return enqueueRange(queue,kernel,dims,wait_for);        
    }

    /**
     * Wraps clSetKernelArg for simple values (integers, doubles etc.) that are not passed via buffer.
     * Takes one less argument (the size, which is auto-deduced) and makes it more obvious that the passed value 
     * is a plain pass-by-value by not visibly taking the adress or reference.
     * Also provides immediate error checking.
     * 
     * On error shere make sure your T is really identical to the value required by the kernel.
     * Forcibly specify template parameter if in doubt.
    **/
    template<typename T> inline void simpleKernelArg(cl_kernel kernel, int nr, T t){
        cl_int error = clSetKernelArg(kernel,nr,sizeof(T),&t); 
        OCLErrorToException(error);
    }

    /**
     * Finish a queue and error check.
     **/
    inline void finish(cl_command_queue queue){
        cl_int error = clFinish(queue); 
        OCLErrorToException(error);
    }

}





#endif 