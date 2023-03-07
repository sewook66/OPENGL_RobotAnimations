# OPENGL_RobotAnimations

### 요약
Visual Studio 2022 17.5.0 에서 C와 OpenGL library를 활용하여 만든 RobotAnimation.
<table>
  <td>
    <img width="250" height="250" alt="C_logo" src="https://user-images.githubusercontent.com/51785417/223395004-25e24609-9123-491e-904a-8977f3816e03.png">
  </td>
  <td>
    <img width="450" height="250" alt="OpenGL_logo" src="https://user-images.githubusercontent.com/51785417/223395098-3cef5831-a8d4-4176-9143-0378e9cea1ba.png">
  </td>
</table>

- 공간(space) <br>
matrix로 2개의 texture space 생성 후 texture mapping 하여 링의 ground와 fence 생성 <br>
생성된 ground와 fence를 cage형태로 만들기 위해 depth buffer를 적용 <br>

- 로봇(robot) <br>
몸(body) -> 머리(head), 팔(arm),(허벅지(leg) -> 종아리) 순으로 연결하여 좌표설정 <br>
중심이동을 하는 body의 움직임을 기반으로 head, arm, leg는 body를 축으로 시간에 따른 각도만 변경하며 움직임을 만들고 종아리는 leg를 축으로 각도를 변경해 걷는 motion을 설정 <br>

- 조작(control) <br>
마우스 왼클릭 : start <br>
마우스 오른클릭 : stop <br>
page up : zoom out <br>
page down : zoom in <br>
방향키 up : +y <br>
방향키 down : -y <br>
방향키 left : -x <br>
방향키 right : +x <br>


### 실행화면
<table>
  <td>
    <img width="450" height="450" alt="RobotAnimations_1" src="https://user-images.githubusercontent.com/51785417/223383711-b726a4f3-5ecc-4b7a-9703-4ffcbd807fa3.png">
  </td>
  <td>
    <img width="450" height="450" alt="RobotAnimations_3" src="https://user-images.githubusercontent.com/51785417/223395994-55763e2a-f229-4a9f-a0ec-3b28dfeadd5c.png">
   </td>
</table>
