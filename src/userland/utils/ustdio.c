#include <ustdio.h>
#include <ustring.h>
#include <stdint.h>
#include <stdarg.h>
#include <unistd.h>

void printf(char *format,...)
{
    char *tr;
	uint8_t *str;
	va_list list;

	va_start(list, format);

    char* ret = convertToStr(format, list);

    write(ret);
}

void scanf(char *format,...)
{
	char *ch;

    va_list list;
	char *ptr;
	uint8_t buff[50];
	ptr=format;
	va_start(list,format);
	while (*ptr)
	{
		/* code */
		if(*ptr == '%') //looking for format of an input
		{
			read(&ch);
			ptr++;
			switch (*ptr)
			{
			case /* constant-expression */ 'c': //charater
				/* code */
				*(uint8_t*)va_arg(list,uint8_t*)=ch[0];
				// kprintf("CC : %s\n", ch);
				break;
			case 'd': //integer number 
				// _USART_READ_STR(USART2,buff,50); 
				*(uint32_t*)va_arg(list,uint32_t*)=__str_to_num(ch,10);	
				break;
			case 's': //need to update -- string
				// _USART_READ_STR(USART2,buff,50); 
				*(uint32_t*)va_arg(list,uint32_t*) = ch;	
				break;
			case 'x': //hexadecimal number
				// _USART_READ_STR(USART2,buff,50); 
				*(uint32_t*)va_arg(list,uint32_t*)= __str_to_num(ch,16);	
				break;	
			case 'o': //octal number
				// _USART_READ_STR(USART2,buff,50); 
				*(uint32_t*)va_arg(list,uint32_t*)=__str_to_num(ch,8);	
				break;	
			case 'f': //floating point number
				// _USART_READ_STR(USART2,buff,50); 
				kprintf("CC : %s\n", ch);
				*(uint32_t*)va_arg(list,float*)=str2float(ch);	
				break;	
			default: //rest not recognized
				break;
			}
		}
		ptr++;
	}
	va_end(list);

}


int getTime()
{
	return utilGetTime();
}

void reboot()
{
	uReboot();
}

int getPid()
{
	return uGetPid();
}

void yield()
{
	uYield();
}