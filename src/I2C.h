#pragma once
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <cerrno>
#include <string>
#include <cstring>
#include <iostream>
#include <cstdint>

class I2CMaster {
public:
    I2CMaster(const std::string& port) : _port(port) {
        if ((_fd = open(_port.c_str(), O_RDWR)) < 0) {
            throw std::runtime_error("I2C open error");
        }
    }

    bool readReg(uint8_t address, uint8_t reg, uint8_t *data, uint32_t len) {
        if (ioctl(_fd, I2C_SLAVE, address) < 0) {
            return false;
        }

        if (write(_fd, &reg, 1) != 1) {
            return false;
        }

        if (read(_fd, data, len) != static_cast<ssize_t>(len)) {
            return false;
        }
        return true;
    }

    bool writeReg(uint8_t address, uint8_t reg, const uint8_t *data, uint32_t len) {
        uint8_t buffer[len + 1];
        buffer[0] = reg;
        memcpy(buffer + 1, data, len);

        if (ioctl(_fd, I2C_SLAVE, address) < 0) {
            return false;
        }

        if (write(_fd, buffer, len + 1) != static_cast<ssize_t>(len + 1)) {
            return false;
        }
        return true;
    }

private:
    std::string _port;
    int _fd;
};