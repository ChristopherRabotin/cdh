/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mylib.hpp"
#include <stdio.h>

void Demo::say_hi(void)
{
	printf("mylib says: Hello World!");
}

void mylib_func(void){
	Demo d;
	d.say_hi();
}
/*
void mylib_func(void){
	printf("test")
}
*/
