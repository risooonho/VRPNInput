/*
The MIT License (MIT)

Copyright (c) 2015 University of Groningen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#if PLATFORM_WINDOWS
	#include "AllowWindowsPlatformTypes.h"
		#include "vrpn_Tracker.h"
		#include "vrpn_Button.h"
	#include "HideWindowsPlatformTypes.h"    
#elif PLATFORM_LINUX
	#include "vrpn_Tracker.h"
	#include "vrpn_Button.h"
#endif

class IVRPNInputDevice
{
public:
	virtual ~IVRPNInputDevice(){};
	virtual void Update() = 0;
	virtual bool ParseConfig(FConfigSection *InConfigSection) = 0;
};

/*
 * Connects to a VRPN button device.
 * gives an event for each putton up/down press
 */
class VRPNButtonInputDevice : public IVRPNInputDevice
{
public:
	/* If a device is not enabled it will still add the blueprints functions but it does not establish a VRPN connection.
	*/
	VRPNButtonInputDevice(const FString &TrackerAddress, bool bEnabled = true);
	virtual ~VRPNButtonInputDevice();

	void Update() override;
	bool ParseConfig(FConfigSection *InConfigSection) override;

private:
	vrpn_Button_Remote *InputDevice;

	static void VRPN_CALLBACK HandleButtonDevice(void *userData, vrpn_BUTTONCB const b);

	TMap<int32, const FKey> ButtonMap;
};

/*
* Connects to a VRPN tracker device.
* each time update is called it will refresh it's value with the latest.
*/
class VRPNTrackerInputDevice : public IVRPNInputDevice
{
public:
	/* If a device is not enabled it will still add the blueprints functions but it does not establish a VRPN connection.
	 */
	VRPNTrackerInputDevice(const FString &TrackerAddress, bool bEnabled = true);
	virtual ~VRPNTrackerInputDevice();

	void Update() override;
	bool ParseConfig(FConfigSection *InConfigSection) override;

private:
	vrpn_Tracker_Remote *InputDevice;

	struct TrackerInput
	{
		FKey MotionXKey;
		FKey MotionYKey;
		FKey MotionZKey;

		FKey RotationYawKey;
		FKey RotationPitchKey;
		FKey RotationRollKey;

		FVector CurrentTrackerPosition;
		FQuat CurrentTrackerRotation;

		bool TrackerDataDirty;
	};

	TMap<int32, TrackerInput> TrackerMap;
	FVector TranslationOffset;
	FQuat RotationOffset; // This rotation will be added to the Yaw/Pitch/Roll
	
	float TrackerUnitsToUE4Units;
	bool FlipZAxis;


	static void VRPN_CALLBACK HandleTrackerDevice(void *userData, vrpn_TRACKERCB const tr);
};