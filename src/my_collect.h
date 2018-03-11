#ifndef MY_COLLECT_H
#define MY_COLLECT_H

#include <stdbool.h>
#include "contiki.h"
#include "core/net/linkaddr.h"
#include "net/netstack.h"
#include "net/rime/rime.h"

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

/* Connection object */
struct my_collect_conn {
  struct broadcast_conn bc;
  struct unicast_conn uc;
  const struct my_collect_callbacks *callbacks;
  linkaddr_t parent;
  struct ctimer beacon_timer;
  uint16_t metric;
  uint16_t beacon_seqn;
};

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


/* Routing table functions ------------------------------------------------------------*/

/**
 * Update the routing table by adding a new <parent, child> pair
 * or replacing it if parent already has a child entry.
 *
 */
void update_routing_table(const linkaddr_t *parent, const linkaddr_t *child);

/**
 * Search the child of a node and return it.
 * Return NULL if node has not a child or entries with
 * the declared node do not exist.
 *
 */
linkaddr_t* get_child_routing_table(const linkaddr_t *parent);

/**
 * Find a routing path to send a "command" packet (from sink to a destination node).
 *
 * Return the number of nodes in the path if path is found or
 *   0   if there are not enough information in routing table to build a path
 *   255 if a loop is detected while building path
 */
uint8_t find_route_path(const linkaddr_t *dest);



#endif  // MY_COLLECT_H
