![image](https://github.com/user-attachments/assets/fd4eb57b-68cf-49e4-85db-1e037b035b90)

사과게임을 대신 풀어주는 Helper 프로그램입니다.
코드는 크게 네 가지 모듈로 구분되어 있습니다.

1. 스크린 캡처 모듈(ScreenCapture) - 게임의 스크린을 캡처합니다. (opencv)
2. 이미지 인식 모듈(ImageProcessor) - 캡처된 스크린샷의 사과의 숫자를 식별하고 벡터에 저장합니다.
3. 사과게임 계산 모듈(AppleGameSolver) - 주어진 사과 배열에서 제거할 수 있는 best bundle을 추천해줍니다.
4. 마우스 조작 모듈(MouseController) - best bundle이 존재하는 위치로 마우스를 이동시켜서 직접 드래그해서 사과를 제거해줍니다.
