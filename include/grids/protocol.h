
#pragma once

#include <map>
#include <string>
#include <queue>
#include <json/value.h>
#include <grids/event.h>
#include <grids/define.h>
#include <QMutex>
#include <QSize>
#include <QWaitCondition>
#include <QTcpSocket>
#include <QTime>

namespace Grids {

    class Protocol : public QObject {
        Q_OBJECT

    public:
        Protocol(std::string name, QObject *parent = 0);
        ~Protocol();

        bool connectToNode(const char *address);
        void sendProtocolInitiationString();


        void protocolFlush();
        void closeConnection();

        std::string stringifyValue(Value *val);
        /* Sends a request to the server. int = BROADCAST or LOCAL. */
        void sendRequest(std::string, int);
        void sendRequest(std::string, Value *args, int);
        void sendRequestLoopback(std::string, Value *args);

        void handleMessage(std::string &msg);
	
        Value parseJson(std::string &msg);
        bool sockConnected();

        // last_event is mainly used for testing
        Event* last_event;

        /* Lock the queue mutex, copy the event queue,
           delete the original queue, return the copy, unlock. */
        EventQueue getEvents();

        int getReceivedQueueLength();
        int getOutboundQueueLength();

    public slots:
        void gridsRead();
	
    signals:
        void receiveEvent(Event*);
        void protocolInitiated(Event*);
        void rawData(QString);

    protected:
        void run();

    private:
        QTcpSocket* sock;
        QMutex finishedMutex;
        QMutex eventLoopRunningMutex;
        QMutex event_queue_mutex;
        QMutex outbound_queue_mutex;
        QMutex proto_write_mutex;
        QTime outbound_timer;
        int outbound_limit;

        int protocolWrite(std::string &str);
        int protocolWrite(const char *str, uint32_t len);
	
        void endianSwap(unsigned int&);
        quint32 byteSwap(quint32);

        /* Add an event to the event queue, locking the mutex as necessary. */
        void pushEvent(Event*);

        /* Add to an event queue, for network speed limiting. */
        void pushOutboundRequest(std::string);

        EventQueue event_queue;

        std::queue<std::string> outbound_queue;

        std::string my_name;

        static const quint32 GRIDS_PORT = 1488;
    };
}


