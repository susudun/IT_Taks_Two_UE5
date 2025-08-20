// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BaseGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClient_0CharacterChoice,int32,Choice);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClient_1CharacterChoice,int32,Choice);
class ABasePlayerState;
class ABaseCharacter;

DECLARE_LOG_CATEGORY_EXTERN(LogGS, Log, All)
/**
 * 
 */
UCLASS()
class PROJECT_ITT_API ABaseGameState : public AGameStateBase
{
	GENERATED_BODY()
private:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

#pragma region Audio
public:
	UPROPERTY(ReplicatedUsing=OnRep_CurrentBGM)
	USoundBase* CurrentBGM;

protected:
	UFUNCTION()
	void OnRep_CurrentBGM();
#pragma endregion
#pragma region Playerlist;
public:
	UPROPERTY(Replicated)
	TArray<ABasePlayerState*> AllPlayerState;

	UPROPERTY(Replicated)
	TArray<ABaseCharacter*> AllPlayerCharacter;
#pragma endregion
#pragma region PlayerCharaterChoiceWidget
public:
	UPROPERTY(Replicated,ReplicatedUsing=OnRep_Client_0)
	int32 Client_0=3;

	UPROPERTY(Replicated,ReplicatedUsing=OnRep_Client_1)
	int32 Client_1=3;

	UFUNCTION()
	void OnRep_Client_0();
	UFUNCTION()
	void OnRep_Client_1();
#pragma endregion
#pragma region DELEGATE
	UPROPERTY(BlueprintAssignable)
	FOnClient_0CharacterChoice OnClient_0CharacterChoice;
	UPROPERTY(BlueprintAssignable)
	FOnClient_1CharacterChoice OnClient_1CharacterChoice;
#pragma endregion
};
