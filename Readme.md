
# Noise Reducing Wrapper for OpenCL

## What is this?

This is a tiny C++ wrapper for OpenCL that aims to reduce some of the noise when using the OpenCL 1.0 API.
The provided function interfaces take fewer, easier to handle arguments.
All contained objects support full interoperability with OpenCL in case rarely used behaviour not covered by the library is needed.

## Where do I start

The Events class simply wraps any amount of cl\_event entries and returns them in the proper form (size(),data()) for use in OpenCL calls.
Events objects may be merged with the + or += operator. Events objects can be trivially created from cl\_event entries by calling Events(cl_event) or extended by ust adding a cl\_event to a Events object using the + operator.

WorkDimensions classes are also easy to integrate in any existing setup, but are even easier to use with the provided wrappers for kernel calls, as the following 4x4 example shows:

    #include "OCLWrapBasics.h"
    WorkDimensions dims{4,4};
    dims.implementation\_chooses\_local_distribution();
    Events kernel_finished = enqueueRange(queue,kernel,dims,wait\_for\_writes);

Note that wait\_for\_writes is of type Events, as well as the output.

    Events enqueue (Read/Write)Buffer(cl\_command_queue queue, cl\_mem buffer, size\_t size\_in_bytes, T* host\_ptr, const Events& wait\_for)

Works just like expected, too. However there is an even more compact way of handling buffers coming up.


##Error handling

None of the library functions return error codes. Instead they catch them internally to throw OCLException
exceptions that describe the kind of error in their what() function.


##Linked Buffers

A LinkedBuffer represents and manages a buffer object. It is associated with a context, device and queue and after setup 
allows the usual manipulations with minimal additional input. The association with a queue may be changed at any later point.
There are a variety of different constructors for the class, but the simplest one just takes a context, a size in elements and a queue. 
Data transfer between host and device needs to be triggered manually. The transfer returns Events to wait on and can of course also wait on Events.
And that's all the parameters needed to initialize an exchange.

Example:

    LinkedBuffer<float> buffer(context,10,queue);
    buffer.host[0] = 1; //Set host side values
    Events wait = buffer.to_device(); //transfer host side values to device\
 