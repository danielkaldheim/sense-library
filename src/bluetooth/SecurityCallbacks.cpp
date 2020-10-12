/*
 * SecurityCallbacks.cpp
 *
 *  Created on: Jan 18, 2019
 *      Author: danielkaldheim
 */
#include "SecurityCallbacks.h"
#if defined(CONFIG_BT_ENABLED)

bool SecurityCallbacks::onConfirmPIN(uint32_t pin)
{
    Serial.print("onConfirmPIN: ");
    Serial.println(pin);
    return (pin == 1234556);
}

uint32_t SecurityCallbacks::onPassKeyRequest()
{
    Serial.println("PassKeyRequest");
    return 123456;
}

void SecurityCallbacks::onPassKeyNotify(uint32_t pass_key)
{
    Serial.print("On passkey Notify number");
    Serial.println(pass_key);
}

bool SecurityCallbacks::onSecurityRequest()
{
    Serial.println("On Security Request");
    return true;
}

void SecurityCallbacks::onAuthenticationComplete(esp_ble_auth_cmpl_t cmpl)
{
    Serial.println("Starting BLE work!");
    if (cmpl.success)
    {
        uint16_t length;
        esp_ble_gap_get_whitelist_size(&length);
    }
}
#endif // CONFIG_BT_ENABLED