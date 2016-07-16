#include "common.h"
#include "include.h"
#include "swj.h"

void swj_lz(uint8 (*ab)[136],uint8 c)
{
  uint32 i,j;
  uint8 a[22440];
  uint8 b[5644];
  for(i=0;i<165;i++)
  {
    for(j=0;j<136;j++)
    {
      a[i*136+j]=ab[i][j];
      //if(a[i*130+j]==0xff) a[i*130+j]==0xfe;
    }
  }
  for(i=0;i<165;i=i+2)
  {
    for(j=0;j<136;j=j+2)
    {
      b[i/2*68+j/2]=ab[i][j];
    }
  }
  if(c==0)
  {
    uart_putchar (UART0,0xff);
    uart_putbuff (UART0,a, 22440);
  }
  if(c==1)
  {
    uart_putchar (UART0,0xff);
    uart_putbuff (UART0,b, 5644);
  }
  if(c==2)
  {
    Draw_BMP(0,0,68,56,b);
    //Draw_BMP(0,0,77,7,yao1);
  }
}  