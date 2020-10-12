/*
 * SecurityCallbacks.h
 *
 *  Created on: Jan 18, 2019
 *      Author: danielkaldheim
 */

#ifndef COMPONENTS_SENSE_SECURITY_CALLBACKS_H_
#define COMPONENTS_SENSE_SECURITY_CALLBACKS_H_
#include <Arduino.h>
#if defined(CONFIG_BT_ENABLED)
#include <BLEDevice.h>

class SecurityCallbacks : public BLESecurityCallbacks
{
  public:
    bool onConfirmPIN(uint32_t pin);
    uint32_t onPassKeyRequest();
    void onPassKeyNotify(uint32_t pass_key);
    bool onSecurityRequest();
    void onAuthenticationComplete(esp_ble_auth_cmpl_t cmpl);
};
#endif // SENSE_BT_ENABLED
#endif // COMPONENTS_SENSE_SECURITY_CALLBACKS_H_