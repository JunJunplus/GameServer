# $Id: Makefile 6798 2006-06-07 17:25:25Z zjw $

CXX =/usr/local/bin/g++
CXXFLAGS = $(ZEBRA_COMPILE_OPT) -I../base -I../script -Wall  -D_USE_CMD_NAMESPACE -D_ENCDEC_MSG  -D_ALL_SUPER_GM -D_GNU_SOURCE -D_REENTRANT `mysql_config --cflags` `xml2-config --cflags`
CXXLFLAGS = $(ZEBRA_COMPILE_OPT) -Wall -L../base -lzebra -lencdec -L../script -llua -lluabind -lpthread -lz `mysql_config --libs_r` `xml2-config --libs` -llog4cxx -L/usr/local/lib -lgd -ljpeg 
LIB_ZEBRA = ../base/libzebra.a

OBJS =SceneSafety.o MiniClient.o Dice.o MiniGame.o CountryAlly.o ScenePk.o ScenePet.o SceneArmy.o SceneUser.o zObject.o SessionClient.o RebuildObject.o  CountryTech.o script_func.o script.o Stock.o Gold.o CartoonPet.o Horse.o SceneNpcAI.o SceneEntryPk.o SceneNpc.o ScenesServer.o SceneTask.o LoadMap.o Scene.o SceneManager.o SceneUserManager.o SceneTaskManager.o RecordClient.o ObjectManager.o TimeTick.o SceneNpcManager.o Chat.o zSceneEntryIndex.o Trade.o NpcTrade.o MessageSystem.o Skill.o SkillManager.o SceneSkill.o Team.o TeamManager.o Property.o WayPoint.o SceneUnion.o MagicRange.o Quest.o QuestCondition.o QuestAction.o QuestEvent.o QuestTable.o SceneSchool.o Mask.o SkillStatusManager.o SceneSept.o SceneRelation.o Leechdom.o SceneDare.o Rush.o GuardNpc.o SceneQuiz.o SceneNpcDare.o CountryDare.o SceneCountry.o

SRCS = $(OBJS:%.o=%.cpp)
DEPS = $(OBJS:%.o=.%.d)

ALL_TARGETS = ScenesServer

all: $(ALL_TARGETS)

include $(DEPS)
$(DEPS): .%.d: %.cpp
	@$(CXX) -M $(CXXFLAGS) $< > $@.$$$$; sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; rm -f $@.$$$$

$(OBJS): %.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(ALL_TARGETS): $(OBJS) $(LIB_ZEBRA)
	$(CXX) $(OBJS) -o $@ $(CXXLFLAGS)

clean:
	@rm -rf *.o $(ALL_TARGETS) core *~

