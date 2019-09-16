/******************************************************************************
*
*  Copyright (c) Phoenix Contact GmbH & Co. KG. All rights reserved.
*	Licensed under the MIT. See LICENSE file in the project root for full license information.
*
*  AXC2152.h
*
*  Created on: July 11, 2019
*      Author: Steven Durbin
*
*  To obtain the latest release or post an issue, please visit the official GitHub repository:
*
*	https://github.com/sdurbin-pxc/plcnext-io-drivers-cpp
*
******************************************************************************/

#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#include <vector>
#include "../AXC2152/AXLModule.h"
#include "../AXC2152/Modules/AXLF_AI4_I.h"
#include "../AXC2152/Modules/AXLF_AI4_U.h"
#include "../AXC2152/Modules/AXLF_AI8.h"
#include "../AXC2152/Modules/AXLF_AO4.h"
#include "../AXC2152/Modules/AXLF_CNT.h"
#include "../AXC2152/Modules/AXLF_DO8.h"
#include "../AXC2152/Modules/AXLF_DI8.h"
#include "../AXC2152/Modules/AXLF_DI16.h"
#include "../AXC2152/Modules/AXLF_DI8DO8.h"
#include "../AXC2152/Modules/AXLF_AI2AO2.h"
#include "../AXC2152/Modules/AXLF_UTH.h"
#include "../AXC2152/Modules/AXLF_RTD.h"

#define PROCESS_DATA_SIZE 512

using namespace std;

namespace PLCnext {


	class AXC2152 {
		friend class AXLModule;
	public:

		struct DiagnosticsInfo
		{
			ushort status;
			ushort param1;
			ushort param2;
		};

		AXC2152();
		void initialize();
		bool initialized;
		uint initError;
		const vector<AXLModule*>& getModules() const { return modules; }
		DiagnosticsInfo getDiagnosticsInfo();
		~AXC2152();

	private:

		struct PDIPipe {
			uint16_t slot;
			uint16_t subSlot;
			uint16_t index;
			uint16_t subIndex;
			uint16_t requestCnt;
			uint16_t finishedCnt;
			uint16_t dataCount;
			uint16_t error;
			uint16_t addInfo[2];
			uint8_t data[128];
		};

		int fd;
		char* map;
		size_t mapSize;
		AXLModule* moduleFromType(AXC2152* axc, ushort slot, uint type, uint &pdInOffset, uint &pdOutOffset);
		void scanModules();
		double timeSpecToSeconds(struct timespec* ts);
		vector<AXLModule*> modules;
	protected:
		bool pdiRead(ushort slot, ushort subSlot, ushort readIndex, ushort readSubIndex, char* data);
		bool pdiWrite(ushort slot, ushort subSlot, ushort readIndex, ushort readSubIndex, char* data, int length);

	};

}