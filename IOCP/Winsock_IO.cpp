
/* Winsock IO */

/*

 ���� ���
 ���� �Լ��� ��� �������� ����

 1. blocking ( ���ϵ� ������ �� )
  - send, recv�� ������ �� ������� ����

 2. non-blocking ( ��� ���� )
  - ó���� ���� ������ �־����
  - ioctlsocket( , FIONBIO, ... )


 -------------------------------------------------
 -------------------------------------------------


 ���� ��
 ���ø����̼��� IO�� ��� ó������ ����

 1. blocking
 2. select
 3. WSAAsyncSelect()
 4. WSAEventSelect()
 5. overlapped i/o
 6. completion port

 */