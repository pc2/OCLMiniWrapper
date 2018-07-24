#ifndef OCLEVENTWRAPPER_H
#define OCLEVENTWRAPPER_H

#include <vector>

#include <CL/cl.hpp>


namespace OCLWRAP {

    class Events{
    friend Events operator+(const Events& lhs, cl_event rhs);
    friend Events operator+(cl_event lhs, const Events& rhs);
    friend Events operator+(Events&& lhs, cl_event rhs);
    friend Events operator+(cl_event lhs, Events&& rhs);
      
    public:
        Events() = default;
        Events(const Events& other) = default;
        Events(Events&& other) = default;
        explicit Events(cl_event ev)
            :wrapped_events{ev}
        {}
        explicit Events(const std::vector<cl_event>& vev)
            :wrapped_events(vev)
        {}
        explicit Events(std::vector<cl_event>&& vev)
            :wrapped_events(std::move(vev))
        {}

        Events& operator=(const Events& other) = default;
        Events& operator=(Events&& other) = default;


        //Joins event-sets
        inline Events operator+(const Events& other){
            std::vector<cl_event> merge = wrapped_events;
            merge.insert(merge.end(),other.wrapped_events.cbegin(),other.wrapped_events.cend());
            return Events(std::move(merge));
        }

        Events& operator+=(const Events& other){
            wrapped_events.insert(wrapped_events.end(),other.wrapped_events.cbegin(),other.wrapped_events.cend());
        }

        int size() const {
            return wrapped_events.size();
        }

        const cl_event* data() const {
            if(wrapped_events.empty()){
                return nullptr; 
                //Reason for special case: 
                //According to OPENCL specification
                //"If event_wait_list is not NULL, the list of events pointed to by event_wait_list must be valid and num_events_in_wait_list must be greater than 0"
                //C++ vectors need not return a nullptr in .data() when empty.
            }
            return wrapped_events.data();
        }

    private:
        std::vector<cl_event> wrapped_events;
    };



    inline Events operator+(const Events& lhs, cl_event rhs){
        Events result(lhs);
        result.wrapped_events.push_back(rhs);
        return result;
    }
    inline Events operator+(cl_event lhs, const Events& rhs){
        Events result(rhs);
        result.wrapped_events.push_back(lhs);
        return result;
    }
    inline Events operator+(Events&& lhs, cl_event rhs){
        Events result(std::move(lhs));
        result.wrapped_events.push_back(rhs);
        return result;
    }
    inline Events operator+(cl_event lhs, Events&& rhs){
        Events result(std::move(rhs));
        result.wrapped_events.push_back(lhs);
        return result;
    }


}

#endif