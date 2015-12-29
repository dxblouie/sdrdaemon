///////////////////////////////////////////////////////////////////////////////////
// SDRdaemon - send I/Q samples read from a SDR device over the network via UDP. //
//                                                                               //
// Copyright (C) 2015 Edouard Griffiths, F4EXB                                   //
//                                                                               //
// This program is free software; you can redistribute it and/or modify          //
// it under the terms of the GNU General Public License as published by          //
// the Free Software Foundation as version 3 of the License, or                  //
//                                                                               //
// This program is distributed in the hope that it will be useful,               //
// but WITHOUT ANY WARRANTY; without even the implied warranty of                //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                  //
// GNU General Public License V3 for more details.                               //
//                                                                               //
// You should have received a copy of the GNU General Public License             //
// along with this program. If not, see <http://www.gnu.org/licenses/>.          //
///////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDE_DECIMATORS_H_
#define INCLUDE_DECIMATORS_H_

#include "SDRDaemon.h"

class Decimators
{
public:
	static void decimate2_inf(unsigned int& sampleSize, const IQSampleVector& in, IQSampleVector& out);
	static void decimate2_sup(unsigned int& sampleSize, const IQSampleVector& in, IQSampleVector& out);
	//void decimate2_cen(unsigned int sampleSize, const IQSampleVector& in, IQSampleVector& out);
};

/** double byte samples to double byte samples decimation by 2 low band */
void Decimators::decimate2_inf(unsigned int& sampleSize, const IQSampleVector& in, IQSampleVector& out)
{
	std::size_t len = in.size();
	out.resize(len/2);
	IQSampleVector::iterator it = out.begin();
	unsigned int decimation_shift = (sampleSize < 15 ? 0 : sampleSize - 15);

	std::int32_t xreal, yimag;

	for (unsigned int pos = 0; pos < len - 3; pos += 4)
	{
		xreal = in[pos+0].real() - in[pos+1].imag();
		yimag = in[pos+0].imag() + in[pos+1].real();
        it->setReal(xreal >> decimation_shift);
        it->setImag(yimag >> decimation_shift);
        ++it;
        xreal =  in[pos+3].imag() - in[pos+2].real();
        yimag = -in[pos+2].imag() - in[pos+3].real();
        it->setReal(xreal >> decimation_shift);
        it->setImag(yimag >> decimation_shift);
        ++it;

        /*
		xreal = (buf[pos+0] - buf[pos+3]) << decimation_shifts<SdrBits, InputBits>::pre2;
		yimag = (buf[pos+1] + buf[pos+2]) << decimation_shifts<SdrBits, InputBits>::pre2;
		(**it).setReal(xreal >> decimation_shifts<SdrBits, InputBits>::post2);
		(**it).setImag(yimag >> decimation_shifts<SdrBits, InputBits>::post2);
		++(*it);

		xreal = (buf[pos+7] - buf[pos+4]) << decimation_shifts<SdrBits, InputBits>::pre2;
		yimag = (- buf[pos+5] - buf[pos+6]) << decimation_shifts<SdrBits, InputBits>::pre2;
		(**it).setReal(xreal >> decimation_shifts<SdrBits, InputBits>::post2);
		(**it).setImag(yimag >> decimation_shifts<SdrBits, InputBits>::post2);
		++(*it);*/
	}

	sampleSize += (1 - decimation_shift);
}

/** double byte samples to double byte samples decimation by 2 high band */
void Decimators::decimate2_sup(unsigned int& sampleSize, const IQSampleVector& in, IQSampleVector& out)
{
	std::size_t len = in.size();
	out.resize(len/2);
	IQSampleVector::iterator it = out.begin();
	unsigned int decimation_shift = (sampleSize < 15 ? 0 : sampleSize - 15);

	std::int32_t xreal, yimag;

	for (unsigned int pos = 0; pos < len - 3; pos += 4)
	{
		xreal =  in[pos+0].imag() - in[pos+1].real();
		yimag = -in[pos+0].real() - in[pos+1].imag();
        it->setReal(xreal >> decimation_shift);
        it->setImag(yimag >> decimation_shift);
        ++it;
        xreal = in[pos+3].real() - in[pos+2].imag();
        yimag = in[pos+2].real() + in[pos+3].imag();
        it->setReal(xreal >> decimation_shift);
        it->setImag(yimag >> decimation_shift);
        ++it;
	}

	sampleSize += (1 - decimation_shift);
}

#endif /* INCLUDE_DECIMATORS_H_ */
