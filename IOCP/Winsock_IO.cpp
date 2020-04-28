
/* Winsock IO */

/*

 소켓 모드
 윈속 함수가 어떻게 동작할지 결정

 1. blocking ( 리턴될 때까지 블럭 )
  - send, recv를 구현할 때 스레드로 구현

 2. non-blocking ( 즉시 리턴 )
  - 처리를 해줄 구조가 있어야함
  - ioctlsocket( , FIONBIO, ... )


 -------------------------------------------------
 -------------------------------------------------


 소켓 모델
 어플리케이션이 IO를 어떻게 처리할지 결정

 1. blocking
 2. select
 3. WSAAsyncSelect()
 4. WSAEventSelect()
 5. overlapped i/o
 6. completion port

 */