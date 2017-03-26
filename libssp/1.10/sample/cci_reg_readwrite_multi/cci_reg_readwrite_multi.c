#include<stdio.h>

#define IMX219_I2C_ADDR 0x10
#define IU233_I2C_ADDR 0x10
#define IMX408_I2C_ADDR 0x1A
#define IMX290_I2C_ADDR 0x1A

#define CCI_SLAVE_ADDR IMX219_I2C_ADDR

int main(){
  unsigned char RegRData[10];
  int i;
  
  if(__CCIRegReadMBySlaveAddress(CCI_SLAVE_ADDR, 0, RegRData, 10) == 10){
    for(i=0;i<10;i++)
      printf("CCI Slave Read Address: 0x%x, Reg Address: 0x%.4x, Reg Data: 0x%.2x\n",
	     CCI_SLAVE_ADDR, i, RegRData[i]);
  }
  else{
    printf("Error.\n");
    return 0;
  }

  for(i=0;i<10;i++)
    RegRData[i] ++;
  if(__CCIRegWriteMBySlaveAddress(CCI_SLAVE_ADDR, 0, RegRData, 10) == 10){
    for(i=0;i<10;i++)
      printf("CCI Slave Write Address: 0x%x, Reg Address: 0x%.4x, Reg Data: 0x%.2x\n",
	     CCI_SLAVE_ADDR, i, RegRData[i]);
  }
  else{
    printf("Error.\n");
    return 0;
  }


  return 1;
}
