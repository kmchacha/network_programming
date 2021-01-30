## Multicast란

<img src="/Multicast.svg" width="450px" height="300px" title="px(픽셀) 크기 설정" alt="Multicast"></img><br/>

1. multicast는 일반적으로 1:n 통신을 하는 기술이다
2. 한 번의 송신으로 메시지나 정보를 목표한 컴퓨터들(그룹)에 동시에 전송하는 것을 말한다

장점) 
multicast 전송이 지원도면 송신자는 여러 수신자에게 한 번에 메시지가 전송되도록 하여, 데이터의 중복전송으로 인한 네트워크 자원 낭비를 최소화 할 수 있게 된다

multicast 전송이 일반적인 유니캐스트 인터넷 응용 분야와 다른 점은 우선 그 전송 패킷에 있다. 일반적으로 TCP/IP 상의 인터넷 응용 프로그램은 데이터의 송신자가 이를 수신할 수신자의 인터넷 주소를 전송 패킷을 전송한다. 그러나 multicast 전송을 위해서는 header에 수신자의 주소 대신 수신자들이 참여하고 있는 그룹 주소를 표시하여 패킷을 전송한다

멀티 캐스트 전송을 위한 그룹 주소는 Class D (224.0.0.0~239.255.255.255)이다. Class A, Class B, Class C의 IP 주소와는 달리 실제의 호스트를 나타내는 주소가 아니며, 그룹 주소를 갖는 multicast 패킷을 전송받은 수신자는 자신이 패킷의 그룹에 속해있는 가를 판단해 패킷의 수용여부를 결정하게 된다



## broadcast란

<img src="/Broadcast.svg" width="450px" height="300px" title="px(픽셀) 크기 설정" alt="Broadcast"></img><br/>

1. broadcastting이란 하나의 서브넷 안에서의 1:All 통신 방식이다. (MAC주소를 알아내기 위해서 동작하는 ARP가 바로 broadcast 이다) 
2. 한 번의 송신으로 메시지나 정보를 같은 서브넷에 동시에 전송하는 것을 말한다 (패킷은 하나가 전달되고 라우터에서 각자 네트워크에 뿌려주는 느낌으로 이해하였다)

장점)
여러 서비스를 관리하는 특정 스테이션이나 장비의 위치 파악 등의 다양한 목적으로 사용된다
ARP는 자신과 데이터 통신을 하기 위한 다른 노드의 맥 주소를 알아내기 위한 프로토콜인데, IP주소는 알고 있지만 맥 주소를 모를 때 사용한다

<img src="cast.gif" width="450px" height="300px" title="px(픽셀) 크기 설정" alt="cast"></img><br/>

** Multicast 와 Broadcast 모두 UDP 방식을 따른다

##### 실습1)
    news_sender.c  -   news_receiver.c
    -> 기존의 파일에서 news.txt 파일을 읽어와서 multicast 해주는 방식이였는데 코드를 살짝 바꾸어 사용자의 입력으로 부터 내용을 받아서 multicast 해주는 방식으로 바꾸었다
    
    ./news_sender 224.0.0.1 50000
    ./news_receiver 224.0.0.1 50000


##### 실습2)
    news_receiver_brd.c - news_sender_brd.c
    -> 
