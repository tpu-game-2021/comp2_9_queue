﻿#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーからほとんど使用されていない部分を除外する
#include "Windows.h"                    // Windows API の機能定義
#include <stdlib.h>

#include "../include/lib_func.h"


// mem_size の容量でキュー用のメモリを確保する
void initialize(QUEUE* q, size_t mem_size)
{
	if (q == NULL) return;

	q->memory_begin = (int*)malloc(mem_size);
	q->memory_end = q->memory_begin + mem_size / sizeof(int);
	q->head = q->memory_begin;
	q->tail = q->memory_begin;
}


// 確保したメモリを解放する
void finalize(QUEUE* q)
{
	if (q == NULL) return;

	free(q->memory_begin);

	q->memory_begin = NULL;
	q->memory_end = NULL;
	q->head = NULL;
	q->tail = NULL;
}


// valの値をキューに入れる。実行の成否を返す
bool enqueue(QUEUE* q, int val)
{
	if (q == NULL) return false;
	// ToDo: valのデータをキューに追加します
	// 上手くいかない場合にはfalseを返します
	// メモリを使い切ったら先頭アドレスに戻って追加して下さい
	int he = q->head - q->memory_begin, ta = q->tail - q->memory_begin, me = q->memory_end - q->memory_begin;
	if (ta + 1 >= me + he) return false;
	q->memory_begin[ta] = val;
	ta++;
	q->tail = ta + q->memory_begin;
	return true;
}
	


// addrから始まるnum個の整数をキューに入れる。実行の成否を返す
bool enqueue_array(QUEUE* q, int* addr, int num)
{
	if (q == NULL || addr == NULL || num <= 0) return false;
	// ToDo: addrからnum個のデータをキューに追加します
	// 上手くいかない場合にはfalseを返します
	// メモリを使い切ったら先頭アドレスに戻って追加して下さい
	int he = q->head - q->memory_begin, ta = q->tail - q->memory_begin, me = q->memory_end - q->memory_begin;
	if (ta + num >= me + he) return false;
	for (int i = 0; i < num; i++)
	{
		if (ta >= me) ta = he;
		q->memory_begin[ta] = addr[i];
		ta++;
		q->tail = ta + q->memory_begin;
	}
	return true;
}

// キューから一つの要素を取り出す(不具合時は0を返す)
int dequeue(QUEUE* q)
{
	if (q == NULL || q->head < 0) return 0;
	// ToDo: 先頭のデータを返します
	int he = q->head - q->memory_begin, ta = q->tail - q->memory_begin, val;
	if (he >= ta) return 0;
	val = q->memory_begin[he];
	he++;
	q->head = he + q->memory_begin;
	return val;
}

// addrにキューからnumの要素を取り出す。取り出せた個数を返す
int dequeue_array(QUEUE* q, int* addr, int num)
{
	if (q == NULL || addr == NULL || num <= 0) return 0;
	// ToDo: 先頭からnum個のデータをaddrに格納します
	int he = q->head - q->memory_begin, ta = q->tail - q->memory_begin, i;
	for (i = 0; i < num; i++)
	{
		if (he >= ta) return i;
		addr[i] = q->memory_begin[he];
		he++;
		q->head = he + q->memory_begin;
	}
	return num;
}

// キューが空かどうかを調べる
bool isEmpty(const QUEUE* q)
{
	if (q == NULL) return false;

	return q->head == q->tail;
}

static int getMaxCount(const QUEUE* q)
{
	if (q == NULL || q->memory_begin == NULL) return 0;

	return q->memory_end - q->memory_begin;
}

// 挿入されたデータ数を得る
int countQueuedElements(const QUEUE* q)
{
	if (q == NULL || q->memory_begin == NULL) return 0;

	int max_counts = getMaxCount(q);
	return (q->head + max_counts - q->tail) % max_counts;
}

// 挿入可能なデータ数を得る
int countQueueableElements(const QUEUE* q)
{
	return getMaxCount(q) - countQueuedElements(q) - 1;
}