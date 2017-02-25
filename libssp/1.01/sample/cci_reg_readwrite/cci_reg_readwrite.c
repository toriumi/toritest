#include<stdio.h>

#define IMX219_I2C_ADDR 0x10
#define IU233_I2C_ADDR 0x10
#define IMX408_I2C_ADDR 0x1A
#define IMX290_I2C_ADDR 0x1A

#define CCI_SLAVE_ADDR IMX219_I2C_ADDR

int main(){
  int RegData;
  int i;
  
  for(i=0;i<10;i++){
    if(__CCIRegReadBySlaveAddress(CCI_SLAVE_ADDR, i*4, &RegData) > 0)
      printf("CCI Slave Address: 0x%x, Reg Address: 0x%.4x, Reg Data: 0x%.2x\n",
	     CCI_SLAVE_ADDR, i*4, RegData);
    else{
      printf("Error.\n");
      break;
    }
  }

  RegData = 0x0;
  if(__CCIRegWriteBySlaveAddress(CCI_SLAVE_ADDR, 0x0, RegData) > 0)
    printf("CCI Slave Address: 0x%x, Reg Address: 0x%.4x, Reg Data: 0x%.2x\n",
	   CCI_SLAVE_ADDR, 0x0, RegData);
  
  RegData = 0x02;
  if(__CCIRegWriteBySlaveAddress(CCI_SLAVE_ADDR, 0x0, RegData) > 0)
    printf("CCI Slave Address: 0x%x, Reg Address: 0x%.4x, Reg Data: 0x%.2x\n",
	   CCI_SLAVE_ADDR, 0x0, RegData);

    for(i=0;i<10;i++){
    if(__CCIRegReadBySlaveAddress(CCI_SLAVE_ADDR, i*4, &RegData) > 0)
      printf("CCI Slave Address: 0x%x, Reg Address: 0x%.4x, Reg Data: 0x%.2x\n",
	     CCI_SLAVE_ADDR, i*4, RegData);
    else{
      printf("Error.\n");
      break;
    }
  }

  return 1;
}
