//
// Created by rxwyun on 2022/7/23.
//

#include "Client.h"
#include <nlohmann/json.hpp>

IOT::CLIENT::MQTT::Client::Client(const string& serverAddress, const string& userName, const string& password) : serverAddress(serverAddress), userName(userName), password(password) {
    this->client = new async_client(this->serverAddress, this->userName);
    this->connectOptions = connect_options_builder()
            .user_name(userName)
            .password(password)
            .keep_alive_interval(chrono::milliseconds(60))
            .clean_session(false)
            .automatic_reconnect(true)
            .finalize();
    this->clientCallback = new ClientCallback(this->client, this->connectOptions);
    this->client->set_callback(*this->clientCallback);
}

async_client *IOT::CLIENT::MQTT::Client::getClient() {
    return this->client;
}

connect_options IOT::CLIENT::MQTT::Client::getClientConnectOptions() {
    return this->connectOptions;
}

IOT::CLIENT::MQTT::ClientCallback *IOT::CLIENT::MQTT::Client::getClientCallback() {
    return this->clientCallback;
}

bool IOT::CLIENT::MQTT::Client::connect() {
    this->getClientCallback()->onConnected = [this](async_client *client, const string& cause) {
        if (this->onConnected) {
            this->onConnected(this, cause);
        }
    };
    this->getClientCallback()->onMessage = [this](async_client *client, const_message_ptr msg) {
        if (this->onMessage) {
            nlohmann::json jsonObj;
            jsonObj["topic"] = msg->get_topic();
            jsonObj["payload"] = msg->get_payload_str();
            this->onMessage(this, to_string(jsonObj));
        }
        if (this->onSubscribe) {
            this->onSubscribe(this, msg->get_topic(), msg->get_payload_str());
        }
    };

    token_ptr res = this->client->connect(this->connectOptions, nullptr,*this->clientCallback);
    res->wait_for(1000 * 100);

    return true;
}

bool IOT::CLIENT::MQTT::Client::subscribe(string topic) {
    this->getClient()->subscribe(topic, 2);
    return true;
}

bool IOT::CLIENT::MQTT::Client::publishMsg(string topic, string content) {
    const_message_ptr msg;

    this->getClient()->publish(msg->create(topic, content));

    return true;
}

IOT::CLIENT::MQTT::Client::~Client() {
    delete this->clientCallback;
    if (this->client) {
        this->client->disable_callbacks();
        this->client->is_connected() && this->client->disconnect();
        delete this->client;
    }
}
