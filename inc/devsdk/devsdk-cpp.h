/*
 * Copyright (c) 2021
 * Eaton Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef _DEVSDK_CPP_H_
#define _DEVSDK_CPP_H_ 1
#ifdef __cplusplus
/* This whole file only counts if C++, since it has C++11 elements. */

/**
 * @file
 * @brief This file defines basic C++ types used by SDK functions
 */

#include <string>
#include <map>
#include <set>
#include <vector>
#include <memory>
#include "iot/data.h"
#include "iot/typecode.h"

// Declare classes reference below
class EdgexResourceValue;
class DeviceProfile;

/** Declare a type for a resource map, keyed by the resource name. */
typedef std::map<std::string, EdgexResourceValue> resource_map_t;

/** The structure of data that describes one Device.
 * It is populated with data from core-metadata.
 * Not every field is retained from the full form it has in the Device Service SDK,
 * since the needs are generally simpler for this Application Service SDK. */
typedef struct appsdk_device_cpp_s
{
  /** The unique device name */
  std::string name;
  
  /** The unique device UUID or other protocol ID. */
  std::string id;
  
  /** The name of the EdgeX Device Profile for this device, from its device service configuration. */
  std::string profile_name;
  
  /** The name of the device service that provides this device. ToDo: Is this needed for App SDK? */
  std::string service_name;

  /** The device's model name, from its device profile. */
  std::string model_name;

  /** The device name of the parent device, if any. Commonly is the publisher's name. */
  std::string parent_name;
  
  /** The administrative state of this device - ie, enabled state. 
   * True => edgex_device_adminstate UNLOCKED = normal operation. */
  bool is_admin_state_unlocked;

  /** The current operating state of the device. 
   * True => edgex_device_operatingstate UP = running and communicating. */
  bool is_operating_state_up;

  /** Flag for the one device which is the publisher device (ie, main device). */
  bool is_publisher;

  /** The map of resource values (ie, channel values) for this device. */
  resource_map_t resource_map;

} appsdk_device_cpp_t;

/** Declare a type for a map of system devices, where they key is the device name,
 * and the value is the device's metadata. */
typedef std::map<std::string, std::shared_ptr<appsdk_device_cpp_t>> device_map_t;

/** Declare a type for the set of expected devices, using the device names. */
typedef std::set<std::string> expected_devices_t;

/**
 * @brief Get the given table's configuration items.
 * @param svc The application service's structure.
 * @param table_name The name of the desired table in the configuration.toml file.
 * @param cfg_map The map of configuration items, with entries added from those found under that table.
 *         This map will be keyed by the items' names, less the table name prefix.
 * @return The size of cfg_map (number of entries).
 */

size_t appsdk_get_config_map (devsdk_service_t *svc, const char *table_name, resource_map_t& cfg_map);

/**
 * @brief Get a reference to the set of expected devices from the service structure.
 *  This set, if not empty, can be used to filter the devices retrieved from core-metadata.
 * 
 * @param svc The service's structure; will extract the set from svc->app_svc_ext,
 *             which is an opaque pointer in the device SDK.
 * @return A reference to the set of expected devices for this service.
 *      The calling function might add to or alter it.
 */
expected_devices_t& get_svc_expected_devices (struct devsdk_service_t* svc);

/**
 * @brief Divide the given string into an array of tokens, separating at the given delimiter(s).
 * Will not return zero-length strings, however.
 * Since argument string_in is a const char*, its contents are not altered by this function (unlike strtok()).
 * 
 * @param string_in The char string to be divided up.
 * @param delimiters Set of one or more characters that will delimit the tokens in the string.
 * @return An array of the tokens found in the string.
 */
std::vector<std::string> tokenize_string (const char* string_in, const char* delimiters);

/**
 * @brief Divide the given topic path into an array of tokens, one for each segment of the path.
 * Will divide at the '/' characters and push each into a vector of strings.
 * 
 * @param topic_path The topic path to be divided up, eg edgex/events/device/Bacon-Cape/Virtual-Bacon-Cape-01/mACIA
 * @return An array of the tokens found in the topic_path
 */
std::vector<std::string> tokenize_topic_path (const char* topic_path);

/**
 * @brief Get the device ptr from the service's device map, given its name.
 * 
 * @param svc The service's structure
 * @param device_name The name of the device to be looked up
 * @return Pointer to the device's data structure, if found; if not found, the pointer will be empty. 
 */
std::shared_ptr<appsdk_device_cpp_t> get_device_ptr_from_name (devsdk_service_t* svc, std::string device_name);

/**
 * @brief Update the value of a Device Resource in the device's map from the Message Bus event payload.
 * This is updating the cached value to the latest value, decoded from the payload contents.
 * If the Device Resource's value is not found in the device's map, it will be added now.
 * The payload contents look like:
 * {"ReceivedTopic":"","CorrelationID":"","Payload":"Big-Base64-String","ContentType":"application/json"}<garbage-at-end>
 * 
 * @param svc The service's structure
 * @param dev_ptr Pointer to the device's data structure
 * @param resource_name The name of the Device Resource to be looked up
 * @param event_payload The text of the Message Bus update event, with the interesting parts Base64 encoded.
 * @return true if no problems were encountered parsing the payload and updating the value.
 * @return false if there was a problem or the value was not updated (eg, it may not have changed value).
 */
bool update_resource_value_from_event (devsdk_service_t* svc, std::shared_ptr<appsdk_device_cpp_t> dev_ptr,
                                       const std::string& resource_name, const char* event_payload);

// Define the labels of interest for the Message Bus Events
#define KEY_PAYLOAD "Payload"
#define KEY_CONTENT_TYPE "ContentType"
#define CONTENT_TYPE_JSON "application/json"
#define KEY_EVENT "event"
#define KEY_READINGS "readings"
#define KEY_DEVICE_NAME "deviceName"
#define KEY_RESOURCE_NAME "resourceName"
#define KEY_VALUE_TYPE "valueType"
#define KEYNAME_VALUE "value"
#define KEYNAME_ORIGIN "origin"

#endif  // __cpluspluss
#endif  // _DEVSDK_CPP_H_
