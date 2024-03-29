#ifndef MY_COLLECT_H
#define MY_COLLECT_H

#include <stdbool.h>
#include "contiki.h"
#include "core/net/linkaddr.h"
#include "net/netstack.h"
#include "net/rime/rime.h"

/* Connection object */
struct my_collect_conn {
  struct broadcast_conn bc;
  struct unicast_conn uc;
  const struct my_collect_callbacks *callbacks;
  linkaddr_t parent;
  struct ctimer beacon_timer;
  uint16_t metric;
  uint16_t beacon_seqn;
  int16_t parent_rssi;
};


/* Callback structure */
struct my_collect_callbacks {
  void (*recv)(const linkaddr_t *originator, uint8_t hops);

  /* Source routing recv function callback:
   *
   * This function must be part of the callbacks structure of
   * the my_collect_conn connection . It should be called when a
   * source routing packet reaches its destination.
   *
   * Params:
   *   c    : pointer to the collection connection structure
   *   hops : number of route hops from the sink to the destination
   */
  void (*sr_recv)(struct my_collect_conn *c, uint8_t hops);
};


struct collect_header { // Header structure for data packets
  linkaddr_t source;
  uint8_t hops;

  // True if the packet is a "command" packet sent from sink to another node (one-to-many) (it is a source routed packet).
  bool is_command;
  // Size of the array of node ids allocated after this header struct that represent the path
  // used by a packet to arrive to the sink or to a node.
  uint8_t path_length;
} __attribute__((packed));


/* Initialize a collect connection
 *  - conn -- a pointer to a connection object
 *  - channels -- starting channel C (the collect uses two: C and C+1)
 *  - is_sink -- initialize in either sink or router mode
 *  - callbacks -- a pointer to the callback structure */
void my_collect_open(struct my_collect_conn *conn, uint16_t channels,
                     bool is_sink,
                     const struct my_collect_callbacks *callbacks);

/* Send packet to the sink */
int my_collect_send(struct my_collect_conn *c);


/**
 * - Update current nose's parent,
 * - Broadcast beacon to other nodes and
 * - Send a dedicated topology report to sink
 *
 */
void update_node_parent(struct my_collect_conn *conn, uint16_t beacon_metric, const linkaddr_t *sender, int16_t parent_rssi);


/**
 * Callback triggered after a certain period of time to send dedicated topology reports to sink.
 */
void send_topology_report_cb(void* ptr);

/* Source routing send function:
 *
 * Params:
 *   c    : pointer to the collection connection structure
 *   dest : pointer to the destination address
 *
 * Returns:
 *   non - zero if the packet could be sent , zero otherwise.
 */
int sr_send(struct my_collect_conn *c, const linkaddr_t *dest);



/**
 * Handle the reception of a data collection packet.
 * If node is sink -> deliver packet to app
 * If node is a common node -> forward packet to parent
 *
 */

void handle_recv_data_collection_packet_sink(struct my_collect_conn *conn, struct collect_header *hdr, const linkaddr_t *from);
void handle_recv_data_collection_packet_node(struct my_collect_conn *conn, struct collect_header *hdr, const linkaddr_t *from);


/**
 * Handle the reception of a command packet.
 *
 */
void handle_recv_command_packet(struct my_collect_conn *conn, struct collect_header *hdr, const linkaddr_t *from);

void initialize_sink(struct my_collect_conn* conn);

#endif  // MY_COLLECT_H
