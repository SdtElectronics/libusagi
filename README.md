# libusagi
libusagi - **U**niversal **S**igma-delta **A**nalo**g** **I**nterface is a library for converting discrete multilevel signal to binary bitstream via Σ-Δ modulation. 

## Highlights
* Full implementation of a Σ-Δ modulator from oversampling, interpolation to modulation with simple interface
* Configurable oversampling rate (OSR), modulation order, interpolation filter and data type.
* No external dependency for core implementation (some interpolation filter requires [liquid DSP](https://github.com/jgaeddert/liquid-dsp/))

## Structure
### Oversampler
Source directory: src/filter

### Modulator
Source directory: src/core
![modulator](doc/fig2.png)

### Convertor
Source directory: src/sdda.h

## Tests
### Test with CERN Root


## License
This project is under the [BSD-3-Clause License](LICENSE).