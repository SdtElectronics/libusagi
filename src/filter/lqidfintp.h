

#include <array>

#include <liquid/liquid.h>

template<std::size_t L>
class finterp{
  public:
    finterp(const std::array<float, L>& coefs);
    inline void operator () (float val, float* out);

    ~finterp();

  private:
    std::array<float, L> _coefs;
    firinterp_crcf firinterp;

};

template<std::size_t L>
finterp<L>::finterp(const std::array<float, L>& coefs): _coefs(coefs),
                                                    firinterp(_coefs.data()){
}

template<std::size_t L>
void finterp<L>::operator () (float val, float* out){
    firinterp_crcf_execute(firinterp, val, out);
}

template<std::size_t L>
finterp<L>::~finterp(){
    firinterp_crcf_destroy(firinterp);
}