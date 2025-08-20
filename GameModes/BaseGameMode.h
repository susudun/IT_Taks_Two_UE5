// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseGameMode.generated.h"

class ABaseCharacter;
class ABasePlayerController;
class ABaseGameState;

DECLARE_LOG_CATEGORY_EXTERN(LogGM, Log, All)
/**
 * 
 */
UCLASS()
class PROJECT_ITT_API ABaseGameMode : public AGameModeBase
{
	GENERATED_BODY()
	ABaseGameMode();
	virtual void BeginPlay() override;
private:
	//로그인시 발생되는 이벤트
	virtual void PostLogin(APlayerController* NewPlayer) override;
	//서버트레블링시 클라이언트들 컨트롤러 설정가능함 PostLogin과 비슷
	virtual void HandleSeamlessTravelPlayer(AController*& C) override;
	
#pragma region Audio
protected:
	UPROPERTY(EditDefaultsOnly, Category="BGM")
	USoundBase* BaseBGM;

public:
	void ChangeBGM();
private:
	bool bIsBgmPlaying = false;
#pragma endregion
#pragma region PlayerCharaterSeting
private:
	//캐릭터 셋팅
	UPROPERTY(EditDefaultsOnly, Category = "PawnClasses")
	TSubclassOf<ABaseCharacter> BP_MayClass;

	UPROPERTY(EditDefaultsOnly, Category = "PawnClasses")
	TSubclassOf<ABaseCharacter> BP_CodyClass;
	
	bool IsClient_0=false;

protected:
	UPROPERTY(EditDefaultsOnly,Category="Maps")
	FName InGameMapName;
	
public:
	UPROPERTY()
	bool CodyPossessed = false;
	UPROPERTY()
	bool MayPossessed = false;
#pragma endregion
#pragma region Functions
	UFUNCTION(BlueprintPure, Category = "Maps")
	FName GetCurrentMapNameAsFName() const;
	
	//캐릭터 선택 전용 카메라
	void GetLavelSetingCamera(ABasePlayerController* PC);
	//캐릭터 생성
	void SpawnCharacterMay(APlayerStart* StartPoint,ABaseGameState* GS);
	void SpawnCharacterCody(APlayerStart* StartPoint,ABaseGameState* GS);
public:
	//서버트레블링
	void HandleTravel(const FString& MapName);
#pragma endregion
};
