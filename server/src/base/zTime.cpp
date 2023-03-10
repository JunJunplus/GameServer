#include "zTime.h"
#include "Zebra.h"

/**
 * \brief 得到系统时区设置字符串
 *
 * \param s 时区将放入此字符串中
 * \return 返回参数s
 */
std::string & zRTime::getLocalTZ(std::string & s)
{
	std::ostringstream so;
	tzset();
	so << tzname[0] << timezone/3600;
	s= so.str();
	return s;
}

void FunctionInterval::interval(const char *func)
{
	gettimeofday(&_tv_2,NULL);
	int end=_tv_2.tv_sec*1000000 + _tv_2.tv_usec;
	int begin= _tv_1.tv_sec*1000000 + _tv_1.tv_usec;
	if(end - begin > _need_log)
	{
		Zebra::logger->debug("%s执行时间间隔%dus",func,end - begin);
	}
	_tv_1=_tv_2;
}
FunctionTime::~FunctionTime()
{
	gettimeofday(&_tv_2,NULL);
	int end=_tv_2.tv_sec*1000000 + _tv_2.tv_usec;
	int begin= _tv_1.tv_sec*1000000 + _tv_1.tv_usec;
	if(end - begin > _need_log)
	{
		char buf[_dis_len+1];
		bzero(buf,sizeof(buf));
		strncpy(buf,_dis,_dis_len);
		Zebra::logger->debug("%s执行时间%dus,描述:%s",_fun_name,end - begin , buf);
	}
}
FunctionTimes::Times FunctionTimes::_times[256]; 
FunctionTimes::FunctionTimes(const int which , const char *dis)
{
	_which = which;
	_times[_which]._mutex.lock(); 
	if(!_times[_which]._times)
	{
		strncpy(_times[_which]._dis,dis,sizeof(_times[_which]._dis));
	}
	gettimeofday(&_tv_1,NULL);
}
FunctionTimes::~FunctionTimes()
{
	gettimeofday(&_tv_2,NULL);
	int end=_tv_2.tv_sec*1000000 + _tv_2.tv_usec;
	int begin= _tv_1.tv_sec*1000000 + _tv_1.tv_usec;
	_times[_which]._times++;
	_times[_which]._total_time += (end - begin);
	zRTime ct;
	if(_times[_which]._log_timer(ct))
	{
		Zebra::logger->debug("执行次数(%d):%d,执行总时间:%dus,说明:%s",_which,_times[_which]._times,_times[_which]._total_time ,_times[_which]._dis);
		_times[_which]._times=0;
		_times[_which]._total_time=0;
	}
	_times[_which]._mutex.unlock(); 
}
