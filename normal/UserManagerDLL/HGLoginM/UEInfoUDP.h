#pragma once

// CUEInfoUDP ÃüÁîÄ¿±ê

class CUEInfoUDP : public CSocket
{
public:
	CUEInfoUDP();
	virtual ~CUEInfoUDP();
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};


