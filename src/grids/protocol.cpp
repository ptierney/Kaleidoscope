
#include <iostream>

#include <json/writer.h>
#include <json/reader.h>
#include <json/value.h>

#include <grids/define.h>
#include <grids/protocol.h>

namespace Grids {

  Protocol::Protocol(std::string name, QObject* parent)
    : QObject(parent) {
    my_name = name;
    /* Limit outbound traffic to with this variable. */
    outbound_limit = 50;
    outbound_timer.start();

    sock = new QTcpSocket(this);

    connect(sock, SIGNAL(readyRead()),
            this, SLOT(gridsRead()));
  }

  Protocol::~Protocol() {
    delete sock;
  }

  bool Protocol::connectToNode(const char *address) {
    sock->connectToHost(tr(address), GRIDS_PORT);

    /* Wait for 5 seconds. */
    if (!sock->waitForConnected(5000)){
      printf("Failed to connect to host\n");
      return 0;
    }

    // hooray we are connnected! initialize protocol
    sendProtocolInitiationString();

    return 1;
  }

  void Protocol::sendProtocolInitiationString() {
    /* Make sure the name is in quotes, atm. */
    std::string initString = "++Grids/1.0/JSON/name=\"";
    initString += my_name;
    initString += "\"";
    protocolWrite(initString);
  }



  void Protocol::protocolFlush() {
    /* If enough time has elapsed. */
    /* Pop a string off the queue. */
    /* Send it on its merry way. */

    if(sock->bytesAvailable())
      gridsRead();

    if(!outbound_queue.empty() &&
       outbound_timer.elapsed() > outbound_limit){

      std::string str = outbound_queue.front();
      outbound_queue.pop();
      outbound_timer.start();

      protocolWrite(str);
    }
  }

  void Protocol::endianSwap(unsigned int& x)
  {
    x = (x>>24) |
        ((x<<8) & 0x00FF0000) |
        ((x>>8) & 0x0000FF00) |
        (x<<24);
  }

  quint32 Protocol::byteSwap (quint32 nLongNumber)
  {
    return (((nLongNumber&0x000000FF)<<24)+((nLongNumber&0x0000FF00)<<8)+
            ((nLongNumber&0x00FF0000)>>8)+((nLongNumber&0xFF000000)>>24));
  }

  int Protocol::protocolWrite(std::string &str) {
    /* Different objects may try to write at the same time. */
    //QMutexLocker write_lock(&proto_write_mutex);

    uint32_t len = str.size();

    return protocolWrite(str.c_str(), len);
  }

  int Protocol::protocolWrite(const char *str, quint32 len) {
    if (!sockConnected()) {
      std::cerr << "No socket exists but Protocol::protocolWrite was called\n";
      return -1;
    }

    unsigned int outstr_len = len + 4;
    char *outstr = (char *)malloc(outstr_len);

    // Add the length of the string to the beginning of the string

    // If we're on LittleEndian, convert to BigEndian (Network byte order)
    quint32 net_len = len;
    if( QSysInfo::ByteOrder == QSysInfo::LittleEndian)
      endianSwap(net_len);
    memcpy(outstr, &net_len, 4);
    memcpy((outstr + 4), str, len);

    unsigned int ret = sock->write(outstr, outstr_len);
    free(outstr);

    if (ret != outstr_len) {
      printf("Error in sending\n");
      // It may be good to disconnect sock because it is likely invalid now.
    }

    return ret;
  }

  void Protocol::closeConnection() {
    sock->close();
  }

  // fast(?) version of turning a Grids::Value into a string
  std::string Protocol::stringifyValue(Value *val) {
    Json::FastWriter *writer = new Json::FastWriter();
    return writer->write(*val);
  }

  void Protocol::sendRequest(std::string event, int broadcast) {
    sendRequest(event, NULL, broadcast);
  }

  void Protocol::sendRequest(std::string event, Value *args, int broadcast) {

    /* Debugging loopback */

    if (event.empty())
      return;

    bool created_value = 0;

    if (args == NULL) {
      args = new Value();

      created_value = 1;
    }

    const static std::string methodkey = "_method";
    (*args)[methodkey] = event;

    const static std::string broadcastkey = "_broadcast";
    (*args)[broadcastkey] = broadcast;

    std::string value_string = stringifyValue(args);

    //std::cerr << "OUTGOING" << std::endl;
    //std::cerr << value_string << std::endl;

    //pushOutboundRequest(value_string);
    protocolWrite(value_string);

    if (created_value)
      delete args;
  }


  void Protocol::sendRequestLoopback( std::string evt, Value* args) {
    if (evt.empty())
      return;

    bool createdVal = 0;

    if (args == NULL) {
      args = new Value();

      createdVal = 1;
    }

    const static std::string methodkey = "_method";
    (*args)[methodkey] = evt;

    std::string valueStr = stringifyValue(args);

    handleMessage(valueStr);
  }

  /*
	  Json::Value Protocol::mapToJsonValue(gridsmap_t *m) {
	  giterator mapIterator;

	  Json::Value jsonVal;

	  for(mapIterator = m->begin();
	  mapIterator != m->end();
	  mapIterator++) {

	  gridskey_t key = mapIterator->first;
	  gridsval_t val = mapIterator->second;

	  jsonVal[key] = val;
	  }

	  return jsonVal;
	  }
	*/

  void Protocol::gridsRead() {
    /*std::cerr << "gridsRead\n";*/
    //int socketReady;
    qint64 bytesRead;
    quint32 incomingLength;
    char *buf;
    char *bufIncoming;

    // Wait for more data if we don't have the length
    if(sock->bytesAvailable() < 4)
      return;

    bytesRead = sock->read((char*)(&incomingLength), 4);

    if( QSysInfo::ByteOrder == QSysInfo::LittleEndian)
      endianSwap(incomingLength);

    if (bytesRead <= 0) {
      std::cerr << "Socket read error\n";
      return;
    }

    if (bytesRead != 4) {
      // socket broken most likely
      std::cerr << "failed to read from socket\n";
      // break;
      return;
    }

    if (incomingLength > 1024 * 1024 * 1024) {
      // not going to read in more than a gig, f that
      std::cerr << "Got incoming message size: " << incomingLength << ". Skipping\n";
      return;
    }

    // allocate space for incoming message + null byte
    buf = (char *)malloc(incomingLength + 1);

    quint32 bytesRemaining = incomingLength;
    bufIncoming = buf;

    do {
      bytesRead = sock->read(bufIncoming, bytesRemaining);

      if (bytesRead > 0) {
        bytesRemaining -= bytesRead;
        bufIncoming += bytesRead;
      }

    } while ((bytesRead > 0) && bytesRemaining);
    buf[incomingLength] = '\0';

    if (bytesRead == -1) {
      // o snap read error
      std::cerr << "Socket read error: " << bytesRead << "\n";
      free(buf);
      return;
    }

    if (bytesRead == 0) {
      // not chill
      std::cerr << "Didn't read any data when expecting message of " << incomingLength << " bytes\n";
      free(buf);
      return;
    }

    if (bytesRead != incomingLength) {
      std::cerr << "Only read " << bytesRead << " bytes when expecting message of "
          << incomingLength << " bytes\n";
      free(buf);
      return;
    }

    std::string msg = buf;

    //std::cerr << "INCOMING" << std::endl;
    //std::cerr << msg << std::endl;

    handleMessage(msg);

    free(buf);
  }

  void Protocol::handleMessage(std::string &msg) {
    if (msg.size() < 2) return; // obv. bogus

    /*std::cerr << "received: " << msg << "\n";*/
    /* I most objects in Qt are not thread safe,
           so all messages must go through device in a queue
        emit rawData( tr(msg.c_str()) );
        */

    if (msg.find("==", 0, 2) == 0) {
      // protocol initiation message
      std::cerr << "Grids session initiated\n";
      emit protocolInitiated(NULL);
    } else if (msg.find("--", 0, 2) == 0) {
      // encrypted protocol message
    } else {
      // assume this is json for now
      Value root = parseJson(msg);

      // FIXME: this is slow and lame
      //gridsmap_t rootMap = jsonToMap(root);

      Event *evt = new Event(root["_method"].asString(), root);

      /*
      if( last_event){
        delete last_event;
        last_event = new Event(*evt);
      } else {
        last_event = new Event(*evt);
      }
      */

      //std::cout << evt->getStyledString() << std::endl;

      pushEvent(evt);

      /* These events will be deleted by the interface, after
               it parses them in another thread. */

      //std::cout << "handleMessage deleting evt" << std::endl;
      //delete evt;
      //std::cout << "handleMessage deleted evt" << std::endl;
    }
  }

  /*
	  Value Protocol::jsonToMap(Value &root) {
	  // ghetto, should fix in the future
	  Json::Value::Members memberList = root.getMemberNames();
	  std::vector<std::string>::iterator iter;

	  Vak outMap;

	  for (iter = memberList.begin(); iter != memberList.end(); iter++) {
	  Json::Value val = root[*iter];
	  outMap[*iter] = val;
	  }

	  return outMap;
	  }
	*/

  Value Protocol::parseJson(std::string &msg) {
    Grids::Value root;
    Json::Reader reader;

    if (reader.parse(msg, root))
      return root;

    std::cerr << "Could not parse JSON: " << msg << "\n";
    return Value();
  }

  bool Protocol::sockConnected(){
    return (sock->state() == QAbstractSocket::ConnectedState);
  }

  /* Lock the queue mutex, copy the event queue,
       delete the original queue, return the copy, unlock. */
  EventQueue& Protocol::getEvents() {
    return event_queue;
  }

  void Protocol::pushEvent(Event *new_event) {
    event_queue.push(new_event);
  }

  void Protocol::pushOutboundRequest(std::string str){
    outbound_queue.push(str);
  }

  int Protocol::getReceivedQueueLength() {
    return event_queue.size();
  }

  int Protocol::getOutboundQueueLength() {
    return outbound_queue.size();
  }





} // end namespace Proto

