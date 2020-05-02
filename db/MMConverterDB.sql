CREATE DATABASE  IF NOT EXISTS `MMConverter` /*!40100 DEFAULT CHARACTER SET latin1 */;
USE `MMConverter`;
-- MySQL dump 10.13  Distrib 5.7.29, for Linux (x86_64)
--
-- Host: 127.0.0.1    Database: MMConverter
-- ------------------------------------------------------
-- Server version	5.7.29-0ubuntu0.18.04.1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `mmMegaBoom`
--

DROP TABLE IF EXISTS `mmMegaBoom`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `mmMegaBoom` (
  `ID` int(11) NOT NULL,
  `movieName` varchar(100) DEFAULT NULL,
  `movieType` varchar(45) DEFAULT NULL,
  `purchaseAmount` varchar(45) DEFAULT NULL,
  `purchaseType` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `mmqueue`
--

DROP TABLE IF EXISTS `mmqueue`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `mmqueue` (
  `idmmqueue` int(11) NOT NULL AUTO_INCREMENT,
  `fileName` varchar(255) DEFAULT NULL,
  `filePath` varchar(255) DEFAULT NULL,
  `outputDir` varchar(255) DEFAULT NULL,
  `process` int(11) DEFAULT NULL,
  `wasRemoved` int(11) DEFAULT NULL,
  `wasDeleted` int(11) DEFAULT NULL,
  `stripSubs` int(11) DEFAULT NULL,
  `useCustomSettings` int(11) DEFAULT NULL,
  `preset` varchar(45) DEFAULT NULL,
  `crf` int(11) DEFAULT NULL,
  `formatDuration` bigint(20) DEFAULT NULL,
  `container` varchar(100) DEFAULT NULL,
  `wasProcessed` int(11) DEFAULT NULL,
  `createdTimeStamp` datetime DEFAULT CURRENT_TIMESTAMP,
  `updateTimeStamp` datetime DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `secProcessing` int(11) DEFAULT NULL,
  `numStreams` int(11) DEFAULT NULL,
  `ffmpgCmd` varchar(2048) DEFAULT NULL,
  `streamsInfo` json DEFAULT NULL,
  `formatInfo` json DEFAULT NULL,
  `conversionError` int(11) DEFAULT NULL,
  `conversionErrorCode` int(11) DEFAULT NULL,
  `startFileSize` bigint(20) DEFAULT NULL,
  `endFileSize` bigint(20) DEFAULT NULL,
  PRIMARY KEY (`idmmqueue`)
) ENGINE=InnoDB AUTO_INCREMENT=278 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2020-05-02  6:35:33
