/**
 * \file
 * \version  $Id: SessionServer.cpp  $
 * \author  
 * \date 
 * \brief ��Ϸȫ��Session������ 
 */

#include "zSubNetService.h"
#include "Zebra.h"
#include "zMisc.h"
#include "zDBConnPool.h"
#include "SessionManager.h"
#include "SessionTaskManager.h"
#include "SessionServer.h"
#include "SessionTask.h"
#include "zConfile.h"
#include "CUnion.h"
#include "CSept.h"
#include "CDare.h"
#include "CQuiz.h"
#include "CNpcDare.h"
#include "CCityManager.h"
#include "CCountryManager.h"
#include "CDareRecordManager.h"
#include "SchoolManager.h"
#include "OfflineMessage.h"
#include "RecordClient.h"
#include "TimeTick.h"
#include "zMetaData.h"
#include "MailService.h"
#include "GmToolCommand.h"
#include "SessionChat.h"
#include "zTime.h"
#include "CSort.h"
#include "CVote.h"
#include "CArmy.h"
#include "CGem.h"
#include "Ally.h"
#include "RecommendManager.h"
#include "AuctionService.h"
#include "CartoonPetService.h"
#include "Gift.h"
#include "EmperorForbid.h"
#include <math.h>

SessionService *SessionService::instance = NULL;

zDBConnPool *SessionService::dbConnPool = NULL;

MetaData* SessionService::metaData = NULL;

std::string COfflineMessage::rootpath = "";
unsigned int dare_active_time = 0;
unsigned int dare_ready_time = 0;

unsigned int quiz_active_time = 0;
unsigned int quiz_ready_time = 0;
int dare_need_gold = 0;
int dare_winner_gold = 0;
int dare_need_gold_sept = 0;
int dare_winner_gold_sept = 0;

std::map<BYTE, worldMsg> SessionService::wMsg;
std::map<DWORD,BYTE> SessionService::userMap;

/**
 * \brief ��ʼ���������������
 *
 * ʵ�����麯��<code>zService::init</code>
 *
 * \return �Ƿ�ɹ�
 */
bool SessionService::init()
{
	//Zebra::logger->debug(__PRETTY_FUNCTION__);
	for(int i=0; i<10; i++) countryLevel[i]=0;
	uncheckCountryProcess=false;

	dbConnPool = zDBConnPool::newInstance(NULL);
	if (NULL == dbConnPool
			|| !dbConnPool->putURL(0, Zebra::global["mysql"].c_str(), false))
	{
		Zebra::logger->error("�������ݿ�ʧ��");
		return false;
	}

	metaData = MetaData::newInstance("");

	if (NULL == metaData
		|| !metaData->init(Zebra::global["mysql"]))
	{
		Zebra::logger->error("�������ݿ�ʧ��");
		return false;
	}

	//��ʼ�������̳߳�
	int state = state_none;
	Zebra::to_lower(Zebra::global["initThreadPoolState"]);
	if ("repair" == Zebra::global["initThreadPoolState"]
			|| "maintain" == Zebra::global["initThreadPoolState"])
		state = state_maintain;
	taskPool = new zTCPTaskPool(atoi(Zebra::global["threadPoolCapacity"].c_str()), state);
	if (NULL == taskPool
			|| !taskPool->init())
		return false;

	strncpy(pstrIP, zSocket::getIPByIfName(Zebra::global["ifname"].c_str()), MAX_IP_LENGTH - 1);
	//Zebra::logger->debug("%s", pstrIP);

	// ��ʼ����սϵͳ��ز���
	dare_active_time = atoi(Zebra::global["dare_active_time"].c_str());
	if (dare_active_time<=0)
	{
		dare_active_time = 3600; // Ĭ��3600��
	}
	
	dare_ready_time  = atoi(Zebra::global["dare_ready_time"].c_str());
	if (dare_ready_time<=0)
	{
		dare_ready_time = 300; // Ĭ��300��
	}

	
	dare_need_gold   = atoi(Zebra::global["dare_need_gold"].c_str());
	if (dare_need_gold<=0)
	{
		dare_need_gold = 5000; // ��ʮ��
	}
	
	dare_winner_gold = atoi(Zebra::global["dare_winner_gold"].c_str());
	if (dare_winner_gold<=0)
	{
		dare_winner_gold = 10000; // ����һ����
	}

	dare_need_gold_sept = atoi(Zebra::global["dare_need_gold_sept"].c_str());
	if (dare_need_gold_sept<=0)
	{
		//dare_need_gold_sept = 1000; // ʮ��
		dare_need_gold_sept = 0;
	}

	dare_winner_gold_sept = atoi(Zebra::global["dare_winner_gold_sept"].c_str());
	if (dare_winner_gold_sept<=0)
	{
		//dare_winner_gold_sept = 2000; // ������ʮ��
		dare_winner_gold_sept = 0;
	}
	
	// ��ʼ������ϵͳ��ز���
	quiz_active_time = atoi(Zebra::global["quiz_active_time"].c_str());
	if (quiz_active_time<=0)
	{
		quiz_active_time = 60; // Ĭ��60��
	}
	
	quiz_ready_time  = atoi(Zebra::global["quiz_ready_time"].c_str());
	if (quiz_ready_time<=0)
	{
		quiz_ready_time = 10; // Ĭ��10��
	}

	if (!zSubNetService::init())
	{
		return false;
	}

	const Cmd::Super::ServerEntry *serverEntry = NULL;
	//Zebra::logger->debug(__PRETTY_FUNCTION__);

	//���ӵ���������
	serverEntry = getServerEntryByType(RECORDSERVER);
	if (NULL == serverEntry)
	{
		Zebra::logger->error("�����ҵ����������������Ϣ���������ӵ���������");
		return false;
	}
	recordClient = new RecordClient("����������", serverEntry->pstrExtIP, serverEntry->wdExtPort);
	if (NULL == recordClient)
	{
		Zebra::logger->error("û���㹻�ڴ棬���ܽ��������������ͻ���ʵ��");
		return false;
	}
	if (!recordClient->connectToRecordServer())
	{
		Zebra::logger->error("���ӵ���������ʧ�� %s", __PRETTY_FUNCTION__);
		return false;
	}
	if(recordClient->start())
		Zebra::logger->info("��ʼ��Record������ģ��(%s:%ld)...�ɹ�",serverEntry->pstrExtIP,serverEntry->wdExtPort);

//	if(SessionTimeTick::getInstance().start())
//		Zebra::logger->info("��ʼ��TimeTickģ��...�ɹ�");


	if(!UserSessionManager::getInstance()->init()) return false;

	if(!SceneSessionManager::getInstance()->init()) return false;

	if(!CSeptM::getMe().init()) return false;

	if(!CUnionM::getMe().init()) return false;

	if(!CSchoolM::getMe().init()) return false;

	if(!COfflineMessage::init()) return false;

	if(!CDareM::getMe().init()) return false;
	
	if (!CSubjectM::getMe().init())
	{
		Zebra::logger->error("��ʼ���������ʧ��");
		return false;
	}
	
	if(!CQuizM::getMe().init()) return false;

	if(!CNpcDareM::getMe().init()) return false;

	if (!CCityM::getMe().init())
	{
		Zebra::logger->error("��ʼ��CITY����ʧ��");
		return false;
	}
	
	if (!CCountryM::getMe().init())
	{
		Zebra::logger->error("��ʼ��COUNTRY����ʧ��");
		return false;
	}

	if (!CDareRecordM::getMe().init())
	{
		Zebra::logger->error("��ʼ��DARERECORD����ʧ��");
		return false;
	}

	if (!CSortM::getMe().init())
	{
		Zebra::logger->error("��ʼ����ɫ����ϵͳʧ��");
		return false;
	}
	
	if (!CVoteM::getMe().init())
	{
		Zebra::logger->error("��ʼ��ͶƱϵͳʧ��");
		return false;
	}
	
	if (!CArmyM::getMe().init())
	{
		Zebra::logger->error("��ʼ����������ʧ��");
		return false;
	}
	
	if (!CGemM::getMe().init())
	{
		Zebra::logger->error("��ʼ����������ʧ��");
		return false;
	}
	
	Gift::getMe().init();

	if (!CAllyM::getMe().init())
	{
		Zebra::logger->error("��ʼ��������������ʧ��");
		return false;
	}
	
	if (!RecommendM::getMe().init())
	{
		Zebra::logger->error("��ʼ���Ƽ�������ʧ��");
		return false;
	}


	MailService::getMe().loadNewMail();

	EmperorForbid::getMe();
	
	if(SessionTimeTick::getInstance().start())
		Zebra::logger->info("��ʼ��TimeTickģ��...�ɹ�");

	return true;
}


struct ss
{
	bool operator()(const DWORD s1, const DWORD s2) const
	{
		return s1>s2;
	}
};

/**
 * \brief ������ǿ��
 *
 */
void SessionService::checkCountry(struct tm &tmValue, bool donow)
{
	std::map<DWORD, BYTE, ss> tempMap;
	typedef std::map<DWORD, BYTE, ss>::value_type tempValueType;
	std::map<DWORD, BYTE>::iterator vIterator;

	if (((tmValue.tm_hour==23) && (tmValue.tm_min>50)) || donow)
	{
		uncheckCountryProcess = false;
		if (!userMap.empty())
		{
			for(BYTE j=0; j<13; j++)
			{
				DWORD value=countryLevel[j]*100+j;
				tempMap.insert(tempValueType(value,j));
				Zebra::logger->info("����ǿ����%d=%u",j,countryLevel[j]);
			}
		
			Cmd::Session::t_countryPowerSort_SceneSession send;
			bzero(send.country, sizeof(send.country));
#if _DEBUGLOG
			Zebra::logger->info("sizeof(send.country)=%u",sizeof(send.country));
#endif

			int value=0;
			for(vIterator = tempMap.begin(); vIterator != tempMap.end(); vIterator++)
			{
				if (vIterator->second == 0 ||
					vIterator->second == 1 ||
					vIterator->second == 6) continue;
				if (vIterator->first < 100) continue;
				value++;
			}
			if (value>2) value-=2;

			int count=0;
			for(vIterator = tempMap.begin(); vIterator != tempMap.end(); vIterator++)
			{
				if (vIterator->second == 0 ||
					vIterator->second == 1 ||
					vIterator->second == 6) continue;

				send.country[vIterator->second]=1;
				count++;
				if (count==value) break;
			}
			SessionTaskManager::getInstance().broadcastScene(&send,sizeof(send));
			if (!donow)
			{
				for(int i=0; i<13; i++) countryLevel[i]=0;
				userMap.clear();
			}
		}
	}
	else
	{
		uncheckCountryProcess = true;
	}
}

/**
 * \brief �½���һ����������
 *
 * ʵ�ִ��麯��<code>zNetService::newTCPTask</code>
 *
 * \param sock TCP/IP����
 * \param addr ��ַ
 */
void SessionService::newTCPTask(const int sock, const struct sockaddr_in *addr)
{
	//Zebra::logger->debug(__PRETTY_FUNCTION__);
	SessionTask *tcpTask = new SessionTask(taskPool, sock, addr);
	if (NULL == tcpTask)
		//�ڴ治�㣬ֱ�ӹر�����
		TEMP_FAILURE_RETRY(::close(sock));
	else if(!taskPool->addVerify(tcpTask))
	{
		//�õ���һ����ȷ���ӣ����ӵ���֤������
		SAFE_DELETE(tcpTask);
	}
}



/**
 * \brief �������Է�������������ָ��
 *
 * ��Щָ�������غͷ�����������������ָ��<br>
 * ʵ�����麯��<code>zSubNetService::msgParse_SuperService</code>
 *
 * \param ptNullCmd ��������ָ��
 * \param nCmdLen ��������ָ���
 * \return �����Ƿ�ɹ�
 */
bool SessionService::msgParse_SuperService(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	switch (ptNullCmd->cmd)
	{
		case Cmd::Session::CMD_SCENE:
			{
				using namespace Cmd::Session;
				switch(ptNullCmd->para)
				{                       
					case PARA_SCENE_FORBID_TALK:
						{
							t_forbidTalk_SceneSession * rev = (t_forbidTalk_SceneSession *)ptNullCmd;
							UserSession *pUser = UserSessionManager::getInstance()->getUserSessionByName(rev->name);
							if (pUser)
								pUser->scene->sendCmd(rev, sizeof(t_forbidTalk_SceneSession));
							else
							{
								Zebra::logger->debug("�������ʱû�ҵ����");
								break;
							}
						}
						break;
				}
			}
			break;
		case Cmd::GmTool::CMD_GMTOOL:
			{
				using namespace Cmd::GmTool;
				switch(ptNullCmd->para)
				{
					case PARA_CHAT_GMTOOL:
						{
							t_Chat_GmTool * rev = (t_Chat_GmTool *)ptNullCmd;
							UserSession *pUser = UserSessionManager::getInstance()->getUserSessionByName(rev->userName);
							if (!pUser) return true;

							SessionChannel::sendPrivate(pUser, rev->gmName, rev->content);
						}
						break;
					case PARA_MSG_REPLY_GMTOOL:
						{
							t_Msg_Reply_GmTool * rev = (t_Msg_Reply_GmTool *)ptNullCmd;
							char buf[255];
							snprintf(buf, sizeof(buf), "GM���������������˻ظ�:\n\t%s\nԭ��:\n\t%s", rev->reply, rev->content);
							MailService::getMe().sendTextMail(rev->gmName, 0, rev->userName, rev->userID, buf, (DWORD)-1, Cmd::Session::MAIL_TYPE_SYS);
							UserSession *pUser = UserSessionManager::getInstance()->getUserSessionByName(rev->userName);
							if (pUser)
								pUser->sendSysChat(Cmd::INFO_TYPE_GAME, "GM����������������˻ظ����뼰ʱ�����ʼ�");
							return true;
						}
						break;
					case PARA_BROADCAST_GMTOOL:
						{
							t_Broadcast_GmTool * rev = (t_Broadcast_GmTool *)ptNullCmd;
							Zebra::logger->debug("[GM����]�յ����� %s:%s id=%u time=%u country=%u mapID=%u", rev->GM, rev->content, rev->id, rev->time, rev->country, rev->mapID);
							if (rev->id>5) return false;
							//�ÿ�ȡ������
							if (0==strcmp(rev->content, ""))
							{
								SessionService::wMsg.erase(rev->id);
								return true;
							}
							if (rev->time)
								if (rev->country)
									if (rev->mapID)
										{
											SceneSession *scene = SceneSessionManager::getInstance()
												->getSceneByID((rev->country<<16)+rev->mapID);
											if (scene)
											{
												Cmd::Session::t_broadcastScene_SceneSession send;
												strncpy(send.info, rev->content, MAX_CHATINFO);
												strncpy(send.GM, rev->GM, MAX_NAMESIZE);
												send.mapID = scene->id;
												scene->sendCmd(&send, sizeof(send));
#ifdef _XWL_DEBUG
												Zebra::logger->debug("GM����:%s mapID=%u mapName=%s", send.info, send.mapID, scene->name);
#endif
											}
#ifdef _XWL_DEBUG
											else
												Zebra::logger->debug("GM����:%s mapID=%u û�ҵ���ͼ", rev->content, (rev->country<<16)+rev->mapID);
#endif
										}
									else
										SessionChannel::sendCountryInfo(Cmd::INFO_TYPE_SCROLL, rev->country, rev->content);
								else
									SessionChannel::sendAllInfo(Cmd::INFO_TYPE_SCROLL, rev->content);
							else
							{
								SessionService::wMsg.erase(rev->id);
								return true;
							}

							if (rev->time-1 && rev->id<=5)
							{
								strncpy(SessionService::wMsg[rev->id].msg, rev->content, 256);
								strncpy(SessionService::wMsg[rev->id].GM, rev->GM, MAX_NAMESIZE);
								SessionService::wMsg[rev->id].time = rev->time-1;
								SessionService::wMsg[rev->id].interval = rev->interval;
								SessionService::wMsg[rev->id].count = rev->interval;
								SessionService::wMsg[rev->id].country = rev->country;
								SessionService::wMsg[rev->id].mapID = rev->mapID;
							}
							return true;
						}
						break;
				}
			}
			break;
		case Cmd::Super::CMD_COUNTRYONLINE:
			switch(ptNullCmd->para)
			{
				case Cmd::Super::PARA_REQUEST_COUNTRYONLINE:
					{
						Cmd::Super::t_Request_CountryOnline *ptCmd = (Cmd::Super::t_Request_CountryOnline *)ptNullCmd;
						unsigned char pBuffer[zSocket::MAX_DATASIZE];
						Cmd::Super::t_CountryOnline *cmd = (Cmd::Super::t_CountryOnline *)pBuffer;
						constructInPlace(cmd);

						cmd->rTimestamp = ptCmd->rTimestamp;
						cmd->infoTempID = ptCmd->infoTempID;
						std::vector<std::pair<DWORD,DWORD> >  cti;
						UserSession::getCountryUser(cti);
						std::vector<std::pair<DWORD,DWORD> >::const_iterator it;
						for(it = cti.begin(); it != cti.end(); ++it)
						{
							if (cmd->OnlineNum < 50)
							{
								cmd->CountryOnline[cmd->OnlineNum].country = (*it).first;
								cmd->CountryOnline[cmd->OnlineNum].num = (*it).second;
								cmd->OnlineNum++;
							}
							else
								break;
						}

						return sendCmdToSuperServer(cmd, sizeof(Cmd::Super::t_CountryOnline) + cmd->OnlineNum * sizeof(Cmd::Super::t_CountryOnline::Online));
					}
					break;
			}
			break;
	}

	Zebra::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}

/**
 * \brief �������������
 *
 * ʵ���˴��麯��<code>zService::final</code>
 *
 */
void SessionService::final()
{
	SessionTimeTick::getInstance().final();
	SessionTimeTick::getInstance().join();
	SessionTimeTick::delInstance();
	if(taskPool)
	{
		taskPool->final();
		SAFE_DELETE(taskPool);
	}

        if (recordClient)
        {
                recordClient->final();
                recordClient->join();
                SAFE_DELETE(recordClient);
        }

	CCityM::destroyMe();
	CCountryM::destroyMe();

	CNpcDareM::destroyMe();

	CDareM::destroyMe();

	CQuizM::destroyMe();

	CSeptM::destroyMe();

	CSchoolM::destroyMe();

	CUnionM::destroyMe();

	CSortM::destroyMe();

	CVoteM::destroyMe();

	AuctionService::delMe();

	CartoonPetService::delMe();

	EmperorForbid::getMe().delMe();
	
	zSubNetService::final();

	UserSessionManager::delInstance();
	SceneSessionManager::delInstance();

	SessionTaskManager::delInstance();

	Zebra::logger->debug(__PRETTY_FUNCTION__);
}

/*
bool SessionService::checkGumu()
{
	struct tm tm_1;
	time_t timValue = time(NULL);
	tm_1=*localtime(&timValue);
	if((tm_1.tm_hour%2) && (tm_1.tm_min >= 55))
	{
		if(!gumutime)
		{
			Zebra::logger->debug("��Ĺ��ͼ����%d���Ӻ󿪷�",60 - tm_1.tm_min);
			SessionChannel::sendAllInfo(Cmd::INFO_TYPE_SCROLL, "��Ĺ��ͼ����%d���Ӻ󿪷�",60 - tm_1.tm_min);
		}
		gumutime = 1;
	}
	else if(((tm_1.tm_hour%2) == 0) && tm_1.tm_min <= 30)
	{
		gumutime = 2;
	}
	else
	{
		gumutime = 0;
	}
	return true;
}
// */
bool SessionService::checkShutdown()
{
	if(shutdown_time.time== 0)
	{
		return false;
	}
	time_t timValue = time(NULL);
	if(shutdown_time.time >= (timValue+60) && shutdown_time.time <= timValue +300)
	{
	    //�ػ�ǰֹͣϵͳ����
	    Cmd::Session::t_SetService_SceneSession send;
	    send.flag &= ~Cmd::Session::SERVICE_MAIL;
	    send.flag &= ~Cmd::Session::SERVICE_AUCTION;
	    SessionTaskManager::getInstance().broadcastScene(&send, sizeof(send));
	    Zebra::logger->trace("ͣ��ǰ5����ֹͣ�ʼ�����������");

	    SessionChannel::sendAllInfo(Cmd::INFO_TYPE_SCROLL, "ϵͳ����%d���Ӻ�ͣ��ά��!",(shutdown_time.time- timValue)/60);
	    SessionChannel::sendAllInfo(Cmd::INFO_TYPE_SYS, "�ʼ�ϵͳ������ϵͳĿǰ�Ѿ�ֹͣ����ά��֮��ָ�ʹ��");
	    return true;
	}

	if(timValue >= shutdown_time.time)
	{
		shutdown_time.time=0; 
		Cmd::Super::t_shutdown_Super shut;
		SessionService::getInstance().sendCmdToSuperServer(&shut,sizeof(shut));
		struct tm tm_1;
		time_t timval=time(NULL);
		//tm_1=*localtime(&timval);
		zRTime::getLocalTime(tm_1, timval);
		Zebra::logger->debug("ͣ��ά��ʱ��%d��%d��%d��%dʱ%d��%d��",tm_1.tm_year+1900,tm_1.tm_mon+1,tm_1.tm_mday,tm_1.tm_hour,tm_1.tm_min,tm_1.tm_sec);
		return true;
	}
	return false;
}
/**
 * \brief �����в���
 *
 */
static struct argp_option session_options[] =
{
	{"daemon",		'd',	0,			0,	"Run service as daemon",						0},
	{"log",			'l',	"level",	0,	"Log level",									0},
	{"logfilename",	'f',	"filename",	0,	"Log file name",								0},
	{"mysql",		'y',	"mysql",	0,	"MySQL[mysql://user:passwd@host:port/dbName]",	0},
	{"ifname",		'i',	"ifname",	0,	"Local network device",							0},
	{"server",		's',	"ip",		0,	"Super server ip address",						0},
	{"port",		'p',	"port",		0,	"Super server port number",						0},
	{0,				0,		0,			0,	0,												0}
};

/**
 * \brief �����в���������
 *
 * \param key ������д
 * \param arg ����ֵ
 * \param state ����״̬
 * \return ���ش������
 */
static error_t session_parse_opt(int key, char *arg, struct argp_state *state)
{
	switch (key)
	{
		case 'd':
			{
				Zebra::global["daemon"] = "true";
			}
			break;
		case 'p':
			{
				Zebra::global["port"]=arg;
			}
			break;
		case 's':
			{
				Zebra::global["server"]=arg;
			}
			break;
		case 'l':
			{
				Zebra::global["log"]=arg;
			}
			break;
		case 'f':
			{
				Zebra::global["logfilename"]=arg;
			}
			break;
		case 'y':
			{
				Zebra::global["mysql"]=arg;
			}
			break;
		case 'i':
			{
				Zebra::global["ifname"]=arg;
			}
			break;
		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

/**
 * \brief ���������Ϣ
 *
 */
static char session_doc[] = "\nSessionServer\n" "\tSession��������";

/**
 * \brief ����İ汾��Ϣ
 *
 */
const char *argp_program_version = "Program version :\t" VERSION_STRING\
									"\nBuild version   :\t" _S(BUILD_STRING)\
									"\nBuild time      :\t" __DATE__ ", " __TIME__;

/**
 * \brief ��ȡ�����ļ�
 *
 */
class SessionConfile:public zConfile
{
	bool parseYour(const xmlNodePtr node)
	{
		if (node)
		{
			xmlNodePtr child=parser.getChildNode(node,NULL);
			while(child)
			{
				parseNormal(child);
				child=parser.getNextNode(child,NULL);
			}
			return true;
		}
		else
			return false;
	}
};

/**
 * \brief ���¶�ȡ�����ļ���ΪHUP�źŵĴ�������
 *
 */
void SessionService::reloadConfig()
{
	Zebra::logger->debug("%s", __PRETTY_FUNCTION__);
	if (!CSubjectM::getMe().init())
	{
		Zebra::logger->error("��ʼ���������ʧ��");
	}

	SessionConfile sc;
	sc.parse("SessionServer");
	//ָ���⿪��
	if(Zebra::global["cmdswitch"] == "true")
	{
		zTCPTask::analysis._switch = true;
		zTCPClient::analysis._switch=true;
	}
	else
	{
		zTCPTask::analysis._switch = false;
		zTCPClient::analysis._switch=false;
	}
}

#define getMessage(msg,msglen,pat)	\
	do	\
{	\
	va_list ap;	\
	bzero(msg, msglen);	\
	va_start(ap, pat);		\
	vsnprintf(msg, msglen - 1, pat, ap);	\
	va_end(ap);	\
}while(false)

/**
 * \brief ���;�����Ϣ��GM����
 *
 */
bool SessionService::reportGm(const char * fromName, const char * msg, ...)
{
	if (!msg) return false;

	//getMessage(buf, MAX_CHATINFO, msg);

	BYTE buf[zSocket::MAX_DATASIZE];
	Cmd::GmTool::t_Chat_GmTool *cmd=(Cmd::GmTool::t_Chat_GmTool *)buf;
	bzero(buf, sizeof(buf));
	constructInPlace(cmd);

	strncpy(cmd->userName, fromName, MAX_NAMESIZE);
	cmd->dwType = Cmd::CHAT_TYPE_ERROR_GM;
	getMessage(cmd->content, MAX_CHATINFO, msg);
	cmd->size = 0;
	SessionService::getInstance().sendCmdToSuperServer(cmd, sizeof(Cmd::GmTool::t_Chat_GmTool));

	return true;
}

/**
 * \brief ���������
 *
 * \param argc ��������
 * \param argv �����б�
 * \return ���н��
 */
int main(int argc, char **argv)
{
	Zebra::logger=new zLogger("SessionServer");

	if (!Zebra::logger)
	{
		return EXIT_FAILURE;
	}

	//����ȱʡ����
	Zebra::global["logfilename"] = "/tmp/sessionserver.log";

	//���������ļ�����
	SessionConfile sc;
	if (!sc.parse("SessionServer"))
		return EXIT_FAILURE;

	//ָ���⿪��
	if(Zebra::global["cmdswitch"] == "true")
	{
		zTCPTask::analysis._switch = true;
		zTCPClient::analysis._switch=true;
	}
	else
	{
		zTCPTask::analysis._switch = false;
		zTCPClient::analysis._switch=false;
	}
	//���������в���
	zArg::getArg()->add(session_options, session_parse_opt, 0, session_doc);
	zArg::getArg()->parse(argc, argv);
	//Zebra::global.dump(std::cout);

	//������־����
	Zebra::logger->setLevel(Zebra::global["log"]);
	//����д������־�ļ�
	if ("" != Zebra::global["logfilename"])
		Zebra::logger->addLocalFileLog(Zebra::global["logfilename"]);

	//�Ƿ��Ժ�̨���̵ķ�ʽ����
	if ("true" == Zebra::global["daemon"]) {
		Zebra::logger->info("Program will be run as a daemon");
		Zebra::logger->removeConsoleLog();
		daemon(1, 1);
	}

	SessionService::getInstance().main();
	SessionService::delInstance();

	return EXIT_SUCCESS;
}
