#include <cstddef>

template<std::size_t L>
class hold{
  public:
    void operator () (int val, float* out);
};