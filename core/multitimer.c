/*
 * multitimer.c
 *
 *  Created on: Mar 22, 2012
 *      Author: liyajie1209
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "multitimer.h"

TimerMagr * timerMgr;
/**
 * 初始化
 * @return
 */
int init_timers() {
	int ret = 0;
	timerMgr = (TimerMagr*) malloc(sizeof(TimerMagr));
	memset(timerMgr, 0, sizeof(TimerMagr));
	timerMgr->head = NULL;
	if ((timerMgr->old_sigfun = signal(SIGALRM, signal_func)) == SIG_ERR) {
		return -1;
	}
	timerMgr->new_sigfun = signal_func;
	timerMgr->value.it_value.tv_sec = 0;
	timerMgr->value.it_value.tv_usec = 500;
	timerMgr->value.it_interval.tv_sec = 1; //每一秒执行一次
	timerMgr->value.it_interval.tv_usec = 0;
	ret = setitimer(ITIMER_REAL, &timerMgr->value, &timerMgr->ovalue);
	return ret;
}

/**
 * 销除
 * @return true or false
 */
int destroy_timers() {
	Timer* node = NULL;
	if ((signal(SIGALRM, timerMgr->old_sigfun)) == SIG_ERR) {
		return -1;
	}
	if ((setitimer(ITIMER_REAL, &timerMgr->value, &timerMgr->ovalue)) < 0) {
		return -1;
	}
	while (timerMgr->head != NULL) {
		node = timerMgr->head;
		timerMgr->head = node->next;
		free(node->user_data);
		free(node);
	}
	memset(timerMgr, 0, sizeof(TimerMagr));
	return 0;
}
/**
 * 添加定时器
 * @param interval 间隔
 * @param callback 回调函数
 * @param tid the timer's id number
 * @param len the data len
 * @return 定时器的编号
 */
int put_timer(int interval, timer_callback callback, int tid) {
	Timer* node = NULL;
	node = (Timer *) malloc(sizeof(Timer));
	timerMgr->num++;
	//需要出现返回结果时才进行数据更新
//	if (u_data != NULL || len != 0) {
//		node->user_data = malloc(len);
//		memcpy(node->user_data, u_data, len);
//		node->len = len;
//	}
	node->callback = callback;
	node->interval = interval;
	node->elapse = 0;
	node->tid = tid;
	//将结点插入链接表
	node->next = timerMgr->head;
	timerMgr->head = node;
	return node->tid;
}

/**
 * 通过指定的编号来移出链接表中的值
 * @param tid the timer's number
 * @return isTure or false
 */
int remove_timer(int tid) {
	Timer* p, *pre;
	p = timerMgr->head;
	while (p != NULL && p->tid != tid) {
		pre = p;
		p = p->next;
	}
	pre->next = p->next;
	free(p->user_data);
	free(p);
	//娄值去一
	-- timerMgr->num;
	return 1;
}

/**
 * <p>根据新的timer来更新连接表中的Timer值</p>
 * <p>以此来达到回调传值的目的</p>
 * @param tid
 * @param paramTimer 时间值
 * @return更新后的值
 */
Timer* update_timer(int tid, Timer* paramTimer) {
	Timer* p = timerMgr->head;
	while (p != NULL && p->tid != tid) {
		p = p->next;
	}
	p->user_data = paramTimer->user_data;
	p->len = paramTimer->len;
	return (p);
}

/**
 * 得到计时器链表的长度
 * @return the len
 */
int get_timer_length() {
	int len = 0;
	if (timerMgr != NULL) {
		len = timerMgr->num;
	}
	return (len);
}
/**
 * 信号回调函数
 * @signo 信号编号
 */
void signal_func(int signo) {
	Timer* node = timerMgr->head;
	for (; node != NULL; node = node->next) {
		node->elapse++;
		if (node->elapse >= node->interval) {
			node->elapse = 0; //重新进入计时状态,调用回调函数
			node->callback(node->tid);
		}
	}
}

