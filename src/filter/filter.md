# Interpolation Filters Implementation Notes
Interpolation filters are used to generate smooth oversampled signals. Instances are passed to `SDDA` constructor to perform interpolation. This note defines the interface of a filter class.

## Formal Interface
```C++
template<std::size_t L>
class filter_name{
  public:
    void operator () (int val, float* out);
    void reset();

    static constexpr std::size_t length = L;
};
```
A filter class should be a template class with numeric parameter `L` setting the oversampling rate (OSR). It should implement `operator ()` taking two parameters `val` and `out`. `val` is the current amplitude of input signal, and `out` is a pointer to an array of `float` with length `L`. A `reset` function should also be implemented to reset the internal state of the filter.
