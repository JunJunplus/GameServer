/**
 * \file
 * \version  $Id: ThreadPoolTest.cpp  $
 * \author 
 * \date 
 * \brief ���������̳߳�ģ��
 *
 * 
 */


#include <iostream>
#include <vector>
#include <list>
#include <iterator>

#include "zTCPServer.h"
#include "zTCPTaskPool.h"
#include "zService.h"
#include "Zebra.h"

class TCPTaskTest : public zTCPTask
{

	public:

		TCPTaskTest(
				zTCPTaskPool *pool,
				const int sock,
				const struct sockaddr_in *addr = NULL) : zTCPTask(pool, sock, addr) {};

		~TCPTaskTest() {};

		bool msgParse(const Cmd::t_NullCmd *, const unsigned int);

};

bool TCPTaskTest::msgParse(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	pSocket->sendCmd(ptNullCmd, nCmdLen);
	return true;
}

class ThreadPoolService : public zService
{

	public:

		static ThreadPoolService &getInstance()
		{
			if (NULL == instance)
				instance = new ThreadPoolService("�������");

			return *instance;
		}

		/**
		 * \brief �ͷ����Ψһʵ��
		 *
		 */
		static void delInstance()
		{
			if (instance)
			{
				delete instance;
				instance = NULL;
			}
		}

	private:

		static ThreadPoolService *instance;

		ThreadPoolService(const std::string &name) : zService(name)
		{
			tcpServer = NULL;
		};

		bool init();
		bool serviceCallback();
		void final();

		zTCPServer *tcpServer;
		zTCPTaskPool *taskPool;

};

ThreadPoolService *ThreadPoolService::instance = NULL;

bool ThreadPoolService::init()
{
	if (!zService::init())
		return false;

	Zebra::logger->debug(__PRETTY_FUNCTION__);

	int state = state_none;
	Zebra::to_lower(Zebra::global["initThreadPoolState"]);
	if ("repair" == Zebra::global["initThreadPoolState"]
			|| "maintain" == Zebra::global["initThreadPoolState"])
		state = state_maintain;
	taskPool = new zTCPTaskPool(atoi(Zebra::global["threadPoolCapacity"].c_str()), state);
	if (NULL == taskPool
			|| !taskPool->init())
		return false;

	tcpServer = new zTCPServer("���Է�����");
	if (NULL == tcpServer)
		return false;
	if (!tcpServer->bind("���Է�����", 9000))
		return false;

	return true;
}

bool ThreadPoolService::serviceCallback()
{
	//Zebra::logger->debug("%s", __PRETTY_FUNCTION__);
	struct sockaddr_in addr;
	int retcode = tcpServer->accept(&addr);
	if (retcode > 0) 
	{
		Zebra::logger->debug("���������յ��ͻ�������", inet_ntoa(addr.sin_addr));
		zTCPTask *tcpTask = new TCPTaskTest(taskPool, retcode, &addr);
		if (!taskPool->addVerify(tcpTask))
		{
			delete tcpTask;
			tcpTask = NULL;
		}
	}

	return true;
}

void ThreadPoolService::final()
{
	Zebra::logger->debug("%s", __PRETTY_FUNCTION__);
	if (tcpServer)
	{
		delete tcpServer;
		tcpServer = NULL;
	}
	if (taskPool)
	{
		taskPool->final();
		delete taskPool;
		taskPool = NULL;
	}
}

int main(int argc, char **argv)
{
	Zebra::logger=new zLogger();

	ThreadPoolService::getInstance().main();
	ThreadPoolService::delInstance();

	return EXIT_SUCCESS;
}

