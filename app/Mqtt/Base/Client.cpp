//
// Created by rxwyun on 2022/7/23.
//

#include "Client.h"

Client::Client(string serverAddress, string userName, string password) : serverAddress(serverAddress), userName(userName), password(password) {
    this->client = new async_client(this->serverAddress, this->userName);
    this->connectOptions = connect_options_builder()
            .user_name(userName)
            .password(password)
            .keep_alive_interval(chrono::milliseconds(60))
            .clean_session(false)
            .automatic_reconnect(true)
            .finalize();
    this->clientCallback = new ClientCallback(this->client, this->connectOptions);
    this->client->set_callback(reinterpret_cast<callback &>(this->clientCallback));
}

async_client *Client::getClient() {
    return this->client;
}

__attribute__((unused)) connect_options Client::getClientConnectOptions() {
    return this->connectOptions;
}

ClientCallback *Client::getClientCallback() {
    return this->clientCallback;
}

void Client::connect() {
    this->client->connect(this->connectOptions, nullptr, reinterpret_cast<iaction_listener &>(this->clientCallback));
}

Client::~Client() {
    if (this->clientCallback) {
        delete this->clientCallback;
    }
    if (this->client) {
        this->client->disable_callbacks();
        this->client->is_connected() && this->client->disconnect();
        delete this->client;
    }
}
