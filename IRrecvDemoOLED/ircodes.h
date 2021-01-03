//*************************************************************************************
// ircodes.h
// Buttons captured for RCI Equipment
// Recorded using IRrecvDumpV2.ino
// More should be OK for ESP8266
//*************************************************************************************

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// SONY Receiver
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//*************************************************************************************
// Power
// Code      : 0x30DF8E3C (48 Bits)
uint16_t rawDataRcvPower[95] = {2544, 442,  1356, 436,  770, 424,  1356, 436,  768, 426,  1354, 436,  770, 424,  770, 424,  770, 424,  768, 424,  770, 424,  770, 424,  1356, 436,  1354, 438,  770, 424,  770, 21896,  2518, 442,  1354, 436,  770, 424,  1356, 434,  768, 426,  1354, 436,  770, 426,  768, 424,  770, 424,  768, 426,  770, 424,  770, 424,  1354, 438,  1354, 438,  768, 426,  770, 21920,  2518, 444,  1354, 438,  768, 426,  1356, 436,  768, 426,  1354, 438,  768, 424,  770, 426,  770, 424,  768, 426,  770, 424,  768, 426,  1354, 436,  1354, 438,  768, 426,  768};  // UNKNOWN 30DF8E3C

//*************************************************************************************
// SAT
// Code      : 0x2F10CFF (48 Bits)
uint16_t rawDataRcvSAT[95] = {2556, 430,  1366, 426,  1366, 426,  774, 420,  774, 420,  774, 420,  772, 420,  774, 420,  774, 420,  774, 420,  772, 420,  774, 420,  1364, 426,  1366, 426,  774, 420,  1366, 22154,  2530, 430,  1366, 426,  1366, 426,  774, 420,  774, 420,  774, 420,  774, 420,  774, 420,  774, 420,  774, 418,  774, 420,  774, 420,  1366, 426,  1364, 426,  774, 420,  1366, 21924,  2530, 430,  1366, 426,  1366, 426,  774, 420,  772, 422,  772, 420,  774, 420,  774, 420,  774, 420,  772, 420,  774, 420,  774, 420,  1366, 426,  1364, 426,  774, 420,  1366};  // UNKNOWN 2F10CFF

//*************************************************************************************
// TV
// Code      : 0xEC5425F3 (48 Bits)
uint16_t rawDataRcvTV[95] = {2550, 438,  782, 412,  1362, 428,  782, 412,  1360, 430,  778, 416,  1358, 432,  1358, 434,  774, 420,  774, 420,  770, 424,  772, 422,  1354, 436,  1354, 438,  764, 430,  764, 21560,  2516, 446,  760, 432,  1350, 442,  760, 434,  1348, 442,  760, 432,  1352, 440,  1350, 440,  762, 432,  762, 432,  764, 430,  766, 430,  1350, 440,  1352, 440,  764, 428,  766, 21328,  2524, 438,  780, 414,  1362, 430,  780, 414,  1362, 430,  782, 412,  1362, 430,  1362, 430,  784, 410,  784, 408,  786, 408,  784, 410,  1362, 430,  1362, 430,  784, 410,  786};  // UNKNOWN EC5425F3

//*************************************************************************************
// CD
// Code      : 0x197218A8 (48 Bits)
uint16_t rawDataRcvCD[95] = {2546, 442,  1354, 436,  766, 428,  1358, 434,  766, 428,  768, 426,  1360, 432,  766, 426,  768, 426,  768, 426,  768, 426,  768, 426,  1360, 430,  1360, 432,  768, 426,  766, 22154,  2518, 444,  1352, 438,  764, 430,  1350, 442,  762, 432,  760, 432,  1350, 440,  760, 434,  760, 434,  760, 434,  760, 434,  760, 434,  1348, 444,  1348, 444,  760, 434,  760, 21932,  2518, 442,  1352, 440,  768, 426,  1354, 438,  766, 426,  770, 424,  1354, 438,  768, 424,  768, 426,  768, 426,  768, 426,  768, 426,  1354, 438,  1354, 436,  768, 426,  768};  // UNKNOWN 197218A8

//*************************************************************************************
// DVD
// Code      : 0x7F5AF0B2 (48 Bits)
uint16_t rawDataRcvDVD[95] = {2592, 396,  1398, 394,  810, 382,  1406, 386,  1402, 388,  1398, 394,  1402, 390,  1398, 392,  814, 380,  814, 380,  806, 386,  812, 382,  1402, 390,  1400, 390,  814, 380,  810, 20192,  2572, 390,  1398, 394,  814, 380,  1396, 396,  1400, 390,  1402, 390,  1406, 386,  1396, 394,  802, 392,  818, 376,  818, 376,  804, 388,  1414, 378,  1398, 392,  818, 376,  816, 20084,  2570, 394,  1394, 394,  818, 376,  1408, 386,  1400, 390,  1408, 384,  1400, 390,  1408, 386,  804, 388,  816, 378,  818, 376,  812, 382,  1402, 390,  1396, 394,  808, 386,  816};  // UNKNOWN 7F5AF0B2

//*************************************************************************************
// Vol Up
// Code      : 0x373C2B61 (48 Bits)
uint16_t rawDataRcvVolUp[95] = {2594, 394,  814, 378,  1402, 390,  800, 392,  814, 380,  1398, 394,  814, 378,  806, 388,  808, 386,  810, 384,  812, 382,  814, 380,  1394, 398,  1394, 398,  808, 386,  812, 22530,  2566, 396,  800, 392,  1394, 398,  808, 384,  810, 384,  1400, 392,  810, 382,  816, 378,  814, 378,  802, 390,  808, 386,  818, 376,  1392, 398,  1400, 392,  806, 386,  814, 22474,  2568, 394,  800, 392,  1398, 394,  802, 390,  810, 382,  1398, 394,  810, 384,  816, 378,  818, 376,  810, 384,  814, 380,  814, 378,  1404, 388,  1404, 388,  814, 378,  812};  // UNKNOWN 373C2B61

//*************************************************************************************
// Vol Down
// Code      : 0xF7A74B46 (48 Bits)
uint16_t rawDataRcvVolDn[95] = {2592, 398,  1392, 398,  1398, 392,  818, 376,  800, 392,  1400, 394,  810, 382,  810, 384,  812, 382,  802, 390,  814, 380,  804, 388,  1398, 392,  1404, 388,  814, 378,  818, 21928,  2566, 398,  1396, 394,  1400, 392,  812, 382,  810, 382,  1398, 394,  808, 386,  812, 380,  814, 380,  810, 384,  812, 382,  816, 378,  1398, 394,  1396, 396,  808, 384,  808, 21882,  2568, 396,  1396, 394,  1396, 394,  812, 380,  814, 378,  1394, 398,  808, 386,  812, 380,  814, 380,  806, 388,  814, 380,  810, 384,  1400, 392,  1394, 396,  806, 388,  816};  // UNKNOWN F7A74B46


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// SONY DVD
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//*************************************************************************************
// POWER
// Code      : 0xE926FD (63 Bits)
uint16_t rawDataDVDPower[125] = {2552, 462,  1338, 452,  760, 430,  1340, 450,  762, 428,  1340, 450,  762, 430,  762, 430,  764, 428,  1340, 450,  762, 430,  1338, 452,  1336, 452,  762, 430,  1340, 450,  760, 432,  764, 430,  762, 428,  1340, 450,  1340, 448,  1340, 12408,  2550, 462,  1340, 452,  760, 430,  1340, 450,  760, 430,  1340, 450,  762, 430,  764, 430,  764, 428,  1340, 450,  762, 430,  1340, 448,  1340, 450,  764, 428,  1342, 448,  764, 428,  766, 426,  766, 424,  1342, 448,  1342, 448,  1340, 12410,  2554, 460,  1342, 448,  764, 426,  1340, 450,  764, 428,  1340, 450,  760, 430,  764, 428,  764, 428,  1340, 452,  760, 430,  1340, 450,  1338, 452,  760, 432,  1338, 452,  762, 430,  764, 430,  762, 430,  1340, 450,  1342, 448,  1340};  // UNKNOWN E926FD

//*************************************************************************************

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// VIZIO TV 50"
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//*************************************************************************************
// POWER Button
//Code      : 0x75159657 (36 Bits)
uint16_t rawDataVizPower[71] = {9166, 4376,  730, 404,  728, 406,  726, 1528,  730, 404,  728, 406,  726, 406,  726, 406,  726, 408,  726, 1530,  730, 1526,  730, 406,  724, 1530,  730, 1526,  734, 1522,  748, 1508,  756, 1502,  756, 376,  752, 380,  748, 386,  744, 1510,  756, 378,  746, 386,  742, 390,  738, 394,  740, 1514,  754, 1502,  758, 1498,  758, 376,  752, 1502,  756, 1502,  750, 1506,  740, 1514,  730, 39326,  9168, 2126,  730};  // UNKNOWN 75159657

//*************************************************************************************
// Netflix Button
//Code      : 0x20DFD728 (32 Bits)
uint16_t rawDataVizNetflix[71] = {9166, 4378,  730, 402,  726, 406,  726, 1528,  730, 402,  726, 406,  728, 406,  726, 406,  726, 406,  726, 1528,  730, 1526,  730, 404,  726, 1528,  730, 1526,  730, 1526,  728, 1528,  730, 1528,  728, 1528,  728, 1528,  728, 404,  726, 1530,  728, 404,  726, 1530,  726, 1528,  728, 1528,  728, 406,  726, 406,  726, 1530,  726, 406,  726, 1530,  728, 404,  726, 406,  724, 404,  724, 39328,  9166, 2126,  724};  // NEC 20DFD728

//*************************************************************************************
// Amazon Button
//Code      : 0x20DF57A8 (32 Bits)
uint16_t rawDataVizAmazon[71] = {9166, 4378,  738, 396,  730, 402,  728, 1526,  736, 398,  732, 400,  730, 402,  728, 404,  728, 404,  730, 1526,  738, 1518,  738, 396,  730, 1524,  738, 1518,  738, 1518,  738, 1518,  738, 1518,  738, 396,  730, 1524,  738, 396,  730, 1526,  736, 396,  730, 1524,  738, 1518,  738, 1518,  736, 1520,  736, 396,  732, 1522,  738, 396,  730, 1526,  736, 396,  730, 404,  726, 400,  728, 39316,  9164, 2128,  734};  // NEC 20DF57A8

//*************************************************************************************
// Input Button
// Code      : 0x20DFF40B (32 Bits)
uint16_t rawDataVizInput[71] = {9164, 4382,  730, 404,  728, 406,  728, 1528,  730, 402,  728, 404,  728, 406,  726, 406,  728, 404,  728, 1528,  730, 1526,  730, 402,  728, 1528,  730, 1526,  730, 1526,  732, 1526,  730, 1526,  730, 1526,  732, 1526,  730, 1526,  730, 1526,  730, 402,  728, 1526,  732, 402,  728, 404,  728, 406,  726, 404,  728, 406,  726, 406,  728, 1528,  730, 402,  728, 1528,  732, 1522,  732, 39324,  9164, 2128,  732};  // NEC 20DFF40B

//*************************************************************************************
// OK Button
//Code      : 0x20DF22DD (32 Bits)
uint16_t rawDataVizOK[67] = {9164, 4378,  724, 410,  718, 414,  718, 1538,  722, 412,  718, 414,  716, 416,  716, 416,  718, 416,  716, 1538,  724, 1532,  722, 412,  718, 1538,  724, 1532,  724, 1532,  724, 1532,  724, 1532,  724, 412,  718, 414,  716, 1538,  724, 410,  718, 414,  716, 416,  716, 1536,  724, 410,  718, 1536,  724, 1532,  724, 410,  718, 1536,  724, 1532,  724, 1532,  724, 410,  718, 1532,  724};  // NEC 20DF22DD

//*************************************************************************************
// Back Button
// Code      : 0x20DF52AD (32 Bits)
uint16_t rawDataVizBack[71] = {9166, 4382,  730, 406,  722, 412,  720, 1536,  728, 406,  720, 412,  720, 414,  716, 416,  720, 414,  718, 1536,  726, 1532,  726, 410,  718, 1538,  726, 1532,  724, 1532,  724, 1534,  724, 1534,  724, 410,  718, 1538,  722, 412,  718, 1538,  724, 410,  718, 414,  718, 1538,  722, 412,  718, 1538,  722, 412,  718, 1538,  722, 412,  718, 1538,  722, 1536,  722, 412,  718, 1534,  722, 39344,  9166, 2130,  724};  // NEC 20DF52AD

//*************************************************************************************
// Exit Button
// Code      : 0x20DF926D (32 Bits)
uint16_t rawDataVizExit[71] = {9162, 4380,  724, 408,  718, 414,  718, 1536,  724, 410,  718, 414,  718, 416,  716, 416,  718, 414,  720, 1536,  724, 1532,  724, 408,  720, 1536,  724, 1532,  724, 1532,  724, 1532,  724, 1532,  724, 1532,  726, 408,  718, 414,  718, 1536,  724, 408,  720, 412,  718, 1536,  724, 410,  720, 414,  718, 1536,  724, 1532,  724, 410,  718, 1538,  726, 1532,  724, 410,  718, 1532,  726, 39324,  9164, 2128,  726};  // NEC 20DF926D

//*************************************************************************************
// Vizio "V" Button
// Code      : 0x20DFB44B (32 Bits)
uint16_t rawDataVizVbut[71] = {9162, 4382,  724, 408,  724, 410,  722, 1534,  724, 408,  724, 408,  724, 408,  724, 408,  722, 410,  724, 1532,  726, 1530,  726, 408,  726, 1530,  726, 1530,  726, 1532,  724, 1530,  724, 1532,  726, 1530,  726, 408,  722, 1534,  724, 1530,  724, 408,  724, 1532,  724, 408,  724, 410,  722, 410,  722, 1532,  726, 406,  722, 410,  724, 1532,  724, 408,  724, 1532,  726, 1526,  724, 39324,  9162, 2130,  726};  // NEC 20DFB44B

//*************************************************************************************
// Vizio VOL + Button
// Code      : 0x20DF40BF (32 Bits)
uint16_t rawDataVizVol+[71] = {9150, 4392,  704, 428,  706, 424,  706, 1552,  706, 426,  706, 426,  704, 426,  704, 428,  704, 428,  704, 1552,  704, 1552,  704, 428,  704, 1552,  706, 1550,  706, 1552,  706, 1550,  706, 1550,  708, 426,  704, 1552,  704, 428,  704, 430,  704, 428,  704, 430,  704, 428,  704, 426,  706, 1552,  704, 428,  706, 1550,  706, 1550,  706, 1550,  706, 1550,  708, 1550,  704, 1546,  704, 39348,  9150, 2142,  706};  // NEC 20DF40BF

//*************************************************************************************
// Vizio VOL - Button
// Code      : 0x15CF9C1B (36 Bits)
uint16_t rawDataVizVol-[71] = {9192, 4348,  738, 394,  744, 388,  738, 1518,  742, 390,  738, 394,  744, 388,  740, 392,  742, 388,  742, 1514,  744, 1510,  744, 390,  736, 1518,  738, 1518,  740, 1514,  748, 1508,  748, 1506,  742, 1514,  740, 1516,  748, 384,  740, 392,  752, 380,  742, 390,  740, 396,  746, 384,  736, 396,  744, 388,  752, 1504,  742, 1514,  744, 1512,  748, 1508,  740, 1516,  744, 1508,  746, 39298,  9188, 2104,  744};  // UNKNOWN 15CF9C1B

//*************************************************************************************
// Vizio CH + Button
// Code      : 0x20DF00FF (32 Bits)
uint16_t rawDataVizCh+[71] = {9148, 4394,  716, 416,  714, 418,  716, 1540,  716, 416,  716, 416,  718, 414,  714, 418,  714, 418,  714, 1542,  716, 1538,  716, 416,  716, 1538,  716, 1540,  718, 1538,  714, 1540,  716, 1540,  716, 416,  716, 416,  712, 418,  716, 416,  716, 416,  716, 416,  714, 418,  714, 418,  714, 1542,  716, 1538,  716, 1540,  716, 1542,  714, 1542,  716, 1540,  714, 1542,  716, 1538,  714, 39334,  9146, 2144,  714};  // NEC 20DF00FF

//*************************************************************************************
// Vizio CH - Button
// Code      : 0x20DF807F (32 Bits)
uint16_t rawDataVizCh-[71] = {9146, 4396,  706, 426,  706, 426,  706, 1548,  706, 426,  704, 428,  704, 428,  706, 426,  706, 426,  706, 1550,  706, 1550,  706, 426,  706, 1550,  706, 1550,  706, 1548,  706, 1550,  706, 1550,  706, 1550,  706, 426,  706, 426,  706, 426,  704, 428,  704, 428,  706, 426,  704, 428,  704, 428,  704, 1552,  704, 1550,  704, 1552,  706, 1550,  704, 1550,  704, 1552,  704, 1548,  706, 39338,  9142, 2148,  706};  // NEC 20DF807F








 
