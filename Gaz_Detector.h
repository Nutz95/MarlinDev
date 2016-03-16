#ifndef MQ2_DETECTOR_h
#define MQ2_DETECTOR_h
#include "MarlinFirmware.h"

#ifdef MQ2_GAZ_DETECTOR


#define         RL_VALUE                     (5)     //define the load resistance on the board, in kilo ohms
#define         RO_CLEAN_AIR_FACTOR          (9.83)  //RO_CLEAR_AIR_FACTOR=(Sensor resistance in clean air)/RO,
                                                     //which is derived from the chart in datasheet

#define         CALIBARAION_SAMPLE_TIMES     (10)    //define how many samples you are going to take in the calibration phase
#define         CALIBRATION_SAMPLE_INTERVAL  (30)   //define the time interal(in milisecond) between each samples in the
                                                     //cablibration phase
#define         GAS_LPG                      (0)
#define         GAS_CO                       (1)
#define         GAS_SMOKE                    (2)



class MQSmokeDetector{ //: public Stream
  public:
    MQSmokeDetector();
    void Calibration(int mq_pin, bool useAsAnalogInput);
    float           Ro;                 //Ro is initialized to 10 kilo ohms
    float ResistanceCalculation(int raw_adc);
 
 float Read(int mq_pin);
 int  GetGasPPM(float rs_ro_ratio, int gas_id);
 int  GetPPM(float rs_ro_ratio,const float *pcurve);
 int ReadDigitalPin(int mq_pin);
 
private: 
    
     
};
#endif 
#endif

