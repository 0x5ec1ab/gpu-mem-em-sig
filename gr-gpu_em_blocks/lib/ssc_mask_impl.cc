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
#include "ssc_mask_impl.h"

namespace gr {
  namespace gpu_em_blocks {

    ssc_mask::sptr
    ssc_mask::make(float freqCenter, float sampRate, int fftSize, 
        float sscFBgn, float sscFEnd, float sscFStep, 
        float thresholdMatch, float fShiftLimit)
    {
      return gnuradio::get_initial_sptr
          (new ssc_mask_impl(freqCenter, sampRate, fftSize, 
              sscFBgn, sscFEnd, sscFStep, thresholdMatch, fShiftLimit));
    }


    ssc_mask_impl::ssc_mask_impl(float freqCenter, float sampRate, 
        int fftSize, float sscFBgn, float sscFEnd, float sscFStep, 
        float thresholdMatch, float fShiftLimit)
      : gr::sync_block("ssc_mask",
            gr::io_signature::make(1, 1, sizeof(float) * fftSize),
            gr::io_signature::make3(1, 3, sizeof(float), sizeof(short), 
                sizeof(float))),
            freqCenter(freqCenter),
            sampRate(sampRate),
            fftSize(fftSize),
            sscFBgn(sscFBgn),
            sscFEnd(sscFEnd),
            sscFStep(sscFStep),
            thresholdMatch(thresholdMatch),
            fShiftLimit(fShiftLimit)
    {
      for (float freq = sscFBgn - freqCenter; freq < sscFEnd - freqCenter;
        freq += sscFStep) {
        float idx = (freq / sampRate + 0.5) * fftSize;
        idxInit.push_back(idx);
        maskIdx.push_back(static_cast<int>(round(idx)));
      }
      subClkNum = idxInit.size();
      idxShiftLimit = ceil(fShiftLimit* fftSize / sampRate);
    }

    ssc_mask_impl::~ssc_mask_impl()
    {
    }

    int
    ssc_mask_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const float *in = (const float *) input_items[0];
      float *out = (float *) output_items[0];
      short *outMatchFlag = (short *)output_items[1];
      float *outMatchOffset = (float *) output_items[2];

      // signal processing
      for (int i = 0; i < noutput_items; i++) {
        const float *sigIn = in + i * fftSize;
        matchFlag = false;

        // compute max power, average offset
        float maxScore = 0;
        int maxIdxInt = 0;
        float scoreFloor = 0;
        float scoreNoise = 0;

        // Step 1: Find the max score and calculate the mean score
        for (int j = -2 * idxShiftLimit; j <= 2 * idxShiftLimit; j++) {
          // calculate the mean score for each candidate offset
          float meanScore = 0;
          for (int idx : maskIdx) {
            meanScore += sigIn[idx + j];
          }
          meanScore /= subClkNum;

          // track the max score, score floor and score noise
          if (abs(j) <= idxShiftLimit && meanScore > maxScore) {
            scoreFloor += maxScore;
            scoreNoise += abs(maxScore - refScore);

            maxScore = meanScore;
            maxIdxInt = j;
          } else {
            scoreFloor += meanScore;
            scoreNoise += abs(meanScore - refScore);
          }
        }
        scoreFloor /= 4 * idxShiftLimit;
        scoreNoise /= 4 * idxShiftLimit;
        // use the score floor as the new reference score 
        refScore = scoreFloor;


        // Step 2 (If the maximum score is significantly higher than the noise):
        // set matchFlag to true, search for more precise matching position
        if (maxScore - refScore > scoreNoise * thresholdMatch) {
          matchFlag = true;
          matchOffset += maxIdxInt;

          // search for the maximum score with finer granularity
          float maxIdxFlt = 0;
          for (float j = -0.5; j < 0.5; j += 0.1) {
            float meanScore = 0;
            for (float idx : idxInit) {
              int idxCand = static_cast<int>(round(idx + matchOffset + j));
              meanScore += sigIn[idxCand];
            }
            meanScore /= subClkNum;

            if (meanScore > maxScore) {
              maxScore = meanScore;
              maxIdxFlt = j;
            }
          }

          // update mask index and matchOffset
          matchOffset += maxIdxFlt;
          for (int k = 0; k < subClkNum; k++) {
            maskIdx[k] = static_cast<int>(round(idxInit[k] + matchOffset));
          }
        }

        // ####################### Produce Output #######################
        // output1: energy collected using SSC mask
        out[i] = 0;
        for (int idx : maskIdx) {
          out[i] += sigIn[idx];
        }
        out[i] /= subClkNum;
        // output2 (optional): matchFlag
        if (outMatchFlag != NULL)
          outMatchFlag[i] = matchFlag;
        // output3 (optional): matchOffset
        if (outMatchOffset != NULL)
          outMatchOffset[i] = matchOffset;
        // ####################### Produce Output #######################
      }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }
  } /* namespace gpu_em_blocks */
} /* namespace gr */

