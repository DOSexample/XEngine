#pragma once

#include "XDevice.h"

class XEngine : public XDevice
{
public:
	static XEngine& Instance() {
		static XEngine x;
		return x;
	}

	XEngine();
	~XEngine();

};

