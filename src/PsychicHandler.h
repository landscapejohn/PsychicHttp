#ifndef PsychicHandler_h
#define PsychicHandler_h

#include "PsychicCore.h"
#include "PsychicRequest.h"
#include "PsychicMiddleware.h"

class PsychicEndpoint;
class PsychicHttpServer;
class PsychicMiddleware;

/*
 * HANDLER :: Can be attached to any endpoint or as a generic request handler.
 */

class PsychicHandler
{
    friend PsychicEndpoint;

  protected:
    std::list<PsychicMiddleware*> _middleware;
    PsychicHttpServer* _server;

    String _username;
    String _password;
    HTTPAuthMethod _method;
    String _realm;
    String _authFailMsg;

    String _subprotocol;

    std::list<PsychicClient*> _clients;

  public:
    PsychicHandler();
    virtual ~PsychicHandler();

    PsychicHandler* setAuthentication(const char* username, const char* password, HTTPAuthMethod method = BASIC_AUTH, const char* realm = "", const char* authFailMsg = "");
    bool needsAuthentication(PsychicRequest* request);
    esp_err_t authenticate(PsychicRequest* request);

    virtual bool isWebSocket() { return false; };

    void setSubprotocol(const String& subprotocol);
    const char* getSubprotocol() const;

    PsychicClient* checkForNewClient(PsychicClient* client);
    void checkForClosedClient(PsychicClient* client);

    virtual void addClient(PsychicClient* client);
    virtual void removeClient(PsychicClient* client);
    virtual PsychicClient* getClient(int socket);
    virtual PsychicClient* getClient(PsychicClient* client);
    virtual void openCallback(PsychicClient* client) {};
    virtual void closeCallback(PsychicClient* client) {};

    bool hasClient(PsychicClient* client);
    int count() { return _clients.size(); };
    const std::list<PsychicClient*>& getClientList();

    // derived classes must implement these functions
    virtual bool canHandle(PsychicRequest* request) { return true; };
//    virtual esp_err_t handleRequest(PsychicRequest* request);
    virtual esp_err_t handleRequest(PsychicRequest* request, PsychicResponse* response) = 0;

    //bool filter(PsychicRequest* request);
    PsychicHandler* setFilter(PsychicRequestFilterFunction fn);
    PsychicHandler* addMiddleware(PsychicMiddleware *middleware);
    PsychicHandler* addMiddleware(PsychicMiddlewareFunction fn);
    bool runMiddleware(PsychicRequest* request, PsychicResponse* response);
};

#endif