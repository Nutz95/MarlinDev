#ifndef MQ2_DETECTOR_CPP
#define MQ2_DETECTOR_CPP
#include "MarlinFirmware.h"


#if ENABLED (MQ2_GAZ_DETECTOR)
#include "Gaz_Detector.h"

const float           LPGCurve[3]  =  {2.3,0.21,-0.47};   //two points are taken from the curve. 
                                                    //with these two points, a line is formed which is "approximately equivalent"
                                                    //to the original curve. 
                                                    //data format:{ x, y, slope}; point1: (lg200, 0.21), point2: (lg10000, -0.59)
const float           COCurve[3]  =  {2.3,0.72,-0.34};    //two points are taken from the curve. 
                                                    //with these two points, a line is formed which is "approximately equivalent" 
                                                    //to the original curve.
                                                    //data format:{ x, y, slope}; point1: (lg200, 0.72), point2: (lg10000,  0.15) 
const float           SmokeCurve[3] ={2.3,0.53,-0.44};    //two points are taken from the curve. 
                                                    //with these two points, a line is formed which is "approximately equivalent" 
                                                    //to the original curve.
                                                    //data format:{ x, y, slope}; point1: (lg200, 0.53), point2: (lg10000,  -0.22) 



MQSmokeDetector::MQSmokeDetector(){ 
Ro = 10;
}

float MQSmokeDetector::ResistanceCalculation(int raw_adc)
{
  return ( ((float)RL_VALUE * (1023 - raw_adc) / raw_adc));
}

void MQSmokeDetector::Calibration(int mq_pin, bool useAsAnalogInput)
{
  int i;
  float val = 0;

  if(useAsAnalogInput == true)
  {
	  for (i = 0; i < CALIBARAION_SAMPLE_TIMES; i++ ) 
	  {            //take multiple samples
		val += ResistanceCalculation(analogRead(mq_pin));
		delay(CALIBRATION_SAMPLE_INTERVAL);
	  }
	  val = val / CALIBARAION_SAMPLE_TIMES;                   //calculate the average value

	  Ro = val / RO_CLEAN_AIR_FACTOR;                        //divided by RO_CLEAN_AIR_FACTOR yields the Ro 
															//according to the chart in the datasheet 
	}
	else
	{
	     pinMode(mq_pin, INPUT);      // sets the digital pin as input
	}

}

 int MQSmokeDetector::ReadDigitalPin(int mq_pin)
 {
      return  digitalRead(mq_pin);   // read the input pin
 }

float MQSmokeDetector::Read(int mq_pin)
{
  return ResistanceCalculation(analogRead(mq_pin)); 
}

int MQSmokeDetector::GetGasPPM(float rs_ro_ratio, int gas_id)
{
  if ( gas_id == GAS_LPG ) 
  {
     return GetPPM(rs_ro_ratio,LPGCurve);
  } else if ( gas_id == GAS_CO ) 
  {
     return GetPPM(rs_ro_ratio,COCurve);
  } else if ( gas_id == GAS_SMOKE ) 
  {
     return GetPPM(rs_ro_ratio,SmokeCurve);
  }    

  return 0;
}

int  MQSmokeDetector::GetPPM(float rs_ro_ratio,const float *pcurve)
{
  return (pow(10,( ((log(rs_ro_ratio) - pcurve[1]) / pcurve[2]) + pcurve[0])));
}

#endif 
#endif


