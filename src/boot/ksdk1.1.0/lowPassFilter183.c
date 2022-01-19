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
#define SIZE 5
#define DELAYLOW 91
#define LENLOW 183

void
lowPassFilterOrder183(float sig [],float sig_filt [], const int N)
{
	const float coeffLow[183] 	= {0.000220, 0.000580, 0.001199, 0.002057, 0.003064, 0.004039, 0.004736, 0.004906, 0.004369, 0.003091, 0.001230, -0.000873, -0.002771, -0.004021, -0.004324, -0.003634, -0.002187, -0.000455, 0.000996, 0.001688, 0.001391, 0.000217, -0.001410, -0.002888, -0.003654, -0.003390, -0.002154, -0.000378, 0.001280, 0.002174, 0.001923, 0.000571, -0.001402, -0.003245, -0.004210, -0.003854, -0.002229, 0.000103, 0.002260, 0.003370, 0.002923, 0.000994, -0.001739, -0.004229, -0.005449, -0.004811, -0.002434, 0.000856, 0.003802, 0.005190, 0.004345, 0.001442, -0.002498, -0.005955, -0.007486, -0.006312, -0.002689, 0.002116, 0.006248, 0.007973, 0.006386, 0.001847, -0.004052, -0.009025, -0.010952, -0.008757, -0.002936, 0.004490, 0.010644, 0.012868, 0.009822, 0.002167, -0.007485, -0.015398, -0.018080, -0.013720, -0.003126, 0.010240, 0.021267, 0.024964, 0.018440, 0.002373, -0.018608, -0.036909, -0.044107, -0.033650, -0.003231, 0.043933, 0.099453, 0.151723, 0.188960, 0.202443, 0.188960, 0.151723, 0.099453, 0.043933, -0.003231, -0.033650, -0.044107, -0.036909, -0.018608, 0.002373, 0.018440, 0.024964, 0.021267, 0.010240, -0.003126, -0.013720, -0.018080, -0.015398, -0.007485, 0.002167, 0.009822, 0.012868, 0.010644, 0.004490, -0.002936, -0.008757, -0.010952, -0.009025, -0.004052, 0.001847, 0.006386, 0.007973, 0.006248, 0.002116, -0.002689, -0.006312, -0.007486, -0.005955, -0.002498, 0.001442, 0.004345, 0.005190, 0.003802, 0.000856, -0.002434, -0.004811, -0.005449, -0.004229, -0.001739, 0.000994, 0.002923, 0.003370, 0.002260, 0.000103, -0.002229, -0.003854, -0.004210, -0.003245, -0.001402, 0.000571, 0.001923, 0.002174, 0.001280, -0.000378, -0.002154, -0.003390, -0.003654, -0.002888, -0.001410, 0.000217, 0.001391, 0.001688, 0.000996, -0.000455, -0.002187, -0.003634, -0.004324, -0.004021, -0.002771, -0.000873, 0.001230, 0.003091, 0.004369, 0.004906, 0.004736, 0.004039, 0.003064, 0.002057, 0.001199, 0.000580, 0.000220};

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
	warpPrint("lowPassFilterOrder183 intro\n");
	float sig[SIZE] = {0};
	float filtered[SIZE] = {0};

	for (int i = 0; i < SIZE; i++)
	{
		// scanf("%*lf,%lf,%*lf",&sig[i]);
		sig[i] = (float) getXAccelerationMeasurement();
	}

	lowPassFilterOrder183(sig,filtered,SIZE);

	for (int i = 0; i < SIZE; i++)
	{
		// printf("%lf\n",filtered[i]);
		noisyWarpPrintFloat32(filtered[i]);
	}
	
	return;
}
