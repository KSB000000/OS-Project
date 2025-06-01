생산자 소비자 문제

MAX = 30 이면 lock을 걸고

생상자는 cond_wait을 해서 x가 30이 되면 +를 진행할 수 없게 spin lock을 걸어 놓는다.

그리고 소비자는 0이 되면 x가 -가 되지 않게 spin lock을 걸어 놓는다.

그렇게 0~30 사이를 반복하며 출력하게 된다.


<img width="697" alt="PD" src="https://github.com/user-attachments/assets/0e1811e7-5027-4caf-8584-f5e0e6ddd1ab" />
