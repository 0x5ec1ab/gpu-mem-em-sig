This code implements the signal processing techniques described in our paper "**Graphics Peeping Unit: Exploiting EM Side-Channel Information of GPUs to Eavesdrop on Your Neighbors**". 
There are two techniques to increase the SNR of the EM signals of our interest. 
Please read the paper for more details. 

# GNU Radio Blocks
## vector_diff
Accept a vector *VecIn* with length of *Vector Length*.

Output a vector *VecOut* with length of *Vector Length*.

Use parameter *D* = *Difference Step*.

*VecOut(x) = VecIn(x) - VecIn(x + D) / 2 - VecIn(x - D) / 2*

**Parameters**

- Vector Length: Length of input and output vectors.
- Difference Step: Step used to calculate the difference.

## ssc_mask
Search for the SSC pattern in the frequency range between
*SSC Begin Frequency* and *SSC End Frequency* with minimum itervals
between two clocks being *SSC Subclock Interval*.

**Parameters**

- Frequency Center: Center frequency used for data collection.
- Sampling Rate: Sampling rate used for data collection.
- FFT Size: FFT size used for generating the spectrum.
- SSC Begin Frequency: The frequency of the first subclock.
- SSC End Frequency: The frequency of the last subclock.
- SSC Subclock Interval: The minimum frequency interval between two subclocks.
- SSC Match Threshold: Define how many times higher than the noise level the
  energy collected using SSC mask needs to be. Tune this parameter to balance
  the sensitivity and false positive rates.
- Clock Frequency Shift Limit: The maximum clock shift frequency of
  the measured component. Tune this parameter to balance the ability of
  tracking targeted clock signal and the ability of
  excluding the interference of the other clock signals.

# Build
mkdir build

cd build

cmake ..

make

sudo make install

sudo ldconfig

# Example 
An example of using these blocks is shown in blockTest.

- A USRP device is used as the input source.
- The parameters in ssc_mask block are configured for GTX 1080. 
- Parameters for other signals with SSC patterns can be determined by examining 
the spectrum.

