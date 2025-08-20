// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPC, Log, All)

class UPlayerCharaterChoice;
class FText;
/**
 * 
 */
UCLASS()
class PROJECT_ITT_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()
	ABasePlayerController();
	
private:
	virtual void BeginPlay() override;
#pragma region Audio
public:
	void HandleBGMChange(USoundBase* NewSound);
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Audio")
	UAudioComponent* BGMAudioComponent;
	
#pragma endregion
#pragma region Widget Functions
#pragma region Character Widget
public:
	//위젯 생성
	UFUNCTION(Client,Reliable)
	void Client_MakePlayerCharacterChoice();
	//클라이언트별 Widget Image 이동
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_Client_0_KeyEvent(int32 LeftOrRight);
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_Client_1_KeyEvent(int32 LeftOrRight);
	//델리게이트 이벤트
	UFUNCTION()
	void HandleClient_0CharaterChoice(int32 MayOrCody);
	UFUNCTION()
	void HandleClient_1CharaterChoice(int32 MayOrCody);
#pragma endregion
#pragma region Multiplayer
public:
	//클라이언트 트레블링
	UFUNCTION(BlueprintCallable)
	void ConnectToServer(const FString& IPAddress);
	//클라이언트 0 / 1 체크
	UFUNCTION(Client, Reliable)
	void Client_Player_0Seting();
	//캐릭터 빙의 설정
	UFUNCTION(Server, Reliable)
	void Server_PossessEvent(int32 MayOrCody);
	//서버트레블링
	UFUNCTION(Server, Reliable)
	void Server_RequestTravel(const FString& MapName);
	//서버에 플레이어 캐릭터 선택 번호 저장
	UFUNCTION(Server, Reliable)
	void Server_PlayerChoiceSave(int32 Number);
#pragma endregion
#pragma endregion
#pragma region Widget Variables
private:
	// 위젯을 저장하였다 사용을 위한
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UPlayerCharaterChoice> CharacterWidgetClass;
public:
	UPROPERTY()
	UPlayerCharaterChoice* PlayerSelectionWidget;
	//플레이어 Client_0 체크
	bool Client_0_Check = false;
	
#pragma endregion
#pragma region Debug
public:
	UFUNCTION(Exec)
	void Debug_RespawnPlayer();

	UFUNCTION(Server, Reliable)
	void ServerRespawnAtLastCheckPoint();
#pragma endregion 
};
