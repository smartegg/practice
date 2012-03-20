
const uint16_t kPort = 10000;

int main(void) {
  InetAddr server_addr(kPort);
  //initialize logging server endpoint and register with dispatcher.
  LoggingAcceptor la(server_addr);

  //event loop
  for (;;)
    InitiationDispatcher::GetInstance()->HandleEvents();
  return 0;
}
