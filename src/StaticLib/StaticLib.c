#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーからほとんど使用されていない部分を除外する
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
	if (q == NULL || q->memory_begin == NULL) return false;

	if (countQueueableElements(q) < 1) return false;

	*q->head++ = val;

	if (q->memory_end <= q->head) q->head -= (q->memory_end - q->memory_begin);

	return true;
}


// addrから始まるnum個の整数をキューに入れる。実行の成否を返す
bool enqueue_array(QUEUE* q, int* addr, int num)
{
	if (q == NULL || q->memory_begin == NULL || addr == NULL || num <= 0) return false;
	if (countQueueableElements(q) < num) return false;

	if (q->head + num < q->memory_end) {
		memcpy(q->head, addr, sizeof(int) * num);
		q->head += num;
	}
	else {
		int n = q->memory_end - q->head;
		memcpy(q->head, addr, sizeof(int) * n);
		memcpy(q->memory_begin, addr + n, sizeof(int) * (num - n));
		q->head = q->memory_begin + (num - n);
	}

	return true;
}

// キューから一つの要素を取り出す(不具合時は0を返す)
int dequeue(QUEUE* q)
{
	if (q == NULL || q->memory_begin == NULL) return 0;
	if (isEmpty(q)) return 0;

	return *q->tail++;
}

// addrにキューからnumの要素を取り出す。取り出せた個数を返す
int dequeue_array(QUEUE* q, int* addr, int num)
{
	if (q == NULL || q->memory_begin == NULL || addr == NULL || num <= 0) return 0;
	if (isEmpty(q)) return 0;

	int n = countQueuedElements(q);
	if (n < num) num = n;

	if (q->tail + num < q->memory_end) {
		memcpy(addr, q->tail, sizeof(int) * num);
		q->tail += num;
	}
	else {
		int n = q->memory_end - q->tail;
		memcpy(addr, q->tail, sizeof(int) * n);
		memcpy(addr, q->memory_begin, sizeof(int) * (num - n));
		q->tail = q->memory_begin + (num - n);
	}

	return num;
}

// キューが空かどうかを調べる
bool isEmpty(const QUEUE* q)
{
	if (q == NULL) return false;

	return q->head == q->tail;
}

// 挿入されたデータ数を得る
int countQueuedElements(const QUEUE* q)
{
	if (q == NULL || q->memory_begin == NULL) return 0;

	int max_counts = q->memory_end - q->memory_begin;
	return (q->head + max_counts - q->tail) % max_counts;
}

// 挿入可能なデータ数を得る
int countQueueableElements(const QUEUE* q)
{
	int max_counts = q->memory_end - q->memory_begin;
	return max_counts - countQueuedElements(q) - 1;

}