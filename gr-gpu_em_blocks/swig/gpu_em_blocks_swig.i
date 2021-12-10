/* -*- c++ -*- */

#define GPU_EM_BLOCKS_API

%include "gnuradio.i"           // the common stuff

//load generated python docstrings
%include "gpu_em_blocks_swig_doc.i"

%{
#include "gpu_em_blocks/vector_diff.h"
#include "gpu_em_blocks/ssc_mask.h"
%}

%include "gpu_em_blocks/vector_diff.h"
GR_SWIG_BLOCK_MAGIC2(gpu_em_blocks, vector_diff);
%include "gpu_em_blocks/ssc_mask.h"
GR_SWIG_BLOCK_MAGIC2(gpu_em_blocks, ssc_mask);
