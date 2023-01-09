/*
 * Copyright (c) 2022 
 * Computer Science and Engineering, University of Dhaka
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE UNIVERSITY AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE UNIVERSITY OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
 
#include <kstring.h>
#include <kmath.h>
#include <kstdio.h>
#include <kfloat.h>
static uint8_t __outbuf[50];
uint32_t __str_to_num(uint8_t* buff,uint8_t base)
{
	volatile uint32_t decimal=0;
    volatile uint32_t val;
	__reverse_str(buff);
	uint32_t i=0;
	while(buff[i]){
		if(buff[i]>='0' && buff[i]<='9'){
			val=(uint32_t)(buff[i]-48);
		}else if (buff[i]>='a' && buff[i]<='f')
		{
			val=(uint32_t)(buff[i]-97+10);
		}else if (buff[i]>='A' && buff[i]<='F')
        {
            val=(uint32_t)(buff[i]-65+10);
        }
        decimal+=val*__pow(base,i);
		i++;
	}
	return decimal;
}
void __reverse_str(uint8_t* buff)
{
	uint32_t n=__strlen(buff);
	uint8_t tmp;
	for(uint32_t i=0,j=n-1;i<j;i++,j--)
	{
		tmp = buff[i];
		buff[i]=buff[j];
		buff[j]=tmp;
	}
}

uint32_t __strlen(uint8_t* buff)
{
	uint32_t i=0;
	while(*buff++)i++;
	return i;
}
uint8_t * convert(uint32_t x,uint8_t base)
{
	static uint8_t baseval [] = "0123456789ABCDEF";
	uint8_t *ptr;
	ptr = &__outbuf[49];
	*ptr = '\0';
	do
	{
		*--ptr = baseval[x%base];
		x /= base;
	} while (x != 0);
	return (ptr);
}
// uint8_t *float2str(float f)
// {
// 	uint8_t *ptr;
// 	static uint8_t baseval [] = "0123456789ABCDEF";
// 	ptr = &__outbuf[49];
// 	*ptr = '\0';
// 	int32_t d = f;
// 	//kprintf("%d\n",d);
// 	uint32_t frac=get_decimal_part(f);
// 	do
// 	{
// 		*--ptr=baseval[frac%10];;
// 		frac/=10;
// 	}while(frac!=0);
// 	*--ptr='.';
// 	do
// 	{
// 		*--ptr=baseval[d%10];;
// 		d=d/10;
// 	}while (d!=0);
// 	// kprintf("Fl to str: %s\n", ptr);
// 	return (ptr);	
// }


void reverse(int lo,int hi, uint8_t* arr)
{
	if(lo >= hi){
		return;
	}
	char temp = arr[lo];
	arr[lo] = arr[hi];
	arr[hi] = temp;
	reverse(lo+1,hi-1,arr);
}


uint8_t arr[1000] = "abcdefgNNNNhijkl";
uint8_t *float2str(float num, char** str) 
{
	int i = 0;
    int isNegative = 0;
    if(num < 0){
		num = -num;
        isNegative = 1;
    }

    int floorVal = num;

    if (floorVal == 0)
    {
        arr[i] = '0';
        i++;
    }

    while(floorVal != 0){

        arr[i] = (floorVal%10)+'0';
        floorVal = floorVal/10;
        i++;
    }

    if (isNegative == 1)
    {
        arr[i] = '-';
        i++;
    }

    reverse(0,i-1,arr);

    arr[i] = '.';
    i++;

    int fractionalPartStartIndex = i;

    int fractionalPart = (num - ((int)num)) * 1000;

    if (fractionalPart == 0)
    {
        arr[i] = '0';
        i++;
    }
    
    int cnt = 3;

    while(cnt--){

        arr[i] = (fractionalPart%10)+'0';
        fractionalPart = fractionalPart/10;
        i++;
    }

    arr[i] = '\0';

    reverse(fractionalPartStartIndex,i-1,arr);

	// kprintf("inside kstring : %s, %x\n", arr, arr);

	(*str) = (char*) arr;
	
	return (&arr[0]);
}


float str2float(uint8_t* buff)
{
	float f=0;
	//first add the fractional part
	uint32_t i=0;
	uint8_t j=0;
	while(buff[i]!='.' && buff[i]!='\0') i++;
	if(buff[i]=='.'){
		buff[i]='\0';
		i++;
		j=1;
		while(buff[i]!='\0')
		{
			f=f+(buff[i]-48)/(__pow(10,j));
			j++;
			i++;
		}
	}
	__reverse_str(buff);
	
	i=0;
	while(buff[i]!='\0')
	{
		f=f+((buff[i]-48)*__pow(10,i));
		i++;
	}
	// kprintf("str to float : %f\n", f);
	return f;
}


// int power(int num,int pow)
// {
// 	int ans = 1;
// 	while(pow != 0){
// 		ans *= num;
// 		pow--;
// 	}
// 	return ans;
// }


// float str2float(uint8_t* arr) 
// {
// 	float* num;
// 	int size = 0;
//     int i = 0;
//     int isNegative = 0;

//     if(arr[i] == '-'){
//         isNegative = 1;
//         size++;
//         i++;
//     }

//     while(arr[i] != '.'){
//         i++;
//         size++;
//     }

//     i = 0;
//     (*num) = 0.0;
//     int fractionalPartStartIndex = size+1;
//     size--;

//     while(arr[size] != '-' && size >= 0){
//         (*num) += (float)((arr[size]-'0')*power(10,i));
//         i++;
//         size--;
//     }

//     i = 1;
//     float multiplier = 1.0;

//     while(arr[fractionalPartStartIndex] != '\0'){

//         multiplier = multiplier*1/10;
//         (*num) += (arr[fractionalPartStartIndex]-'0')*multiplier;
//         i++;
//         fractionalPartStartIndex++;
//     }

//     if (isNegative == 1){
//         (*num) = -(*num);
//     }
// 	return *num;
// }