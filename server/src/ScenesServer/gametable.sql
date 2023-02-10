-- MySQL Administrator dump 1.4
--
-- ------------------------------------------------------
-- Server version	4.1.12


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;


--
-- Create schema BillServer
--

CREATE DATABASE /*!32312 IF NOT EXISTS*/ BillServer;
USE BillServer;
CREATE TABLE `BALANCE` (
  `ACCOUNT` varchar(48) NOT NULL default '',
  `ACCID` int(10) unsigned NOT NULL default '0',
  `PASSWORD` varchar(16) default '',
  `ALLGOLDIN` int(10) unsigned NOT NULL default '0',
  `ALLGOLDOUT` int(10) unsigned NOT NULL default '0',
  `ALLMONEYIN` int(10) unsigned NOT NULL default '0',
  `ALLMONEYOUT` int(10) unsigned NOT NULL default '0',
  `LASTTIME` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
  `GOLD` int(10) unsigned NOT NULL default '0',
  `MONEY` int(10) unsigned NOT NULL default '0',
  `GOLDTAX` int(10) unsigned NOT NULL default '0',
  `MONEYTAX` int(10) unsigned NOT NULL default '0',
  `GOLDLIST` int(10) unsigned NOT NULL default '0',
  `MONEYLIST` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`ACCID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
INSERT INTO `BALANCE` VALUES  ('test001',0,'123456',0,0,0,0,'2006-08-18 15:07:20',0,0,0,0,0,0),
 ('test002',2,'123456',0,0,0,0,'0000-00-00 00:00:00',0,0,0,0,0,0);
CREATE TABLE `CONSIGNGOLD` (
  `ID` int(10) unsigned NOT NULL auto_increment,
  `ACCID` int(10) unsigned NOT NULL default '0',
  `NUM` int(10) unsigned NOT NULL default '0',
  `PRICE` int(10) unsigned NOT NULL default '0',
  `TIME` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`ID`),
  KEY `ACCID` (`ACCID`),
  KEY `PRICE` (`PRICE`,`NUM`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE `CONSIGNGOLDHISTORY` (
  `ID` int(10) NOT NULL default '0',
  `ACCID` int(10) unsigned NOT NULL default '0',
  `NUM` int(10) unsigned NOT NULL default '0',
  `COMMITPRICE` int(10) unsigned NOT NULL default '0',
  `PRICE` int(10) unsigned NOT NULL default '0',
  `COMMITTIME` int(10) unsigned NOT NULL default '0',
  `SYSMONEY` int(10) unsigned NOT NULL default '0',
  `OKTIME` int(10) unsigned NOT NULL default '0',
  KEY `ACCID` (`ACCID`),
  KEY `OKTIME` (`OKTIME`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE `CONSIGNMONEY` (
  `ID` int(10) unsigned NOT NULL auto_increment,
  `ACCID` int(10) unsigned NOT NULL default '0',
  `NUM` int(10) unsigned NOT NULL default '0',
  `PRICE` int(10) unsigned NOT NULL default '0',
  `TIME` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`ID`),
  KEY `ACCID` (`ACCID`),
  KEY `PRICE` (`PRICE`,`NUM`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE `CONSIGNMONEYHISTORY` (
  `ID` int(10) NOT NULL default '0',
  `ACCID` int(10) unsigned NOT NULL default '0',
  `NUM` int(10) unsigned NOT NULL default '0',
  `COMMITPRICE` int(10) unsigned NOT NULL default '0',
  `PRICE` int(10) unsigned NOT NULL default '0',
  `COMMITTIME` int(10) unsigned NOT NULL default '0',
  `SYSMONEY` int(10) unsigned NOT NULL default '0',
  `OKTIME` int(10) unsigned NOT NULL default '0',
  KEY `ACCID` (`ACCID`),
  KEY `OKTIME` (`OKTIME`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
--
-- Create schema FLServer
--

CREATE DATABASE /*!32312 IF NOT EXISTS*/ FLServer;
USE FLServer;
CREATE TABLE `LOGIN` (
  `USERID` int(10) unsigned NOT NULL auto_increment,
  `LOGINID` varchar(32) default NULL,
  `PASSWORD` varchar(16) NOT NULL default '',
  `ISUSED` tinyint(3) unsigned NOT NULL default '0',
  `ISFORBID` tinyint(3) unsigned NOT NULL default '0',
  `NAME` varchar(32) default NULL,
  `BIRTH` varchar(16) default NULL,
  `PHONE` varchar(32) default NULL,
  `QUESTION1` varchar(64) default NULL,
  `ANSWER1` varchar(64) default NULL,
  `QUESTION2` varchar(64) default NULL,
  `ANSWER2` varchar(64) default NULL,
  `EMAIL` varchar(64) default NULL,
  `IDCARD` varchar(32) default NULL,
  `MOBILE` varchar(32) default NULL,
  `COMMENDNAME` varchar(32) default NULL,
  `CREATEDATE` datetime default NULL,
  `LASTACTIVEDATE` datetime default NULL,
  PRIMARY KEY  (`USERID`),
  KEY `unique` (`LOGINID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
INSERT INTO `LOGIN` VALUES  (1,'test001','123456',0,0,'Test','','',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'2006-08-20 11:00:00'),
 (2,'test002','123456',0,0,'ttt',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'2006-08-20 11:00:00');
CREATE TABLE `LOGINSTAT` (
  `STATDAY` varchar(64) NOT NULL default '',
  `LOGINCOUNT` int(10) unsigned NOT NULL default '0',
  `LOGINSUCCESSCOUNT` int(10) unsigned NOT NULL default '0',
  `CREATEACCOUNT` int(10) unsigned NOT NULL default '0',
  `CREATESUCCESSACCOUNT` int(10) unsigned NOT NULL default '0',
  `CREATERECORD` int(10) unsigned NOT NULL default '0',
  `CREATESUCCESSRECORD` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`STATDAY`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
INSERT INTO `LOGINSTAT` VALUES  ('2006073017',1,0,0,0,0,0),
 ('2006073018',6,0,0,0,0,0);
--
-- Create schema MiniServer
--

CREATE DATABASE /*!32312 IF NOT EXISTS*/ MiniServer;
USE MiniServer;
CREATE TABLE `MINIGAME` (
  `CHARID` int(10) unsigned NOT NULL default '0',
  `NAME` char(32) NOT NULL default '',
  `COUNTRY` int(10) unsigned NOT NULL default '0',
  `FACE` int(10) unsigned NOT NULL default '0',
  `WIN` int(10) unsigned NOT NULL default '0',
  `LOSE` int(10) unsigned NOT NULL default '0',
  `DRAW` int(10) unsigned NOT NULL default '0',
  `MONEY` int(10) unsigned NOT NULL default '0',
  `SCORE` int(10) NOT NULL default '0',
  PRIMARY KEY  (`CHARID`),
  KEY `TOP` (`SCORE`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
--
-- Create schema RecordServer
--

CREATE DATABASE /*!32312 IF NOT EXISTS*/ RecordServer;
USE RecordServer;
CREATE TABLE `ACCPRIV` (
  `ACCID` int(10) unsigned NOT NULL default '0',
  `PRIV` int(10) unsigned NOT NULL default '0'
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE `ACT` (
  `ID` int(10) unsigned NOT NULL default '0',
  `NAME` varchar(32) NOT NULL default '',
  `STATE` tinyint(3) unsigned NOT NULL default '0',
  `TEXT` varchar(255) NOT NULL default ''
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE `CHARBASE` (
  `CHARID` int(10) unsigned NOT NULL auto_increment,
  `ACCID` int(10) unsigned NOT NULL default '0',
  `NAME` varchar(33) NOT NULL default '',
  `TYPE` smallint(5) unsigned NOT NULL default '0',
  `LEVEL` smallint(5) unsigned NOT NULL default '1',
  `FACE` smallint(5) unsigned NOT NULL default '0',
  `HAIR` int(10) unsigned NOT NULL default '0',
  `BODYCOLOR` int(10) unsigned NOT NULL default '0',
  `GOODNESS` int(10) unsigned NOT NULL default '0',
  `PKADDITION` int(10) unsigned NOT NULL default '0',
  `MAPID` int(10) unsigned NOT NULL default '0',
  `MAPNAME` varchar(33) default '',
  `X` int(10) unsigned NOT NULL default '0',
  `Y` int(10) unsigned NOT NULL default '0',
  `HP` int(10) unsigned NOT NULL default '0',
  `MP` int(10) unsigned NOT NULL default '0',
  `SP` int(10) unsigned NOT NULL default '0',
  `CREATEIP` int(10) unsigned NOT NULL default '0',
  `EXP` bigint(20) unsigned NOT NULL default '0',
  `LUCKY` smallint(5) unsigned NOT NULL default '0',
  `SKILLPOINTS` smallint(5) unsigned NOT NULL default '0',
  `POINTS` smallint(5) unsigned NOT NULL default '0',
  `COUNTRY` int(10) unsigned NOT NULL default '0',
  `UNIONID` int(10) unsigned NOT NULL default '0',
  `CONSORT` int(10) unsigned NOT NULL default '0',
  `SEPTID` int(10) unsigned NOT NULL default '0',
  `SCHOOLID` int(10) unsigned NOT NULL default '0',
  `SYSSET` int(10) unsigned zerofill NOT NULL default '0000000000',
  `FORBIDTALK` bigint(20) unsigned NOT NULL default '0',
  `BITMASK` int(10) unsigned NOT NULL default '0',
  `ONLINETIME` int(10) unsigned NOT NULL default '0',
  `AVAILABLE` tinyint(3) unsigned NOT NULL default '1',
  `LASTACTIVEDATE` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
  `MEN` smallint(5) unsigned NOT NULL default '0',
  `INT` smallint(5) unsigned NOT NULL default '0',
  `DEX` smallint(5) unsigned NOT NULL default '0',
  `STR` smallint(5) unsigned NOT NULL default '0',
  `CON` smallint(5) unsigned NOT NULL default '0',
  `RELIVEWEAKTIME` smallint(5) unsigned NOT NULL default '0',
  `GRACE` int(10) unsigned NOT NULL default '0',
  `EXPLOIT` int(10) unsigned NOT NULL default '0',
  `TIRETIME` tinyblob NOT NULL,
  `OFFLINETIME` int(10) unsigned NOT NULL default '0',
  `FIVETYPE` int(10) unsigned NOT NULL default '0',
  `FIVELEVEL` int(10) unsigned NOT NULL default '0',
  `ALLBINARY` blob,
  `ANSWERCOUNT` int(10) unsigned NOT NULL default '0',
  `MONEY` int(10) unsigned NOT NULL default '0',
  `HONOR` int(10) unsigned NOT NULL default '0',
  `GOMAPTYPE` int(10) unsigned NOT NULL default '0',
  `MAXHONOR` int(10) unsigned NOT NULL default '0',
  `MSGTIME` int(10) unsigned NOT NULL default '0',
  `ACCPRIV` int(10) unsigned NOT NULL default '0',
  `GOLD` int(10) unsigned NOT NULL default '0',
  `TICKET` int(10) unsigned NOT NULL default '0',
  `CREATETIME` int(10) unsigned NOT NULL default '0',
  `GOLDGIVE` int(10) unsigned NOT NULL default '0',
  `PETPACK` int(3) unsigned NOT NULL default '0',
  `PETPOINT` int(10) unsigned NOT NULL default '0',
  `LEVELSEPT` int(3) unsigned NOT NULL default '0',
  `PUNISHTIME` int(10) unsigned NOT NULL default '0',
  `TRAINTIME` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`CHARID`),
  KEY `ACCID` (`ACCID`),
  KEY `NAME` (`NAME`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
INSERT INTO `CHARBASE` VALUES  (6,0,'3334',2,150,2,267116544,4284943588,0,38,1060069,'ËÎ¹ú¡¤Íõ³Ç',803,480,2750,210,840,135440576,9,10,141,940,2,0,0,0,0,0000000000,0,1,2305,1,'2006-08-20 11:39:17',0,0,0,0,0,0,0,0,0x00000000000000000000000000000000300000000000000000000000000000000000000000,1156045157,5,1,0x86020000789C9B2961C4C8C1C0C0907CE1B94BF03CAD0C109EC9C4C070E9C7F5CB0C78002394FE0F043E7D33F0298503905A048F95283D830B301256020439179FBB4C3CC6B71F84576A32305CF877EEC9CEEF844D650443186887D2EF185219F731CE6542D5F32DF3C1200FCB856C4BD870C9ADE125362CB700D36567A2EC42104E916460387C63CF3ADDEBDB2F1CDFF317AB7A84A94C1836A005211C0CED7409722FF1612932759A29082B303330ECF97B6D393EF588F06226D286A110964F985F30E392636420362CBF5F7FEE32399485038445B519188EEFDFBD7EEB91336F8E2CDFFBE4DA0E4CF588B06487D800CE1B5D50B1450C1750544B8049916C2FA4B044389A9101775A46062F802A03888E3972C00C8E191CB8E44458212E3DF5ECF87EFCA61C06A64BE6E2F57C20FC15A8E5F0BDF3F7F1A967C4C1C607067FBAC4076C1888F5E94E60BA94B3D43F05C2AE9AB07409AE81D662538F48472C101BF84004ACEE99C3B2151E54FC7095410A59486189008C60530883025AA44936502E84812544D43D84D325A8BC5CEE6DAA03C25B8001B5ED0CF630840146249A1143948B41082E8608CB079FF60EF2747987F50E4E67115F5E824AAFEF50D56E0CA05210D8E644D2CDC263FF9C15A8EABF2130AF324115FD3762D8BE8809AAF3BF3130133341B5FD67A8FF03CCD1FBFFFFFB8F0A182E02F12ABFFF121EFFFFEFFE6FF25FFCCB8CFF354940897A86FF3BB81918AE3CDBF5E6D05BDCEEE422CA3708003213EC0941750686ADB74EFCDDBEF7F4D76D67B02B067B368311166E2008F2901110CB00B10A92523190910CD8CA79468C0007A901852F0B9A7A904250D415972496A48224740D2FE397BE8257DAF8A32713766910C710D61001F727400C76B8624E0606009764EBBA,1,9997900,0,0,0,0,0,0,0,1156042851,0,3,0,0,0,0);
CREATE TABLE `CHARTEST` (
  `NAME` varchar(33) NOT NULL default '',
  `LEVEL` int(10) unsigned NOT NULL default '1',
  `UPDATETIME` int(10) unsigned NOT NULL default '0',
  `UPDATEUSETIME` int(10) unsigned NOT NULL default '0',
  `DEATHTIMES` int(10) NOT NULL default '0',
  `HPLEECHDOM` int(10) unsigned NOT NULL default '0',
  `MPLEECHDOM` int(10) unsigned NOT NULL default '0',
  `SPLEECHDOM` int(10) unsigned NOT NULL default '0',
  `GETMONEY` int(10) unsigned NOT NULL default '0',
  `GETHEIGH` int(10) unsigned NOT NULL default '0',
  `GETSOCKET` int(10) unsigned NOT NULL default '0',
  `GETMATERIAL` int(10) unsigned NOT NULL default '0',
  `GETSTONE` int(10) unsigned NOT NULL default '0',
  `GETSCROLL` int(10) unsigned NOT NULL default '0',
  `MONEY` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`NAME`,`LEVEL`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE `GIFT` (
  `ACTID` int(10) unsigned NOT NULL default '0',
  `NAME` varchar(32) NOT NULL default '',
  `CHARID` int(10) unsigned NOT NULL default '0',
  `ITEMGOT` tinyint(3) unsigned NOT NULL default '0',
  `ITEMID` int(10) unsigned NOT NULL default '0',
  `ITEMTYPE` tinyint(3) unsigned NOT NULL default '0',
  `ITEMNUM` int(10) unsigned NOT NULL default '0',
  `BIND` tinyint(3) unsigned NOT NULL default '0',
  `MONEY` int(10) unsigned NOT NULL default '0',
  `MAILTEXT` varchar(255) NOT NULL default ''
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE `RESTORECARD` (
  `ACCID` int(10) unsigned NOT NULL default '0',
  `SUBAT` int(10) unsigned NOT NULL default '0',
  `BALANCE` int(10) unsigned NOT NULL default '0',
  `TID` char(100) NOT NULL default '',
  PRIMARY KEY  (`ACCID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE `RESTOREGOLD` (
  `ACCID` int(10) unsigned NOT NULL default '0',
  `RESTOREGOLD` int(10) unsigned NOT NULL default '0',
  `BALANCE` int(10) unsigned NOT NULL default '0',
  `TID` char(100) NOT NULL default '',
  PRIMARY KEY  (`ACCID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
--
-- Create schema SessionServer
--

CREATE DATABASE /*!32312 IF NOT EXISTS*/ SessionServer;
USE SessionServer;
CREATE TABLE `ACT` (
  `ID` int(10) unsigned NOT NULL default '0',
  `NAME` varchar(32) NOT NULL default '',
  `STATE` tinyint(3) unsigned NOT NULL default '0',
  `TEXT` varchar(255) NOT NULL default ''
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE `ALLY` (
  `COUNTRYID` int(10) unsigned NOT NULL default '0',
  `ALLYCOUNTRYID` int(10) unsigned NOT NULL default '0',
  `CREATETIME` int(10) unsigned NOT NULL default '0',
  `LASTUPTIME` int(10) unsigned NOT NULL default '0',
  `FRIENDDEGREE` int(10) unsigned NOT NULL default '0'
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE `ANSWER` (
  `id` bigint(20) unsigned NOT NULL auto_increment,
  `type` varchar(100) NOT NULL default '',
  `ask` blob NOT NULL,
  `answer_a` blob NOT NULL,
  `answer_b` blob NOT NULL,
  `answer_c` blob NOT NULL,
  `answer_d` blob NOT NULL,
  `answer_e` blob NOT NULL,
  `answer_f` blob NOT NULL,
  `the_answer` bigint(20) NOT NULL default '0',
  `quiz_type` int(11) NOT NULL default '0',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE `ARMY` (
  `ID` int(10) unsigned NOT NULL auto_increment,
  `COUNTRYID` int(10) unsigned NOT NULL default '0',
  `CITYID` int(10) unsigned NOT NULL default '0',
  `NAME` varchar(33) NOT NULL default '',
  `GENNAME` varchar(33) NOT NULL default '',
  `GENID` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE `AUCTION` (
  `ID` int(10) unsigned NOT NULL auto_increment,
  `OWNER` varchar(100) NOT NULL default '',
  `STATE` tinyint(3) unsigned NOT NULL default '0',
  `CHARGE` int(10) unsigned NOT NULL default '0',
  `NAME` varchar(32) NOT NULL default '',
  `TYPE` tinyint(3) NOT NULL default '0',
  `QUALITY` tinyint(3) NOT NULL default '0',
  `NEEDLEVEL` smallint(5) NOT NULL default '0',
  `MINMONEY` int(10) unsigned NOT NULL default '0',
  `MAXMONEY` int(10) unsigned NOT NULL default '0',
  `MINGOLD` int(10) unsigned NOT NULL default '0',
  `MAXGOLD` int(10) unsigned NOT NULL default '0',
  `BIDDER` varchar(32) NOT NULL default '',
  `BIDDER2` varchar(32) NOT NULL default '',
  `STARTTIME` int(10) unsigned NOT NULL default '0',
  `ENDTIME` int(10) unsigned NOT NULL default '0',
  `BIDTYPE` tinyint(3) unsigned NOT NULL default '0',
  `ITEM` blob NOT NULL,
  `OWNERID` int(10) unsigned NOT NULL default '0',
  `BIDDERID` int(10) unsigned NOT NULL default '0',
  `BIDDER2ID` int(10) unsigned NOT NULL default '0',
  `ITEMID` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE `CAPTAIN` (
  `ARMYID` int(10) unsigned NOT NULL default '0',
  `CHARID` int(10) unsigned NOT NULL default '0',
  `CHARNAME` varchar(33) NOT NULL default '',
  `NPCNUM` int(10) NOT NULL default '0',
  PRIMARY KEY  (`ARMYID`,`CHARID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE `CARTOONPET` (
  `CARTOONID` int(10) unsigned NOT NULL auto_increment,
  `NAME` char(32) NOT NULL default '0',
  `NPCID` int(10) unsigned NOT NULL default '0',
  `MASTERID` int(10) unsigned NOT NULL default '0',
  `MASTERNAME` char(32) NOT NULL default '',
  `LEVEL` tinyint(3) unsigned NOT NULL default '0',
  `EXP` int(10) unsigned NOT NULL default '0',
  `MAXEXP` int(10) unsigned NOT NULL default '0',
  `ADDEXP` int(10) unsigned NOT NULL default '0',
  `STATE` tinyint(3) unsigned NOT NULL default '0',
  `ADOPTER` char(32) NOT NULL default '',
  `TIME` int(10) unsigned NOT NULL default '0',
  `SP` int(10) unsigned NOT NULL default '0',
  `MAXSP` int(10) unsigned NOT NULL default '0',
  `MASTERLEVEL` int(10) unsigned NOT NULL default '0',
  `REPAIR` tinyint(3) unsigned NOT NULL default '0',
  PRIMARY KEY  (`CARTOONID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
INSERT INTO `CARTOONPET` VALUES  (2,'Ð¡ÏÉ×Ó',9002,6,'3334',10,1,260,0,0,'',1440000,100,100,150,0);
CREATE TABLE `CITY` (
  `COUNTRY` int(10) unsigned NOT NULL default '0',
  `CITYID` int(10) unsigned NOT NULL default '0',
  `UNIONID` int(10) unsigned NOT NULL default '0',
  `LASTAWARDTIME` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
  `ISAWARD` int(11) NOT NULL default '0',
  `DAREUNIONID` int(10) unsigned NOT NULL default '0',
  `GOLD` int(10) unsigned NOT NULL default '0'
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE `COUNTRY` (
  `ID` int(10) unsigned NOT NULL default '0',
  `KINGNAME` varchar(100) NOT NULL default '',
  `KINGUNIONID` int(10) unsigned NOT NULL default '0',
  `DARETIME` int(10) unsigned NOT NULL default '0',
  `LASTDARETIME` int(10) unsigned NOT NULL default '0',
  `LASTDAILYMONEY` int(10) unsigned NOT NULL default '0',
  `NAME` varchar(33) NOT NULL default '',
  `DIPLOMATNAME` varchar(33) NOT NULL default '',
  `CATCHERNAME` varchar(33) NOT NULL default '',
  `FORMALWIN` int(10) unsigned NOT NULL default '0',
  `FORMALFAIL` int(10) unsigned NOT NULL default '0',
  `ANNOYWIN` int(10) unsigned NOT NULL default '0',
  `ANNOYFAIL` int(10) unsigned NOT NULL default '0',
  `DARECOUNTRYID` int(10) unsigned NOT NULL default '0',
  `STAR` int(10) unsigned NOT NULL default '0',
  `TAX` int(10) unsigned NOT NULL default '0',
  `GOLD` bigint(20) unsigned NOT NULL default '0',
  `SILK` bigint(20) unsigned NOT NULL default '0',
  `ORE` bigint(20) unsigned NOT NULL default '0',
  `BOWLDER` bigint(20) unsigned NOT NULL default '0',
  `WOOD` bigint(20) unsigned NOT NULL default '0',
  `COAT` bigint(20) unsigned NOT NULL default '0',
  `HERBAL` bigint(20) unsigned NOT NULL default '0',
  `MATERIAL` bigint(20) unsigned NOT NULL default '0',
  `NOTE` varchar(255) NOT NULL default '',
  `STOCK` bigint(20) unsigned NOT NULL default '0',
  `FORBIDTALK` int(10) NOT NULL default '0',
  `SENDPRISON` int(10) NOT NULL default '0',
  `GEN_EXP` int(10) unsigned NOT NULL default '0',
  `GEN_MAXEXP` int(10) unsigned NOT NULL default '0',
  `GEN_LEVEL` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE `DARERECORD` (
  `ID` int(11) NOT NULL auto_increment,
  `DARETIME` int(10) unsigned NOT NULL default '0',
  `ATTCOUNTRY` int(10) unsigned NOT NULL default '0',
  `DEFCOUNTRY` int(10) unsigned NOT NULL default '0',
  `ATTKINGNAME` varchar(33) NOT NULL default '',
  `DEFKINGNAME` varchar(33) NOT NULL default '',
  `RESULT` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE `EMPERORFORBID` (
  `DATA` char(40) character set latin1 collate latin1_bin NOT NULL default '0'
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
INSERT INTO `EMPERORFORBID` VALUES  ('0:«úÔæ¿ú~È¬`æ¿æ¿Â~È¬`æ¿');
CREATE TABLE `FORBIDTALK` (
  `NAME` varchar(33) NOT NULL default '',
  `LEVEL` smallint(5) unsigned NOT NULL default '0',
  `ACCID` int(10) unsigned NOT NULL default '0',
  `SERVER` int(10) unsigned NOT NULL default '0',
  `COUNTRY` int(10) unsigned NOT NULL default '0',
  `REASON` varchar(255) NOT NULL default '',
  `OPERATION` smallint(5) unsigned NOT NULL default '0',
  `STARTTIME` bigint(20) unsigned NOT NULL default '0',
  `DELAY` int(10) NOT NULL default '0',
  `ISVALID` smallint(5) unsigned NOT NULL default '0',
  `GM` varchar(33) NOT NULL default '0',
  PRIMARY KEY  (`NAME`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE `GIFT` (
  `ACTID` int(10) unsigned NOT NULL default '0',
  `NAME` varchar(32) NOT NULL default '',
  `CHARID` int(10) unsigned NOT NULL default '0',
  `ITEMGOT` tinyint(3) unsigned NOT NULL default '0',
  `ITEMID` int(10) unsigned NOT NULL default '0',
  `ITEMTYPE` tinyint(3) unsigned NOT NULL default '0',
  `ITEMNUM` int(10) unsigned NOT NULL default '0',
  `BIND` tinyint(3) unsigned NOT NULL default '0',
  `MONEY` int(10) unsigned NOT NULL default '0',
  `MAILTEXT` varchar(255) NOT NULL default ''
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE `MAIL` (
  `ID` int(10) unsigned NOT NULL auto_increment,
  `STATE` tinyint(3) unsigned NOT NULL default '0',
  `FROMNAME` varchar(32) NOT NULL default '',
  `TONAME` varchar(32) NOT NULL default '',
  `TITLE` varchar(32) NOT NULL default '',
  `TYPE` tinyint(3) unsigned NOT NULL default '0',
  `CREATETIME` int(10) unsigned NOT NULL default '0',
  `DELTIME` int(10) unsigned NOT NULL default '0',
  `ACCESSORY` tinyint(3) unsigned NOT NULL default '0',
  `ITEMGOT` tinyint(3) unsigned NOT NULL default '0',
  `TEXT` varchar(255) NOT NULL default '',
  `SENDMONEY` int(10) unsigned NOT NULL default '0',
  `RECVMONEY` int(10) unsigned NOT NULL default '0',
  `SENDGOLD` int(10) unsigned NOT NULL default '0',
  `RECVGOLD` int(10) unsigned NOT NULL default '0',
  `BIN` blob,
  `TOID` int(10) unsigned NOT NULL default '0',
  `FROMID` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE `NPCDARE` (
  `COUNTRY` int(10) unsigned NOT NULL default '0',
  `MAPID` int(10) unsigned NOT NULL default '0',
  `NPCID` int(10) unsigned NOT NULL default '0',
  `POSX` int(10) unsigned NOT NULL default '0',
  `POSY` int(10) unsigned NOT NULL default '0',
  `HOLDSEPTID` int(10) unsigned NOT NULL default '0',
  `DARESEPTID` int(10) unsigned NOT NULL default '0',
  `GOLD` int(10) unsigned NOT NULL default '0'
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE `RECOMMEND` (
  `ID` int(10) unsigned NOT NULL default '0',
  `NAME` varchar(33) NOT NULL default '',
  `TOTAL` int(10) unsigned NOT NULL default '0',
  `BALANCE` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE `RECOMMENDSUB` (
  `ID` int(10) unsigned NOT NULL default '0',
  `NAME` varchar(33) NOT NULL default '',
  `LASTLEVEL` int(10) unsigned NOT NULL default '0',
  `TOTAL` int(10) unsigned NOT NULL default '0',
  `RECOMMENDID` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE `SAMPLERELATION` (
  `CHARID` int(10) unsigned NOT NULL default '0',
  `RELATIONID` int(10) unsigned NOT NULL default '0',
  `RELATIONNAME` varchar(33) NOT NULL default '',
  `TYPE` tinyint(3) unsigned NOT NULL default '0',
  `LASTTIME` int(10) unsigned NOT NULL default '0',
  `OCCUPATION` smallint(5) unsigned NOT NULL default '0',
  `DEGREE` smallint(5) unsigned NOT NULL default '0',
  PRIMARY KEY  (`CHARID`,`RELATIONID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE `SCHOOL` (
  `SCHOOLID` int(10) unsigned NOT NULL auto_increment,
  `NAME` varchar(33) NOT NULL default '',
  `MASTERSERIAL` int(10) unsigned NOT NULL default '0',
  `BULLETIN` blob NOT NULL,
  PRIMARY KEY  (`SCHOOLID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE `SCHOOLMEMBER` (
  `SERIALID` int(10) unsigned NOT NULL auto_increment,
  `MASTERID` int(10) unsigned NOT NULL default '0',
  `PRESERIALID` int(10) unsigned NOT NULL default '0',
  `CHARID` int(10) unsigned NOT NULL default '0',
  `NAME` varchar(33) NOT NULL default '',
  `LEVEL` smallint(5) unsigned NOT NULL default '0',
  `JOINTIME` int(10) unsigned NOT NULL default '0',
  `DEGREE` smallint(5) unsigned NOT NULL default '0',
  `LASTTIME` int(10) unsigned NOT NULL default '0',
  `SCHOOLID` int(10) unsigned NOT NULL default '0',
  `OCCUPATION` smallint(5) unsigned NOT NULL default '0',
  `TAG` tinyint(3) unsigned NOT NULL default '0',
  `MASTERBALANCE` int(10) unsigned NOT NULL default '0',
  `PRENTICETOTAL` int(10) unsigned NOT NULL default '0',
  `PRENTICELASTLVL` int(10) unsigned NOT NULL default '0',
  `MASTERTOTAL` bigint(20) unsigned NOT NULL default '0',
  PRIMARY KEY  (`SERIALID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE `SEPT` (
  `SEPTID` int(10) unsigned NOT NULL auto_increment,
  `NAME` varchar(33) NOT NULL default '',
  `CHARID` int(10) unsigned NOT NULL default '0',
  `MASTER` varchar(33) NOT NULL default '',
  `VOTE` tinyint(3) unsigned NOT NULL default '0',
  `CREATETIME` int(10) unsigned NOT NULL default '0',
  `COUNTRYID` int(10) unsigned NOT NULL default '0',
  `REPUTE` int(10) unsigned NOT NULL default '0',
  `UNIONID` int(10) unsigned NOT NULL default '0',
  `LEVEL` int(10) unsigned NOT NULL default '0',
  `NOTE` varchar(255) NOT NULL default '',
  `SPENDGOLD` int(10) unsigned NOT NULL default '0',
  `ISEXP` int(10) unsigned NOT NULL default '0',
  `NORMALEXPTIME` int(10) unsigned NOT NULL default '0',
  `CALLTIMES` int(10) unsigned NOT NULL default '0',
  `CALLDAYTIME` timestamp NOT NULL default CURRENT_TIMESTAMP,
  PRIMARY KEY  (`CHARID`,`SEPTID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE `SEPTMEMBER` (
  `SEPTID` int(10) unsigned NOT NULL default '0',
  `CHARID` int(10) unsigned NOT NULL default '0',
  `ALIASNAME` varchar(33) NOT NULL default '',
  `NAME` varchar(33) NOT NULL default '',
  `OCCUPATION` smallint(5) unsigned NOT NULL default '0',
  PRIMARY KEY  (`CHARID`,`SEPTID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE `SN` (
  `SN` varchar(16) NOT NULL default '',
  PRIMARY KEY  (`SN`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE `SORTLIST` (
  `CHARID` int(10) unsigned NOT NULL default '0',
  `LEVEL` smallint(5) unsigned NOT NULL default '0',
  `EXP` bigint(20) unsigned NOT NULL default '0',
  PRIMARY KEY  (`CHARID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE `TECH` (
  `UID` int(10) unsigned NOT NULL auto_increment,
  `COUNTRYID` int(10) unsigned NOT NULL default '0',
  `TYPE` int(10) unsigned NOT NULL default '0',
  `NAME` varchar(33) NOT NULL default '',
  `PROGRESS` int(10) unsigned NOT NULL default '0',
  `RESEARCHID` int(10) unsigned default '0',
  `RESEARCHNAME` varchar(33) default '',
  `LEVEL` int(10) unsigned NOT NULL default '1',
  `STATUS` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`UID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE `UNION` (
  `UNIONID` int(10) unsigned NOT NULL auto_increment,
  `NAME` varchar(33) NOT NULL default '',
  `CHARID` int(10) unsigned NOT NULL default '0',
  `LEVEL` smallint(5) NOT NULL default '0',
  `MASTER` varchar(33) NOT NULL default '',
  `EXP` bigint(20) NOT NULL default '0',
  `VOTE` tinyint(3) unsigned NOT NULL default '0',
  `CREATETIME` int(10) unsigned NOT NULL default '0',
  `COUNTRYID` int(10) unsigned NOT NULL default '0',
  `MANA` int(10) unsigned NOT NULL default '0',
  `ACTIONPOINT` int(10) unsigned NOT NULL default '0',
  `MONEY` int(10) unsigned NOT NULL default '0',
  `NOTE` varchar(255) NOT NULL default '',
  `CALLTIMES` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`UNIONID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE `UNIONMEMBER` (
  `UNIONID` int(10) unsigned NOT NULL default '0',
  `CHARID` int(10) unsigned NOT NULL default '0',
  `ALIASNAME` varchar(33) NOT NULL default '',
  `NAME` varchar(33) NOT NULL default '',
  `OCCUPATION` smallint(5) unsigned NOT NULL default '0',
  `POWER` int(10) unsigned NOT NULL default '0',
  `SEPTID` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`CHARID`,`UNIONID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE `VOTE` (
  `ID` int(10) unsigned NOT NULL auto_increment,
  `COUNTRYID` int(10) unsigned NOT NULL default '0',
  `STATUS` int(10) unsigned NOT NULL default '0',
  `TYPE` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE `VOTEDPLAYER` (
  `VOTEID` int(10) unsigned NOT NULL default '0',
  `CHARID` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`VOTEID`,`CHARID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE `VOTEITEM` (
  `VOTEID` int(10) unsigned NOT NULL default '0',
  `OPTIONID` int(11) NOT NULL default '0',
  `OPTIONDESC` varchar(100) NOT NULL default '',
  `BALLOT` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`VOTEID`,`OPTIONID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
--
-- Create schema SuperServer
--

CREATE DATABASE /*!32312 IF NOT EXISTS*/ SuperServer;
USE SuperServer;
CREATE TABLE `GAMETIME` (
  `GAMETIME` bigint(20) unsigned NOT NULL default '0'
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
INSERT INTO `GAMETIME` VALUES  (130200);
CREATE TABLE `SERVERLIST` (
  `ID` int(10) unsigned NOT NULL auto_increment,
  `TYPE` int(10) unsigned NOT NULL default '0',
  `NAME` varchar(32) NOT NULL default '',
  `IP` varchar(16) NOT NULL default '127.0.0.1',
  `PORT` int(10) unsigned NOT NULL default '0',
  `EXTIP` varchar(16) NOT NULL default '127.0.0.1',
  `EXTPORT` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
INSERT INTO `SERVERLIST` VALUES  (1,1,'·þÎñÆ÷¹ÜÀíÆ÷','192.168.18.10',10000,'192.168.18.10',10000),
 (20,20,'Session·þÎñÆ÷','192.168.18.10',7000,'192.168.18.10',7000),
 (21,21,'³¡¾°·þÎñÆ÷','192.168.18.10',6010,'192.168.18.10',6010),
 (2200,22,'Íø¹Ø·þÎñÆ÷','192.168.18.10',6020,'192.168.18.10',6020),
 (11,11,'µµ°¸·þÎñÆ÷','192.168.18.10',7010,'192.168.18.10',7010),
 (12,12,'¼Æ·Ñ·þÎñÆ÷','192.168.18.10',7020,'192.168.18.10',7020),
 (23,23,'Mini·þÎñÆ÷','192.168.18.10',6030,'192.168.18.10',6030),
 (112,10,'µÇÂ½·þÎñÆ÷','192.168.18.10',8001,'192.168.18.10',8001);
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
