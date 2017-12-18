#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include <stdarg.h>
#include<float.h>

double bit_and(double num,double num2){
	uint64_t prim,seg;
	union {
		double num;
		uint64_t u64;
	} u = {num};
	prim=u.u64;
	u.num=num2;
	seg=u.u64;
	u.u64=prim&seg;
	return u.num;
}

double bit_or(double num,double num2){
	uint64_t prim,seg;
	union {
		double num;
		uint64_t u64;
	} u = {num};
	prim=u.u64;
	u.num=num2;
	seg=u.u64;
	u.u64=prim|seg;
	return u.num;
}

double bit_xor(double num,double num2){
	uint64_t prim,seg;
	union {
		double num;
		uint64_t u64;
	} u = {num};
	prim=u.u64;
	u.num=num2;
	seg=u.u64;
	u.u64=prim^seg;
	return u.num;
}

double bit_not(double num){
	uint64_t prim,seg;
	union {
		double num;
		uint64_t u64;
	} u = {num};
	u.u64=~u.u64;
	return u.num;
}

double bit_lshift(double num,int shift){
	uint64_t prim,seg;
	union {
		double num;
		uint64_t u64;
	} u = {num};
	u.u64<<=shift;
	return u.num;
}

double bit_rshift(double num,int shift){
	uint64_t prim,seg;
	union {
		double num;
		uint64_t u64;
	} u = {num};
	u.u64>>=shift;
	return u.num;
}


double average(int num,...) {

	va_list valist;
	double sum = 0.0;
	int i;
	double bla;

	/* initialize valist for num number of arguments */
	va_start(valist, num);

	/* access all the arguments assigned to valist */
	for (i = 0; i <= num; i++) {
	sum+=va_arg(valist, double);
	}

	/* clean memory reserved for valist */
	va_end(valist);

	return sum/(num+1);
}

double sumatorio(int num,...) {

	va_list valist;
	double sum = 0.0;
	int i;

	/* initialize valist for num number of arguments */
	va_start(valist, num);

	/* access all the arguments assigned to valist */
	for (i = 0; i <= num; i++) {
  		sum+=va_arg(valist, double);
	}

	/* clean memory reserved for valist */
	va_end(valist);

	return sum;
}

double maximo(int num,...) {

	va_list valist;
	double max=-DBL_MAX,aux;
	int i;
	
	/* initialize valist for num number of arguments */
	va_start(valist, num);

	/* access all the arguments assigned to valist */
	for (i = 0; i <= num; i++) {
		aux=va_arg(valist,double);
		if(aux>max){
			max=aux;
		}
	}

	/* clean memory reserved for valist */
	va_end(valist);

	return max;
}


double minimo(int num,...) {

	va_list valist;
	double min=DBL_MAX,aux;
	int i;
	
	/* initialize valist for num number of arguments */
	va_start(valist, num);

	/* access all the arguments assigned to valist */
	for (i = 0; i <= num; i++) {
		aux=va_arg(valist,double);
		if(aux<min){
			min=aux;
		}
	}

	/* clean memory reserved for valist */
	va_end(valist);

	return min;
}
