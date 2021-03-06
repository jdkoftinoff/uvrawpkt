#ifndef UV_RAWPKT_LINUX_H
#define UV_RAWPKT_LINUX_H

/*
  Copyright (c) 2015, J.D. Koftinoff Software, Ltd.
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

   3. Neither the name of J.D. Koftinoff Software, Ltd. nor the names of its
      contributors may be used to endorse or promote products derived from
      this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
*/

#include "uv-rawpkt-common.h"

#if UV_RAWPKT_ENABLE_PCAP==0 && defined(__linux__)

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief The uv_rawpkt_s struct
 *
 * Manages the send/receive packets on a single ethernet port
 */
struct uv_rawpkt_s
{
    uv_poll_t handle;
    void *data;
    int fd;
    uint16_t ethertype;
    uv_loop_t *loop;
    uv_rawpkt_link_status_cb link_status_cb;
    uv_rawpkt_recv_cb recv_cb;
    uv_close_cb close_cb;
    struct uv_rawpkt_network_port_s *owner_network_port;
    struct uv_rawpkt_s *prev;
    struct uv_rawpkt_s *next;
};

/**
 * @brief The uv_rawpkt_send_s struct
 *
 * Maintains the context for a rawpkt send request
 */
struct uv_rawpkt_send_s
{
    uv_req_t req;
    void *data;
    uv_rawpkt_t* rawpkt;
    uv_rawpkt_send_cb cb;
};

/**
 * @brief The uv_rawpkt_iter_network_port_s struct
 *
 * Maintains the context of a single discovered network port
 */
struct uv_rawpkt_network_port_s
{
    uv_timer_t link_status_timer;
    int link_status;
    struct uv_rawpkt_network_port_s *next;
    struct uv_rawpkt_network_port_s *prev;
    int seen;
    char *device_name;
    char *device_description;
    int interface_id;
    uint8_t mac[6];
    struct uv_rawpkt_s *first_rawpkt;
    struct uv_rawpkt_s *last_rawpkt;
    struct uv_rawpkt_network_port_iterator_s *owner;
};

/**
 * @brief The uv_rawpkt_network_port_iterator_s struct
 *
 * Manages the discovery and removal events of an ethernet network port
 */
struct uv_rawpkt_network_port_iterator_s
{
    uv_timer_t scan_timer;
    void *data;
    uv_loop_t *loop;
    uv_rawpkt_network_port_iterator_cb added_cb;
    uv_rawpkt_network_port_iterator_cb removed_cb;

    struct uv_rawpkt_network_port_s *first;
    struct uv_rawpkt_network_port_s *last;
};


/**
 * @brief uv_rawpkt_open
 * @param rawpkt
 * @param network_port
 * @param snaplen
 * @param promiscuous
 * @param to_ms
 * @param ethertype
 * @param close_cb
 * @return
 */
UVRAWPKT_EXTERN int uv_rawpkt_open(uv_rawpkt_t* rawpkt,
                                   struct uv_rawpkt_network_port_s *network_port,
                                   int snaplen,
                                   int promiscuous,
                                   int to_ms,
                                   uint16_t *ethertype,
                                   uv_close_cb close_cb );

/**
 * @brief uv_rawpkt_closed
 * @param handle
 */
UVRAWPKT_EXTERN
void uv_rawpkt_closed( uv_handle_t *handle );

/**
 * @brief uv_rawpkt_close
 * @param rawpkt
 * @param close_cb
 */
UVRAWPKT_EXTERN
void uv_rawpkt_close(uv_rawpkt_t* rawpkt);

/**
 * @brief uv__rawpkt_network_port_link_status_timer
 * @param handle
 */
void uv__rawpkt_network_port_link_status_timer(uv_timer_t* handle);

/**
 * @brief uv__rawpkt_readable
 * @param handle
 * @param status
 * @param events
 */
void uv__rawpkt_readable(uv_poll_t* handle, int status, int events);


/**
 * @brief uv__rawpkt_iter_timer
 * @param handle
 */
void uv__rawpkt_iter_timer(uv_timer_t* handle);

#ifdef __cplusplus
}
#endif

#endif
#endif

