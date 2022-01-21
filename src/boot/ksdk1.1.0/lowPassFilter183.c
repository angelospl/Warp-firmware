/*
	Authored 2020, James Timothy Meech.
	Authored 2021, Orestis Kaparounakis.

	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:

	*	Redistributions of source code must retain the above
		copyright notice, this list of conditions and the following
		disclaimer.

	*	Redistributions in binary form must reproduce the above
		copyright notice, this list of conditions and the following
		disclaimer in the documentation and/or other materials
		provided with the distribution.

	*	Neither the name of the author nor the names of its
		contributors may be used to endorse or promote products
		derived from this software without specific prior written
		permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
	FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
	COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
	BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
	ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
	POSSIBILITY OF SUCH DAMAGE.
*/
#include <stdlib.h>
#include "config.h"
#include "warp.h"
#include "devMMA8451Q.h"
#include "noisyLib.h"
#define SIZE 20
#define DELAYLOW 6
#define LENLOW 13

void
lowPassFilterOrder183(float sig [],float sig_filt [], const int N)
{
	const float coeffLow[13] 	= {0.000220, 0.000580, 0.001199, 0.002057, 0.003064, 0.004039, 0.004736, 0.004906, 0.004369, 0.003091, 0.001230, -0.000873, -0.002771};

	float ext_sig_low[SIZE+DELAYLOW];
	float y_low_ext[SIZE+DELAYLOW];

	for (int i = 0; i < N; i++)
	{
		ext_sig_low[i] = sig[i];
	}
	for (int i = N; i < N + DELAYLOW; i++)
	{
		ext_sig_low[i] = 0;
	}

	for (int n = 0; n < (N + DELAYLOW); n++)
	{
		y_low_ext[n] = 0;
		for (int i = 0; i < LENLOW; i++)
		{
			if ((n - i) >= 0)
			{
				y_low_ext[n] = y_low_ext[n] + coeffLow[i] * ext_sig_low[n - i];
			}
		}
	}

	for (int i = DELAYLOW; i < (DELAYLOW + N); i++)
	{
		sig_filt[i - DELAYLOW] = y_low_ext[i];
	}

}



void
lowPassFilter()
{
	warpPrint("lowPassFilterOrder183 prelude\n");
	float sig[SIZE] = {0};
	float filtered[SIZE] = {0};

	for (int i = 0; i < SIZE; i++)
	{
		// scanf("%*lf,%lf,%*lf",&sig[i]);
		warpPrint("getXAccelerationMeasurement %d\n", i);
		sig[i] = (float) getXAccelerationMeasurement();
	}
	warpPrint("lowPassFilterOrder183: start\n");
	lowPassFilterOrder183(sig,filtered,SIZE);

	for (int i = 0; i < SIZE; i++)
	{
		// printf("%lf\n",filtered[i]);
		noisyWarpPrintFloat32(filtered[i]);
	}
	
	return;
}
