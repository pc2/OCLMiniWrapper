
# Noise Reducing Wrapper for OpenCL

## What is this?

This is a tiny C++ wrapper for OpenCL that aims to reduce some of the noise when using the OpenCL 1.0 API.
The provided function interfaces take fewer, easier to handle arguments.
All contained objects support full interoperability with OpenCL in case rarely used behaviour not covered by the library is needed.

## Where do I start

The Events class simply wraps any amount of cl_event entries and returns them in the proper form (size(),data()) for use in OpenCL calls.
Events objects may be merged with the + or += operator. Events objects can be trivially created from cl_event entries by calling Events(cl_event) or extended by ust adding a cl_event to a Events object using the + operator.

WorkDimensions classes are also easy to integrate in any existing setup, but are even easier to use with the provided wrappers for kernel calls, as the following example shows:

> #include "OCLWrapBasics.h"\
> WorkDimensions dims{number_of_workers};\
> dims.implementation_chooses_local_distribution();\
> Events kernel_finished = enqueueRange(state.queue,state.kernel,dims,state.wait_for_writes);\


