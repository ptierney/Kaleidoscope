
#pragma once

#include <map>
#include <string>
#include <queue>
#include <json/value.h>
#include <grids/event.h>
#include <grids/define.h>
#include <QMutex>
#include <QSize>
#include <QThread>
#include <QWaitCondition>
#include <QTcpSocket>

namespace Grids {

    class Protocol : public QThread {
        Q_OBJECT

    public:
        Protocol(std::string name, QObject *parent = 0);
        ~Protocol();

        bool connectToNode(const char *address);
        void sendProtocolInitiationString();
        int protocolWrite(std::string &str);
        int protocolWrite(const char *str, uint32_t len);
        void closeConnection();

        std::string stringifyValue(Value *val);
        void sendRequest(std::string);
        void sendRequest(std::string, Value *args);
        void sendRequestLoopback(std::string, Value *args);

        void handleMessage(std::string &msg);
	
        Value parseJson(std::string &msg);
        bool sockConnected();

        int runEventLoopThreaded();
        void stopEventLoopThread();

        bool getEventLoopRunning();
        void setEventLoopRunning(bool);

        // last_event is mainly used for testing
        Event* last_event;

        /* Lock the queue mutex, copy the event queue,
           delete the original queue, return the copy, unlock. */
        EventQueue getEvents();

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
        QMutex proto_write_mutex;
        bool running;
	
        void endianSwap(unsigned int&);
        quint32 byteSwap(quint32);

        /* Add an event to the event queue, locking the mutex as necessary. */
        void pushEvent(Event*);

        EventQueue event_queue;

        std::string my_name;

        static const quint32 GRIDS_PORT = 1488;
    };
}


