/*! @mainpage Broccoli: The Bro Client Communications Library
 * See broccoli.h for the main API reference.
 */

/*
       B R O C C O L I  --  The Bro Client Communications Library

Copyright (C) 2004-2007 Christian Kreibich <christian (at) icir.org>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to
deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies of the Software and its documentation and acknowledgment shall be
given in the documentation and software packages that this Software was
used.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/
#ifndef broccoli_h
#define broccoli_h

#include <inttypes.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#ifdef __MINGW32__
#include <winsock.h>
#else
#include <netinet/in.h>
#endif
#include <openssl/crypto.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * bro_debug_calltrace - Debugging output switch for call tracing.
 *
 * If you have debugging support built in (i.e., your package was configured
 * with --enable-debug), you can enable/disable debugging output for
 * call tracing by setting this to 0 (off) or 1 (on). Default is off.
 */
extern int bro_debug_calltrace;

/**
 * bro_debug_messages - Output switch for debugging messages.
 *
 * If you have debugging support built in (i.e., your package was configured
 * with --enable-debug), you can enable/disable debugging messages
 * by setting this to 0 (off) or 1 (on). Default is off.
 */
extern int bro_debug_messages;

#ifndef	FALSE
#define	FALSE	(0)
#endif

#ifndef	TRUE
#define	TRUE	(!FALSE)
#endif

/* Numeric values of Bro type identifiers, corresponding
 * to the values of the TypeTag enum in Bro's Type.h. Use
 * these values with bro_event_add_val(), bro_record_add_val(),
 * bro_record_get_nth_val() and bro_record_get_named_val().
 */
/**
 *
 * BRO_TYPE_UNKNOWN is not used in the data exchange,
 * see bro_record_get_{nth,named}_val() for its use.
 */
#define BRO_TYPE_UNKNOWN           0
#define BRO_TYPE_BOOL              1
#define BRO_TYPE_INT               2
#define BRO_TYPE_COUNT             3
#define BRO_TYPE_COUNTER           4
#define BRO_TYPE_DOUBLE            5
#define BRO_TYPE_TIME              6
#define BRO_TYPE_INTERVAL          7
#define BRO_TYPE_STRING            8
#define BRO_TYPE_PATTERN           9
#define BRO_TYPE_ENUM             10
#define BRO_TYPE_TIMER            11
#define BRO_TYPE_PORT             12
#define BRO_TYPE_IPADDR           13
#define BRO_TYPE_SUBNET           14
#define BRO_TYPE_ANY              15
#define BRO_TYPE_TABLE            16
#define BRO_TYPE_UNION            17
#define BRO_TYPE_RECORD           18
#define BRO_TYPE_LIST             19
#define BRO_TYPE_FUNC             20
#define BRO_TYPE_FILE             21
#define BRO_TYPE_VECTOR           22
#define BRO_TYPE_ERROR            23
#define BRO_TYPE_PACKET           24 /* CAUTION -- not defined in Bro! */
#define BRO_TYPE_SET              25 /* CAUTION -- not defined in Bro! */
#define BRO_TYPE_MAX              26

/* Flags for new connections, to pass to bro_conn_new()
 * and bro_conn_new_str(). See manual for details.
 */
#define BRO_CFLAG_NONE                      0
#define BRO_CFLAG_RECONNECT           (1 << 0) /**< Attempt transparent reconnects */
#define BRO_CFLAG_ALWAYS_QUEUE        (1 << 1) /**< Queue events sent while disconnected */
#define BRO_CFLAG_SHAREABLE           (1 << 2) /**< DO NOT USE -- no longer supported */
#define BRO_CFLAG_DONTCACHE           (1 << 3) /**< Ask peer not to use I/O cache (default) */
#define BRO_CFLAG_YIELD               (1 << 4) /**< Process just one event at a time */
#define BRO_CFLAG_CACHE               (1 << 5) /**< Ask peer to use I/O cache */


/* ---------------------------- Typedefs ----------------------------- */


typedef uint64_t uint64;
typedef uint32_t uint32;
typedef uint16_t uint16;
typedef uint8_t uint8;
typedef unsigned char  uchar;

typedef struct bro_conn BroConn;
typedef struct bro_event BroEvent;
typedef struct bro_buf BroBuf;
typedef struct bro_record BroRecord;
typedef struct bro_table BroTable;
typedef struct bro_table BroSet;
typedef struct bro_vector BroVector;
typedef struct bro_ev_meta BroEvMeta;
typedef struct bro_packet BroPacket;

/* ----------------------- Callback Signatures ----------------------- */

/**
 * BroEventFunc - The signature of expanded event callbacks.
 * @param bc Bro connection handle.
 * @param user_data user data provided to bro_event_registry_add().
 * @param ... varargs.
 *
 * This is the signature of callbacks for handling received
 * Bro events, called in the argument-expanded style.  For details
 * see bro_event_registry_add().
 */
typedef void (*BroEventFunc) (BroConn *bc, void *user_data, ...);

/**
 * BroCompactEventFunc - The signature of compact event callbacks.
 * @param bc Bro connection handle.
 * @param user_data user data provided to bro_event_registry_add_compact().
 * @param meta metadata for the event.
 *
 * This is the signature of callbacks for handling received
 * Bro events, called in the compact-argument style. For details
 * see bro_event_registry_add_compact().
 */
typedef void (*BroCompactEventFunc) (BroConn *bc, void *user_data, BroEvMeta *meta);

typedef void (*BroPacketFunc) (BroConn *bc, void *user_data,
			       const BroPacket *packet);

/**
 * OpenSSL_lockfunc - locking function for OpenSSL thread safeness.
 * @param mode acquire nth lock if (mode & CRYPTO_LOCK) is true, release otherwise.
 * @param n lock index. You need to support at least CRYPTO_num_locks().
 * @param file file from which OpenSSL invokes the callback.
 * @param line line in file from which OpenSSL invokes the callback.
 *
 * If you are using Broccoli in a multithreaded environment, you need
 * to use bro_init() with a BroCtx structure and use it to point at an
 * implementation of this callback. Refer to pages 74ff in O'Reilly's
 * OpenSSL book (by Viega et al.) for details. You could also look at 
 * 
 *   http://www.openssl.org/support/faq.html#PROG1
 *   http://www.openssl.org/docs/crypto/threads.html
 *
 * but you will only curse OpenSSL even more than you already do after
 * reading those.
 */
typedef void (*OpenSSL_lock_func) (int mode, int n, const char *file, int line);

/**
 * OpenSSL_thread_id_func - thread ID function for OpenSSL thread safeness.
 * @param id target pointer into which the current thread's numeric ID must be written.
 * 
 * Please refer to pages 74ff in O'Reilly's OpenSSL book, and also see
 * the comments for OpenSSL_lockfunc().
 */
typedef unsigned long (*OpenSSL_thread_id_func) (void);


/**
 * OpenSSL_dynlock_create_func - allocator for dynamic locks, for OpenSSL thread safeness.
 * @param file file from which OpenSSL invokes the callback.
 * @param line line in file from which OpenSSL invokes the callback.
 * 
 * Please refer to pages 74ff in O'Reilly's OpenSSL book, and also see
 * the comments for OpenSSL_lockfunc().
 */
typedef struct CRYPTO_dynlock_value* (*OpenSSL_dynlock_create_func) (const char *file, int line);

/**
 * OpenSSL_dynlock_lock_func - lock/unlock dynamic locks, for OpenSSL thread safeness.
 * @param mode acquire nth lock if (mode & CRYPTO_LOCK) is true, release otherwise.
 * @param mutex lock to lock/unlock.
 * @param file file from which OpenSSL invokes the callback.
 * @param line line in file from which OpenSSL invokes the callback.
 * 
 * Please refer to pages 74ff in O'Reilly's OpenSSL book, and also see
 * the comments for OpenSSL_lockfunc().
 */
typedef void (*OpenSSL_dynlock_lock_func) (int mode, struct CRYPTO_dynlock_value *mutex,
					   const char *file, int line);

/**
 * OpenSSL_dynlock_free_func - dynamic lock deallocator, for OpenSSL thread safeness.
 * @param mutex lock to deallocate.
 * @param file file from which OpenSSL invokes the callback.
 * @param line line in file from which OpenSSL invokes the callback.
 * 
 * Please refer to pages 74ff in O'Reilly's OpenSSL book, and also see
 * the comments for OpenSSL_lockfunc().
 */
typedef void (*OpenSSL_dynlock_free_func) (struct CRYPTO_dynlock_value *mutex,
					   const char *file, int line);


/* ---------------------------- Structures --------------------------- */


/** Initialization context for the Broccoli library. */
typedef struct bro_ctx {
  OpenSSL_lock_func lock_func;
  OpenSSL_thread_id_func id_func;
  OpenSSL_dynlock_create_func dl_create_func;
  OpenSSL_dynlock_lock_func dl_lock_func;
  OpenSSL_dynlock_free_func dl_free_func;
} BroCtx;

/** Statistical properties of a given connection. */
typedef struct bro_conn_stats {
  int tx_buflen; ///< Number of bytes to process in output buffer.
  int rx_buflen; ///< Number of bytes to process in input buffer.
} BroConnStats;

/** BroStrings are used to access string parameters in received events.
 */
typedef struct bro_string {
  uint32       str_len;
  uchar       *str_val;
} BroString;

/** Ports in Broccoli do not only consist of a number but also indicate
 * whether they are TCP or UDP.
 */
typedef struct bro_port {
  uint64       port_num;   /**< Port number in host byte order */
  int          port_proto; /**< IPPROTO_xxx */
} BroPort;

/** IP addresses are 16-bytes in network byte order.
 * IPv4 addresses use the standard IPv4-in-IPv6 mapping: 10 bytes off, 2 bytes
 * on, then 4 bytes of the address.
 */
typedef struct bro_addr
{
  uint32       addr[4];    /**< IP address in network byte order */
} BroAddr;

/** Subnets are a 16-byte address with a prefix width in bits.
 */
typedef struct bro_subnet
{
  BroAddr      sn_net;     /**< IP address in network byte order */
  uint32       sn_width;   /**< Length of prefix to consider. */
} BroSubnet;

/** Encapsulation of arguments passed to an event callback,
 * for the compact style of argument passing.
 */
typedef struct bro_ev_arg
{
  void        *arg_data;   /**< Pointer to the actual event argument */
  int          arg_type;   /**< A BRO_TYPE_xxx constant */
} BroEvArg;

/** Metadata for an event, passed to callbacks of the BroCompactEventFunc
 * prototype.
 */
struct bro_ev_meta
{
  const char  *ev_name;   /**< The name of the event */
  double       ev_ts;     /**< Timestamp of event, taken from BroEvent itself */
  int          ev_numargs;/**< How many arguments are passed */
  BroEvArg    *ev_args;   /**< Array of BroEvArgs, one for each argument */
  const uchar *ev_start;  /**< Start pointer to serialized version of currently processed event */
  const uchar *ev_end;    /**< End pointer to serialized version of currently processed event */
};

#define BRO_PCAP_SUPPORT
#ifdef BRO_PCAP_SUPPORT
#include <pcap.h>

/** Broccoli can send and receive pcap-captured packets, wrapped into
 * the following structure:
 */
struct bro_packet
{
  double       pkt_time;
  uint32       pkt_hdr_size;
  uint32       pkt_link_type;
  
  struct pcap_pkthdr  pkt_pcap_hdr;
  const u_char       *pkt_data;
  const char         *pkt_tag;

};

#endif

/* ============================ API ================================== */

/* -------------------------- Initialization ------------------------- */

/**
 * bro_init - Initializes the library.
 * @param ctx pointer to a BroCtx structure.
 *
 * The function initializes the library. It MUST be called before
 * anything else in Broccoli. Specific initialization context may be
 * provided using a BroCtx structure pointed to by ctx. It may be
 * omitted by passing %NULL, for default values. See bro_init_ctx() for
 * initialization of the context structure to default values.
 *
 * @returns %TRUE if initialization succeeded, %FALSE otherwise.
 */
int            bro_init(const BroCtx *ctx);


/**
 * bro_ctx_init - Initializes initialization context to default values.
 * @param ctx pointer to a BroCtx structure.
 */
void           bro_ctx_init(BroCtx *ctx);


/* ----------------------- Connection Handling ----------------------- */

/**
 * bro_conn_new - Creates and returns a handle for a connection to a remote Bro.
 * @param ip_addr 4-byte IP address of Bro to contact, in network byte order.
 * @param port port of machine at @p ip_addr to contact, in network byte order.
 * @param flags an or-combination of the %BRO_CONN_xxx flags.
 *
 * The function creates a new Bro connection handle for communication with
 * Bro through a network. Depending on the flags passed in, the connection
 * and its setup process can be adjusted. If you don't want to pass any
 * flags, use %BRO_CFLAG_NONE.
 *
 * @returns pointer to a newly allocated and initialized
 * Bro connection structure. You need this structure for all
 * other calls in order to identify the connection to Bro.
 */
BroConn       *bro_conn_new(struct in_addr *ip_addr, uint16 port, int flags);

/**
 * bro_conn_new6 - Creates and returns a handle for a connection to a remote Bro.
 * @param ip_addr 16-byte IPv6 address of Bro to contact, in network byte order.
 * @param port port of machine at @p ip_addr to contact, in network byte order.
 * @param flags an or-combination of the %BRO_CONN_xxx flags.
 *
 * The function creates a new Bro connection handle for communication with
 * Bro through a network. Depending on the flags passed in, the connection
 * and its setup process can be adjusted. If you don't want to pass any
 * flags, use %BRO_CFLAG_NONE.
 *
 * @returns pointer to a newly allocated and initialized
 * Bro connection structure. You need this structure for all
 * other calls in order to identify the connection to Bro.
 */
BroConn       *bro_conn_new6(struct in6_addr *ip_addr, uint16 port, int flags);

/**
 * bro_conn_new_str - Same as bro_conn_new(), but accepts strings for hostname and port.
 * @param hostname string describing the host and port to connect to.
 * @param flags an or-combination of the %BRO_CONN_xxx flags.
 *
 * The function is identical to bro_conn_new(), but allows you to specify the
 * host and port to connect to in a string as "<hostname>:<port>". @p flags can
 * be used to adjust the connection features and the setup process. If you don't
 * want to pass any flags, use %BRO_CFLAG_NONE.
 *
 * @returns pointer to a newly allocated and initialized
 * Bro connection structure. You need this structure for all
 * other calls in order to identify the connection to Bro.
 */
BroConn       *bro_conn_new_str(const char *hostname, int flags);

/**
 * bro_conn_new_socket - Same as bro_conn_new(), but uses existing socket.
 * @param socket open socket.
 * @param flags an or-combination of the %BRO_CONN_xxx flags.
 *
 * The function is identical to bro_conn_new(), but allows you to pass in an
 * open socket to use for the communication. @p flags can be used to
 * adjust the connection features and the setup process. If you don't want to
 * pass any flags, use %BRO_CFLAG_NONE.
 *
 * @returns pointer to a newly allocated and initialized
 * Bro connection structure. You need this structure for all
 * other calls in order to identify the connection to Bro.
 */
BroConn       *bro_conn_new_socket(int socket, int flags);

/**
 * bro_conn_set_class - Sets a connection's class identifier.
 * @param bc connection handle.
 * @param classname class identifier.
 *
 * Broccoli connections can indicate that they belong to a certain class
 * of connections, which is needed primarily if multiple Bro/Broccoli
 * instances are running on the same node and connect to a single remote
 * peer. You can set this class with this function, and you have to do so
 * before calling bro_connect() since the connection class is determined
 * upon connection establishment. You remain responsible for the memory
 * pointed to by @p classname.
 */
void           bro_conn_set_class(BroConn *bc, const char *classname);

/**
 * bro_conn_get_peer_class - Reports connection class indicated by peer.	
 * @param bc connection handle.
 *
 * @returns a string containing the connection class indicated by the peer,
 * if any, otherwise %NULL.
 */
const char    *bro_conn_get_peer_class(const BroConn *bc);


/**
 * bro_conn_get_connstats - Reports connection properties.
 * @param bc connection handle.
 * @param cs BroConnStats handle.
 * 
 * The function fills the BroConnStats structure provided via @p cs with
 * information about the given connection.
 */
void           bro_conn_get_connstats(const BroConn *bc, BroConnStats *cs);


/**
 * bro_conn_connect - Establish connection to peer.
 * @param bc connection handle.
 *
 * The function attempts to set up and configure a connection to
 * the peer configured when the connection handle was obtained.
 *
 * @returns %TRUE on success, %FALSE on failure.
 */
int            bro_conn_connect(BroConn *bc);


/**
 * bro_conn_reconnect - Drop the current connection and reconnect, reusing all settings.
 * @param bc Bro connection handle.
 *
 * The functions drops the current connection identified by @p bc and attempts
 * to establish a new one with all the settings associated with @p bc,
 * including full handshake completion.
 *
 * @returns %TRUE if successful, %FALSE otherwise. No matter what the outcome,
 * you can continue to use @p bc as normal (e.g. you have to release it using
 * bro_conn_delete()).
 */
int            bro_conn_reconnect(BroConn *bc);


/**
 * bro_conn_delete - terminates and releases connection.
 * @param bc Bro connection handle.
 *
 * This function will terminate the given connection if necessary
 * and release all resources associated with the connection handle.
 * 
 *
 * @returns %FALSE on error, %TRUE otherwise.
 */
int            bro_conn_delete(BroConn *bc);


/**
 * bro_conn_alive - Reports whether a connection is currently alive or has died.
 * @param bc Bro connection handle.
 
 * This predicate reports whether the connection handle is currently
 * usable for sending/receiving data or not, e.g. because the peer
 * died. The function does not actively check and update the
 * connection's state, it only reports the value of flags indicating
 * its status. In particular, this means that when calling
 * bro_conn_alive() directly after a select() on the connection's
 * descriptor, bro_conn_alive() may return an incorrent value. It will
 * however return the correct value after a subsequent call to
 * bro_conn_process_input(). Also note that the connection is also
 * dead after the connection handle is obtained and before
 * bro_conn_connect() is called.
 * 
 * @returns %TRUE if the connection is alive, %FALSE otherwise.
 */
int            bro_conn_alive(const BroConn *bc);


/**
 * bro_conn_adopt_events - Makes one connection send out the same events as another.
 * @param src Bro connection handle for connection whose event list to adopt.
 * @param dst Bro connection handle for connection whose event list to change.
 *
 * The function makes the connection identified by @p dst use the same event
 * mask as the one identified by @p src.
 */
void           bro_conn_adopt_events(BroConn *src, BroConn *dst);


/**
 * bro_conn_get_fd - Returns file descriptor of a Bro connection.
 * @param bc Bro connection handle.
 *
 * If you need to know the file descriptor of the connection
 * (such as when select()ing it, etc.), use this accessor function.
 *
 * @returns file descriptor for connection @p bc, or negative value
 * on error.
 */
int            bro_conn_get_fd(BroConn *bc);


/**
 * bro_conn_process_input - Processes input sent to the sensor by Bro.
 * @param bc Bro connection handle.
 *
 * The function reads all input sent to the local sensor by the
 * Bro peering at the connection identified by @p bc. It is up
 * to you to find a spot in the application you're instrumenting
 * to make sure this is called. This function cannot block.
 * bro_conn_alive() will report the actual state of the connection
 * after a call to bro_conn_process_input().
 *
 * @returns %TRUE if any input was processed, %FALSE otherwise.
 */
int            bro_conn_process_input(BroConn *bc);


/* ---------------------- Connection data storage -------------------- */

/* Connection handles come with a faciity to store and retrieve
 * arbitrary data items. Use the following functions to store,
 * query, and remove items from a connection handle.
 */

/**
 * bro_conn_data_set - Puts a data item into the registry.
 * @param bc Bro connection handle.
 * @param key name of the data item.
 * @param val data item.
 *
 * The function stores @p val under name @p key in the connection handle @p bc.
 * @p key is copied internally so you do not need to duplicate it before
 * passing.
 */
void           bro_conn_data_set(BroConn *bc, const char *key, void *val);


/**
 * bro_conn_data_get - Looks up a data item.
 * @param bc Bro connection handle.
 * @param key name of the data item.
 *
 * The function tries to look up the data item with name @p key and
 * if found, returns it.
 * 
 * @returns data item if lookup was successful, %NULL otherwise.
 */
void          *bro_conn_data_get(BroConn *bc, const char *key);


/**
 * bro_conn_data_del - Removes a data item.
 * @param bc Bro connection handle.
 * @param key name of the data item.
 *
 * The function tries to remove the data item with name @p key.
 * 
 * @returns the removed data item if it exists, %NULL otherwise.
 */
void          *bro_conn_data_del(BroConn *bc, const char *key);


/* ----------------------------- Bro Events -------------------------- */

/**
 * bro_event_new - Creates a new empty event with a given name.
 * @param event_name name of the Bro event.
 *
 * The function creates a new empty event with the given
 * name and returns it.
 *
 * @returns new event, or %NULL if allocation failed.
 */
BroEvent      *bro_event_new(const char *event_name);


/**
 * bro_event_free - Releases all memory associated with an event.
 * @param be event to release.
 *
 * The function releases all memory associated with @p be. Note 
 * that you do NOT have to call this after sending an event.
 */
void           bro_event_free(BroEvent *be);


/**
 * bro_event_add_val - Adds a parameter to an event.
 * @param be event to add to.
 * @param type numerical type identifier (a %BRO_TYPE_xxx constant).
 * @param type_name optional name of specialized type.
 * @param val value to add to event.
 *
 * The function adds the given @p val to the argument list of
 * event @p be. The type of @p val is derived from @p type, and may be
 * specialized to the type named @p type_name. If @p type_name is not
 * desired, use %NULL.
 *
 * @p val remains the caller's responsibility and is copied internally.
 *
 * @returns %TRUE if the operation was successful, %FALSE otherwise.
 */
int            bro_event_add_val(BroEvent *be, int type,
				 const char *type_name,const void *val);


/**
 * bro_event_set_val - Replace a value in an event.
 * @param be event handle.
 * @param val_num number of the value to replace, starting at 0.
 * @param type numerical type identifier (a %BRO_TYPE_xxx constant).
 * @param type_name optional name of specialized type.
 * @param val value to put in.
 *
 * The function replaces whatever value is currently stored in the
 * event pointed to by @p be with the value specified through the @p type and
 * @p val arguments. If the event does not currently hold enough
 * values to replace one in position @p val_num, the function does
 * nothing. If you want to indicate a type specialized from @p type,
 * use @p type_name to give its name, otherwise pass %NULL for @p type_name.
 *
 * @returns %TRUE if successful, %FALSE on error.
 */
int            bro_event_set_val(BroEvent *be, int val_num,
				 int type, const char *type_name,
				 const void *val);

/**
 * bro_event_send - Tries to send an event to a Bro agent.
 * @param bc Bro connection handle.
 * @param be event to send.
 *
 * The function tries to send @p be to the Bro agent connected
 * through @p bc. Regardless of the outcome, you do NOT have
 * to release the event afterwards using bro_event_free().
 * 
 * @returns %TRUE if the event got sent or queued for later transmission,
 * %FALSE on error. There are no automatic repeated send attempts
 * (to minimize the effect on the code that Broccoli is linked to).
 * If you have to make sure that everything got sent, you have
 * to try to empty the queue using bro_event_queue_flush(), and
 * also look at bro_event_queue_empty().
 */
int            bro_event_send(BroConn *bc, BroEvent *be);
	

/**
 * bro_event_send_raw - Enqueues a serialized event directly into a connection's send buffer.
 * @param bc Bro connection handle.
 * @param data pointer to serialized event data.
 * @param data_len length of buffer pointed to by @p data.
 *
 * The function enqueues the given event data into @p bc's transmit buffer.
 * @p data_len bytes at @p data must correspond to a single event.
 *
 * @returns %TRUE if successful, %FALSE on error.
 */
int            bro_event_send_raw(BroConn *bc, const uchar *data, int data_len);


/**
 * bro_event_queue_length - Returns current queue length.
 * @param bc Bro connection handle.
 *
 * Use this function to find out how many events are currently queued
 * on the client side.
 *
 * @returns number of items currently queued.
 */
int            bro_event_queue_length(BroConn *bc);


/**
 * bro_event_queue_length_max - Returns maximum queue length.
 * @param bc Bro connection handle.
 *
 * Use this function to find out how many events can be queued before
 * events start to get dropped.
 *
 * @returns maximum possible queue size.
 */
int            bro_event_queue_length_max(BroConn *bc);


/**
 * bro_event_queue_flush - Tries to flush the send queue of a connection.
 * @param bc Bro connection handle.
 *
 * The function tries to send as many queued events to the Bro
 * agent as possible.
 *
 * @returns remaining queue length after flush.
 */
int            bro_event_queue_flush(BroConn *bc);


/* ------------------------ Bro Event Callbacks ---------------------- */

/**
 * bro_event_registry_add - Adds an expanded-argument event callback to the event registry.
 * @param bc Bro connection handle.
 * @param event_name Name of events that trigger callback.
 * @param func callback to invoke.
 * @param user_data user data passed through to the callback.
 *
 * This function registers the callback @p func to be invoked when events
 * of name @p event_name arrive on connection @p bc. @p user_data is passed
 * along to the callback, which will receive it as the second parameter. You
 * need to ensure that the memory @p user_data points to is valid during the
 * time the callback might be invoked.
 *
 * Note that this function only registers the callback in the state
 * associated with @p bc. If you use bro_event_registry_add() and @p bc
 * has not yet been connected via bro_conn_connect(), then no further
 * action is required. bro_conn_connect() requests any registered event types.
 * If however you are requesting additional event types after the connection has
 * been established, then you also need to call bro_event_registry_request()
 * in order to signal to the peering Bro that you want to receive those events.
 */
void           bro_event_registry_add(BroConn *bc,
				      const char *event_name,
				      BroEventFunc func,
				      void *user_data);

/**
 * bro_event_registry_add_compact - Adds a compact-argument event callback to the event registry.
 * @param bc Bro connection handle.
 * @param event_name Name of events that trigger callback.
 * @param func callback to invoke.
 * @param user_data user data passed through to the callback.
 *
 * This function registers the callback @p func to be invoked when events
 * of name @p event_name arrive on connection @p bc. @p user_data is passed
 * along to the callback, which will receive it as the second parameter. You
 * need to ensure that the memory @p user_data points to is valid during the
 * time the callback might be invoked. See bro_event_registry_add() for
 * details.
 */
void           bro_event_registry_add_compact(BroConn *bc,
					      const char *event_name,
					      BroCompactEventFunc func,
					      void *user_data);

/**
 * bro_event_registry_remove - Removes an event handler.
 * @param bc Bro connection handle.
 * @param event_name event to ignore from now on.
 *
 * The function removes all callbacks for event @p event_name from the
 * event registry for connection @p bc.
 */
void           bro_event_registry_remove(BroConn *bc, const char *event_name);

/**
 * bro_event_registry_request - Notifies peering Bro to send events.
 * @param bc Bro connection handle.
 *
 * The function requests the events you have previously requested using
 * bro_event_registry_add() from the Bro listening on @p bc.
 */
void           bro_event_registry_request(BroConn *bc);



/* ------------------------ Dynamic-size Buffers --------------------- */

/**
 * bro_buf_new - Creates a new buffer object.
 *
 * @returns a new buffer object, or %NULL on error. Use paired with
 * bro_buf_free().
 */
BroBuf        *bro_buf_new(void);

/**
 * bro_buf_free - Releases a dynamically allocated buffer object.
 * @param buf buffer pointer.
 *
 * The function releases all memory held by the buffer pointed
 * to by @p buf. Use paired with bro_buf_new().
 */
void           bro_buf_free(BroBuf *buf);

/**
 * bro_buf_append - appends data to the end of the buffer.
 * @param buf buffer pointer.
 * @param data new data to append to buffer.
 * @param data_len size of @p data.
 *
 * The function appends data to the end of the buffer,
 * enlarging it if necessary to hold the @p len new bytes.
 * NOTE: it does not modify the buffer pointer. It only
 * appends new data where buf_off is currently pointing
 * and updates it accordingly. If you DO want the buffer
 * pointer to be updated, have a look at bro_buf_ptr_write()
 * instead.
 *
 * @returns %TRUE if successful, %FALSE otherwise.
 */
int            bro_buf_append(BroBuf *buf, void *data, int data_len);


/**
 * bro_buf_consume - shrinks the buffer.
 * @param buf buffer pointer.
 *
 * The function removes the buffer contents between the start
 * of the buffer and the point where the buffer pointer
 * currently points to. The idea is that you call bro_buf_ptr_read()
 * a few times to extract data from the buffer, and then
 * call bro_buf_consume() to signal to the buffer that the
 * extracted data are no longer needed inside the buffer.
 */
void           bro_buf_consume(BroBuf *buf);


/**
 * bro_buf_reset - resets the buffer.
 * @param buf buffer pointer.
 *
 * The function resets the buffer pointers to the beginning of the
 * currently allocated buffer, i.e., it marks the buffer as empty.
 */
void           bro_buf_reset(BroBuf *buf);


/**
 * bro_buf_get - Returns pointer to actual start of buffer.
 * @param buf buffer pointer.
 *
 * @returns the entire buffer's contents.
 */
uchar         *bro_buf_get(BroBuf *buf);


/**
 * bro_buf_get_end - Returns pointer to the end of the buffer.
 * @param buf buffer pointer.
 *
 * @returns a pointer to the first byte in the
 * buffer that is not currently used.
 */ 
uchar         *bro_buf_get_end(BroBuf *buf);


/**
 * bro_buf_get_size - Returns number of bytes allocated for buffer.
 * @param buf buffer pointer.
 *
 * @returns the number of actual bytes allocated for the
 * buffer.
 */
uint           bro_buf_get_size(BroBuf *buf);


/**
 * bro_buf_get_used_size - Returns number of bytes currently used.
 * @param buf buffer pointer.
 *
 * @returns number of bytes currently used.
 */
uint           bro_buf_get_used_size(BroBuf *buf);


/**
 * bro_buf_ptr_get - Returns current buffer content pointer.
 * @param buf buffer pointer.
 *
 * @returns current buffer content pointer.
 */
uchar         *bro_buf_ptr_get(BroBuf *buf);


/**
 * bro_buf_ptr_tell - Returns current offset of buffer content pointer.
 * @param buf buffer pointer.
 *
 * @returns current offset of buffer content pointer.
 */
uint32         bro_buf_ptr_tell(BroBuf *buf);


/**
 * bro_buf_ptr_seek - Adjusts buffer content pointer.
 * @param buf buffer pointer.
 * @param offset number of bytes by which to adjust pointer, positive or negative.
 * @param whence location relative to which to adjust.
 *
 * The function adjusts the position of @p buf's content
 * pointer. Call semantics are identical to fseek(), thus
 * use @p offset to indicate the offset by which to jump and
 * use %SEEK_SET, %SEEK_CUR, or %SEEK_END to specify the
 * position relative to which to adjust.
 *
 * @returns %TRUE if adjustment could be made, %FALSE
 * if not (e.g. because the offset requested is not within
 * legal bounds).
 */
int            bro_buf_ptr_seek(BroBuf *buf, int offset, int whence);


/** 
 * bro_buf_ptr_check - Checks whether a number of bytes can be read.
 * @param buf buffer pointer.
 * @param size number of bytes to check for availability.
 *
 * The function checks whether @p size bytes could be read from the
 * buffer using bro_buf_ptr_read().
 *
 * @returns %TRUE if @p size bytes can be read, %FALSE if not.
 */
int            bro_buf_ptr_check(BroBuf *buf, int size);


/**
 * bro_buf_ptr_read - Extracts a number of bytes from buffer.
 * @param buf buffer pointer.
 * @param data destination area.
 * @param size number of bytes to copy into @p data.
 *
 * The function copies @p size bytes into @p data if the buffer
 * has @p size bytes available from the current location of
 * the buffer content pointer onward, incrementing the content
 * pointer accordingly. If not, the function doesn't do anything.
 * It behaves thus different from the normal read() in that
 * it either copies the amount requested or nothing.
 * 
 * @returns %TRUE if @p size bytes were copied, %FALSE if not.
 */
int            bro_buf_ptr_read(BroBuf *buf, void *data, int size);

/**
 * bro_buf_ptr_write - Writes a number of bytes into buffer.
 * @param buf buffer pointer.
 * @param data data to write.
 * @param size number of bytes to copy into @p data.
 *
 * The function writes @p size bytes of the area pointed to by @p data
 * into the buffer @p buf at the current location of its content pointer,
 * adjusting the content pointer accordingly. If the buffer doesn't have
 * enough space to receive @p size bytes, more space is allocated.
 *
 * @returns %TRUE if @p size bytes were copied, %FALSE if an error
 * occurred and the bytes could not be copied.
 */
int            bro_buf_ptr_write(BroBuf *buf, void *data, int size);


/* ------------------------ Configuration Access --------------------- */

/**
 * bro_conf_set_domain - Sets the current domain to use in a config file.
 * @param domain name of the domain, or %NULL.
 *
 * Broccoli's config files are divided into sections. At the beginning of
 * each config file you can have an unnamed section that will be used by
 * default. Case is irrelevant. By passing %NULL for @p domain, you select
 * the default domain, otherwise the one that matches @p domain. @p domain is
 * copied internally.
 */
void           bro_conf_set_domain(const char *domain);


/**
 * bro_conf_get_int - Retrieves an integer from the configuration.
 * @param val_name key name for the value.
 * @param val result pointer for the value.
 *
 * The function tries to find an integer item named @p val_name in the
 * configuration. If it is found, its value is placed into the
 * int pointed to by @p val.
 *
 * @returns %TRUE if @p val_name was found, %FALSE otherwise.
 */
int            bro_conf_get_int(const char *val_name, int *val);


/**
 * bro_conf_get_dbl - Retrieves a double float from the configuration.
 * @param val_name key name for the value.
 * @param val result pointer for the value.
 *
 * The function tries to find a double float item named @p val_name 
 * in the configuration. If it is found, its value is placed into the
 * double pointed to by @p val.
 *
 * @returns %TRUE if @p val_name was found, %FALSE otherwise.
 */
int            bro_conf_get_dbl(const char *val_name, double *val);


/**
 * bro_conf_get_str - Retrieves an integer from the configuration.
 * @param val_name key name for the value.
 *
 * The function tries to find a string item named @p val_name in the
 * configuration.
 *
 * @returns the config item if @p val_name was found, %NULL otherwise.
 * A returned string is stored internally and not to be modified. If
 * you need to keep it around, strdup() it.
 */
const char    *bro_conf_get_str(const char *val_name);



/* ------------------------------ Strings ---------------------------- */

/**
 * bro_string_init - Initializes an existing string structure.
 * @param bs string pointer.
 *
 * The function initializes the BroString pointed to by @p bs. Use this
 * function before using the members of a BroString you're using on the
 * stack.
 */
void           bro_string_init(BroString *bs);

/**
 * bro_string_set - Sets a BroString's contents.
 * @param bs string pointer.
 * @param s C ASCII string.
 *
 * The function initializes the BroString pointed to by @p bs to the string
 * given in @p s. @p s's content is copied, so you can modify or free @p s
 * after calling this, and you need to call bro_string_cleanup() on the
 * BroString pointed to by @p bs.
 *
 * @returns %TRUE if successful, %FALSE otherwise.
 */
int            bro_string_set(BroString *bs, const char *s);

/**
 * bro_string_set_data - Sets a BroString's contents.
 * @param bs string pointer.
 * @param data arbitrary data.
 * @param data_len length of @p data.
 *
 * The function initializes the BroString pointed to by @p bs to @p data_len
 * bytes starting at @p data. @p data's content is copied, so you can modify
 * or free @p data after calling this.
 *
 * @returns %TRUE if successful, %FALSE otherwise.
 */
int            bro_string_set_data(BroString *bs, const uchar *data, int data_len);

/**
 * bro_string_get_data - Returns pointer to the string data.
 * @param bs string pointer.
 *
 * The function returns a pointer to the string's internal data. You
 * can copy out the string using this function in combination with
 * bro_string_get_length(), for obtaining the string's length.
 *
 * @returns pointer to string, or %NULL on error.
 */
const uchar   *bro_string_get_data(const BroString *bs);

/**
 * bro_string_get_length - Returns string's length.
 * @param bs string pointer.
 *
 * @returns the string's length.
 */
uint32         bro_string_get_length(const BroString *bs);
  
/**
 * bro_string_copy - Duplicates a BroString.
 * @param bs string pointer.
 *
 * @returns a deep copy of the BroString pointed to by @p bs, or %NULL on
 * error.
 */
BroString     *bro_string_copy(BroString *bs);

/**
 * bro_string_assign - Duplicates a BroString's content, assigning it to an existing one.
 * @param src source string.
 * @param dst target string.
 *
 * Copies the string content pointed to by @p src into the existing
 * BroString pointed to by @p dst. bro_string_cleanup() is called on
 * @p dst before the assignment.
 */
void           bro_string_assign(BroString *src, BroString *dst);

/**
 * bro_string_cleanup - Cleans up existing BroString.
 * @param bs string pointer.
 *
 * This function releases all contents claimed by the BroString pointed
 * to by @p bs, without releasing that BroString structure itself. Use
 * this when manipulating a BroString on the stack, paired with
 * bro_string_init().
 */
void           bro_string_cleanup(BroString *bs);

/**
 * bro_string_free - Cleans up dynamically allocated BroString.
 * @param bs string pointer.
 * 
 * This function releases the entire BroString pointed to by @p bs, including
 * the BroString structure itself.
 */
void           bro_string_free(BroString *bs);


/* -------------------------- Record Handling ------------------------ */

/**
 * bro_record_new - Creates a new record.
 *
 * The function allocates and initializes a new empty record. BroRecords
 * are used for adding and retrieving record values to/from events. You
 * do not have to specify a record type separately when you create a
 * record. The type is defined implicitly by the sequence of types formed
 * by the sequence of values added to the record, along with the names for
 * each value. See the manual for details.
 *
 * @returns a new record, or %NULL on error.
 */
BroRecord     *bro_record_new(void);

/**
 * bro_record_free - Releases a record.
 * @param rec record handle.
 *
 * The function releases all memory consumed by the record pointed to
 * by @p rec.
 */
void           bro_record_free(BroRecord *rec);
 
/**
 * bro_record_get_length - Returns number of fields in record.
 * @param rec record handle.
 *
 * @returns the number of fields in the record.
 */
int            bro_record_get_length(BroRecord *rec);

/**
 * bro_record_add_val - Adds a value to a record.
 * @param rec record handle.
 * @param name field name of the added val.
 * @param type numerical type tag of the new val.
 * @param type_name optional name of specialized type.
 * @param val pointer to the new val.
 *
 * The function adds a new field to the record pointed to by @p rec and
 * assigns the value passed in to that field. The field name is given
 * in @p name, the type of the value is given in @p type and must be one of
 * the %BRO_TYPE_xxx constants defined in broccoli.h. The type you give
 * implies what data type @p val must be pointing to; see the manual for
 * details. If you want to indicate a type specialized from @p type,
 * use @p type_name to give its name, otherwise pass %NULL for @p type_name.
 * It is possible to leave fields unassigned, in that case, pass in
 * %NULL for @p val.
 *
 * @p val remains the caller's responsibility and is copied internally.
 *
 * @returns %TRUE on success, %FALSE on error.
 */
int            bro_record_add_val(BroRecord *rec, const char *name,
				  int type, const char *type_name,
				  const void *val);

/**
 * bro_record_get_nth_val - Retrieves a value from a record by field index.
 * @param rec record handle.
 * @param num field index, starting from 0.
 * @param type value-result argument for the expected/actual type of the value.
 *
 * The function returns the @p num'th value of the record pointed to
 * by @p rec, expected to be of @p type. The returned value is internal
 * and needs to be duplicated if you want to keep it around. Upon
 * return, the int pointed to by @p type tells you the type of the returned
 * value, as a BRO_TYPE_xxx type tag. If the int pointed to upon calling
 * the function has the value BRO_TYPE_UNKNOWN, no type checking is
 * performed and the value is returned. If it is any other type tag,
 * its value is compared to that of the value, and if they match, the
 * value is returned. Otherwise, the return value is %NULL. If you don't
 * care about type enforcement and don't want to know the value's type,
 * you may pass %NULL for @p type.
 *
 * @returns pointer to queried value on success, %NULL on error.
 */
void*          bro_record_get_nth_val(BroRecord *rec, int num, int *type);


/**
 * bro_record_get_nth_name - Retrieves a name from a record by field index.
 * @param rec record handle.
 * @param num field index, starting from 0.
 *
 * The function returns the @p num'th name of the record pointed to by @p rec. 
 *
 * @returns field name on success, %NULL on error.
 */
const char*    bro_record_get_nth_name(BroRecord *rec, int num);


/**
 * bro_record_get_named_val - Retrieves a value from a record by field name.
 * @param rec record handle.
 * @param name field name.
 * @param type value-result argument for the expected/actual type of the value.
 *
 * The function returns the value of the field named @p name in the
 * record pointed to by @p rec. The returned value is internal and needs
 * to be duplicated if you want to keep it around. @p type works as with
 * bro_record_get_nth_val(), see there for more details.
 *
 * @returns pointer to queried value on success, %NULL on error.
 */
void*          bro_record_get_named_val(BroRecord *rec, const char *name, int *type);


/**
 * bro_record_set_nth_val - Replaces a value in a record, identified by field index.
 * @param rec record handle.
 * @param num field index, starting from 0.
 * @param type expected type of the value.
 * @param type_name optional name of specialized type.
 * @param val pointer to new val.
 *
 * The function replaces the @p num'th value of the record pointed to
 * by @p rec, expected to be of @p type. All values are copied internally
 * so what @p val points to stays unmodified. The value of @p type implies
 * what @p result must be pointing to. See the manual for details.
 * If you want to indicate a type specialized from @p type, use
 * @p type_name to give its name, otherwise pass %NULL for @p type_name.
 *
 * @returns %TRUE on success, %FALSE on error.
 */
int            bro_record_set_nth_val(BroRecord *rec, int num,
				      int type, const char *type_name,
				      const void *val);

/**
 * bro_record_set_named_val - Replaces a value in a record, identified by name.
 * @param rec record handle.
 * @param name field name.
 * @param type expected type of the value.
 * @param type_name optional name of specialized type.
 * @param val pointer to new val.
 *
 * The function replaces the value named @p name in the record pointed to
 * by @p rec, expected to be of @p type. All values are copied internally
 * so what @p val points to stays unmodified. The value of @p type implies
 * what @p result must be pointing to. See the manual for details.
 * If you want to indicate a type specialized from @p type,
 * use @p type_name to give its name, otherwise pass %NULL for @p type_name.
 *
 * @returns %TRUE on success, %FALSE on error.
 */
int            bro_record_set_named_val(BroRecord *rec, const char *name,
					int type, const char *type_name,
					const void *val);


/* -------------------------- Tables & Sets -------------------------- */

/**
 * BroTableCallback - The signature of callbacks for iterating over tables.
 * @param key a pointer to the key of a key-value pair.
 * @param val a pointer to @p key's corresponding value.
 * @param user_data user data passed through.
 *
 * This is the signature of callbacks used when iterating over all
 * elements stored in a BroTable.
 *
 * @returns TRUE if iteration should continue, FALSE if done.
 */
typedef int (*BroTableCallback) (void *key, void *val, void *user_data);


BroTable      *bro_table_new(void);
void           bro_table_free(BroTable *tbl);

int            bro_table_insert(BroTable *tbl,
				int key_type, const void *key,
				int val_type, const void *val);

void          *bro_table_find(BroTable *tbl, const void *key);

int            bro_table_get_size(BroTable *tbl);

void           bro_table_foreach(BroTable *tbl, BroTableCallback cb,
				 void *user_data);

void           bro_table_get_types(BroTable *tbl,
				   int *key_type, int *val_type);


/**
 * BroTableCallback - The signature of callbacks for iterating over sets.
 * @param val a pointer to an element in the set.
 * @param user_data user data passed through.
 *
 * This is the signature of callbacks used when iterating over all
 * elements stored in a BroSet.
 *
 * @returns TRUE if iteration should continue, FALSE if done.
 */
typedef int (*BroSetCallback) (void *val, void *user_data);

BroSet        *bro_set_new(void);
void           bro_set_free(BroSet *set);

int            bro_set_insert(BroSet *set, int type, const void *val);

int            bro_set_find(BroSet *set, const void *key);

int            bro_set_get_size(BroSet *set);

void           bro_set_foreach(BroSet *set, BroSetCallback cb,
			       void *user_data);

void           bro_set_get_type(BroSet *set, int *type);

/* ----------------------------- Vectors ----------------------------- */

/**
 * bro_vector_new - Creates a new vector.
 *
 * The function allocates and initializes a new empty vector.
 *
 * @returns a new vector, or %NULL on error.
 */
BroVector     *bro_vector_new(void);

/**
 * bro_vector_free - Releases a vector.
 * @param vec vector handle.
 *
 * The function releases all memory consumed by the vector pointed to
 * by @p vec.
 */
void           bro_vector_free(BroVector *vec);

/**
 * bro_vector_get_length - Returns number of elements in vector.
 * @param vec vector handle.
 *
 * @returns the number of elements in the vector.
 */
int            bro_vector_get_length(BroVector *vec);

/**
 * bro_vector_add_val - Adds a value to a vector.
 * @param vec vector handle.
 * @param type numerical type tag of the new val.
 * @param type_name optional name of specialized type.
 * @param val pointer to the new val.
 *
 * The function adds a new element to the vector pointed to by @p vec and
 * assigns the value passed in to that field. The type of the value is given
 * in @p type and must be one of
 * the %BRO_TYPE_xxx constants defined in broccoli.h. The type you give
 * implies what data type @p val must be pointing to; see the manual for
 * details. If you want to indicate a type specialized from @p type,
 * use @p type_name to give its name, otherwise pass %NULL for @p type_name.
 * It is possible to leave fields unassigned, in that case, pass in
 * %NULL for @p val.
 *
 * @p val remains the caller's responsibility and is copied internally.
 *
 * @returns %TRUE on success, %FALSE on error.
 */
int            bro_vector_add_val(BroVector *vec,
				  int type, const char *type_name,
				  const void *val);

/**
 * bro_vector_get_nth_val - Retrieves a value from a vector by index.
 * @param vec vector handle.
 * @param num index, starting from 0.
 * @param type value-result argument for the expected/actual type of the value.
 *
 * The function returns the @p num'th value of the vector pointed to
 * by @p vec, expected to be of @p type. The returned value is internal
 * and needs to be duplicated if you want to keep it around. Upon
 * return, the int pointed to by @p type tells you the type of the returned
 * value, as a BRO_TYPE_xxx type tag. If the int pointed to upon calling
 * the function has the value BRO_TYPE_UNKNOWN, no type checking is
 * performed and the value is returned. If it is any other type tag,
 * its value is compared to that of the value, and if they match, the
 * value is returned. Otherwise, the return value is %NULL. If you don't
 * care about type enforcement and don't want to know the value's type,
 * you may pass %NULL for @p type.
 *
 * @returns pointer to queried value on success, %NULL on error.
 */
void*          bro_vector_get_nth_val(BroVector *vec, int num, int *type);


/**
 * bro_vector_set_nth_val - Replaces a value in a vector, identified by index.
 * @param vec vector handle.
 * @param num index, starting from 0.
 * @param type expected type of the value.
 * @param type_name optional name of specialized type.
 * @param val pointer to new val.
 *
 * The function replaces the @p num'th value of the vector pointed to
 * by @p vec, expected to be of @p type. All values are copied internally
 * so what @p val points to stays unmodified. The value of @p type implies
 * what @p result must be pointing to. See the manual for details.
 * If you want to indicate a type specialized from @p type, use
 * @p type_name to give its name, otherwise pass %NULL for @p type_name.
 *
 * @returns %TRUE on success, %FALSE on error.
 */
int            bro_vector_set_nth_val(BroVector *vec, int num,
				      int type, const char *type_name,
				      const void *val);


/* ----------------------- Pcap Packet Handling ---------------------- */
#ifdef BRO_PCAP_SUPPORT

/**
 * bro_conn_set_packet_ctxt - Sets current packet context for connection.
 * @param bc connection handle.
 * @param link_type libpcap DLT linklayer type.
 * 
 * The function sets the packet context for @p bc for future BroPackets
 * handled by this connection.
 */
void           bro_conn_set_packet_ctxt(BroConn *bc, int link_type);

/**
 * bro_conn_get_packet_ctxt - Gets current packet context for connection.
 * @param bc connection handle.
 * @param link_type result pointer for libpcap DLT linklayer type.
 * 
 * The function returns @p bc's current packet context through @p link_type.
 */
void           bro_conn_get_packet_ctxt(BroConn *bc, int *link_type);

/** 
 * bro_packet_new - Creates a new packet.
 * @param hdr pointer to libpcap packet header.
 * @param data pointer to libpcap packet data.
 * @param tag pointer to ASCII tag (0 for no tag).
 * 
 * @returns a new BroPacket by copying @p hdr and @p data
 * internally.
 * Release the resulting packet using bro_packet_free().
 */
BroPacket     *bro_packet_new(const struct pcap_pkthdr *hdr, const u_char *data, const char* tag);

/**
 * bro_packet_clone - Clones a packet.
 * @param packet packet to clone.
 *
 * @returns a copy of @p packet, or %NULL on error.
 */
BroPacket     *bro_packet_clone(const BroPacket *packet);

/**
 * bro_packet_free - Releases a packet.
 * @param packet packet to release.
 * 
 * The function releases all memory occupied by a packet previously allocated
 * using bro_packet_new().
 */
void           bro_packet_free(BroPacket *packet);

/**
 * bro_packet_send - Sends a packet over a given connection.
 * @param bc connection on which to send packet.
 * @param packet packet to send.
 *
 * The function sends @p packet to the Bro peer connected via @p bc.
 *
 * @returns %TRUE if successful, %FALSE otherwise.
 */
int            bro_packet_send(BroConn *bc, BroPacket *packet);

#endif

/* --------------------------- Miscellaneous ------------------------- */

/**
 * bro_util_current_time - Gets current time.
 * 
 * @returns the current system time as a double, in seconds, suitable
 * for passing to bro_event_add_time().
 */
double         bro_util_current_time(void);

/**
 * bro_util_timeval_to_double - Converts timeval struct to double.
 * @param tv pointer to timeval structure.
 *
 * @returns a double encoding the timestamp given in @p tv in a floating
 * point double, with the fraction of a second between 0.0 and 1.0.
 */
double         bro_util_timeval_to_double(const struct timeval *tv);

/**
 * bro_util_is_v4_addr - Checks if an address is IPv4.
 * @param a pointer to address structure.
 *
 * @returns %TRUE if the address is IPv4, else %FALSE.
 */
int            bro_util_is_v4_addr(const BroAddr *a);

/**
 * The standard 12-byte prefix of a 16-byte IPv6 address that indicates
 * it's a mapped IPv4 address.
 */
extern const uint8 BRO_IPV4_MAPPED_PREFIX[12];

#ifdef __cplusplus
}
#endif

#endif
