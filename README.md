# NewStart
 새출발 C++ 서버 클라이언트 프로그래밍



<hr/>

## 2020.07.17

#### 변경 사항

	1. WindowsNetworking/server.cpp -- putty로 접속한 클라이언트와 채팅 UI가 이상했던 부분 수정 



#### 추가 사항

 1. new_server_1.cpp, new_client_1.cpp -- 새롭게 추가



#FD_SET의 동작 원리 #select() 함수 동작 원리 #다중 접속시 벌어지는 과부하 현상 예측

이후엔 해당 네트워킹 컴포넌트를 활용할 수 있는 프로젝트를 만들어보자

<hr/>

## 2020.07.16

 #### 변경사항

 1. C++ Windows 프로그래밍 추가 -- MSDN 개발자 사이트에서 튜토리얼을 따라해봤지만 결과적으로 동작하지 않음
 2. server.cpp 기존 C++ 서버 코드 중 putty로 접속한 클라이언트에 전시되는 형식이 이상해 buffer 값을 확인하고 주석을 추가 

[MSDN Docs](https://docs.microsoft.com/ko-kr/windows/win32/learnwin32/learn-to-program-for-windows, "MSDN Docs link")

<hr/>

## 2020.07.15 

server.cpp 

Refer [Sloan kelly's youtube](https://www.youtube.com/watch?v=dquxuXeZXgo&list=PLZo2FfoMkJeEogzRXEJeTb3xpA2RAzwCZ&index=11)

putty 툴을 이용해 서버에 접속하는 채팅 프로그램

[putty]([http://www.chiark.greenend.org.uk/~sg...](https://www.youtube.com/redirect?v=dquxuXeZXgo&event=video_description&q=http%3A%2F%2Fwww.chiark.greenend.org.uk%2F~sgtatham%2Fputty%2Flatest.html&redir_token=QUFFLUhqbGVGSTFudERQT1BUVVcwaGlHR09MeDJ6WlNqUXxBQ3Jtc0trVXpDMlNfckJXZGRXRlFiR3A2amUtWVNNYm5QLXlpcmF1MUNxTjZSN1IyallUU0R3SUltRnNXX25teUtTalBjc0Vsb3pmX1BTNTh4M3VYMEZudkc1RF9ZWmpTaTYxSFFYZXAyQXlIbnNHenZZVlFRUQ%3D%3D))

<hr/>