/* -*- c++ -*- */
/*
 * Copyright 2021 zihao.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_GPU_EM_BLOCKS_SSC_MASK_IMPL_H
#define INCLUDED_GPU_EM_BLOCKS_SSC_MASK_IMPL_H

#include <gpu_em_blocks/ssc_mask.h>

namespace gr {
  namespace gpu_em_blocks {

    class ssc_mask_impl : public ssc_mask
    {
     private:
      // variables storing input arguments
      const float freqCenter;
      const float sampRate; 
      const int fftSize;
      const float sscFBgn;
      const float sscFEnd;
      const float sscFStep;
      const float thresholdMatch;
      const float fShiftLimit;
      // initialized variables
      std::vector<float> idxInit;
      int subClkNum;
      int idxShiftLimit;

      // runtime variables
      bool matchFlag = false;
      float matchOffset = 0;
      float refScore = 0;
      std::vector<int> maskIdx;

     public:
      ssc_mask_impl(float freqCenter, float sampRate, int fftSize, float sscFBgn, float sscFEnd, float sscFStep, float thresholdMatch, float fShiftLimit);
      ~ssc_mask_impl();

      // Where all the action really happens
      int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
      );
    };

  } // namespace gpu_em_blocks
} // namespace gr

#endif /* INCLUDED_GPU_EM_BLOCKS_SSC_MASK_IMPL_H */

