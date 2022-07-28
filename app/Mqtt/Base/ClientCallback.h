//
// Created by rxwyun on 2022/7/24.
//

#ifndef ARM_FACE_IDENTIFY_CLIENTCALLBACK_H
#define ARM_FACE_IDENTIFY_CLIENTCALLBACK_H

#include <functional>
#include <utility>
#include "mqtt/include/mqtt/async_client.h"

using namespace mqtt;

using OnConnected = std::function<void (async_client *client, const string& cause)>; //可以这样写，更直观
using OnMessage = std::function<void (async_client *client, const_message_ptr msg)>; //可以这样写，更直观

class ClientCallback : public virtual callback,
                 public virtual iaction_listener
{
protected:
    int reConnectRetry{};
    int maxConnectRetry = 5;
    async_client *client;
    connect_options connectOptions;

public:

    OnConnected onConnected;
    OnMessage onMessage;

public:
    ClientCallback(async_client *client, connect_options connectOptions, int maxConnectRetry = 5) : client(client), connectOptions(std::move(connectOptions)), maxConnectRetry(maxConnectRetry) {}
    ~ClientCallback() {}

    // This deomonstrates manually reconnecting to the broker by calling
    // connect() again. This is a possibility for an application that keeps
    // a copy of it's original connect_options, or if the app wants to
    // reconnect with different options.
    // Another way this can be done manually, if using the same options, is
    // to just call the async_client::reconnect() method.
    void reconnect() {
        if (!this->connectOptions.get_automatic_reconnect()) {
            throw "mqtt client lost";
        }

        std::this_thread::sleep_for(this->connectOptions.get_keep_alive_interval());
        this->client->connect(this->connectOptions, nullptr, *this);
    }

    // Re-connection failure
    void on_failure(const token& tok) override {
        if (!this->connectOptions.get_automatic_reconnect()) {
            return;
        }

        if (++this->reConnectRetry > this->maxConnectRetry) {
            throw "mqtt retry too many times";
        }

        this->reconnect();
    }

    // (Re)connection success
    // Either this or connected() can be used for callbacks.
    void on_success(const token& tok) override {

    }

    // (Re)connection success
    void connected(const string& cause) override {
        if (this->onConnected != nullptr) {
            this->onConnected(this->client, cause);
        }
    }

    // Callback for when the connection is lost.
    // This will initiate the attempt to manually reconnect.
    void connection_lost(const string& cause) override {
        this->reConnectRetry = 0;
        this->reconnect();
    }

    // Callback for when a message arrives.
    void message_arrived(mqtt::const_message_ptr msg) override {
        if (this->onMessage != nullptr) {
            this->onMessage(this->client, msg);
        }
    }

    void delivery_complete(mqtt::delivery_token_ptr token) override {}
};

#endif //ARM_FACE_IDENTIFY_CLIENTCALLBACK_H
