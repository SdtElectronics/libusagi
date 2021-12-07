# Interpolation Filters Implementation Notes
Interpolation filters are used to generate smooth oversampled signals. Instances are passed to `SDDA` constructor to perform interpolation. This note defines the interface of a filter class.

## Formal Interface
```C++
template<typename T>
class filterName{
  public:
    void operator () (T val, T* out);
    std::size_t getOSR();
    std::size_t getDelay();
    void reset();
};
```
A filter is a template class with parameter `T` setting the data type of samples to be processed.

## Member Functions
* `void operator () (T val, T* out)` processes a sample `val` and stores the interpolated values to `out`, which is a pointer to an array of `float` with length equals to OSR.
* `std::size_t getOSR()` returns the oversampling rate set at construction
* `std::size_t getDelay()` returns the delay of the filter in regard to input symbol
* `void reset()` resets the internal state of the filter
