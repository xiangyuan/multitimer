/*
 * multitimer.h
 *
 *  Created on: Mar 22, 2012
 *      Author: liyajie1209
 */
#ifndef MULTITIMER_H_
#define MULTITIMER_H_
#include <stddef.h>
#include <sys/time.h>

/**
 * 回调函数
 */
typedef int (*timer_callback)(int tid);

/**
 * 定时器类型
 */
typedef struct timer_node Timer;

struct timer_node {
	Timer * next;
	int tid;//the timer id
	int interval;//timer interval
	int elapse;//控制部分
	timer_callback callback;
	void * user_data;
	int len;
};


struct timer_list {
	Timer * head;
	int num;
	void (*old_sigfun)(int);
	void (*new_sigfun)(int);
	struct itimerval ovalue;
	struct itimerval value;
};
/**
 * 定时器们的管理部分
 */
typedef struct timer_list TimerMagr;

/**
 * 初始化
 * @return
 */
int init_timers();

/**
 * 销除
 * @return true or false
 */
int destroy_timers();
/**
 * 添加定时器
 * @param interval 间隔
 * @param callback 回调函数
 * @return 定时器的编号
 */
int put_timer(int interval,timer_callback callback,int tid);

/**
 * 通过指定的编号来移出链接表中的值
 * @param tid the timer's number
 * @return isTure or false
 */
int remove_timer(int tid);

/**
 * <p>根据新的timer来更新连接表中的Timer值</p>
 * <p>以此来达到回调传值的目的</p>
 * @param tid
 * @param paramTimer 时间值
 * @return更新后的值
 */
Timer* update_timer(int tid,Timer* paramTimer);
/**
 * 得到计时器链表的长度
 * @return the len
 */
int get_timer_length();

/**
 * 信号回调函数
 * @signo 信号编号
 */
void signal_func(int signo);

#endif /* MULTITIMER_H_ */
