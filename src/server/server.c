/**
 * The server is going to be a struct that holds a couple of things in
 * memory
 *  - epoll_events array
 *  - length of the array
 *  - current size of that array
 *  - listen_sd ( copy of 0th elem in above array )
 *  - state
 *  - timeout
 *  - epoll_events buffer
 *
 * When we initialize, we basically need to spin up the mem for everything above
 * and set everything that is not a pointer to 0 or NUL or something like that
**/
server_initialize()  {

}
/**
 * to set things up, we need to take in all of the external information that we
 * need for this server to run.
 *  - port
 *  - host
 *  - backlog
 *  - max events
 * essentially this is our configuration
 *
 * we want to create the listening socket and put it into an epoll event
**/
server_setup()  {

}

/**
 * running is where all of the serious logic comes into play
 * PSUEDOCODE:
 * run()
 *   DO WHILE state != stop
 *   | poll()
 *
 *   | FOR EACH fd
 *   | | IF listen_sd
 *   | | | state = server_listener()
 *
 *   | | ELSE
 *   | | | IF readable
 *   | | | | conn_state = server_reader()
 *   | | | IF writable
 *   | | | | conn_state = server_writer()
 *
 *   | | | IF conn_state == close
 *   | | | | close_conn()
 *
 *   | IF compress_array
 *   | | compress_array()
 *   | IF expand_array
 *   | | expand_array()
 *
 *   clean_up()
**/
server_run()  {

}

/**
 * This method simply takes an array and will compress it
 * i.e. move all elements so that they are contiguous in memory
 * This will eventually be taken care of by a dynamic array
**/
compress_array()  {

}

/**
 * Expand array will concatenate one array onto another
 * This will eventually be taken care of by a dynamic array
**/
expand_array()  {

}
