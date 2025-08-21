# IT_Taks_Two_UE5
- IT_TAKS_Two 언리얼 모작
구현 기능 코드 및 코드 내용 정리
1. 싱글플레이로 진행 하여 Player Name 과 서버 IP 를 수동으로 입력 받은 이후 ClientTravel 을 통하여 서버에 접속
2. dedicated server 는 사전에 캐릭터 선택 레벨을 오픈하고 플레이어 접속을 대기 2명이 접속시 Gamemode PostLogin 에서 PlayerController에 Widget 생성 함수 호출
3.  Widget 에서 이벤트 처리후 GameMode에서 ServerTravel 을 진행하여 inGame Map 으로 이동
4.  멀티환경 UI 동기화 작업 (내용이 복잡하며 설명은 생략은 간단하게 플로우만 작성하겠습니다.)
   - Widget -> PC -> GS (DELEGATE) -> PC -> Widget 수정

- 언리얼 멀티환경 프레임워크 설명
싱글환경에서 ClientTravel 이 진행 된다면 게임이 오픈되면서 가지고있던 GameInstances 를 제외하고는 접속 서버에 GameMode 및 모든 프레임워크들을 서버에서 발급받아 사용
서버에서 ServerTravel 진행시 기존 레벨에 모든 프레임워크를 두고 새로운 레벨에 프레임워크를 사용하기에 필요한 프레임워크 정보는 저장 후 다시 전달해야 함

- GameMode
  - Server에서 모든 동작이 발생
  - PostLogin에서 Player 접속시 모든것을 처리 
  - Widget->PlayerController->Gamemode.HandleTravel(); Widget 이벤트 종료후 ServerTravel 진행하면 GameMode 기능 완료

- GameInstances
  - CachedUserName으로 플레이어 이름저장
 
- PlayerController
  - 위젯 동기화 및 ClientTravel 발생

- PlayerState
  - 선택한 캐릭터를 저장 시키기 위한 용도

- GameState
  - 위젯 동기화를 위해 DELEGATE 를 사용하기위해
 
- Widget
  - PlayerCharaterChoice 멀티 시작시 캐릭터 선택 관련 위젯
  - PlayerSelect: 상대 캐릭터 머리 위에 Character Tick 표시
      - Location: 레벨에 배치된 상대 캐릭터 머리 위
      - Rotation: 자기 기준 ForwardVector - 180° (플레이어 시야 기준 정렬)  
