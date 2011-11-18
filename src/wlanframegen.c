/*
 * Copyright (c) 2011 Joseph Gaeddert
 * Copyright (c) 2011 Virginia Polytechnic Institute & State University
 *
 * This file is part of liquid.
 *
 * liquid is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * liquid is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with liquid.  If not, see <http://www.gnu.org/licenses/>.
 */

//
// wlanframegen.c
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "liquid-wlan.internal.h"

#define DEBUG_WLANFRAMEGEN            1

struct wlanframegen_s {
    // scaling factors
    float g_data;

    // transform object
    FFT_PLAN ifft;          // ifft object
    float complex * X;      // frequency-domain buffer
    float complex * x;      // time-domain buffer

    // PLCP short
    float complex * S0;     // short sequence (frequency)
    float complex * s0;     // short sequence (time)

    // PLCP long
    float complex * S1;     // long sequence (frequency)
    float complex * s1;     // long sequence (time)
};

wlanframegen wlanframegen_create()
{
    wlanframegen q = (wlanframegen) malloc(sizeof(struct wlanframegen_s));

    // allocate memory for transform objects
    q->X = (float complex*) malloc(64*sizeof(float complex));
    q->x = (float complex*) malloc(64*sizeof(float complex));
    q->ifft = FFT_CREATE_PLAN(64, q->X, q->x, FFT_DIR_BACKWARD, FFT_METHOD);

    // compute scaling factor
    q->g_data = 1.0f / sqrtf(52.0f);

    return q;
}

void wlanframegen_destroy(wlanframegen _q)
{
    // free transform array memory
    free(_q->X);
    free(_q->x);
    FFT_DESTROY_PLAN(_q->ifft);

    // free main object memory
    free(_q);
}

void wlanframegen_print(wlanframegen _q)
{
    printf("wlanframegen:\n");
}

void wlanframegen_reset(wlanframegen _q)
{
}

void wlanframegen_write_S0(wlanframegen _q,
                           float complex * _y)
{
    //memmove(_y, _q->s0, (_q->M)*sizeof(float complex));
}


void wlanframegen_write_S1(wlanframegen _q,
                           float complex * _y)
{
    //memmove(_y, _q->s1, (_q->M)*sizeof(float complex));
}

// assemble frame (see Table 76)
//  _q          :   framing object
//  _payload    :   raw payload data [size: _opts.LENGTH x 1]
//  _opts       :   framing options
void wlanframegen_assemble(wlanframegen _q,
                           unsigned char * _payload,
                           struct wlan_txvector_s _opts);


// write OFDM symbol to buffer, returning '1' when frame is complete
//  _q              :   framing generator object
//  _buffer         :   output buffer, [size: 80 x 1]
//  _num_written    :   number of samples written to buffer
int wlanframegen_writesymbol(wlanframegen    _q,
                             float complex * _buffer,
                             unsigned int  * _num_written)
{
#if 0
    // move frequency data to internal buffer
    unsigned int i;
    unsigned int k;
    int sctype;
    for (i=0; i<_q->M; i++) {
        // start at mid-point (effective fftshift)
        k = (i + _q->M/2) % _q->M;

        sctype = _q->p[k];
        if (sctype==WLANFRAME_SCTYPE_NULL) {
            // disabled subcarrier
            _q->X[k] = 0.0f;
        } else if (sctype==WLANFRAME_SCTYPE_PILOT) {
            // pilot subcarrier
            _q->X[k] = (msequence_advance(_q->ms_pilot) ? 1.0f : -1.0f) * _q->g_data;
        } else {
            // data subcarrier
            _q->X[k] = _x[k] * _q->g_data;
        }

        //printf("X[%3u] = %12.8f + j*%12.8f;\n",i+1,crealf(_q->X[i]),cimagf(_q->X[i]));
    }

    // execute transform
    FFT_EXECUTE(_q->ifft);

    // copy result to output
    memmove( _y, &_q->x[_q->M - _q->cp_len], (_q->cp_len)*sizeof(float complex));
    memmove(&_y[_q->cp_len], _q->x, (_q->M)*sizeof(float complex));
#endif

    return 1;
}


