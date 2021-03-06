
#ifndef OCLWRAP_ERRORTOEXCEPTION_H
#define OCLWRAP_ERRORTOEXCEPTION_H

/**
 * If USE_OCL_EXCEPTIONS is defined:
 * Contains the OCLException class, which respresents any exception equivalent to an error code. 
 * OCLErrorToException(cl_int error) checks whether the error code handed to it is 0, raises a fitting OCLException otherwise.
 * 
 * If USE_OCL_EXCEPTIONS is not defined:
 * OCLErrorToException(cl_int error) checks whether the error code handed to it is 0, prints the name of the code. 
 * 
 * @author: Erik Messerli (2018)
 */
#define USE_OCL_EXCEPTIONS


#include <iostream>
#include <string>
#include <CL/cl.hpp>
#include <stdexcept>

namespace OCLWRAP{


#ifndef USE_OCL_EXCEPTIONS
#include "utils/inc/AOCLUtils/opencl.h"
inline void OCLErrorToException(cl_int error){
    if(error!=0){ 
        aocl_utils::printError(error);
        std::cout<<" in DataAndBuffer::to_device \n"; 
    }
}
#else


inline const char * error_to_text(cl_int error){
    if(error == 0) {
        return "";
    }
    switch(error)  {
        case -1:
            return("CL_DEVICE_NOT_FOUND");
        case -2:
            return("CL_DEVICE_NOT_AVAILABLE");
        case -3:
            return("CL_COMPILER_NOT_AVAILABLE");
        case -4:
            return("CL_MEM_OBJECT_ALLOCATION_FAILURE ");
        case -5:
            return("CL_OUT_OF_RESOURCES");
        case -6:
            return("CL_OUT_OF_HOST_MEMORY");
        case -7:
            return("CL_PROFILING_INFO_NOT_AVAILABLE");
        case -8:
            return("CL_MEM_COPY_OVERLAP");
        case -9:
            return("CL_IMAGE_FORMAT_MISMATCH");
        case -10:
            return("CL_IMAGE_FORMAT_NOT_SUPPORTED");
        case -11:
            return("CL_BUILD_PROGRAM_FAILURE");
        case -12:
            return("CL_MAP_FAILURE");
        case -13:
            return("CL_MISALIGNED_SUB_BUFFER_OFFSET");
        case -14:
            return("CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST");
        case -30:
            return("CL_INVALID_VALUE");
        case -31:
            return("CL_INVALID_DEVICE_TYPE");
        case -32:
            return("CL_INVALID_PLATFORM");
        case -33:
            return("CL_INVALID_DEVICE");
        case -34:
            return("CL_INVALID_CONTEXT");
        break;
        case -35:
            return("CL_INVALID_QUEUE_PROPERTIES");
        case -36:
            return("CL_INVALID_COMMAND_QUEUE");
        case -37:
            return("CL_INVALID_HOST_PTR");
        case -38:
            return("CL_INVALID_MEM_OBJECT");
        case -39:
            return("CL_INVALID_IMAGE_FORMAT_DESCRIPTOR");
        case -40:
            return("CL_INVALID_IMAGE_SIZE");
        case -41:
            return("CL_INVALID_SAMPLER");
        case -42:
            return("CL_INVALID_BINARY");
        case -43:
            return("CL_INVALID_BUILD_OPTIONS");
        case -44:
            return("CL_INVALID_PROGRAM");
        case -45:
            return("CL_INVALID_PROGRAM_EXECUTABLE");
        case -46:
            return("CL_INVALID_KERNEL_NAME");
        case -47:
            return("CL_INVALID_KERNEL_DEFINITION");
        case -48:
            return("CL_INVALID_KERNEL");
        case -49:
            return("CL_INVALID_ARG_INDEX");
        case -50:
            return("CL_INVALID_ARG_VALUE");
        case -51:
            return("CL_INVALID_ARG_SIZE");
        case -52:
            return("CL_INVALID_KERNEL_ARGS");
        case -53:
            return("CL_INVALID_WORK_DIMENSION");
        case -54:
            return("CL_INVALID_WORK_GROUP_SIZE");
        case -55:
            return("CL_INVALID_WORK_ITEM_SIZE");
        case -56:
            return("CL_INVALID_GLOBAL_OFFSET");
        case -57:
            return("CL_INVALID_EVENT_WAIT_LIST");
        case -58:
            return("CL_INVALID_EVENT");
        case -59:
            return("CL_INVALID_OPERATION");
        case -60:
            return("CL_INVALID_GL_OBJECT");
        case -61:
            return("CL_INVALID_BUFFER_SIZE");
        case -62:
            return("CL_INVALID_MIP_LEVEL");
        case -63:
            return("CL_INVALID_GLOBAL_WORK_SIZE");
    }

    return (std::string("Unknown: ") + std::to_string(error)).c_str();
}



class OCLException : public std::runtime_error{
public:    
    OCLException(std::string text)
        :runtime_error(text)
    {}

    OCLException(cl_int error)
        :runtime_error(error_to_text(error))
    {}


	const char * what () const noexcept{
    	return (std::string("OpenCL Error: ")+std::runtime_error::what()).c_str();
    }
};


//TODO: Use better exceptions for this
inline void OCLErrorToException(cl_int error){
    if (error != 0){
        //throw std::runtime_error(std::string("OCL Runtime error ID: ") + std::to_string(error));
        throw OCLException(error);
    }
}
#endif 




}

#endif