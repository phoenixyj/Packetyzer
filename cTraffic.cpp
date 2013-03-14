#include "stdafx.h"
#include "cTraffic.h"

cTraffic::cTraffic()
{
	nConStreams = 0;
	ConStreams = (cConStream**)malloc( sizeof(cConStream*) * nConStreams);

}

BOOL cTraffic::AddPacket(cPacket* Packet, UINT TimeStamp)
{
	if (nConStreams > 0)
	{
		for (UINT j=0; j<nConStreams; j++)
		{
			if (ConStreams[j]->isIPPacket && Packet->isIPPacket)
			{
				if (ConStreams[j]->AddPacket(Packet)) return TRUE;
				else if (j == (nConStreams - 1))
				{
					if (cDNSStream::Identify(Packet))
					{
						cDNSStream* tmp = new cDNSStream();
						tmp->AddPacket(Packet);

						nConStreams++;
						ConStreams = (cConStream**)realloc((void*)ConStreams, nConStreams * sizeof(cConStream*));
						memcpy((void**)&ConStreams[nConStreams-1],(void**)&tmp, sizeof(cConStream*));
						return TRUE;
					}
					else
					{
						cConStream* tmp = new cConStream();
						tmp->AddPacket(Packet);

						nConStreams++;
						ConStreams = (cConStream**)realloc((void*)ConStreams, nConStreams * sizeof(cConStream*));
						memcpy((void**)&ConStreams[nConStreams-1],(void**)&tmp, sizeof(cConStream*));
						return TRUE;
					}
				}
			}
		}
	}
	else
	{
		if (Packet->isIPPacket && ( (Packet->isTCPPacket) || (Packet->isUDPPacket)))
		{
			if (cDNSStream::Identify(Packet))
			{
				cDNSStream* tmp = new cDNSStream();
				tmp->AddPacket(Packet);

				nConStreams++;
				ConStreams = (cConStream**)realloc((void*)ConStreams, nConStreams * sizeof(cConStream*));
				memcpy((void**)&ConStreams[nConStreams-1],(void**)&tmp, sizeof(cConStream*));
			}
			else
			{
				cConStream* tmp = new cConStream();
				tmp->AddPacket(Packet);

				nConStreams++;
				ConStreams = (cConStream**)realloc((void*)ConStreams, nConStreams * sizeof(cConStream*));
				memcpy((void**)&ConStreams[nConStreams-1],(void**)&tmp, sizeof(cConStream*));
			}
		}
	}

	return TRUE;
}

cTraffic::~cTraffic()
{
}