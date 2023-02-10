/**
 * \file
 * \version	$Id: script.cpp  $
 * \author	
 * \date	
 * \brief	脚本，新的任务引擎
 * 
 */
#include "script.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
#include <luabind/luabind.hpp>

//------------------------------------------------------
LuaScript::LuaScript() 
{
}

//------------------------------------------------------
LuaScript::~LuaScript()
{
}

//------------------------------------------------------
/**     
 * \brief 判断脚本是否被载入
 *
  * \return 已经载入返回true，否则返回false
 */   
bool LuaScript::isLoaded() 
{
	return mFileName.size() != 0;
}
	
//------------------------------------------------------
/**     
 * \brief 设置脚本文件
 *
 */   
void LuaScript::setData( const std::string& rData )
{
	mFileName = rData;
}

/**     
 * \brief 取得脚本文件名
 *
  * \return 脚本文件名称
 */   
//------------------------------------------------------
const std::string& LuaScript::getData() const 
{
	return mFileName;
}

#define IS_MASK_SET(val, mask) ((mask & val) == mask)

//------------------------------------------------------
/**     
 * \brief 构造函数，创建一个虚拟机上下文用于执行脚本
 * \param libs: 用于指定在虚拟机可用的lua库
 */   
LuaVM::LuaVM( DWORD libs ) 
{
	mLuaState = lua_open();
     #if LUA_VERSION_NUM >=501
	if (IS_MASK_SET(libs, LUALIB_BASE))
		lua_cpcall(mLuaState,luaopen_base,0);		
	if (IS_MASK_SET(libs, LUALIB_TABLE))
		lua_cpcall(mLuaState,luaopen_table,0);			
	if (IS_MASK_SET(libs, LUALIB_IO))
		lua_cpcall(mLuaState,luaopen_io,0);			
	if (IS_MASK_SET(libs, LUALIB_STRING))
		lua_cpcall(mLuaState,luaopen_string,0);		
	if (IS_MASK_SET(libs, LUALIB_MATH))
		lua_cpcall(mLuaState,luaopen_math,0);			
	if (IS_MASK_SET(libs, LUALIB_DEBUG))
		lua_cpcall(mLuaState,luaopen_debug,0);		
     #else
	if (IS_MASK_SET(libs, LUALIB_BASE))
		luaopen_base( mLuaState );
	if (IS_MASK_SET(libs, LUALIB_TABLE))
		luaopen_table( mLuaState );
	if (IS_MASK_SET(libs, LUALIB_IO))
		luaopen_io( mLuaState );
	if (IS_MASK_SET(libs, LUALIB_STRING))
		luaopen_string( mLuaState );
	if (IS_MASK_SET(libs, LUALIB_MATH))
		luaopen_math( mLuaState );
	if (IS_MASK_SET(libs, LUALIB_DEBUG))
		luaopen_debug( mLuaState );
     #endif
	luabind::open( mLuaState );
}

//------------------------------------------------------
/**     
 * \brief 析构函数，关闭虚拟机
 *
 */   
LuaVM::~LuaVM()
{
	if (mLuaState)
	{
		lua_close( mLuaState );
		mLuaState = 0;
	}
}

//------------------------------------------------------
/**     
 * \brief 执行一段脚本
 *
 * \param rData: 被执行的脚本内容
 */   
void LuaVM::execute( const std::string & rData )
{
	if (mLuaState)
	#if LUA_VERSION_NUM >=501
		luaL_dostring( mLuaState, rData.c_str() );
	#else
		lua_dostring( mLuaState, rData.c_str() );
	#endif
}

//------------------------------------------------------
//------------------------------------------------------
/**     
 * \brief 执行一段脚本
 *
 * \param pScript: 被执行的脚本
 */   
void LuaVM::execute( LuaScript* pScript )
{

	const std::string& rFileName = pScript->getData();
	#if LUA_VERSION_NUM >=501
	luaL_dofile( mLuaState, rFileName.c_str() );
	#else
	lua_dofile( mLuaState, rFileName.c_str() );
	#endif 
}

ScriptingSystemLua* ScriptingSystemLua::_instance = NULL;

//------------------------------------------------------
/**     
 * \brief 单键模式，用于取得脚本系统接口
 *
 * \return 脚本系统接口
 */  
ScriptingSystemLua& ScriptingSystemLua::instance()
{
	if (!_instance) {
		_instance = new ScriptingSystemLua();
	}
	
	return *_instance;
}

//------------------------------------------------------
ScriptingSystemLua::ScriptingSystemLua()
{
}

//------------------------------------------------------
/**     
 * \brief 析构函数，清除已经创建的虚拟机
 *
 */  
ScriptingSystemLua::~ScriptingSystemLua()
{
	mVMs.clear();
}

//------------------------------------------------------
/**     
 * \brief 创建一个虚拟机
 *
 * \return 被创建的虚拟机
 */  
LuaVM* ScriptingSystemLua::createVM()
{
	LuaVM* pVM = new LuaVM( );
	mVMs.push_back( pVM );
	return pVM;
}

//------------------------------------------------------
/**     
 * \brief 取得一个已经被创建的虚拟机
 *
 * \param index: 要得到的虚拟机索引
 * \return 得到的虚拟机
 */  
LuaVM* ScriptingSystemLua::getVM(int index)
{
	return mVMs[index];
}

//------------------------------------------------------
//------------------------------------------------------
/**     
 * \brief 从给定的文件中创建一个脚本
 *
 * \param rFile: 文件名
 * \return 被创建的脚本
 */  
LuaScript* ScriptingSystemLua::createScriptFromFile( const std::string & rFile )
{
	LuaScript* pLuaScript = new LuaScript( );

	pLuaScript->setData( rFile );

	return pLuaScript;
}

ScriptQuest* ScriptQuest::_instance = NULL;

/**     
 * \brief 单键模式，用于取得系统中唯一的全局索引
 *
 * \return 脚本全局索引
 */  
ScriptQuest& ScriptQuest::get_instance()
{
	if (!_instance) {
		_instance = new ScriptQuest();
	}
	
	return *_instance;
}

/**     
 * \brief 在全局索引中添加一个项
 *
 * \param type : 事件类型
 * \param id : 事件id
 */  
void ScriptQuest::add(int type, int id)
{
	if (!has(type, id)) {
		int h = hash(type, id);
		_sq.insert(h);
	}
}

/**     
 * \brief 判断某事件是否在全局索引中存在
 *
 * \param type : 事件类型
 * \param id : 事件id
 * \return: 存在返回true，否则返回false
 */  
bool ScriptQuest::has(int type, int id) const
{
	return _sq.find(hash(type, id)) != _sq.end();
}

/**     
 * \brief hash函数，根据事件类型和id算出一个唯一值
 *
 * \param type : 事件类型
 * \param id : 事件id
 * \return : hash后得到的唯一值
 */  
int ScriptQuest::hash(int type, int id) const
{
	return ( (type & 0xff) << 24 ) | (id & 0x00ffffff);
}

#include <algorithm>

/**     
 * \brief 排序函数，加速以后的查找
 *
 */  
void ScriptQuest::sort()
{
	//std::stable_sort(_sq.begin(), _sq.end());	
}

/**     
 * \brief 取得全局索引
 *
 * \return : 全局索引
 */  
ScriptQuest& the_script()
{
	return ScriptQuest::get_instance();
}


#include "script_func.h"
#include "Chat.h"
#include "SceneUser.h"
#include "Quest.h"
#include "Scene.h"

extern SceneUser* current_user;

/**     
 * \brief 绑定一个虚拟机，使特定的接口对该虚拟机可用
 *
 * \param vm : 要绑定的虚拟机
 */  
void Binder::bind(LuaVM* vm)
{

#define _MODULE vm->getLuaState()

using namespace luabind;
module (_MODULE)
[
	class_<Channel>( "Channel" )
//		.def( "sys", (bool(Channel::*) (SceneUser*)) &Channel::add )
//		.def( "sys", (bool(Channel::*) (SceneUser*, int,  const char*, ...)) &Channel::sendSys )
];

/*
	INFO_TYPE_SYS	=	1,	/// 系统信息、GM信息
	INFO_TYPE_GAME,		/// 游戏信息
	INFO_TYPE_STATE,		/// 状态转换
	INFO_TYPE_FAIL,			/// 失败信息
	INFO_TYPE_EXP,		/// 特殊信息,获得经验、物品，在人物头上
	INFO_TYPE_MSG,		/// 弹出用户确认框的系统消息
	INFO_TYPE_KING,		/// 国王发出的聊天消息
	INFO_TYPE_CASTELLAN,	/// 城主发出的聊天消息
	INFO_TYPE_SCROLL	/// 屏幕上方滚动的系统信息
*/

module (_MODULE)
[
	def("me", &me),
	def("sys", &sys),
	def("show_dialog", &show_dialog),
	def("refresh_npc", &refresh_npc),
	def("the_script", &the_script),

	//--------------------------------------------------------
	def("time", &get_time),
	def("difftime", &diff_time)


];

int s = Cmd::USTATE_START_QUEST;
int d = Cmd::USTATE_DOING_QUEST;
int f = Cmd::USTATE_FINISH_QUEST;

module (_MODULE)
[
	class_<std::string>("String")
		,
		
	class_<zObject>( "Object" )
		,

	class_<Scene>("Scene")
		.def("country", &Scene::getCountryID)
		,
	
	class_<CharBase>( "CharBase" )
		.def_readonly("level", &CharBase::level)
		.def_readwrite("fivelevel", &CharBase::fivelevel)
		.def_readwrite("fivetype", &CharBase::fivetype)
		.def_readwrite("honor", &CharBase::honor)
		.def_readwrite("maxhonor", &CharBase::maxhonor)
		.def_readonly("name", &CharBase::name)		
		.def_readonly("map", &CharBase::mapName)
		,

	class_<zPos>("Pos")
		.def(constructor<DWORD, DWORD>())
		,
		
	class_<SceneUser>( "SceneUser" )
		.def("levelup", &SceneUser::upgrade )
		.def("add_exp", &add_exp)
		.def("goto", &SceneUser::goTo)
		.def("gomap", &Gm::gomap)

		.def("add_ob", &SceneUser::addObjectNum)
		.def("remove_ob", &SceneUser::reduceObjectNum)
		.def("have_ob", &have_ob)
		.def("get_ob", &get_ob)
		.def("del_ob", &del_ob)
		.def("space", &space)

		.def("check_money", &check_money)
		.def("add_money", &add_money)
		.def("remove_money", &remove_money)

		.def_readwrite("charbase", &SceneUser::charbase)
		.def_readwrite("quest", &SceneUser::quest_list)
		.def_readonly("scene", (Scene*SceneUser::*)&SceneUser::scene)
		
		//-----------------------------------------------
		.def("get_familyvar", &get_familyvar)
		.def("get_tongvar", &get_tongvar)
		.def("get_uservar", &get_uservar)
		.def_readonly("union_name", &SceneUser::unionName)
		.def_readonly("sept_name", &SceneUser::septName)
		.def_readonly("caption", &SceneUser::caption)
		.def_readonly("is_king", &SceneUser::king)
		.def_readonly("is_union_master", &SceneUser::unionMaster)
		.def_readonly("is_sept_master", &SceneUser::septMaster)
		.def_readonly("sept_repute", &SceneUser::dwSeptRepute)
		.def_readonly("sept_level", &SceneUser::dwSeptLevel)
		.def_readonly("action_point", &SceneUser::dwUnionActionPoint)
		,

	class_<zNpcB>("NpcBase")
		.def_readonly("level", &zNpcB::level)
		,

	class_<SceneNpc>( "SceneNpc" )
		.def_readonly("base", &SceneNpc::npc)
		.def("refresh", &refresh_status)
		.def("tempid", &npc_tempid)
		.def("id", &npc_id)
		,

	class_<Quest>( "Quest" )
		.enum_("constants")
		[
			value("START", s),
			value("DOING", d),
			value("FINISH", f)
		]
		,

	class_<Vars>("Vars")
		.def(constructor<DWORD>())
		.def("set", &set_var)
		.def("get", &get_var)
		.def("sets", &set_varS)
		.def("gets", &get_varS)
		.def("refresh",(int(Vars::*) (SceneUser&, const std::string&) const)  &Vars::notify)
		,

	class_<GlobalVars>("GlobalVars")
		.def("add_g", &GlobalVars::add_g)
		.def("add_f", &GlobalVars::add_f)
		.def("add_t", &GlobalVars::add_t)
		,
		
	class_<QuestList>("QuestList")
		.def("vars", &QuestList::vars)
		.def("add", &QuestList::add_quest)
		.def("refresh", &refresh_quest)
		,
		
	class_<ScriptQuest>("ScriptQuest")
		.enum_("constants")
		[
			value("NPC_VISIT", 1),
			value("NPC_KILL", 2),
			value("OBJ_USE", 4),
			value("OBJ_GET", 8),
			value("OBJ_DROP", 16)						
		]
		.def("add", &ScriptQuest::add)

];

}


