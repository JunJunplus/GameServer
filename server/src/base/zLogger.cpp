/**
 * \file
 * \version  $Id: zLogger.cpp  $
 * \author  
 * \date 
 * \brief Zebra��Ŀ��־ϵͳ�����ļ�
 *
 */

#include "zLogger.h"
#include "zTime.h"
#include <stdarg.h>
#include <log4cxx/patternlayout.h>
#include <log4cxx/consoleappender.h>
#include <log4cxx/helpers/dateformat.h>
#include <log4cxx/net/syslogappender.h>
#include <sys/stat.h>
#include <time.h>

using namespace log4cxx::net;

const LevelPtr zLogger::zLevel::LEVELALARM(new Level(ALARM_INT,_T("ALARM"),3));
const LevelPtr zLogger::zLevel::LEVELIFFY(new Level(IFFY_INT,_T("IFFY"),3));
const LevelPtr zLogger::zLevel::LEVELTRACE(new Level(TRACE_INT,_T("TRACE"),3));
const LevelPtr zLogger::zLevel::LEVELGBUG(new Level(GBUG_INT,_T("GBUG"),3));

const zLogger::zLevel *  zLogger::zLevel::OFF=new zLevel(Level::OFF);
const zLogger::zLevel *  zLogger::zLevel::FATAL=new zLevel(Level::FATAL);
const zLogger::zLevel *  zLogger::zLevel::ALARM=new zLevel(LEVELALARM);
const zLogger::zLevel *  zLogger::zLevel::ERROR=new zLevel(Level::ERROR);
const zLogger::zLevel *  zLogger::zLevel::IFFY=new zLevel(LEVELIFFY);
const zLogger::zLevel *  zLogger::zLevel::WARN=new zLevel(Level::WARN);
const zLogger::zLevel *  zLogger::zLevel::TRACE=new zLevel(LEVELTRACE);
const zLogger::zLevel *  zLogger::zLevel::INFO=new zLevel(Level::INFO);
const zLogger::zLevel *  zLogger::zLevel::GBUG=new zLevel(LEVELGBUG);
const zLogger::zLevel *  zLogger::zLevel::DEBUG=new zLevel(Level::DEBUG);
const zLogger::zLevel *  zLogger::zLevel::ALL=new zLevel(Level::ALL);

/**
 * \brief zLevel���캯��
 * \param  level �ȼ����֣����ڲ�����
 */
zLogger::zLevel::zLevel(LevelPtr level):zlevel(level)
{
}


TimeZonePtr zLogger::zLoggerLocalFileAppender::tz(TimeZone::getDefault());

/**
 * \brief ����һ�������ļ�Appender 
 */
zLogger::zLoggerLocalFileAppender::zLoggerLocalFileAppender()
{
}

/**
 * \brief ����ʱ������DateFormat�ڴ�
 */
zLogger::zLoggerLocalFileAppender::~zLoggerLocalFileAppender()
{
	SAFE_DELETE(df);
}

/**
 * \brief ����ʱ��
 *
 * \param timeZone ʱ���ַ��� 
 */
void zLogger::zLoggerLocalFileAppender::setTimeZone(const std::string &timeZone)
{
	tz=TimeZone::getTimeZone(timeZone);
}

/**
 * \brief ���������õ�ѡ��
 */
void zLogger::zLoggerLocalFileAppender::activateOptions()
{
	//����ʱ����Ԥ���ļ�
	rc.setTimeZone(tz);
	std::string ss;
	zRTime::getLocalTZ(ss);
	char tzstr[64];
	strncpy(tzstr,"TZ=",3);
	strncpy(tzstr+3,ss.c_str(),60);
	DailyRollingFileAppender::activateOptions();
	//����DailyRollingFileAppender::activateOptions()���޸�ʱ��������������ִ��������޸Ļ��� 
	putenv(tzstr);
	tzset();
	if (!datePattern.empty() && !fileName.empty())
	{
		SAFE_DELETE(df);
		df=new helpers::DateFormat(datePattern,tz);

		int64_t lastModified = 0;
		struct stat fileStats;
		if (::stat(T2A(fileName.c_str()), &fileStats) == 0)
			lastModified = (int64_t)fileStats.st_mtime * 1000; 

		scheduledFilename = fileName + df->format(lastModified);
	} 
}

/**
 * \brief ����һ��zLogger 
 *
 * \param  name zLogger�����֣�����������������־�е�ÿһ��
 */
zLogger::zLogger(const std::string &name)
{
	///std::cout << __PRETTY_FUNCTION__ << std::endl;
	bzero(message, sizeof(message));
	logger = Logger::getLogger(name);
	logger->setLevel(Level::DEBUG);

	addConsoleLog();
}

/**
 * \brief ��������
 */
zLogger::~zLogger()
{
}

/**
 * \brief �õ�Logger�����֣���������ÿ����־��Ϣ��
 * \return	Logger����
 */
const std::string & zLogger::getName()
{
	return logger->getName();
}

/**
 * \brief ����Logger�����֣���������ÿ����־��Ϣ��
 * \param Ҫ�����õ�����
 */
void zLogger::setName(const std::string & setName)
{
	//PowerLogger *log=(PowerLogger *)logger;
	//PowerLogger * pl=logger.p;
	((PowerLogger *)logger.p)->setName(setName);
}

/**
 * \brief ��ӿ���̨���Log
 * \return	�ɹ�����true�����򷵻�false 
 */
bool zLogger::addConsoleLog()
{
	PatternLayoutPtr cpl = new PatternLayout("\%d{\%y\%m\%d-\%H:\%M:\%S }%c %5p: %m%n");
	std::string s;
	cpl->setTimeZone(zRTime::getLocalTZ(s));
	cpl->activateOptions();

	ConsoleAppenderPtr carp = new ConsoleAppender(cpl);
	carp->setName("console");

	logger->addAppender(carp);
	return true;
}

/**
 * \brief �Ƴ�����̨Log���
 */
void zLogger::removeConsoleLog()
{
	AppenderPtr ap=logger->getAppender("console");
	logger->removeAppender(ap);
	ap->close();
}

/**
 * \brief ��һ�������ļ�Log���
 *
 * \param file Ҫ������ļ�����Logger���Զ������ʱ���׺ 
 * \return �ɹ�����true�����򷵻�false
 */
bool zLogger::addLocalFileLog(const std::string &file)
{
	PatternLayoutPtr fpl = new PatternLayout("\%d{\%y\%m\%d-\%H:\%M:\%S }%c %5p: %m%n");
	std::string s;
	fpl->setTimeZone(zRTime::getLocalTZ(s));
	fpl->activateOptions();

	zLoggerLocalFileAppender * farp = new  zLoggerLocalFileAppender();
	farp->setDatePattern(".\%y\%m\%d-\%H");
	farp->setTimeZone(s);
	farp->setFile(file);
	farp->setLayout(fpl);
	farp->activateOptions();
	farp->setName("localfile:"+file);

	logger->addAppender(farp);
	return true;
}

/**
 * \brief �Ƴ�һ�������ļ�Log���
 * \param file Ҫ�Ƴ���Log�ļ��� 
 */
void zLogger::removeLocalFileLog(const std::string &file)
{
	AppenderPtr ap=logger->getAppender("localfile:"+file);
	logger->removeAppender(ap);
	ap->close();
}

/**
 * \brief ���һ��Syslog���
 * \param host Syslog��������ַ,Ĭ��Ϊ������
 * \return �ɹ�����true�����򷵻�false
 */
bool zLogger::addSysLog(const std::string & host="127.0.0.1")
{
	PatternLayoutPtr pls = new PatternLayout("%c %5p: %m%n");
	SyslogAppenderPtr sarp=new SyslogAppender(pls,SyslogAppender::getFacility("user"));
	sarp->setName(host);
	sarp->setSyslogHost (host);
	sarp->activateOptions();
	logger->addAppender(sarp);
	return true;
}

/**
 * \brief �Ƴ�һ��Syslog���
 * \param host Ҫ�Ƴ���Syslog��������ַ,Ĭ��Ϊ������
 */
void zLogger::removeSysLog(const std::string & host="127.0.0.1")
{
	AppenderPtr ap=logger->getAppender(host);
	logger->removeAppender(ap);
	ap->close();
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
 * \brief ����д��־�ȼ�
 * \param  zLevelPtr ��־�ȼ�.�μ� #zLogger::zLevel
 */
void zLogger::setLevel(const zLevel * zLevelPtr)
{
	logger->setLevel(zLevelPtr->zlevel);
}

/**
 * \brief ����д��־�ȼ�
 * \param  level ��־�ȼ�
 */
void zLogger::setLevel(const std::string &level)
{
	if ("off" == level)
		setLevel(zLevel::OFF);
	else if ("fatal" == level)
		setLevel(zLevel::FATAL);
	else if ("alarm" == level)
		setLevel(zLevel::ALARM);
	else if ("error" == level)
		setLevel(zLevel::ERROR);
	else if ("iffy" == level)
		setLevel(zLevel::IFFY);
	else if ("warn" == level)
		setLevel(zLevel::WARN);
	else if ("trace" == level)
		setLevel(zLevel::TRACE);
	else if ("info" == level)
		setLevel(zLevel::INFO);
	else if ("gbug" == level)
		setLevel(zLevel::GBUG);
	else if ("debug" == level)
		setLevel(zLevel::DEBUG);
	else if ("all" == level)
		setLevel(zLevel::ALL);
}

/**
 * \brief д��־
 * \param  zLevelPtr ��־�ȼ��μ� #zLogger::zLevel
 * \param  pattern �����ʽ��������printfһ��
 * \return �ɹ�����true�����򷵻�false
 */
bool zLogger::log(const zLevel * zLevelPtr,const char * pattern, ...)
{
	msgMut.lock();
	getMessage(message,MSGBUF_MAX,pattern);

	logger->log(zLevelPtr->zlevel,message);
	msgMut.unlock();
	return true;
}

/**
 * \brief ǿ��д��־,������־�ȼ�����
 * \param  zLevelPtr ��־�ȼ��μ� #zLogger::zLevel
 * \param  pattern �����ʽ��������printfһ��
 * \return �ɹ�����true�����򷵻�false
 */
bool zLogger::forceLog(const zLevel * zLevelPtr,const char * pattern, ...)
{
	msgMut.lock();
	getMessage(message,MSGBUF_MAX,pattern);

	logger->forcedLog(zLevelPtr->zlevel,message);
	msgMut.unlock();
	return true;
}

/**
 * \brief дfatal������־
 * \param  pattern �����ʽ��������printfһ��
 * \return �ɹ�����true�����򷵻�false
 */
bool zLogger::fatal(const char * pattern, ...)
{
	msgMut.lock();
	getMessage(message,MSGBUF_MAX,pattern);

	logger->fatal(message);
	msgMut.unlock();
	return true;
}

/**
 * \brief дerror������־
 * \param  pattern �����ʽ��������printfһ��
 * \return �ɹ�����true�����򷵻�false
 */
bool zLogger::error(const char * pattern, ...)
{
	msgMut.lock();
	getMessage(message,MSGBUF_MAX,pattern);

	logger->error(message);
	msgMut.unlock();
	return true;
}

/**
 * \brief дerror������־
 * \param  buf �������
 * \return �ɹ�����true�����򷵻�false
 */
bool zLogger::error_out(const char * buf)
{
	msgMut.lock();
	logger->error(buf);
	msgMut.unlock();
	return true;
}

/**
 * \brief дwarn������־
 * \param  pattern �����ʽ��������printfһ��
 * \return �ɹ�����true�����򷵻�false
 */
bool zLogger::warn(const char * pattern, ...)
{
	msgMut.lock();
	getMessage(message,MSGBUF_MAX,pattern);

	logger->warn(message);
	msgMut.unlock();
	return true;
}

/**
 * \brief дinfo������־
 * \param  pattern �����ʽ��������printfһ��
 * \return �ɹ�����true�����򷵻�false
 */
bool zLogger::info(const char * pattern, ...)
{
	msgMut.lock();
	getMessage(message,MSGBUF_MAX,pattern);

	logger->info(message);
	msgMut.unlock();
	return true;
}

/**
 * \brief дdebug������־
 * \param  pattern �����ʽ��������printfһ��
 * \return �ɹ�����true�����򷵻�false
 */
bool zLogger::debug(const char * pattern, ...)
{
	msgMut.lock();
	getMessage(message,MSGBUF_MAX,pattern);

	logger->debug(message);
	msgMut.unlock();
	return true;
}

/**
 * \brief дalarm��Ϸ��־
 * \param  pattern �����ʽ��������printfһ��
 * \return �ɹ�����true�����򷵻�false
 */
bool zLogger::alarm(const char * pattern, ...)
{
	msgMut.lock();
	getMessage(message,MSGBUF_MAX,pattern);

	logger->log(zLevel::ALARM->zlevel,message);
	msgMut.unlock();
	return true;
}

/**
 * \brief дiffy��Ϸ��־
 * \param  pattern �����ʽ��������printfһ��
 * \return �ɹ�����true�����򷵻�false
 */
bool zLogger::iffy(const char * pattern, ...)
{
	msgMut.lock();
	getMessage(message,MSGBUF_MAX,pattern);

	logger->log(zLevel::IFFY->zlevel,message);
	msgMut.unlock();
	return true;
}

/**
 * \brief дtrace��Ϸ��־
 * \param  pattern �����ʽ��������printfһ��
 * \return �ɹ�����true�����򷵻�false
 */
bool zLogger::trace(const char * pattern, ...)
{
//soke
/*	msgMut.lock();
	getMessage(message,MSGBUF_MAX,pattern);

	logger->log(zLevel::TRACE->zlevel,message);
	msgMut.unlock();
*/
	return true;
}

/**
 * \brief дgbug��Ϸ��־
 * \param  pattern �����ʽ��������printfһ��
 * \return �ɹ�����true�����򷵻�false
 */
bool zLogger::gbug(const char * pattern, ...)
{
	msgMut.lock();
	getMessage(message,MSGBUF_MAX,pattern);

	logger->log(zLevel::GBUG->zlevel,message);
	msgMut.unlock();
	return true;
}
