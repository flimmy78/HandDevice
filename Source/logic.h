#ifndef LOGIC_H
#define LOGIC_H

#include "basedef.h"
#include "db.h"

extern U8 logic_setTime(U8*);
extern U8 logic_readGatewayId(U8* gatewayId);
extern U8 logic_issueMeterInfo(U8* gatewayId);
extern U8 logic_queryOneMeterInfo(U8* gatewayId, U16 meterId, db_meterinfo_ptr pDbInfo);
extern U8 logic_issueOneMeterInfo(U8* gatewayId, db_meterinfo_ptr pDbInfo);
extern U8 logic_genTimeNodesStr(U8* buf, U16 bufSize, U8* startTime, U16 nodes);
extern U8 logic_issueTimeNodes(U8* buf, U16 bufSize, U8* gatewayId);
extern U8 logic_modifyGatewayId(U8* originalId, U8* targetId);
extern U8 logic_readGPRSParam(U8* gatewayId, U8* apnId, U8* svrId, U8* port);
extern U8 logic_modifyGPRSParam(U8* gatewayId, U8* apnId, U8* svrId, U8* port);
extern U8 logic_reboot(U8* gatewayId);
extern U8 logit_issueRereadParam(U8* gatewayId, U8* mReadCnt, U8* mReadIntv, U8* vReadCnt, U8* vReadIntv);
extern U8 logic_readMeterImmd(U8* gatewayId);
extern U8 logic_readVersion(U8* gatewayId, U8* hardVer, U8* softVer);
extern U8 logic_readBaseInfo(U8* gatewayId, db_meterinfo_ptr pDbInfo);
extern logic_readNextTempInfo(db_meterinfo_ptr pDbInfo);
#endif
