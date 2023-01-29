/*
 * Copyright (C) 2015-2020 "IoT.bzh"
 * Author José Bollo <jose.bollo@iot.bzh>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <json-c/json.h>

#define AFB_BINDING_VERSION 3
#include <afb/afb-binding.h>

//ping
static void ping(afb_req_t req)
{
	static int ping_count = 0;

	afb_req_success_f(req, json_object_new_int(ping_count), "Ping count = 1860 + %d", ping_count);
	AFB_API_NOTICE(afbBindingV3root, "Verbosity macro at level notice invoked at ping invocation count = %d", ping_count);
	ping_count++;
}

static void rewr(afb_req_t req)
{
	//retrieve the radio
	const char *index;
	//Gets from the request 'req' the string value of the argument of 'index'. Returns NULL if there is no argument of 'index'
	index = afb_req_value(req, "index");
	if (index == NULL)
	{
		AFB_API_NOTICE(afbBindingV3root, "No valid index-Argument!");
		afb_req_fail(req, "Kein Index-Argument erhalten", "Error!");
	}
	else
	{
		AFB_API_NOTICE(afbBindingV3root, "ReadWrite binding test, %s", index);
		afb_req_success_f(req, json_object_new_string(index), "Gesendeter Index: %s", index);
	}

}

//connection
static void connection(afb_req_t req)
{
	//AFB_INFO("method 'connection' called!\n");
	AFB_API_NOTICE(afbBindingV3root, "connection called!\n");
	//replies
	afb_req_success(req, NULL, NULL);
}

//pre-init-function
static int bindingPreInit(afb_api_t api)
{
         //if(parse_config() != 0)
                 //AFB_WARNING("Default values are being used!\n"); 
	AFB_INFO("Normally bindingPreInit\n");
        return 0;
}

//init-function
static int bindingInit(afb_api_t api)
{
	AFB_INFO("Normally bindingInit\n");
	/*
	event = afb_daemon_make_event("language");

	if(afb_daemon_require_api("identity", 1))
		return -1;

	if(afb_daemon_require_api("low-can", 1))
		return -1;

	if (afb_api_call_sync(api, "low-can", "auth", NULL, NULL, NULL, NULL))
		return -1;

	return afb_api_call_sync(api, "identity", "subscribe", json_object_new_object(), NULL, NULL, NULL);
	*/
	return 0;
}


/*
static const struct afb_auth _afb_auths_v2_monitor[] = {
	{.type = afb_auth_Permission, .text = "urn:AGL:permission:monitor:public:set"},
	{.type = afb_auth_Permission, .text = "urn:AGL:permission:monitor:public:get"},
	{.type = afb_auth_Or, .first = &_afb_auths_v2_monitor[1], .next = &_afb_auths_v2_monitor[0]}
};
*/

/*
 * array of the verbs exported to afb-daemon
 */
static const afb_verb_t verbs[] = {
   { .verb="connection", .session= AFB_SESSION_NONE,  .callback=connection, .info= "Connection test" },
   { .verb="ping", .session=AFB_SESSION_NONE, .callback=ping, .info="ping test"},
   { .verb="rewr", .session=AFB_SESSION_NONE, .callback=rewr, .info="readwrite test"},
   { .verb=NULL }
};


/*
 * description of the binding for afb-daemon
 */
const afb_binding_t afbBindingV3 = {
	//.api = "motalk",
	.api = "motalk2021",	//Length of api must be correct that buffer is not overwritten
	.info = "API of helloworld(motalk-App)",
	//.api = "helloworld",
	.preinit = bindingPreInit, //callback at load of the binding
	.init = bindingInit, //callback for starting the service
	//.onevent = onEvent //callback for handling events
	.specification = NULL,
	.verbs = verbs,
	.noconcurrency = 1
};


