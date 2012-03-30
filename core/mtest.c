/*
 * mulittimer.cpp
 *
 *  Created on: Mar 24, 2012
 *      Author: liyajie1209
 */
#include <stdio.h>
#include "multitimer.h"

int i = 0;


int fun2(int tid) {
        i ++;
        printf("fun2 %d\n",tid);
        return 1;
}
int fun1(int tid) {
        i ++;
        printf("fun 1%d\n",tid);
        return 1;
}

int main(int argc, char **argv) {

        init_timers();//初始化操作

        put_timer(3,fun2,3);
        put_timer(4,fun1,4);
//        remove_timer(3);//成功
        while (1) {
                if (i > 10) {
                        printf("msg....");
                        destroy_timers();
                        break;
                }
//              printf("msg..");
        }
        return 0;
}


