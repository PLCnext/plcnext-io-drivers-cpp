/******************************************************************************
*
*  Copyright (c) Phoenix Contact GmbH & Co. KG. All rights reserved.
*	Licensed under the MIT. See LICENSE file in the project root for full license information.
*
*  AXLF_AI4_U.h
*
*  Created on: July 11, 2019
*      Author: Steven Durbin
*
******************************************************************************/

#pragma once

#include <sys/types.h>
#include "../AXLModule.h"
#include "../AXLInput.h"

using namespace std;

namespace PLCnext {

	const uint AXLF_MODULE_AI4_U = 0xA5;

	class AXC2152;

	class AXLF_AI4_U : public AXLModule
	{
	public:

		class AI4_U_Channel : public AXLInput
		{
		public:
			AI4_U_Channel(char* _pd, AXLF_AI4_U* ai4u, uint channelNum);
			uint getValue(double &ret);
			bool setMeasuringRange(ushort measuringRange);
			bool setFilter(ushort filter);
			bool setMeanValue(ushort meanValue);

			ushort getMeasuringRange();
			ushort getFilter();
			ushort getMeanValue();

			enum MeasuringRange {
				V_0_P10 = 0,
				V_N10_P10 = 1,
				V_0_P5 = 2,
				V_N5_P5 = 3,
				INACTIVE = 15
			};

			enum MeanValue {
				Sample16 = 0,
				NoMean = 1,
				Sample4 = 2,
				Sample32 = 3
			};

			enum Filter {
				Hz_30 = 0,
				kHz_16 = 1
			};

			enum Error {
				NoError = 0x0000,
				InactiveChannel = 0x0001,
				UnknownError = 0x0002,
				OverRange = 0x8001, // Measuring range exceeded(overrange)
				OpenCircuit = 0x8002, // Open circuit
				Invalid = 0x8004, // Measured value invalid / no valid measured value available
				SupplyVoltage = 0x8020, // Faulty supply voltage
				DeviceFault = 0x8040, // Device faulty
				UnderRange = 0x8080  // Below measuring range(underrange)
			};

			// AXLInput Overrides

			Variant getVariantValue();
			string getUnitsString();
			bool executeFunction(int id, vector<Variant> params);

		private:
			char* pd;
			AXLF_AI4_U* ai4u;
			uint channelNum;
			char measuringRange;
			char meanValue;
			bool filter;
			string errorToString(uint);
			ushort getCfgWord(ushort measuringRange, ushort filter, ushort meanValue);
		};

		AXLF_AI4_U(AXC2152* _axc, ushort _slot, uint pdInOffset);
		AXLF_AI4_U();
		AI4_U_Channel* channel[4];
		virtual const string name();
	private:
		bool _pdiRead(ushort subSlot, ushort readIndex, ushort readSubIndex, char* data);
		bool _pdiWrite(ushort subSlot, ushort writeIndex, ushort writeSubIndex, char* data, int length);
	};

}


