#ifndef OCLEVENTWRAPPER_H
#define OCLEVENTWRAPPER_H
/**
 * The Events class wraps any amount of cl_events (even 0).
 * The + operator can be used to concatenate the contents of two Event-objects into a new one.
 * Events-objects are returned by functions of this library or can be constructed manually.
 * They can be passed to functions of this library or via the size() and data() methofs output the events in the  
 * form required by the OpenCL C-API.
 * Aside from the += operator all methods and operators are const.
 * 
 * @author: Erik Messerli (2018)
 */
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
        //Events(const Events& other) = default;
        Events(Events&& other) = default;
        explicit Events(cl_event& ev)
            :wrapped_events{ev}
        {
          ev = nullptr;
        }        
        explicit Events(cl_event&& ev)
            :wrapped_events{ev}
        {
          ev = nullptr;
        }
        explicit Events(std::vector<cl_event>&& vev)
            :wrapped_events(std::move(vev))
        {}

        ~Events() {
          for (auto ev : wrapped_events) {
            clReleaseEvent(ev);
          }
        }

        //Events& operator=(const Events& other) = default;
        Events& operator=(Events&& other) = default;


        inline void absorb(Events& other) {
          wrapped_events.insert(wrapped_events.end(),other.wrapped_events.begin(),other.wrapped_events.end());
          other.wrapped_events.clear();
        }
        inline void absorb(Events&& other) {
          wrapped_events.insert(wrapped_events.end(),other.wrapped_events.begin(),other.wrapped_events.end());
          other.wrapped_events.clear();
        }
        inline void absorb(cl_event& event) {
          wrapped_events.push_back(event);
          event = nullptr;
        }
        inline void absorb(cl_event&& event) {
          wrapped_events.push_back(event);   
          event = nullptr; 
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



}

#endif