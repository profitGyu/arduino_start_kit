#ifndef MODBUS_MANAGER_H
#define MODBUS_MANAGER_H

#include <Arduino.h>
#include <ModbusRTU.h>

// 메인 파일에서도 Modbus 객체에 접근할 수 있도록 extern 선언
extern ModbusRTU mb;

// Modbus 초기화 및 관리 함수 선언
void initModbus(HardwareSerial* serialPort, uint8_t slaveId);
void updateModbusRegisters();

#endif