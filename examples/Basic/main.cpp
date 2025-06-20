#include <iostream>
#include <iomanip>
#include <math.h>
#include <thread>
#include <chrono>
#include "ICM_20948.h"

int main()
{
    ICM_20948 myICM ("/dev/i2c-1", 0x68);
    
    ICM_20948_fss_t myFSS;
    myFSS.a = gpm4; // (ICM_20948_ACCEL_CONFIG_FS_SEL_e)
                    // gpm2
                    // gpm4
                    // gpm8
                    // gpm16
    
    myFSS.g = dps500;   // (ICM_20948_GYRO_CONFIG_1_FS_SEL_e)
                        // dps250
                        // dps500
                        // dps1000
                        // dps2000
    
    auto status = myICM.setFullScale((ICM_20948_Internal_Acc | ICM_20948_Internal_Gyr), myFSS);
    std::cout << "Set full scale returned: " << myICM.statusString(status) << std::endl;
    
    status = myICM.setSampleMode((ICM_20948_Internal_Acc | ICM_20948_Internal_Gyr), ICM_20948_Sample_Mode_Continuous);
    ICM_20948_smplrt_t mySmplrt;
    //mySmplrt.g = 19; // ODR is computed as follows: 1.1 kHz/(1+GYRO_SMPLRT_DIV[7:0]). 19 = 55Hz. InvenSense Nucleo example uses 19 (0x13).
    //mySmplrt.a = 19; // ODR is computed as follows: 1.125 kHz/(1+ACCEL_SMPLRT_DIV[11:0]). 19 = 56.25Hz. InvenSense Nucleo example uses 19 (0x13).
    mySmplrt.g = 4; // 225Hz
    mySmplrt.a = 4; // 225Hz
    //mySmplrt.g = 8; // 112Hz
    //mySmplrt.a = 8; // 112Hz
    status = myICM.setSampleRate((ICM_20948_Internal_Acc | ICM_20948_Internal_Gyr), mySmplrt); 
    std::cout << "Set sample rate returned: " << myICM.statusString(status) << std::endl;

    // Set up Digital Low-Pass Filter configuration
    ICM_20948_dlpcfg_t myDLPcfg;    // Similar to FSS, this uses a configuration structure for the desired sensors
    myDLPcfg.a = acc_d5bw7_n8bw3; // (ICM_20948_ACCEL_CONFIG_DLPCFG_e)
                                    // acc_d246bw_n265bw      - means 3db bandwidth is 246 hz and nyquist bandwidth is 265 hz
                                    // acc_d111bw4_n136bw
                                    // acc_d50bw4_n68bw8
                                    // acc_d23bw9_n34bw4
                                    // acc_d11bw5_n17bw
                                    // acc_d5bw7_n8bw3        - means 3 db bandwidth is 5.7 hz and nyquist bandwidth is 8.3 hz
                                    // acc_d473bw_n499bw

    myDLPcfg.g = gyr_d5bw7_n8bw9; // (ICM_20948_GYRO_CONFIG_1_DLPCFG_e)
                                        // gyr_d196bw6_n229bw8
                                        // gyr_d151bw8_n187bw6
                                        // gyr_d119bw5_n154bw3
                                        // gyr_d51bw2_n73bw3
                                        // gyr_d23bw9_n35bw9
                                        // gyr_d11bw6_n17bw8
                                        // gyr_d5bw7_n8bw9
                                        // gyr_d361bw4_n376bw5

    status = myICM.setDLPFcfg((ICM_20948_Internal_Acc | ICM_20948_Internal_Gyr), myDLPcfg);

    status = myICM.enableDLPF(ICM_20948_Internal_Acc, true);
    std::cout << "Enable DLPF for Accelerometer returned: " << myICM.statusString(status) << std::endl;
    status = myICM.enableDLPF(ICM_20948_Internal_Gyr, true);
    std::cout << "Enable DLPF for Gyroscope returned: " << myICM.statusString(status) << std::endl;

    while(true) {
        if (myICM.dataReady())
        {
            auto agmt = myICM.getAGMT();
            std::cout << "Accel X: " << agmt.acc.axes.x << "\tY: " << agmt.acc.axes.y << "\tZ: " << agmt.acc.axes.z; 
            std::cout << "\tGyro X: " << agmt.gyr.axes.x << "\tY: " << agmt.gyr.axes.y << "\tZ: " << agmt.gyr.axes.z; 
            std::cout << "\tMag X: " << agmt.mag.axes.x << "\tY: " << agmt.mag.axes.y << "\tZ: " << agmt.mag.axes.z; 
            std::cout << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
}