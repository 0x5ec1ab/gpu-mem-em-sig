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

#ifndef INCLUDED_GPU_EM_BLOCKS_SSC_MASK_H
#define INCLUDED_GPU_EM_BLOCKS_SSC_MASK_H

#include <gpu_em_blocks/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace gpu_em_blocks {

    /*!
     * \brief <+description of block+>
     * \ingroup gpu_em_blocks
     *
     */
    class GPU_EM_BLOCKS_API ssc_mask : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<ssc_mask> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of gpu_em_blocks::ssc_mask.
       *
       * To avoid accidental use of raw pointers, gpu_em_blocks::ssc_mask's
       * constructor is in a private implementation
       * class. gpu_em_blocks::ssc_mask::make is the public interface for
       * creating new instances.
       */
      static sptr make(float freqCenter, float sampRate, int fftSize, float sscFBgn, float sscFEnd, float sscFStep, float thresholdMatch, float fShiftLimit);
    };

  } // namespace gpu_em_blocks
} // namespace gr

#endif /* INCLUDED_GPU_EM_BLOCKS_SSC_MASK_H */

