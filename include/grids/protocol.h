
#pragma once

#include <map>
#include <string>

#include <QMutex>
#include <QSize>
#include <QThread>
#include <QWaitCondition>
#include <QTcpSocket>

#include <json/value.h>
#include <grids/event.h>
#include <grids/define.h>

namespace Grids {

  class Protocol : public QThread {
	Q_OBJECT

	  public:
	Protocol(QObject *parent = 0);
	~Protocol();

	bool connectToNode(const char *address);
	void sendProtocolInitiationString();
	int protocolWrite(std::string &str);
	int protocolWrite(const char *str, uint32_t len);
	void closeConnection();

	std::string stringifyValue(Value *val);
	void sendRequest(std::string);
	void sendRequest(std::string, Value *args);

	void handleMessage(std::string &msg);
	
	Value parseJson(std::string &msg);
	bool sockConnected();

	int runEventLoopThreaded();
	void stopEventLoopThread();

	bool getEventLoopRunning();
	void setEventLoopRunning(bool);

	// last_event is mainly used for testing 
	Event* last_event;
	
  signals:
	void receiveEvent(Protocol*, Event*);
	void protocolInitiated(Protocol*, Event*);

  protected:
	void run();

  private:
	QTcpSocket sock;
	QMutex finishedMutex;
	QMutex eventLoopRunningMutex;
	bool running;
	
	void endianSwap(unsigned int&);
	quint32 byteSwap(quint32);

	static const quint32 GRIDS_PORT = 1488;
  };
}


