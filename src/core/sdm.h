#include <algorithm>
#include <array>

template <typename T, std::size_t A>
class SDM{
  public:
    SDM(const std::array<T, A>& coefs);
    std::array<T, A> integrater;

    int operator () (T input);

  private:
    std::array<T, A> _coefs;
};

template <typename T, std::size_t A>
SDM<T, A>::SDM(const std::array<T, A>& coefs): _coefs(coefs),
                                               integrater(){
}

template <typename T, std::size_t A>
int SDM<T, A>::operator () (T input){
    auto coefItr = _coefs.cbegin();
    int step = integrater.back() > 0 ? -1 : 1;
    T forward = input;
    for(T& integral: integrater){
        input -= *(coefItr++) * step;
        input = integral += input;
    }
    return step;
}