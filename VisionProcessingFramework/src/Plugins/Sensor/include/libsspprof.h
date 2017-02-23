#ifndef __LIBSSPPROFILE_H
#define __LIBSSPPROFILE_H

struct RegisterSetting{
  int Address;
  int Data;
  char *Comment;
};

struct ssp_profile {
  char *Comment;
  struct __ImageProperty{
    int Width;
    int Height;
    int Frequency;
    int BayerBits;
  }ImageProperty;
  int CCIAddress;
  int NumLanes;
  struct RegisterSetting *SensorMatching;
  int NumSensorMatching;
  struct RegisterSetting *RegisterSettings;
  int NumRegisterSettings;
  struct RegisterSetting *StartStreamSettings;
  int NumStartStreamSettings;
  struct RegisterSetting *StopStreamSettings;
  int NumStopStreamSettings;
  char *CheckSum;
};

SSP_RESULT ssp_read_profile(char *profile_uri, struct ssp_profile **profile) ;


#endif //__LIBSSPPROFILE_H
