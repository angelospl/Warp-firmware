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

/*
	Low-pass filter from MATLAB filterDesigner.
*/
void
lowPassFilterOrder61(float x[], float y[], const int N)
{
	static const float coeff[5]  = {0.000227909254804652, 0.000433867017147087, 0.000780047144163945, 0.001226644358132, 0.00173493436088177};
	static float 		window[3] = {0};

	float acc1;
	float acc2;

	/* System object Outputs function: dsp.FIRFilter */
	/* Consume delay line and beginning of input samples */
	for (int i = 0; i < 4; i++)
	{
		acc1 = 0.0;
		for (int j = 0; j < i + 1; j++)
		{
			acc2 = x[i - j] * coeff[j];
			acc1 += acc2;
		}

		for (int j = 0; j < 4 - i; j++)
		{
			acc2 = window[j] * coeff[(i + j) + 1];
			acc1 += acc2;
		}

		y[i] = acc1;
	}
	/* Consume remaining input samples */
	for (int i = 0; i < N-4; i++)
	{
		acc1 = 0.0;
		for (int j = 0; j < 5; j++)
		{
			acc2 = x[(i - j) + 4] * coeff[j];
			acc1 += acc2;
		}

		y[i + 4] = acc1;
	}

	/* Update delay line for next frame */
	for (int i = 0; i < 4; i++)
	{
		window[3 - i] = x[i + (N-4)];
	}
}

void
lowPassFilter()
{
	// warpPrint("This never prints!\n");

	float sig[SIZE] = {0};
	float filtered[SIZE] = {0};

	for (int i = 0; i < SIZE; i++)
	{
		// scanf("%*lf,%lf,%*lf",&sig[i]);
		sig[i] = (float) getXAccelerationMeasurement();
	}

	lowPassFilterOrder61(sig,filtered,SIZE);

	for (int i = 0; i < SIZE; i++)
	{
		// printf("%lf\n",filtered[i]);
		noisyWarpPrintFloat32(filtered[i]);
	}
	
	return;
}
