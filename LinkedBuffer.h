#ifndef OCLBUFFERWRAPPER_H
#define OCLBUFFERWRAPPER_H
/**
 * A linked buffer wraps both the local and remote part of a buffer.
 * A buffer is associated to a cl_context and a cl_command_queue, though the later may be changed.
 * The "host" field represents the data on the host side. It can be transported by calling to_device(). 
 * The host side bufer can be updated with the data from the device by calling from_device().
 * These functions use the given queue, can be passed Events-objects to wait for and will return an Events object that can be waited on.
 * Make sure to update only Host OR Device side at any point in time because no mechanism exists to merge them.
 * All of the other functions are just utilities for the host to get data in/out of the host side array.
 * 
 * @author: Erik Messerli (2018)
 * */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <fstream>
#include <CL/cl.hpp>
#include <cassert>

#include "Events.h"
#include "OCLErrorToException.h"

/**
 * Altera requires a 64 bit alignment of data to effectively transfer it between Host and Board. Change as required.
 * */
#define AOCL_ALIGNMENT 64


namespace OCLWRAP{

template<typename T> class LinkedBuffer{
public:
  LinkedBuffer(cl_context context, const std::vector<T>& initial, cl_command_queue queue)
    :size(initial.size()),host(nullptr),queue(queue)
  {
    void *ptr = NULL;
    posix_memalign (&ptr, AOCL_ALIGNMENT, sizeof(T)*size);
    host = (T*)ptr;

    for(size_t ii=0;ii<initial.size();++ii){
      host[ii] = initial[ii];
    }

    cl_int error;
    buffer = clCreateBuffer(context, CL_MEM_USE_HOST_PTR  | CL_MEM_READ_WRITE, sizeof(T)*size, host ,&error);
    OCLErrorToException(error);
  }

  LinkedBuffer(cl_context context, double* orig, size_t len, cl_command_queue queue)
    :size(len),host(nullptr),queue(queue)
  {
    void *ptr = NULL;
    posix_memalign (&ptr, AOCL_ALIGNMENT, sizeof(T)*size);
    host = (T*)ptr;

    for(size_t ii=0;ii<len;++ii){
      host[ii] = orig[ii];
    }

    cl_int error;
    buffer = clCreateBuffer(context, CL_MEM_USE_HOST_PTR  | CL_MEM_READ_WRITE, sizeof(T)*size, host_ptr() ,&error);
    OCLErrorToException(error);
  }


  LinkedBuffer(cl_context context, size_t len, cl_command_queue queue)
    :size(len),host(nullptr),queue(queue)
  {
    void *ptr = NULL;
    posix_memalign (&ptr, AOCL_ALIGNMENT, sizeof(T)*size);
    host = (T*)ptr;

    cl_int error;
    buffer = clCreateBuffer(context, CL_MEM_USE_HOST_PTR  | CL_MEM_READ_WRITE, sizeof(T)*size, host_ptr() ,&error);
    OCLErrorToException(error);
  }

  LinkedBuffer(const LinkedBuffer& other) = delete;
  LinkedBuffer& operator=(const LinkedBuffer& other) = delete;
  
  //TODO: Could create reasonable move operators if needed. 
  LinkedBuffer(LinkedBuffer&& other) = delete;
  LinkedBuffer& operator=(LinkedBuffer&& other) = delete;
  

  ~LinkedBuffer(){
    free(host);
    //Free OpenCL buffer
    cl_int error = clReleaseMemObject(buffer);
    assert(error == 0);
  }

  
  void useThisQueue(cl_command_queue new_queue){
    /*  
      Some implementations have terrible performance when using queues too long. 
      This switches the buffer object to use the new queue for all operations started afterwards.
    */
    queue = new_queue;
  }


  void set_host(const std::vector<T>& new_values){
    assert(new_values.size() == size);
    for(size_t ii=0;ii<new_values.size();++ii){
      host[ii] = new_values[ii];
    }
  }

  void copy_into_c_style(T* loc){
    for(size_t ii=0;ii<size;++ii){
      loc[ii] = host[ii];
    }   
  }


  void set_host(T* new_values_start, size_t nr_new_values){
    assert(nr_new_values == size);
    for(size_t ii=0;ii<nr_new_values;++ii){
      host[ii] = new_values_start[ii];
    }
  }


  Events to_device(const Events& wait_for = Events()){
    cl_event result_event;
    cl_int error = clEnqueueWriteBuffer(queue,buffer,CL_TRUE,0,sizeof(T)*size,host_ptr(),wait_for.size(),wait_for.data(),&result_event);
    OCLErrorToException(error);
    return Events(result_event);
  }


  Events from_device(const Events& wait_for){
    cl_event result_event;
    cl_int error = clEnqueueReadBuffer(queue,buffer,CL_TRUE,0,sizeof(T)*size,host_ptr(),wait_for.size(),wait_for.data(), &result_event);
    OCLErrorToException(error);
    return Events(result_event);
  }

  void as_kernel_arg(cl_kernel kernel, cl_uint arg_c){
    cl_int error = clSetKernelArg(kernel,arg_c++,sizeof(cl_mem),&buffer); 
    OCLErrorToException(error);
    //cl_int clSetKernelArg ( 	cl_kernel kernel,cl_uint arg_index,size_t arg_size,const void *arg_value)
  }

  inline T* host_ptr(){
    return host;
  }


  // ---- fields ----
  const size_t size;
  T* host;
  cl_mem buffer;
  cl_command_queue queue;
};

}

template<typename T> std::ostream& operator<<(std::ostream& os, const OCLWRAP::LinkedBuffer<T>& dt)  
{  
  for(int ii=0; ii<dt.size;++ii){
    os<<dt.host[ii]<<' ';
  }
  return os;  
}  

#endif