#include <cstddef>
#include <cstdint>
#include <array>

inline std::uint8_t setbit(std::uint8_t c, int i, bool bit){
    return c |= bit << i;
}

template <size_t L>
class bcbuf{
  public:
    bcbuf();
    using buf_data = std::array<std::uint8_t, L>;
    inline void write(bool bit);
    template <typename T>
    inline void write(T begin, T end);
    inline buf_data& data();
    inline void reset();

  private:
    buf_data buf;
    typename buf_data::iterator cur;
    size_t bcount;
    
    //inline void write(bool bit, std::uint8_t& tmp);
};

template <size_t L>
bcbuf<L>::bcbuf(): cur(buf.begin()){
}

template <size_t L>
typename bcbuf<L>::buf_data& bcbuf<L>::data(){
    return buf;
}

template <size_t L>
void bcbuf<L>::reset(){
    bcount = 0;
    cur = buf.begin();
}

/*
template <size_t L>
void bcbuf<L>::write(bool bit, std::uint8_t& tmp){
    size_t mod = (bcount++) & 0x07u;
    tmp = mod ? tmp : 0x00;
    tmp = setbit(tmp, mod, bit);
}
*/

template <size_t L>
void bcbuf<L>::write(bool bit){
    size_t mod = (bcount++) & 0x07u;
    std::uint8_t tmp = *cur;
    tmp = mod ? tmp : 0x00;
    *cur = setbit(tmp, mod, bit);
    cur += mod == 0x07u;
    //cur = mod == 0x07u ? cur + 1 : cur;
}

template <size_t L>
template <typename T>
inline void bcbuf<L>::write(T begin, T end){
    while(begin != end){
        std::uint8_t tmp = 0x00;
        // -O3 would unroll this automatically
        for(size_t i = 0; i != 8; ++i){
            tmp = setbit(tmp, i, *(begin++));
        }
        *(cur++) = tmp;
    }
}