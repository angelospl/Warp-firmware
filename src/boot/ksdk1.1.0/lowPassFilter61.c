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
#include "noisyLib.h"
#define SIZE 5000

signed short getXAccelerationMeasurement();

/*
	Low-pass filter from MATLAB filterDesigner.
*/
void
lowPassFilterOrder61(float x[], float y[], const int N)
{
	static const float coeff[62]  = {0.000227909254804652, 0.000433867017147087, 0.000780047144163945, 0.001226644358132, 0.00173493436088177, 0.00223040625529593, 0.00260254806414593, 0.00271072221695493, 0.00239864483731844, 0.00151692430516425, -4.86575750771237E-5, -0.00234573131843931, -0.00532536253847712, -0.00881938374990202, -0.0125298792794148, -0.0160349103690561, -0.018813405060611, -0.0202891356213464, -0.0198903644123104, -0.0171195614002299, -0.0116253062362801, -0.00326657439016242, 0.00784023827215585, 0.0212993822122494, 0.0364444562113838, 0.0523797566570426, 0.0680515698623637, 0.0823424873483821, 0.09417875558847, 0.102638312576606, 0.107046310421825, 0.107046310421825, 0.102638312576606, 0.09417875558847, 0.0823424873483821, 0.0680515698623637, 0.0523797566570426, 0.0364444562113838, 0.0212993822122494, 0.00784023827215585, -0.00326657439016242, -0.0116253062362801, -0.0171195614002299, -0.0198903644123104, -0.0202891356213464, -0.018813405060611, -0.0160349103690561, -0.0125298792794148, -0.00881938374990202, -0.00532536253847712, -0.00234573131843931, -4.86575750771237E-5, 0.00151692430516425, 0.00239864483731844, 0.00271072221695493, 0.00260254806414593, 0.00223040625529593, 0.00173493436088177, 0.001226644358132, 0.000780047144163945, 0.000433867017147087, 0.000227909254804652};
	static float 		window[60] = {0};

	float acc1;
	float acc2;

	/* System object Outputs function: dsp.FIRFilter */
	/* Consume delay line and beginning of input samples */
	for (int i = 0; i < 61; i++)
	{
		acc1 = 0.0;
		for (int j = 0; j < i + 1; j++)
		{
			acc2 = x[i - j] * coeff[j];
			acc1 += acc2;
		}

		for (int j = 0; j < 61 - i; j++)
		{
			acc2 = window[j] * coeff[(i + j) + 1];
			acc1 += acc2;
		}

		y[i] = acc1;
	}
	/* Consume remaining input samples */
	for (int i = 0; i < N-61; i++)
	{
		acc1 = 0.0;
		for (int j = 0; j < 62; j++)
		{
			acc2 = x[(i - j) + 61] * coeff[j];
			acc1 += acc2;
		}

		y[i + 61] = acc1;
	}

	/* Update delay line for next frame */
	for (int i = 0; i < 61; i++)
	{
		window[60 - i] = x[i + (N-61)];
	}
}

void
lowPassFilterOrder183(float sig [],float sig_filt [], const int N)
{
	const int 	 delayLow 		= 91;
	const int 	 lenLow 		= 183;
	const float coeffLow[183] 	= {0.000220, 0.000580, 0.001199, 0.002057, 0.003064, 0.004039, 0.004736, 0.004906, 0.004369, 0.003091, 0.001230, -0.000873, -0.002771, -0.004021, -0.004324, -0.003634, -0.002187, -0.000455, 0.000996, 0.001688, 0.001391, 0.000217, -0.001410, -0.002888, -0.003654, -0.003390, -0.002154, -0.000378, 0.001280, 0.002174, 0.001923, 0.000571, -0.001402, -0.003245, -0.004210, -0.003854, -0.002229, 0.000103, 0.002260, 0.003370, 0.002923, 0.000994, -0.001739, -0.004229, -0.005449, -0.004811, -0.002434, 0.000856, 0.003802, 0.005190, 0.004345, 0.001442, -0.002498, -0.005955, -0.007486, -0.006312, -0.002689, 0.002116, 0.006248, 0.007973, 0.006386, 0.001847, -0.004052, -0.009025, -0.010952, -0.008757, -0.002936, 0.004490, 0.010644, 0.012868, 0.009822, 0.002167, -0.007485, -0.015398, -0.018080, -0.013720, -0.003126, 0.010240, 0.021267, 0.024964, 0.018440, 0.002373, -0.018608, -0.036909, -0.044107, -0.033650, -0.003231, 0.043933, 0.099453, 0.151723, 0.188960, 0.202443, 0.188960, 0.151723, 0.099453, 0.043933, -0.003231, -0.033650, -0.044107, -0.036909, -0.018608, 0.002373, 0.018440, 0.024964, 0.021267, 0.010240, -0.003126, -0.013720, -0.018080, -0.015398, -0.007485, 0.002167, 0.009822, 0.012868, 0.010644, 0.004490, -0.002936, -0.008757, -0.010952, -0.009025, -0.004052, 0.001847, 0.006386, 0.007973, 0.006248, 0.002116, -0.002689, -0.006312, -0.007486, -0.005955, -0.002498, 0.001442, 0.004345, 0.005190, 0.003802, 0.000856, -0.002434, -0.004811, -0.005449, -0.004229, -0.001739, 0.000994, 0.002923, 0.003370, 0.002260, 0.000103, -0.002229, -0.003854, -0.004210, -0.003245, -0.001402, 0.000571, 0.001923, 0.002174, 0.001280, -0.000378, -0.002154, -0.003390, -0.003654, -0.002888, -0.001410, 0.000217, 0.001391, 0.001688, 0.000996, -0.000455, -0.002187, -0.003634, -0.004324, -0.004021, -0.002771, -0.000873, 0.001230, 0.003091, 0.004369, 0.004906, 0.004736, 0.004039, 0.003064, 0.002057, 0.001199, 0.000580, 0.000220};

	float *ext_sig_low = malloc((N + delayLow) * sizeof(float));
	if (ext_sig_low == NULL)
	{
		perror("Not enough memory available.");
		exit(1);
	}

	float *y_low_ext = malloc((N + delayLow) * sizeof(float));
	if (y_low_ext == NULL)
	{
		perror("Not enough memory available.");
		exit(1);
	}

	if (sig_filt == NULL)
	{
		perror("Not enough memory available.");
		exit(1);
	}

	
	for (int i = 0; i < N; i++)
	{
		ext_sig_low[i] = sig[i];
	}
	for (int i = N; i < N + delayLow; i++)
	{
		ext_sig_low[i] = 0;
	}

	for (int n = 0; n < (N + delayLow); n++)
	{
		y_low_ext[n] = 0;
		for (int i = 0; i < lenLow; i++)
		{
			if ((n - i) >= 0)
			{
				y_low_ext[n] = y_low_ext[n] + coeffLow[i] * ext_sig_low[n - i];
			}
		}
	}

	for (int i = delayLow; i < (delayLow + N); i++)
	{
		sig_filt[i - delayLow] = y_low_ext[i];
	}

	

	free(ext_sig_low);
	free(y_low_ext);
}



void
lowPassFilter()
{
	float sig[SIZE] = {0};
	float filtered[SIZE] = {0};

	for (int i = 0; i < SIZE; i++)
	{
		// scanf("%*lf,%lf,%*lf",&sig[i]);
		sig[i] = (float) getXAccelerationMeasurement();
	}

	lowPassFilter(sig,filtered,SIZE);

	for (int i = 0; i < SIZE; i++)
	{
		// printf("%lf\n",filtered[i]);
		noisyWarpPrintFloat32(filtered[i]);
	}
	
	return;
}