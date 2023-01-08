#include <ustring.h>
#include <stdint.h>
#include <stdarg.h>
#include <kstdio.h>

char *convertToStr(char *format, va_list list) {
    char *tr;
	uint32_t i;
	uint8_t *str;
	double dval;

    char ret[256];
    int ptr = 0;
	//uint32_t *intval;


	for(tr = format;*tr != '\0';tr++)
	{
		while(*tr != '%' && *tr!='\0')
		{
		// UART_SendChar(USART2,*tr);
            ret[ptr++] = *tr;
		    tr++;
		}

		if(*tr == '\0') break;

		tr++;

		switch (*tr)
		{
        case 'c': i = va_arg(list,int);
            // UART_SendChar(USART2,i);
            ret[ptr++] = i;
            break;
        case 'd': i = va_arg(list,int);
            if(i<0)
            {
                // UART_SendChar(USART2,'-');
                ret[ptr++] = '-';
                i=-i;				
            }
            // _USART_WRITE(USART2,(uint8_t*)convert(i,10));
            
            uint8_t* cur = convert(i, 10);
            int cur_ptr = 0;
            while(cur[cur_ptr] != 0) {
                ret[ptr++] = cur[cur_ptr++];
            }
            break;
        case 'o': i = va_arg(list,int);
            if(i<0)
            {
                // UART_SendChar(USART2,'-');
                ret[ptr++] = '-';
                i=-i;				
            }
            // _USART_WRITE(USART2,(uint8_t*)convert(i,8));

            cur = convert(i, 8);
            cur_ptr = 0;
            while(cur[cur_ptr] != 0) {
                ret[ptr++] = cur[cur_ptr++];
            }

            break;

        case 'x': i = va_arg(list,int);
            if(i<0)
            {
                // UART_SendChar(USART2,'-');
                ret[ptr++] = '-';
                i=-i;				
            }
            // _USART_WRITE(USART2,(uint8_t*)convert(i,16));

            cur = convert(i, 16);
            cur_ptr = 0;
            while(cur[cur_ptr] != 0) {
                ret[ptr++] = cur[cur_ptr++];
            }

            break;
        case 's': str = va_arg(list,uint8_t*);
            // _USART_WRITE(USART2,str);

            cur = str;
            cur_ptr = 0;
            while(cur[cur_ptr] != 0) {
                ret[ptr++] = cur[cur_ptr++];
            }

            break;
        case 'f': 
            dval = va_arg(list,double);
            // _USART_WRITE(USART2,(uint8_t*)float2str(dval));

            cur = (uint8_t*)float2str(dval);
            cur_ptr = 0;
            while(cur[cur_ptr] != 0) {
                ret[ptr++] = cur[cur_ptr++];
            }

            break;	
        default:
            break;
		}
	}
    ret[ptr] = 0;
	va_end(list);
    // kprintf("Inside Ustring : %s\n", ret);

    char *ret_ptr = ret;
    return ret_ptr;
}