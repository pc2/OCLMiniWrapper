#ifndef OCLWORKDIMS_H
#define OCLWORKDIMS_H

#include <CL/cl.hpp>
#include <vector>
#include <initializer_list>

namespace OCLWRAP {

struct WorkDimensions{
    WorkDimensions()
    {
        dimension_count = 1;
        size = std::vector<size_t>{1};
        local_size = std::vector<size_t>(); 
    }

    WorkDimensions(std::initializer_list<size_t> args){
        dimension_count = args.size();
        size = std::vector<size_t>(args);
        local_size = std::vector<size_t>(dimension_count,0);  
    }

    void implementation_chooses_local_distribution(){
        local_size = std::vector<size_t>();
    }

    //Empty vector = nullptr = let implementation choose the distribution
    const size_t* get_local_size() const{
        if(local_size.empty()){
            return nullptr; //C++ is not guaranteed to return nullptr for empty vectors
        }
        return local_size.data();
    }




    cl_uint dimension_count;
  	std::vector<size_t> offset; //global [Unused value, implementation lacking]
  	std::vector<size_t> size; //global
  	std::vector<size_t> local_size;

};

}
#endif