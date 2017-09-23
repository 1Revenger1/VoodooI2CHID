//
//  VoodooI2CPrecisionTouchpadHIDEventDriver.cpp
//  VoodooI2CHID
//
//  Created by Alexandre on 21/09/2017.
//  Copyright © 2017 Alexandre Daoud. All rights reserved.
//

#include "VoodooI2CPrecisionTouchpadHIDEventDriver.hpp"

#define super VoodooI2CMultitouchHIDEventDriver
OSDefineMetaClassAndStructors(VoodooI2CPrecisionTouchpadHIDEventDriver, VoodooI2CMultitouchHIDEventDriver);

void VoodooI2CPrecisionTouchpadHIDEventDriver::enterPrecisionTouchpadMode() {
    // We should really do this using `input_mode_element->setValue(INPUT_MODE_TOUCHPAD)`
    // but I am not able to get it to work.
    
    VoodooI2CPrecisionTouchpadFeatureReport buffer;
    buffer.value = INPUT_MODE_TOUCHPAD;
    buffer.reserved = 0x00;
    
    IOBufferMemoryDescriptor* report = IOBufferMemoryDescriptor::inTaskWithOptions(kernel_task, 0, sizeof(VoodooI2CPrecisionTouchpadFeatureReport));
    report->writeBytes(0, &buffer, sizeof(VoodooI2CPrecisionTouchpadFeatureReport));
    
    hid_interface->setReport(report, kIOHIDReportTypeFeature, input_mode_element->getReportID());
}

bool VoodooI2CPrecisionTouchpadHIDEventDriver::handleStart(IOService* provider) {
    if (!super::handleStart(provider))
        return false;

    if (!input_mode_element)
        return false;

    IOLog("%s Putting device into Precision Touchpad Mode\n", getName());

    enterPrecisionTouchpadMode();

    return true;
}

IOReturn VoodooI2CPrecisionTouchpadHIDEventDriver::setPowerState(unsigned long whichState, IOService* whatDevice) {
    if (whatDevice != this)
        return kIOReturnInvalid;
    if (!whichState) {
        if (awake)
            awake = false;
    } else {
        if (!awake) {
            enterPrecisionTouchpadMode();

            awake = true;
        }
    }
    return kIOPMAckImplied;
}