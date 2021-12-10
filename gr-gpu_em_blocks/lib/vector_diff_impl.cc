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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "vector_diff_impl.h"

namespace gr {
  namespace gpu_em_blocks {

    vector_diff::sptr
    vector_diff::make(int vLen, int dStep)
    {
      return gnuradio::get_initial_sptr
        (new vector_diff_impl(vLen, dStep));
    }


    /*
     * The private constructor
     */
    vector_diff_impl::vector_diff_impl(int vLen, int dStep)
      : gr::sync_block("vector_diff",
            gr::io_signature::make(1, 1, sizeof(float) * vLen),
            gr::io_signature::make(1, 1, sizeof(float) * vLen)),
	vLen(vLen),
	dStep(dStep)
    {}

    /*
     * Our virtual destructor.
     */
    vector_diff_impl::~vector_diff_impl()
    {
    }

    int
    vector_diff_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const float *in = (const float *) input_items[0];
      float *out = (float *) output_items[0];
      for (int i = 0; i < noutput_items; i++) {
        for (int j = 0; j < vLen; j++) {
          out[i * vLen + (j + 1) % vLen] = \
              in[i * vLen + (j + 1) % vLen] -\
              (in[i * vLen + (j + 1 - dStep) % vLen] +\
              in[i * vLen + (j + 1 + dStep) % vLen]) / 2;
        }
      }
      return noutput_items;
    }

  } /* namespace gpu_em_blocks */
} /* namespace gr */

