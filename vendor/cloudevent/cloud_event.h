// Copyright 2020 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef FUNCTIONS_FRAMEWORK_CPP_GOOGLE_CLOUD_FUNCTIONS_CLOUD_EVENT_H
#define FUNCTIONS_FRAMEWORK_CPP_GOOGLE_CLOUD_FUNCTIONS_CLOUD_EVENT_H

#include <chrono>
#include <optional>
#include <string>

using namespace std;

/**
 * Represents a Cloud Event.
 *
 * Functions to handle Cloud Events receive an object of this class.
 *
 * @see The [Cloud Events Primer][cloud-events-primer] for a general
 *     introduction to Cloud Events.
 *
 * [cloud-events-primer]: https://github.com/cloudevents/spec
 */
namespace google_function {
    class CloudEvent {
    public:
        inline static auto constexpr kDefaultSpecVersion = "1.0";
        using ClockType = chrono::system_clock;
        using time_point = ClockType::time_point;

        CloudEvent(string id, string source, string type,
                   string spec_version = kDefaultSpecVersion)
                   : id_(move(id)),
                   source_(move(source)),
                   type_(move(type)),
                   spec_version_(move(spec_version)) {}

                   string id() const {
            return id_;
        }
        string source() const {
            return source_;
        }
        string type() const {
            return type_;
        }
        string spec_version() const {
            return spec_version_;
        }
        string data_content_type() const {
            return data_content_type_;
        }
        string data_schema() const {
            return data_schema_;
        }
        string subject() const {
            return subject_;
        }
        string time() const {
            return time_;
        }
        string data() const& {
            return data_;
        }

        void set_data_content_type(string v) {
            data_content_type_ = v;
        }
        void set_data_schema(string v) {
            data_schema_ = v;
        }
        void set_subject(string v) {
            subject_ = v;
        }
        void set_time(string tp) {
            time_ = tp;
        }
        void set_data(string v) {
            data_ = v;
        }

    private:
        string const id_;
        string const source_;
        string const type_;
        string const spec_version_;

        string data_content_type_ = "";
        string data_schema_ = "";
        string subject_ = "";
        string time_ = "";
        string data_ = "";
    };
}

#endif  // FUNCTIONS_FRAMEWORK_CPP_GOOGLE_CLOUD_FUNCTIONS_CLOUD_EVENT_H
