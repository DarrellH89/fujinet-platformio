#ifndef ADAMNET_H
#define ADAMNET_H

/**
 * AdamNet Routines
 */

#include <forward_list>
#include "fnSystem.h"

#define ADAMNET_BAUD 62500

#define MN_RESET 0x00   // command.control (reset)
#define MN_STATUS 0x01  // command.control (status)
#define MN_ACK 0x02     // command.control (ack)
#define MN_CLR 0x03     // command.control (clr) (aka CTS)
#define MN_RECEIVE 0x04 // command.control (receive)
#define MN_CANCEL 0x05  // command.control (cancel)
#define MN_SEND 0x06    // command.control (send)
#define MN_NACK 0x07    // command.control (nack)
#define MN_READY 0x0D   // command.control (ready)

#define NM_STATUS 0x08 // response.control (status)
#define NM_ACK 0x09    // response.control (ack)
#define NM_CANCEL 0x0A // response.control (cancel)
#define NM_SEND 0x0B   // response.data (send)
#define NM_NACK 0x0C   // response.control (nack)

class adamNetBus;

/**
 * @brief Calculate checksum for AdamNet packets. Uses a simple 8-bit XOR of each successive byte.
 * @param buf pointer to buffer
 * @param len length of buffer
 * @return checksum value (0x00 - 0xFF)
 */
uint8_t adamnet_checksum(uint8_t *buf, unsigned short len);

/**
 * @brief An AdamNet Device
 */
class adamNetDevice
{
protected:
    friend adamNetBus; // We exist on the AdamNet Bus, and need its methods.

    /**
     * @brief Send Byte to AdamNet
     * @param b Byte to send via AdamNet
     * @return was byte sent?
     */
    void adamnet_send(uint8_t b);

    /**
     * @brief Send buffer to AdamNet
     * @param buf Buffer to send to AdamNet
     * @param len Length of buffer
     * @return number of bytes sent.
     */
    void adamnet_send_buffer(uint8_t *buf, unsigned short len);

    /**
     * @brief Receive byte from AdamNet
     * @return byte received
     */
    uint8_t adamnet_recv();

    /**
     * @brief Receive desired # of bytes into buffer from AdamNet
     * @param buf Buffer in which to receive
     * @param len length of buffer
     * @return # of bytes received.
     */
    unsigned short adamnet_recv_buffer(uint8_t *buf, unsigned short len);

    /**
     * @brief Wait for AdamNet bus to become idle.
     */
    void adamnet_wait_for_idle();

    /**
     * @brief Device Number: 0-15
     */
    uint8_t _devnum;

    virtual void shutdown() {}

    /**
     * @brief process the next packet with the active device.
     * @param b first byte of packet.
     */
    virtual void adamnet_process(uint8_t b);

public:
    /**
     * @brief return the device number (0-15) of this device
     * @return the device # (0-15) of this device
     */
    uint8_t id() { return _devnum; }
};

/**
 * @brief The AdamNet Bus
 */
class adamNetBus
{
private:
    std::forward_list<adamNetDevice *> _daisyChain;
    adamNetDevice *_activeDev = nullptr;

    void _adamnet_process_cmd();
    void _adamnet_process_queue();

public:
    void setup();
    void service();
    void shutdown();

    int numDevices();
    void addDevice(adamNetDevice *pDevice, int device_id);
    void remDevice(adamNetDevice *pDevice);
    adamNetDevice *deviceById(int device_id);
    void changeDeviceId(adamNetDevice *pDevice, int device_id);
    QueueHandle_t qAdamNetMessages = nullptr;
};

extern adamNetBus AdamNet;

#endif /* ADAMNET_H */