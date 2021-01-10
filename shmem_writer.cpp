 /******************************************************************
	@file    shmem_writer.cpp
	@author  yll(1711019653@qq.com)
	@date    2020-08-14 14:14:08
	@description: 
 *******************************************************************/
 
#include <iostream>
#include <stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>
using std::cout;
using std::endl;

#define BUF_SIZE 1024
#define SHM_KEY  0x1234

typedef struct shmseg 
{
	int cnt;
	int complete;
	char buf[BUF_SIZE];
}shmseg;

int fill_buffer(char* bufptr, int size);

int main(void)
{
	int iShmID, iNumTimes;
	shmseg* pShm = NULL;
	int iSpace;
	char* pBuf= NULL;
	//创建共享内存
	iShmID = shmget(SHM_KEY, sizeof(shmseg), 0644|IPC_CREAT);//0644有什么含义
	/* 
	 * 0为八进制标识符, 644分别表示归属者、同组者、其它权限: 读写执行 110 010 010
	 * IPC_CREAT 表示创建一个新的shmem 
	 * IPC_EXCL
	 * 表示将该SHM_KEY附着一个已经存在的共享内存.若不存在，则返回错误码
	 * */
	if(iShmID == -1)
	{
		perror("Shared memoey");
		return -1;
	}
	
	//Attach to segment for a pointer to it;
	pShm = shmat(iShmID, NULL, 0);// difference by second param whether is NULL or not ;
	if(pShm == (void*) -1)
	{
		perror("Shared memory attach");
		return -1;
	}

	pBuf = pShm->buf;
	iSpace = BUF_SIZE;
	for(iNumTimes = 0;iNumTimes < 5; ++iNumTimes)
	{
		pShm->cnt = fill_buffer(pBuf, iSpace);
		cout << "Writing Process: Shared Memory: Wrote " << pShm->cnt << " bytes." << endl;
		pBuf = pShm->buf;
		iSpace = BUF_SIZE;
		sleep(3);
	}

	printf("Writing Process: Wrote %d times\n", iNumTimes);
	shm->complete = 1;
}

int fill_buffer(char * bufptr, int size) {
	static char ch = 'A';
	int filled_count;
		     
	printf("size is %d\n", size);
	memset(bufptr, ch, size - 1);
	bufptr[size-1] = '\0';
	if (ch > 122)
	ch = 65;
	if ( (ch >= 65) && (ch <= 122) ) {
		if ( (ch >= 91) && (ch <= 96) ) {
	   	ch = 65;
		}
	}
	filled_count = strlen(bufptr);
	
	printf("buffer count is: %d\n", filled_count);
	printf("buffer filled is:%s\n", bufptr);
	ch++;
	return filled_count;
}


