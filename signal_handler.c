/**********************************/
/*  File Signal Handler Example        */
/**********************************/

typdef void (*sighandler_t) int  // Type definition for easy reading

void new_handler(int receivedSignal){
  fprintf(stdout, "I have just received the signal %d \n", receivedSignal);

  signalhandler_t return_value_signal;
  return_value_signal = signal(SIGINT, new_handler)
}
