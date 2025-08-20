# IT_Taks_Two_UE5
-IT_TAKS_Two 언리얼 모작
구현 기능 코드 및 코드 내용 정리
1. 싱글플레이로 진행 하여 Player Name 과 서버 IP 를 수동으로 입력 받은 이후 ClientTravel 을 통하여 서버에 접속
2. dedicated server 는 사전에 캐릭터 선택 레벨을 오픈하고 플레이어 접속을 대기 2명이 접속시 Gamemode PostLog 에서 PlayerController에 Widget 생성 함수 호출
3.  Widget 에서 이벤트 처리후 GameMode에서 ServerTravel 을 진행하여 inGame Map 으로 이동

-언리얼 멀티환경 프레임워크 설명
싱글환경에서 ClientTravel 이 진행 된다면 게임이 오픈되면서 가지고있던 GameInstances 를 제외하고는 접속 서버에 GameMode 및 모든 프레임워크들을 서버에서 발급받아 사용
서버에서 ServerTravel 진행시 기존 레벨에 모든 프레임워크를 두고 새로운 레벨에 프레임워크를 사용하기에 필요한 프레임워크 정보는 저장 후 다시 전달해야 함
-GameMode
```cpp
ABaseGameMode::ABaseGameMode()
{
	bUseSeamlessTravel = true;
}
```
