// Copyright (C) 2014 BMW Group
// Author: Lutz Bichler (lutz.bichler@bmw.de)
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef VSOMEIP_ROUTING_MANAGER_STUB
#define VSOMEIP_ROUTING_MANAGER_STUB

#include <list>
#include <map>
#include <memory>
#include <set>

#include <boost/asio/io_service.hpp>
#include <boost/asio/system_timer.hpp>

#include "../../endpoints/include/endpoint_host.hpp"

namespace vsomeip {

class routing_manager_stub:
		public endpoint_host,
		public std::enable_shared_from_this< routing_manager_stub > {
public:
	routing_manager_stub(routing_manager *_its_routing);
	~routing_manager_stub();

	void init();
	void start();
	void stop();

	routing_manager * get_manager();
	void on_message(const byte_t *_data, const length_t _length, endpoint *_receiver);

private:
	void broadcast(std::vector< byte_t > &_command) const;

	void on_register_application(client_t _client);
	void on_deregister_application(client_t _client);

	void on_offer_service(client_t _client, service_t _service, instance_t _instance);
	void on_stop_offer_service(client_t _client, service_t _service, instance_t _instance);

	void broadcast_routing_info();
	void send_routing_info(client_t _client);

	void broadcast_ping() const;
	void on_pong(client_t _client);
	void start_watchdog();
	void check_watchdog();
	void send_application_lost(std::list< client_t > &_lost);

private:
	boost::asio::io_service &io_;
	boost::asio::system_timer watchdog_timer_;

	routing_manager *routing_;
	std::shared_ptr< endpoint > endpoint_;
	std::map< client_t,
			  std::pair< uint8_t,
			     	     std::map< service_t,
			     	     	 	   std::set< instance_t > > > > routing_info_;
};

} // namespace vsomeip

#endif // VSOMEIP_ROUTING_MANAGER_STUB



