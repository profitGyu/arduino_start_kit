#include "ModbusManager.h"

// Modbus 실제 객체 생성
ModbusRTU mb;

// 내부에서만 사용하는 레지스터 초기 등록 함수 (static)
static void regBankInit() {
    // 1. Coils (00001 ~ 00008)
    mb.addCoil(0, true);
    mb.addCoil(1, false);
    mb.addCoil(2, false);
    mb.addCoil(3, false);
    mb.addCoil(4, false);
    mb.addCoil(5, false);
    mb.addCoil(6, false);
    mb.addCoil(7, false);
    mb.addCoil(8, false);

    // 2. Discrete Inputs (10001 ~ 10005)
    mb.addIsts(0, true);
    mb.addIsts(1, true);
    mb.addIsts(2, true);
    mb.addIsts(3, true);
    mb.addIsts(4, false);

    // 3. Input Registers (30001 ~ 30005)
    mb.addIreg(0, 1);
    mb.addIreg(1, 2);
    mb.addIreg(2, 3);
    mb.addIreg(3, 4);
    mb.addIreg(4, 5);

    // Holding Registers
    mb.addHreg(0, 1);      
    mb.addHreg(1, 2);      
    mb.addHreg(2, 2);      
    mb.addHreg(3, 4);      
    mb.addHreg(4, 5);
    mb.addHreg(5, 6);      
    mb.addHreg(6, 7);      
    mb.addHreg(7, 8);      
    mb.addHreg(8, 9);      
    mb.addHreg(9, 10);
}

// 외부에서 호출할 전체 초기화 함수
void initModbus(HardwareSerial* serialPort, uint8_t slaveId) {
    mb.begin(serialPort);
    mb.slave(slaveId);
    
    regBankInit();       
    // updateModbusRegisters();
}

// 레지스터 값을 일괄 갱신하거나 강제 세팅할 때 호출하는 함수
void updateModbusRegisters() {
    // Coils
    mb.Coil(0, true);   
    mb.Coil(1, true);   
    mb.Coil(2, false);  
    mb.Coil(3, true);   
    mb.Coil(4, true);
    
    // Discrete Inputs
    mb.Ists(0, true);   
    mb.Ists(1, true);   
    mb.Ists(2, true);   
    mb.Ists(3, true);   
    mb.Ists(4, false);

    // Input Registers
    mb.Ireg(0, 1);      
    mb.Ireg(1, 2);      
    mb.Ireg(2, 3);      
    mb.Ireg(3, 4);      
    mb.Ireg(4, 5);

    // Holding Registers
    mb.Hreg(0, 1);      
    mb.Hreg(1, 2);      
    mb.Hreg(2, 2);      
    mb.Hreg(3, 4);      
    mb.Hreg(4, 5);
    mb.Hreg(5, 6);      
    mb.Hreg(6, 7);      
    mb.Hreg(7, 8);      
    mb.Hreg(8, 9);      
    mb.Hreg(9, 10);
}